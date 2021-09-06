#include <QtWebEngineWidgets/QWebEngineView>

#include "iservicewindow.h"
#include "ui_iservicewindow.h"

#include "manager/airablemanager.h"
#include "manager/qobuzmanager.h"

#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"
#include "util/iservice.h"

#include "base/menuinfo.h"
#include "base/menuicon.h"
#include "base/menuicondelegate.h"
#include "base/menulist.h"
#include "base/menulistdelegate.h"

#include "dialog/logindialog.h"
#include "dialog/webengineviewdialog.h"

#include "manager/airablemanager.h"
#include "manager/qobuzmanager.h"

#define ISERVICE_TITLE	"Internet Service"
#define QOBUZ_TITLE	"Qobuz"



IServiceWindow::IServiceWindow(QWidget *parent, const QString &addr)
	: QWidget(parent)
	, m_pAirableMgr(new AirableManager)
	, m_pQobuzMgr(new QobuzManager)
	, m_pMenuInfo(new MenuInfo(this))
	, m_pMenuIcon(new MenuIcon(this))
	, m_pMenuList(new MenuList(this))
	, m_pLoading(new Loading(parent))
	, m_WebURL("")
	, m_ServiceType(-1)
	, ui(new Ui::IServiceWindow)
{
	ui->setupUi(this);

	m_pAirableMgr->SetAddr(addr);
	m_pQobuzMgr->SetAddr(addr);

	ConnectSigToSlot();
}

IServiceWindow::~IServiceWindow()
{
	delete ui;

	if (m_pAirableMgr)
	{
		delete m_pAirableMgr;
		m_pAirableMgr = nullptr;
	}

	if (m_pQobuzMgr)
	{
		delete m_pQobuzMgr;
		m_pQobuzMgr = nullptr;
	}

	if (m_pMenuInfo)
	{
		delete m_pMenuInfo;
		m_pMenuInfo = nullptr;
	}

	if (m_pMenuIcon)
	{
		delete m_pMenuIcon;
		m_pMenuIcon = nullptr;
	}

	if (m_pMenuList)
	{
		delete m_pMenuList;
		m_pMenuList = nullptr;
	}

	if (m_pLoading)
	{
		delete m_pLoading;
		m_pLoading = nullptr;
	}
}

void IServiceWindow::RequestIServiceHome(QList<CJsonNode> list)
{
	ui->gridLayoutTop->addWidget(m_pMenuInfo);
	ui->gridLayoutBottom->addWidget(m_pMenuIcon);

	SetCoverArtServiceHome(list);

	m_pMenuInfo->SetTitle(ISERVICE_TITLE);
	m_pMenuIcon->SetNodeList(list);
}

void IServiceWindow::RequestIServiceURL(int nServiceType, QString url)
{
	ui->gridLayoutTop->addWidget(m_pMenuInfo);
	ui->gridLayoutBottom->addWidget(m_pMenuList);

	m_pAirableMgr->RequestURL(nServiceType, url);
}

void IServiceWindow::RequestQobuzHome()
{
	// 검색, 추천채널, 선호채널, 재생목록
	ui->gridLayoutTop->addWidget(m_pMenuInfo);
	ui->gridLayoutBottom->addWidget(m_pMenuIcon);

	QList<CJsonNode> list;
	SetQobuzHome(list);

	m_pMenuInfo->SetTitle(QOBUZ_TITLE);
	m_pMenuIcon->SetNodeList(list);
}

AirableManager *IServiceWindow::GetAirableManager()
{
	return m_pAirableMgr;
}

QobuzManager *IServiceWindow::GetQobuzManager()
{
	return m_pQobuzMgr;
}

void IServiceWindow::SlotAddWidget(QWidget *widget)
{
	emit SigAddWidget(widget);		// recursive
}

void IServiceWindow::SlotRespAirableLogout()
{
	emit SigRespLogout();			// recursive
}

void IServiceWindow::SlotPlayAll()
{
	LogDebug("click play all");
}

void IServiceWindow::SlotPlayRandom()
{
	LogDebug("click play random");
}

void IServiceWindow::SlotSubmenu()
{
	LogDebug("click submenu");
}

void IServiceWindow::SlotSort()
{
	LogDebug("click sort");
}

void IServiceWindow::SlotSelectIService(int nServiceType)
{
	if (nServiceType == iIServiceType_Qobuz)
	{
		m_pQobuzMgr->RequestLogin();
	}
	else
	{
		m_pAirableMgr->RequestAuth(nServiceType);
	}
}

void IServiceWindow::SlotSelectURL(QString rawData)
{
	CJsonNode node;
	if (!node.SetContent(rawData))
	{
		LogCritical("invalid json");
		return;
	}

	LogDebug("node [%s]", node.ToTabedByteArray().data());

	int nType = node.GetInt(KEY_TYPE);
	QString url = node.GetString(KEY_URL);

	//LogDebug("click url [%d] [%s]", nType, url.toUtf8().data());

	if (nType & iAirableType_Mask_Dir)
	{
//		emit SigSelectURL(m_ServiceType, url);
		IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
		emit SigAddWidget(widget);
		widget->RequestIServiceURL(m_ServiceType, url);
	}
	else if (nType & iAirableType_Mask_Play)
	{
		m_pAirableMgr->RequestPlay(m_ServiceType, node);
	}
	else if (nType & iAirableType_Mask_Logout)
	{
		if (m_ServiceType == iIServiceType_Qobuz)
		{

		}
		else
		{
			m_pAirableMgr->RequestLogout(m_ServiceType, url);
		}
	}

}

void IServiceWindow::SlotReqArt(QString url, int index)
{
	m_pAirableMgr->RequestCoverArt(url, index, QListView::ListMode);
}

void IServiceWindow::SlotRespQobuzLoginFail(CJsonNode node)
{
	QString user = node.GetString(KEY_USER);
	QString pass = node.GetString(KEY_PASS);

	LoginDialog dialog;
	dialog.SetUserID(user);
	dialog.SetPassword(pass);
	if (dialog.exec() == QDialog::Accepted)
	{
		user = dialog.GetUserID();
		pass = dialog.GetPassword();
		m_pQobuzMgr->RequestLogin(user, pass);
	}
	else
	{
//		emit SigBtnPrev();
	}

}

void IServiceWindow::SlotRespQobuzLoginSuccess()
{
	IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
	emit SigAddWidget(widget);
	widget->RequestQobuzHome();
}

void IServiceWindow::SlotRespAirableLoginFail(CJsonNode node)
{
	bool bOAuth = false;
	int nServiceType = node.GetInt(KEY_TYPE);
	QString user = node.GetString(KEY_USER);
	QString pass = node.GetString(KEY_PASS);
	m_WebURL = node.GetString(KEY_URL);
	if (m_WebURL.contains("oauth"))
	{
		bOAuth = true;
	}

	LoginDialog dialog;
	dialog.SetUserID(user);
	dialog.SetPassword(pass);
	if (dialog.exec() == QDialog::Accepted)
	{
		user = dialog.GetUserID();
		pass = dialog.GetPassword();

		m_pAirableMgr->RequestLogin(nServiceType, user, pass, bOAuth);
	}
	else
	{
//		emit SigBtnPrev();
	}
}

void IServiceWindow::SlotRespAirableLoginSuccess(int nServiceType, bool bSaveAuth)
{
	if (bSaveAuth)
	{
		WebEngineViewDialog dialog(this, m_WebURL);
		if (dialog.exec() == QDialog::Accepted && dialog.GetLogin())
		{
			m_pAirableMgr->RequestURL(nServiceType);
		}
	}
	else
	{
		m_pAirableMgr->RequestURL(nServiceType);
	}

}

void IServiceWindow::SlotRespAuth(int nServiceType)
{
	IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
	emit SigAddWidget(widget);
	widget->RequestIServiceURL(nServiceType);
}


void IServiceWindow::SlotRespURL(int nServiceType, QList<CJsonNode> list)
{
	m_ServiceType = nServiceType;
	m_pMenuInfo->SetTitle(ISERVICE_TITLE);
	m_pMenuList->SetNodeList(list);
}

void IServiceWindow::SlotCoverArtUpdate(QString fileName, int nIndex, int mode)
{
	if (QListView::IconMode == mode)
	{
		QStandardItem *item = m_pMenuIcon->GetModel()->item(nIndex);
		item->setData(fileName, MenuIconDelegate::MENU_ICON_ROLE_COVER);
		m_pMenuIcon->GetModel()->setItem(nIndex, item);
	}
	else
	{
		QStandardItem *item = m_pMenuList->GetModel()->item(nIndex);
		item->setData(fileName, MenuListDelegate::MENU_LIST_ROLE_ART);
		m_pMenuList->GetModel()->setItem(nIndex, item);
	}
}

void IServiceWindow::ConnectSigToSlot()
{
	// recursive
	connect(this, SIGNAL(SigAddWidget(QWidget*)), parent(), SLOT(SlotAddWidget(QWidget*)));
	connect(this, SIGNAL(SigRespLogout()), parent(), SLOT(SlotRespAirableLogout()));

	connect(m_pMenuInfo, SIGNAL(SigPlayAll()), this, SLOT(SlotPlayAll()));
	connect(m_pMenuInfo, SIGNAL(SigPlayRandom()), this, SLOT(SlotPlayRandom()));
	connect(m_pMenuInfo, SIGNAL(SigSubmenu()), this, SLOT(SlotSubmenu()));
	connect(m_pMenuInfo, SIGNAL(SigSort()), this, SLOT(SlotSort()));

	connect(m_pMenuIcon->GetDelegate(), SIGNAL(SigSelectCoverArt(int)), this, SLOT(SlotSelectIService(int)));
	connect(m_pMenuList->GetDelegate(), SIGNAL(SigSelectIcon(QString)), this, SLOT(SlotSelectURL(QString)));
	connect(m_pMenuList->GetDelegate(), SIGNAL(SigSelectTitle(QString)), this, SLOT(SlotSelectURL(QString)));
	connect(m_pMenuList, SIGNAL(SigReqArt(QString, int)), this, SLOT(SlotReqArt(QString, int)));

	connect(m_pQobuzMgr, SIGNAL(SigRespLoginFail(CJsonNode)), this, SLOT(SlotRespQobuzLoginFail(CJsonNode)));
	connect(m_pQobuzMgr, SIGNAL(SigRespLoginSuccess()), this, SLOT(SlotRespQobuzLoginSuccess()));

	connect(m_pAirableMgr, SIGNAL(SigRespLogout()), parent(), SLOT(SlotRespAirableLogout()));
	connect(m_pAirableMgr, SIGNAL(SigRespLoginFail(CJsonNode)), this, SLOT(SlotRespAirableLoginFail(CJsonNode)));
	connect(m_pAirableMgr, SIGNAL(SigRespLoginSuccess(int, bool)), this, SLOT(SlotRespAirableLoginSuccess(int, bool)));

	connect(m_pAirableMgr, SIGNAL(SigRespAuth(int)), this, SLOT(SlotRespAuth(int)));
	connect(m_pAirableMgr, SIGNAL(SigRespURL(int, QList<CJsonNode>)), this, SLOT(SlotRespURL(int, QList<CJsonNode>)));
	connect(m_pAirableMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));

}

void IServiceWindow::SetCoverArtServiceHome(QList<CJsonNode> &srcList)
{
	QList<CJsonNode> dstList;
	QString strCover = "";
	foreach (CJsonNode node, srcList)
	{
		if (node.GetInt(KEY_TYPE) == iIServiceType_Qobuz)
		{
			strCover = ":/resource/baseline_search_black_24dp.png";
		}
		else if (node.GetInt(KEY_TYPE) == iIServiceType_Radios)
		{
			strCover = ":/resource/baseline_search_black_24dp.png";
		}
		else if (node.GetInt(KEY_TYPE) == iIServiceType_Podcasts)
		{
			strCover = ":/resource/baseline_search_black_24dp.png";
		}
		else if (node.GetInt(KEY_TYPE) == iIServiceType_Tidal)
		{
			strCover = ":/resource/baseline_search_black_24dp.png";
		}
		else if (node.GetInt(KEY_TYPE) == iIServiceType_Deezer)
		{
			strCover = ":/resource/baseline_search_black_24dp.png";
		}
		else if (node.GetInt(KEY_TYPE) == iIServiceType_Napster)
		{
			strCover = ":/resource/baseline_search_black_24dp.png";
		}
		else if (node.GetInt(KEY_TYPE) == iIServiceType_HighResAudio)
		{
			strCover = ":/resource/baseline_search_black_24dp.png";
		}
		else if (node.GetInt(KEY_TYPE) == iIServiceType_AmazonMusic)
		{
			strCover = ":/resource/baseline_search_black_24dp.png";
		}
		else
		{
			LogDebug("not found type");
			continue;
		}
		node.Add(KEY_COVER_ART, strCover);
		dstList.append(node);
	}
	srcList = dstList;
}

void IServiceWindow::SetQobuzHome(QList<CJsonNode> &srcList)
{

}
