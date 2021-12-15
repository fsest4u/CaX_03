#include <QtWebEngineWidgets/QWebEngineView>
#include <QThread>

#include "iservicewindow.h"
#include "ui_iservicewindow.h"

#include "dialog/commondialog.h"
#include "dialog/logindialog.h"
#include "dialog/webengineviewdialog.h"
#include "dialog/searchdialog.h"

#include "manager/airablemanager.h"
#include "manager/qobuzmanager.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/utilnovatron.h"

#include "widget/form/formplay.h"
#include "widget/form/formsort.h"

#include "widget/airable.h"
#include "widget/qobuz.h"

#include "widget/formTop/infoservice.h"
#include "widget/formTop/infobrowser.h"
#include "widget/formBottom/iconservice.h"
#include "widget/formBottom/iconservicedelegate.h"
#include "widget/formBottom/listbrowser.h"
#include "widget/formBottom/listbrowserdelegate.h"
//#include "widget/formBottom/listservice.h"
//#include "widget/formBottom/listservicedelegate.h"

#define ISERVICE_TITLE	"Internet Service"
#define QOBUZ_TITLE		"Qobuz"

IServiceWindow::IServiceWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pAirableMgr(new AirableManager),
	m_pQobuzMgr(new QobuzManager),
	m_pInfoService(new InfoService(this)),
	m_pInfoBrowser(new InfoBrowser(this)),
	m_pIconService(new IconService(this)),
	m_pListBrowser(new ListBrowser(this)),
//	m_pListService(new ListService(this)),
	m_pListThread(new QThread),
	m_WebURL(""),
	m_ServiceType(-1),
	m_bGenre(false),
	ui(new Ui::IServiceWindow)
{
	ui->setupUi(this);

	m_pAirableMgr->SetAddr(addr);
	m_pQobuzMgr->SetAddr(addr);

	ConnectSigToSlot();

	Initialize();

}

IServiceWindow::~IServiceWindow()
{
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

	if (m_pInfoService)
	{
		delete m_pInfoService;
		m_pInfoService = nullptr;
	}

	if (m_pInfoBrowser)
	{
		delete m_pInfoBrowser;
		m_pInfoBrowser = nullptr;
	}

	if (m_pIconService)
	{
		delete m_pIconService;
		m_pIconService = nullptr;
	}

	if (m_pListBrowser)
	{
		delete m_pListBrowser;
		m_pListBrowser = nullptr;
	}

//	if (m_pListService)
//	{
//		delete m_pListService;
//		m_pListService = nullptr;
//	}

	if (m_pListThread)
	{
		ThreadTerminateList();
		delete m_pListThread;
		m_pListThread = nullptr;
	}

	delete ui;

}

void IServiceWindow::IServiceHome(QList<CJsonNode> list)
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pIconService);

	SetIServiceHome(list);

	m_pInfoService->SetTitle(ISERVICE_TITLE);
	m_pIconService->SetNodeList(list, IconService::ICON_SERVICE_ISERVICE);
}

void IServiceWindow::RequestIServiceURL(int nServiceType, QString url)
{
	m_pAirableMgr->RequestURL(nServiceType, url);
}

void IServiceWindow::RequestQobuzSearch(int nType, QString keyword, int nStart, int nCount)
{
	m_pQobuzMgr->RequestSearch(nType, keyword, nStart, nCount);
}

void IServiceWindow::RequestQobuzGenre(QString strID)
{
	m_pQobuzMgr->RequestGenre(strID);
}

void IServiceWindow::RequestQobuzRecommend(int nType, QString strID, int nStart, int nCount, QString strGenreID)
{
	m_pQobuzMgr->RequestRecommend(nType, strID, nStart, nCount, strGenreID);
}

void IServiceWindow::RequestQobuzFavorite(int nType, int nStart, int nCount)
{
	m_pQobuzMgr->RequestFavorite(nType, nStart, nCount);
}

void IServiceWindow::RequestQobuzPlaylist(int nStart, int nCount)
{
	m_pQobuzMgr->RequestPlaylist(nStart, nCount);
}

void IServiceWindow::RequestQobuzCategory(int nType, QString strID, int nStart, int nCount)
{
	m_pQobuzMgr->RequestCategory(nType, strID, nStart, nCount);
}

void IServiceWindow::RequestQobuzPlay(QMap<int, CJsonNode> nodeMap, int nWhere)
{
	m_pQobuzMgr->RequestPlay(nodeMap, nWhere);
}

AirableManager *IServiceWindow::GetAirableManager()
{
	return m_pAirableMgr;
}

QobuzManager *IServiceWindow::GetQobuzManager()
{
	return m_pQobuzMgr;
}

ListBrowser *IServiceWindow::GetListBrowser() const
{
	return m_pListBrowser;
}

InfoBrowser *IServiceWindow::GetInfoBrowser() const
{
	return m_pInfoBrowser;
}

int IServiceWindow::GetServiceType() const
{
	return m_ServiceType;
}

void IServiceWindow::SetServiceType(int ServiceType)
{
	m_ServiceType = ServiceType;
}

QThread *IServiceWindow::GetListThread() const
{
	return m_pListThread;
}

void IServiceWindow::AddWidgetItem()
{
	ui->gridLayoutTop->addWidget(m_pInfoBrowser);
	ui->gridLayoutBottom->addWidget(m_pListBrowser);

//	m_pInfoBrowser->GetFormPlay()->ShowMenu();
//	m_pInfoBrowser->GetFormSort()->ShowResize();

	m_pInfoBrowser->SetCoverArt(UtilNovatron::GetCoverArtIcon(SIDEMENU_ISERVICE, m_ServiceType));

}

bool IServiceWindow::GetGenre() const
{
	return m_bGenre;
}

void IServiceWindow::SetGenre(bool bGenre)
{
	m_bGenre = bGenre;
}

void IServiceWindow::SlotAddWidget(QWidget *widget, QString title)
{
	emit SigAddWidget(widget, title);		// recursive
}

void IServiceWindow::SlotRespAirableLogout()
{
	emit SigRespLogout();			// recursive
}

//void IServiceWindow::SlotResize()
//{
//	LogDebug("click resize");
//}

void IServiceWindow::SlotRespError(QString errMsg)
{
	CommonDialog dialog(this, STR_WARNING, errMsg);
	dialog.exec();
}

void IServiceWindow::SlotSelectIconTitle(int nType)
{
	if (iIServiceType_Qobuz == nType)
	{
		UtilNovatron::DebugTypeForQobuz("SlotSelectIconTitle", nType);
		m_pQobuzMgr->RequestLogin();
	}
	else if (iIServiceType_Radios <= nType && nType <= iIServiceType_AmazonMusic)
	{
		UtilNovatron::DebugTypeForIService("SlotSelectIconTitle", nType);
		m_pAirableMgr->RequestAuth(nType);
	}
}

void IServiceWindow::SlotSelectTitle(int nType, CJsonNode node)
{
	if (iIServiceType_Qobuz == m_ServiceType)
	{
		if (iQobuzType_Mask_Search & nType)
		{
			SelectSearchForQobuz(nType, node);
		}
		else if (iQobuzType_Mask_Recommend & nType)
		{
			SelectRecommendForQobuz(nType, node);
		}
		else if (iQobuzType_Mask_Favorite & nType)
		{
			SelectFavoriteForQobuz(nType, node);
		}
		else if ((iQobuzType_Mask_UserPlaylist & nType))
		{
			SelectUserPlaylistForQobuz(nType, node);
		}
	}
	else
	{
		SelectTitleForAirable(nType, node);
	}
}

//void IServiceWindow::SlotSelectURL(QString rawData)
//{
//	CJsonNode node;
//	if (!node.SetContent(rawData))
//	{
//		SlotRespError(STR_INVALID_JSON);
//		return;
//	}

//	LogDebug("node [%s]", node.ToTabedByteArray().data());

//	if (m_ServiceType == iIServiceType_Qobuz || m_ServiceType == -1)
//	{
//		int nType = node.GetInt(KEY_TYPE);
//		UtilNovatron::DebugTypeForAirable("SlotSelectURL", nType);

//		QString strID = node.GetString(KEY_ID_UPPER);

//		if (nType & iQobuzType_Mask_Track)
//		{
//			m_pQobuzMgr->RequestPlay(node);

//		}
//		else if (m_bGenreSubmenu)
//		{
//			LogDebug("fixed genre submenu display...");
//			IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
//			emit SigAddWidget(widget, STR_ISERVICE);
//			widget->DoRecommendGenre(nType, strID);
//		}
//		else if (nType & iQobuzType_Mask_Album)
//		{
//			IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
//			emit SigAddWidget(widget, STR_ISERVICE);
//			widget->RequestQobuzCategory(nType, strID, QOBUZ_START, QOBUZ_COUNT);
//		}
//		else if (nType & iQobuzType_Mask_Menu_Genre)
//		{
//			int nItemType = node.GetInt(KEY_ITEM_TYPE);
//			if (nItemType & iQobuzType_Mask_Album)
//			{
//				QString strGenreID = node.GetString(KEY_GENRE_ID);

//				IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
//				emit SigAddWidget(widget, STR_ISERVICE);
//				widget->RequestQobuzRecommend(nType, strID, QOBUZ_START, QOBUZ_COUNT, strGenreID);
//			}
//			else
//			{
//				IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
//				emit SigAddWidget(widget, STR_ISERVICE);
//				widget->RequestQobuzGenre(strID);
//			}
//		}
//		else
//		{
//			LogDebug("not supported");
//		}
//	}
//	else
//	{
//		int nType = node.GetInt(KEY_TYPE);
//		QString url = node.GetString(KEY_URL);

//		//LogDebug("click url [%d] [%s]", nType, url.toUtf8().data());

//		if (nType & iAirableType_Mask_Dir)
//		{
//	//		emit SigSelectURL(m_ServiceType, url);
//			IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
//			emit SigAddWidget(widget, STR_ISERVICE);
//			widget->RequestIServiceURL(m_ServiceType, url);
//		}
//		else if (nType & iAirableType_Mask_Play)
//		{
//			m_pAirableMgr->RequestPlay(m_ServiceType, node);
//		}
//		else if (nType & iAirableType_Mask_Logout)
//		{
//			m_pAirableMgr->RequestLogout(m_ServiceType, url);
//		}
//	}
//}

void IServiceWindow::SlotReqCoverArt(QString url, int index)
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
	DoQobuzHome();
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

void IServiceWindow::SlotRespQobuzList(QList<CJsonNode> list, bool genre)
{
	IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
	emit SigAddWidget(widget, STR_ISERVICE);
	widget->SetServiceType(iIServiceType_Qobuz);
	widget->AddWidgetItem();

	widget->GetInfoBrowser()->SetSubtitle(QOBUZ_TITLE);

	widget->GetListBrowser()->ClearNodeList();
	widget->GetListBrowser()->SetNodeList(list, -1);

	widget->SetGenre(genre);
}

void IServiceWindow::SlotRespAuth(int nServiceType)
{
	RequestIServiceURL(nServiceType);
}

void IServiceWindow::SlotRespURL(int nServiceType, QString title, QList<CJsonNode> list)
{
	IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
	emit SigAddWidget(widget, STR_ISERVICE);
	widget->SetServiceType(nServiceType);
	widget->AddWidgetItem();

	widget->GetInfoBrowser()->SetSubtitle(title);

	widget->GetListBrowser()->SetNodeInfo(m_Node);
	widget->GetListBrowser()->SetBackgroundTask(widget->GetListThread());
	widget->GetListBrowser()->ClearNodeList();
	widget->GetListBrowser()->SetNodeList(list, SIDEMENU_ISERVICE);
	widget->ThreadStartList();
}

void IServiceWindow::SlotRespForm(int nServiceType, CJsonNode node)
{
	SearchDialog dialog;
	if (dialog.exec() == QDialog::Accepted)
	{
		QString keyword = dialog.GetKeyword();
		QString url = node.GetString(KEY_URL);

		url = url + "?q=" + keyword;

		RequestIServiceURL(nServiceType, url);
	}
}

void IServiceWindow::SlotCoverArtUpdate(QString fileName, int nIndex, int mode)
{
	Q_UNUSED(mode);

	QStandardItem *item = m_pListBrowser->GetModel()->item(nIndex);
	item->setData(fileName, ListBrowserDelegate::LIST_BROWSER_COVER);
	m_pListBrowser->GetModel()->setItem(nIndex, item);

}

void IServiceWindow::SlotOptionMenu(int index, int type, QString menuName)
{
	SetOptionMenu(type, menuName);

	m_pListBrowser->SetEditor(index);
}

void IServiceWindow::SlotOptionMenuAction(QString url, int type, int menuID)
{
	LogDebug("click option menu [%s] [%d] [%d]", url.toUtf8().data(), type, menuID);
	switch (menuID) {
	case OPTION_MENU_ADD_FAVORITE:
	case OPTION_MENU_DELETE_FAVORITE:
		m_pAirableMgr->RequestActionUrl(m_ServiceType, url);
		break;
	}
}

void IServiceWindow::ConnectSigToSlot()
{
	connect(this, SIGNAL(SigAddWidget(QWidget*, QString)), parent(), SLOT(SlotAddWidget(QWidget*, QString)));		// recursive

	connect(this, SIGNAL(SigRespLogout()), parent(), SLOT(SlotRespAirableLogout()));

//	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectPlay(int)), this, SLOT(SlotSelectPlay(int)));
	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int)), this, SLOT(SlotSelectIconTitle(int)));
//	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));

	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigSelectPlay(int, CJsonNode)), this, SLOT(SlotSelectTitle(int, CJsonNode)));
	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigSelectTitle(int, CJsonNode)), this, SLOT(SlotSelectTitle(int, CJsonNode)));
	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigMenu(int, int, QString)), this, SLOT(SlotOptionMenu(int, int, QString)));
	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigMenuAction(QString, int, int)), this, SLOT(SlotOptionMenuAction(QString, int, int)));
	connect(m_pListBrowser, SIGNAL(SigReqCoverArt(QString, int)), this, SLOT(SlotReqCoverArt(QString, int)));

//	connect(m_pListService->GetDelegate(), SIGNAL(SigSelectCoverArt(QString)), this, SLOT(SlotSelectURL(QString)));
//	connect(m_pListService->GetDelegate(), SIGNAL(SigSelectTitle(QString)), this, SLOT(SlotSelectURL(QString)));
//	connect(m_pListService, SIGNAL(SigReqArt(QString, int)), this, SLOT(SlotReqArt(QString, int)));

	connect(m_pQobuzMgr, SIGNAL(SigRespLoginFail(CJsonNode)), this, SLOT(SlotRespQobuzLoginFail(CJsonNode)));
	connect(m_pQobuzMgr, SIGNAL(SigRespLoginSuccess()), this, SLOT(SlotRespQobuzLoginSuccess()));
	connect(m_pQobuzMgr, SIGNAL(SigRespList(QList<CJsonNode>, bool)), this, SLOT(SlotRespQobuzList(QList<CJsonNode>, bool)));


	connect(m_pAirableMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pAirableMgr, SIGNAL(SigRespLogout()), parent(), SLOT(SlotRespAirableLogout()));
	connect(m_pAirableMgr, SIGNAL(SigRespLoginFail(CJsonNode)), this, SLOT(SlotRespAirableLoginFail(CJsonNode)));
	connect(m_pAirableMgr, SIGNAL(SigRespLoginSuccess(int, bool)), this, SLOT(SlotRespAirableLoginSuccess(int, bool)));

	connect(m_pAirableMgr, SIGNAL(SigRespAuth(int)), this, SLOT(SlotRespAuth(int)));
	connect(m_pAirableMgr, SIGNAL(SigRespURL(int, QString, QList<CJsonNode>)), this, SLOT(SlotRespURL(int, QString, QList<CJsonNode>)));
	connect(m_pAirableMgr, SIGNAL(SigRespForm(int, CJsonNode)), this, SLOT(SlotRespForm(int, CJsonNode)));
	connect(m_pAirableMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));

//	connect(m_pInfoService->GetFormSort(), SIGNAL(SigResize()), this, SLOT(SlotResize()));

}

void IServiceWindow::Initialize()
{
//	m_pInfoService->GetFormSort()->ShowResize();

	m_OptionMenuMap.clear();
}

void IServiceWindow::SelectTitleForQobuz(int nType, CJsonNode node)
{
	UtilNovatron::DebugTypeForQobuz("SelectTitleForQobuz", nType);

	m_Node.Clear();
	m_Node = node;

	LogDebug("node [%s]", m_Node.ToCompactByteArray().data());

	if (iQobuzType_Mask_Search == nType)
	{
		DoQobuzSearch();
	}
	else if (iQobuzType_Mask_Recommend == nType)
	{
		DoQobuzRecommend();
	}
	else if (iQobuzType_Mask_Favorite == nType)
	{
		DoQobuzFavorite();
	}
	else if (iQobuzType_Mask_UserPlaylist == nType)
	{
		RequestQobuzPlaylist(QOBUZ_START, QOBUZ_COUNT);
	}

}

void IServiceWindow::SelectTitleForQobuzSubMenu(int nType, CJsonNode node)
{
	UtilNovatron::DebugTypeForQobuz("SelectTitleForQobuzSubMenu", nType);

	m_Node.Clear();
	m_Node = node;

	LogDebug("node [%s]", m_Node.ToCompactByteArray().data());

	if ((iQobuzType_Mask_Search | iQobuzType_Mask_Artist) == nType
		  || (iQobuzType_Mask_Search | iQobuzType_Mask_Album) == nType
		  || (iQobuzType_Mask_Search | iQobuzType_Mask_Track) == nType
		  || (iQobuzType_Mask_Search | iQobuzType_Mask_Playlist) == nType)
	 {
	   LogDebug("search ");
//	   SearchDialog dialog;
//	   if (dialog.exec() == QDialog::Accepted)
//	   {
//		 QString keyword = dialog.GetKeyword();
//		 IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
//		 emit SigAddWidget(widget, STR_ISERVICE);
//		 widget->RequestQobuzSearch(nType, keyword, QOBUZ_START, QOBUZ_COUNT);
//	   }
	}
	else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album) == nType)
	{
		LogDebug("recommend ");
//		IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
//		emit SigAddWidget(widget, STR_ISERVICE);
//		widget->DoRecommendAlbum();
	}
	else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist) == nType)
	{
		LogDebug("recommend ");
//		IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
//		emit SigAddWidget(widget, STR_ISERVICE);
//		widget->DoRecommendPlaylist();
	}
	else if ((iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist) == nType
			 || (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Album) == nType
			 || (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Track) == nType)
	{
		LogDebug("favorite ");
//		IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
//		emit SigAddWidget(widget, STR_ISERVICE);
//		widget->RequestQobuzFavorite(nType, QOBUZ_START, QOBUZ_COUNT);
	}
	else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre) == nType
			 || (iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist | iQobuzType_Mask_Playlist) == nType)
	{
		LogDebug("recommend ");
//		IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
//		emit SigAddWidget(widget, STR_ISERVICE);

//		QString strID = node.GetString(KEY_ID_UPPER);
//		if (!strID.compare(VAL_GENRE))
//		{
//			widget->RequestQobuzGenre();
//		}
//		else
//		{
//			widget->RequestQobuzRecommend(nType, strID, QOBUZ_START, QOBUZ_COUNT);
//		}
	}
}

void IServiceWindow::SelectSearchForQobuz(int nType, CJsonNode node)
{
	UtilNovatron::DebugTypeForQobuz("SelectSearchForQobuz", nType);

	m_Node.Clear();
	m_Node = node;

	LogDebug("node [%s]", m_Node.ToCompactByteArray().data());

	int itemType = node.GetInt(KEY_ITEM_TYPE);

	if (iQobuzType_Mask_Search == nType && itemType < 0)
	{
		DoQobuzSearch();
	}
	else if (iQobuzType_Mask_Search == nType
			 && ((iQobuzType_Mask_Search | iQobuzType_Mask_Artist) == itemType
				 || (iQobuzType_Mask_Search | iQobuzType_Mask_Album) == itemType
				 || (iQobuzType_Mask_Search | iQobuzType_Mask_Track) == itemType
				 || (iQobuzType_Mask_Search | iQobuzType_Mask_Playlist) == itemType))
	{
		SearchDialog dialog;
		if (dialog.exec() == QDialog::Accepted)
		{
			QString keyword = dialog.GetKeyword();

			RequestQobuzSearch(itemType, keyword, QOBUZ_START, QOBUZ_COUNT);
		}
	}
	else if ((iQobuzType_Mask_Search | iQobuzType_Mask_Artist) == nType
		  || (iQobuzType_Mask_Search | iQobuzType_Mask_Album) == nType
		  || (iQobuzType_Mask_Search | iQobuzType_Mask_Playlist) == nType)
	{
		QString strID = m_Node.GetString(KEY_ID_UPPER);
		RequestQobuzCategory(nType, strID, QOBUZ_START, QOBUZ_COUNT);
	}
	else if ((iQobuzType_Mask_Search | iQobuzType_Mask_Track) == nType)
	{
		QMap<int, CJsonNode> map;
		map.insert(0, m_Node);
		RequestQobuzPlay(map, PLAY_CLEAR);
	}

}

void IServiceWindow::SelectRecommendForQobuz(int nType, CJsonNode node)
{
	UtilNovatron::DebugTypeForQobuz("SelectRecommendForQobuz", nType);

	m_Node.Clear();
	m_Node = node;

	LogDebug("node [%s]", m_Node.ToCompactByteArray().data());

	if (iQobuzType_Mask_Recommend == nType)
	{
		DoQobuzRecommend();
	}
	else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album) == nType)
	{
		DoRecommendAlbum();
	}
	else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist) == nType)
	{
		DoRecommendPlaylist();
	}
	else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre) == nType)
	{
		QString strID = node.GetString(KEY_ID_UPPER);
		if (!strID.compare(VAL_GENRE))
		{
			RequestQobuzGenre();
		}
		else if (!strID.compare(VAL_ID_BEST_SELLERS)
				 || !strID.compare(VAL_ID_MOST_STREAMED)
				 || !strID.compare(VAL_ID_NEW_RELEASES)
				 || !strID.compare(VAL_ID_PRESS_AWARDS)
				 || !strID.compare(VAL_ID_EDITOR_PICKS)
				 || !strID.compare(VAL_ID_MOST_FEATURED))
		{
			QString strGenreID = m_Node.GetString(KEY_GENRE_ID);
			RequestQobuzRecommend(nType, strID, QOBUZ_START, QOBUZ_COUNT, strGenreID);
		}
		else
		{
			if (m_bGenre)
			{
				m_bGenre = false;
				DoRecommendGenre(nType, strID);
			}
			else
			{
				RequestQobuzGenre(strID);
			}
		}
	}
	else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Album) == nType
			 || (iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist | iQobuzType_Mask_Playlist) == nType)
	{
		QString strID = node.GetString(KEY_ID_UPPER);
		if (!strID.compare(VAL_ID_BEST_SELLERS)
				 || !strID.compare(VAL_ID_MOST_STREAMED)
				 || !strID.compare(VAL_ID_NEW_RELEASES)
				 || !strID.compare(VAL_ID_PRESS_AWARDS)
				 || !strID.compare(VAL_ID_EDITOR_PICKS)
				 || !strID.compare(VAL_ID_MOST_FEATURED)
				 || !strID.compare(VAL_ID_LAST_CREATED))
		{
			RequestQobuzRecommend(nType, strID, QOBUZ_START, QOBUZ_COUNT);
		}
		else
		{
			QString strID = m_Node.GetString(KEY_ID_UPPER);
			RequestQobuzCategory(nType, strID, QOBUZ_START, QOBUZ_COUNT);
		}
	}
	else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Track) == nType
			 || (iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Track) == nType
			 || (iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist | iQobuzType_Mask_Track) == nType)
	{
		QMap<int, CJsonNode> map;
		map.insert(0, m_Node);
		RequestQobuzPlay(map, PLAY_CLEAR);
	}
}

void IServiceWindow::SelectFavoriteForQobuz(int nType, CJsonNode node)
{
	UtilNovatron::DebugTypeForQobuz("SelectFavoriteForQobuz", nType);

	m_Node.Clear();
	m_Node = node;

	LogDebug("node [%s]", m_Node.ToCompactByteArray().data());

	if (iQobuzType_Mask_Favorite == nType)
	{
		DoQobuzFavorite();
	}
	else if ((iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist) == nType
			 || (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Album) == nType
			 || (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Track) == nType)
	{
		RequestQobuzFavorite(nType, QOBUZ_START, QOBUZ_COUNT);
	}
	else if ((iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist | iQobuzType_Mask_Artist) == nType
			 || (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist | iQobuzType_Mask_Album) == nType
			 || (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Album) == nType)
	{
		QString strID = m_Node.GetString(KEY_ID_UPPER);
		RequestQobuzCategory(nType, strID, QOBUZ_START, QOBUZ_COUNT);
	}
	else if ((iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist | iQobuzType_Mask_Track) == nType
			 || (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Track) == nType
			 || (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Track | iQobuzType_Mask_Track) == nType)
	{
		QMap<int, CJsonNode> map;
		map.insert(0, m_Node);
		RequestQobuzPlay(map, PLAY_CLEAR);
	}
}

void IServiceWindow::SelectUserPlaylistForQobuz(int nType, CJsonNode node)
{
	UtilNovatron::DebugTypeForQobuz("SelectUserPlaylistForQobuz", nType);

	m_Node.Clear();
	m_Node = node;

	LogDebug("node [%s]", m_Node.ToCompactByteArray().data());

	if (iQobuzType_Mask_UserPlaylist == nType)
	{
		RequestQobuzPlaylist(QOBUZ_START, QOBUZ_COUNT);
	}
	else if ((iQobuzType_Mask_UserPlaylist | iQobuzType_Mask_Playlist) == nType)
	{
		QString strID = m_Node.GetString(KEY_ID_UPPER);
		RequestQobuzCategory(nType, strID, QOBUZ_START, QOBUZ_COUNT);
	}
	else if ((iQobuzType_Mask_UserPlaylist | iQobuzType_Mask_Track) == nType)
	{
		QMap<int, CJsonNode> map;
		map.insert(0, m_Node);
		RequestQobuzPlay(map, PLAY_CLEAR);
	}
}

void IServiceWindow::SelectTitleForAirable(int nType, CJsonNode node)
{
	UtilNovatron::DebugTypeForAirable("SlotSelectTitle", nType);

	m_Node.Clear();
	m_Node = node;

	LogDebug("node [%s]", m_Node.ToCompactByteArray().data());

	QString url = m_Node.GetString(KEY_URL);

	if (nType & iAirableType_Mask_Dir)
	{
////		emit SigSelectURL(m_ServiceType, url);
//		IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
//		emit SigAddWidget(widget, STR_ISERVICE);
		RequestIServiceURL(m_ServiceType, url);
	}
	else if (nType & iAirableType_Mask_Track
			 || nType & iAirableType_Mask_Program
			 || nType & iAirableType_Mask_Radio
			 || nType & iAirableType_Mask_Feed
			 || nType & iAirableType_Mask_Play)
	{
		m_pAirableMgr->RequestPlay(m_ServiceType, m_Node);
	}
	else if (nType & iAirableType_Mask_Logout)
	{
		m_pAirableMgr->RequestLogout(m_ServiceType, url);
	}
}

void IServiceWindow::SetOptionMenu(int type, QString menuName)
{
	UtilNovatron::DebugTypeForAirable(menuName, type);
	m_OptionMenuMap.clear();

	if (type & iAirableType_Mask_Art)
	{
		if (menuName.toLower().contains("add"))
		{
			m_OptionMenuMap.insert(OPTION_MENU_ADD_FAVORITE, menuName);
		}
		else if (menuName.toLower().contains("remove"))
		{
			m_OptionMenuMap.insert(OPTION_MENU_DELETE_FAVORITE, menuName);
		}
	}

	if (m_OptionMenuMap.count() > 0)
	{
		m_pListBrowser->GetDelegate()->SetOptionMenuMap(m_OptionMenuMap);
	}

}


void IServiceWindow::DoQobuzHome()
{
	IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
	emit SigAddWidget(widget, STR_ISERVICE);
	widget->SetServiceType(iIServiceType_Qobuz);
	widget->AddWidgetItem();

	QList<CJsonNode> list;
	widget->SetQobuzHome(list);

	widget->GetInfoBrowser()->SetSubtitle(QOBUZ_TITLE);

	widget->GetListBrowser()->ClearNodeList();
	widget->GetListBrowser()->SetNodeList(list, -1);
}

void IServiceWindow::DoQobuzSearch()
{
	IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
	emit SigAddWidget(widget, STR_ISERVICE);
	widget->SetServiceType(iIServiceType_Qobuz);
	widget->AddWidgetItem();

	QList<CJsonNode> list;
	SetQobuzSearch(list);

	widget->GetInfoBrowser()->SetSubtitle(STR_SEARCH);

	widget->GetListBrowser()->ClearNodeList();
	widget->GetListBrowser()->SetNodeList(list, -1);
}

void IServiceWindow::DoQobuzRecommend()
{
	IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
	emit SigAddWidget(widget, STR_ISERVICE);
	widget->SetServiceType(iIServiceType_Qobuz);
	widget->AddWidgetItem();

	QList<CJsonNode> list;
	SetQobuzRecommend(list);

	widget->GetInfoBrowser()->SetSubtitle(STR_RECOMMEND);

	widget->GetListBrowser()->ClearNodeList();
	widget->GetListBrowser()->SetNodeList(list, -1);
}

void IServiceWindow::DoQobuzFavorite()
{
	IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
	emit SigAddWidget(widget, STR_ISERVICE);
	widget->SetServiceType(iIServiceType_Qobuz);
	widget->AddWidgetItem();

	QList<CJsonNode> list;
	SetQobuzFavorite(list);

	widget->GetInfoBrowser()->SetSubtitle(STR_FAVORITE);

	widget->GetListBrowser()->ClearNodeList();
	widget->GetListBrowser()->SetNodeList(list, -1);
}

void IServiceWindow::DoRecommendAlbum()
{
	IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
	emit SigAddWidget(widget, STR_ISERVICE);
	widget->SetServiceType(iIServiceType_Qobuz);
	widget->AddWidgetItem();

	QList<CJsonNode> list;
	SetRecommendAlbum(list);

	widget->GetInfoBrowser()->SetSubtitle(STR_ALBUM);

	widget->GetListBrowser()->ClearNodeList();
	widget->GetListBrowser()->SetNodeList(list, -1);
}

void IServiceWindow::DoRecommendPlaylist()
{
	IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
	emit SigAddWidget(widget, STR_ISERVICE);
	widget->SetServiceType(iIServiceType_Qobuz);
	widget->AddWidgetItem();

	QList<CJsonNode> list;
	SetRecommendPlaylist(list);

	widget->GetInfoBrowser()->SetSubtitle(STR_PLAYLIST);

	widget->GetListBrowser()->ClearNodeList();
	widget->GetListBrowser()->SetNodeList(list, -1);
}

void IServiceWindow::DoRecommendGenre(int nType, QString strID)
{
	IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
	emit SigAddWidget(widget, STR_ISERVICE);
	widget->SetServiceType(iIServiceType_Qobuz);
	widget->AddWidgetItem();

	QList<CJsonNode> list;
	SetRecommendGenre(list, strID);

	widget->GetInfoBrowser()->SetSubtitle(STR_GENRE);

	widget->GetListBrowser()->ClearNodeList();
	widget->GetListBrowser()->SetNodeList(list, -1);
}

void IServiceWindow::SetIServiceHome(QList<CJsonNode> &list)
{
	QList<CJsonNode> tempList;
	QString strCover = "";
	foreach (CJsonNode node, list)
	{
		strCover = UtilNovatron::GetCoverArtIcon(SIDEMENU_ISERVICE, node.GetInt(KEY_TYPE));
		node.Add(KEY_COVER_ART, strCover);
		tempList.append(node);
	}
	list = tempList;
}

void IServiceWindow::SetQobuzHome(QList<CJsonNode> &list)
{
	CJsonNode nodeSearch(JSON_OBJECT);
	nodeSearch.AddInt(KEY_TYPE, iQobuzType_Mask_Search);
	nodeSearch.AddString(KEY_COVER_ART, ":/resource/internets-icon40-search.png");
	nodeSearch.AddString(KEY_TOP, VAL_SEARCH);

	CJsonNode nodeRecommend(JSON_OBJECT);
	nodeRecommend.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend);
	nodeRecommend.AddString(KEY_COVER_ART, "");
	nodeRecommend.AddString(KEY_TOP, VAL_RECOMMEND);

	CJsonNode nodeFavorite(JSON_OBJECT);
	nodeFavorite.AddInt(KEY_TYPE, iQobuzType_Mask_Favorite);
	nodeFavorite.AddString(KEY_COVER_ART, "");
	nodeFavorite.AddString(KEY_TOP, VAL_FAVORITE);

	CJsonNode nodePlaylist(JSON_OBJECT);
	nodePlaylist.AddInt(KEY_TYPE, iQobuzType_Mask_UserPlaylist);
	nodePlaylist.AddString(KEY_COVER_ART, "");
	nodePlaylist.AddString(KEY_TOP, VAL_PLAYLIST);

	list.append(nodeSearch);
	list.append(nodeRecommend);
	list.append(nodeFavorite);
	list.append(nodePlaylist);

}

void IServiceWindow::SetQobuzSearch(QList<CJsonNode> &list)
{
	CJsonNode nodeArtist(JSON_OBJECT);
	nodeArtist.AddInt(KEY_TYPE, iQobuzType_Mask_Search);
	nodeArtist.Add(KEY_TOP, VAL_ARTIST);
	nodeArtist.Add(KEY_CMD1, "m_SearchArtist");
	nodeArtist.Add(KEY_COVER_ART, ":/resource/internets-icon40-search.png");
	nodeArtist.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeArtist.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Search | iQobuzType_Mask_Artist);
	nodeArtist.AddInt(KEY_MENU_DEPTH, 1);

	CJsonNode nodeAlbum(JSON_OBJECT);
	nodeAlbum.AddInt(KEY_TYPE, iQobuzType_Mask_Search);
	nodeAlbum.Add(KEY_TOP, VAL_ALBUM);
	nodeAlbum.Add(KEY_CMD1, "m_SearchAlbum");
	nodeAlbum.Add(KEY_COVER_ART, ":/resource/internets-icon40-search.png");
	nodeAlbum.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeAlbum.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Search | iQobuzType_Mask_Album);
	nodeAlbum.AddInt(KEY_MENU_DEPTH, 1);

	CJsonNode nodeTrack(JSON_OBJECT);
	nodeTrack.AddInt(KEY_TYPE, iQobuzType_Mask_Search);
	nodeTrack.Add(KEY_TOP, VAL_TRACK);
	nodeTrack.Add(KEY_CMD1, "m_SearchTrack");
	nodeTrack.Add(KEY_COVER_ART, ":/resource/internets-icon40-search.png");
	nodeTrack.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeTrack.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Search | iQobuzType_Mask_Track);
	nodeTrack.AddInt(KEY_MENU_DEPTH, 1);

	CJsonNode nodePlaylist(JSON_OBJECT);
	nodePlaylist.AddInt(KEY_TYPE, iQobuzType_Mask_Search);
	nodePlaylist.Add(KEY_TOP, VAL_PLAYLIST);
	nodePlaylist.Add(KEY_CMD1, "m_SearchPlaylist");
	nodePlaylist.Add(KEY_COVER_ART, ":/resource/internets-icon40-search.png");
	nodePlaylist.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodePlaylist.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Search | iQobuzType_Mask_Playlist);
	nodePlaylist.AddInt(KEY_MENU_DEPTH, 1);

	list.append(nodeArtist);
	list.append(nodeAlbum);
	list.append(nodeTrack);
	list.append(nodePlaylist);
}

void IServiceWindow::SetQobuzRecommend(QList<CJsonNode> &list)
{
	CJsonNode nodeAlbum(JSON_OBJECT);
	nodeAlbum.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album);
	nodeAlbum.Add(KEY_TOP, VAL_ALBUM);
	nodeAlbum.Add(KEY_CMD1, "m_RecommendAlbum");
	nodeAlbum.Add(KEY_COVER_ART, "");
	nodeAlbum.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeAlbum.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album);
	nodeAlbum.AddInt(KEY_MENU_DEPTH, 1);

	CJsonNode nodePlaylist(JSON_OBJECT);
	nodePlaylist.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist);
	nodePlaylist.Add(KEY_TOP, VAL_PLAYLIST);
	nodePlaylist.Add(KEY_CMD1, "m_RecommendPlaylist");
	nodePlaylist.Add(KEY_COVER_ART, "");
	nodePlaylist.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodePlaylist.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist);
	nodePlaylist.AddInt(KEY_MENU_DEPTH, 1);

	list.append(nodeAlbum);
	list.append(nodePlaylist);
}

void IServiceWindow::SetQobuzFavorite(QList<CJsonNode> &list)
{
	// m_QobuzFavorite
	CJsonNode nodeArtist(JSON_OBJECT);
	nodeArtist.AddInt(KEY_TYPE, iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist);
	nodeArtist.Add(KEY_TOP, VAL_ARTIST);
	nodeArtist.Add(KEY_CMD1, "m_FavoriteArtist");
	nodeArtist.Add(KEY_COVER_ART, "");
	nodeArtist.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeArtist.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist);
	nodeArtist.AddInt(KEY_MENU_DEPTH, 1);

	CJsonNode nodeAlbum(JSON_OBJECT);
	nodeAlbum.AddInt(KEY_TYPE, iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Album);
	nodeAlbum.Add(KEY_TOP, VAL_ALBUM);
	nodeAlbum.Add(KEY_CMD1, "m_FavoriteAlbum");
	nodeAlbum.Add(KEY_COVER_ART, "");
	nodeAlbum.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeAlbum.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Album);
	nodeAlbum.AddInt(KEY_MENU_DEPTH, 1);

	CJsonNode nodeTrack(JSON_OBJECT);
	nodeTrack.AddInt(KEY_TYPE, iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Track);
	nodeTrack.Add(KEY_TOP, VAL_TRACK);
	nodeTrack.Add(KEY_CMD1, "m_FavoriteTrack");
	nodeTrack.Add(KEY_COVER_ART, "");
	nodeTrack.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeTrack.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Track);
	nodeTrack.AddInt(KEY_MENU_DEPTH, 1);

	list.append(nodeArtist);
	list.append(nodeAlbum);
	list.append(nodeTrack);

}

void IServiceWindow::SetRecommendAlbum(QList<CJsonNode> &list)
{
	CJsonNode nodeGenre(JSON_OBJECT);
	nodeGenre.Add(KEY_ID_UPPER, VAL_GENRE);
	nodeGenre.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeGenre.Add(KEY_COVER_ART, "");
	nodeGenre.Add(KEY_TOP, VAL_GENRE);
	nodeGenre.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeGenre.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeGenre.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodeBestSellers(JSON_OBJECT);
	nodeBestSellers.Add(KEY_ID_UPPER, VAL_ID_BEST_SELLERS);
	nodeBestSellers.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeBestSellers.Add(KEY_COVER_ART, "");
	nodeBestSellers.Add(KEY_TOP, VAL_BEST_SELLERS);
	nodeBestSellers.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeBestSellers.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeBestSellers.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodeMostStreamd(JSON_OBJECT);
	nodeMostStreamd.Add(KEY_ID_UPPER, VAL_ID_MOST_STREAMED);
	nodeMostStreamd.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeMostStreamd.Add(KEY_COVER_ART, "");
	nodeMostStreamd.Add(KEY_TOP, VAL_MOST_STREAMED);
	nodeMostStreamd.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeMostStreamd.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeMostStreamd.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodeNewReleases(JSON_OBJECT);
	nodeNewReleases.Add(KEY_ID_UPPER, VAL_ID_NEW_RELEASES);
	nodeNewReleases.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeNewReleases.Add(KEY_COVER_ART, "");
	nodeNewReleases.Add(KEY_TOP, VAL_NEW_RELEASES);
	nodeNewReleases.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeNewReleases.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeNewReleases.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodePressAward(JSON_OBJECT);
	nodePressAward.Add(KEY_ID_UPPER, VAL_ID_PRESS_AWARDS);
	nodePressAward.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodePressAward.Add(KEY_COVER_ART, "");
	nodePressAward.Add(KEY_TOP, VAL_PRESS_AWARDS);
	nodePressAward.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodePressAward.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodePressAward.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodeEditorPicks(JSON_OBJECT);
	nodeEditorPicks.Add(KEY_ID_UPPER, VAL_ID_EDITOR_PICKS);
	nodeEditorPicks.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeEditorPicks.Add(KEY_COVER_ART, "");
	nodeEditorPicks.Add(KEY_TOP, VAL_EDITOR_PICKS);
	nodeEditorPicks.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeEditorPicks.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeEditorPicks.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodeMostFeatured(JSON_OBJECT);
	nodeMostFeatured.Add(KEY_ID_UPPER, VAL_ID_MOST_FEATURED);
	nodeMostFeatured.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeMostFeatured.Add(KEY_COVER_ART, "");
	nodeMostFeatured.Add(KEY_TOP, VAL_MOST_FEATURED);
	nodeMostFeatured.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeMostFeatured.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeMostFeatured.AddInt(KEY_MENU_DEPTH, 2);

	list.append(nodeGenre);
	list.append(nodeBestSellers);
	list.append(nodeMostStreamd);
	list.append(nodeNewReleases);
	list.append(nodePressAward);
	list.append(nodeEditorPicks);
	list.append(nodeMostFeatured);
}

void IServiceWindow::SetRecommendPlaylist(QList<CJsonNode> &list)
{
	CJsonNode nodeLastCreated(JSON_OBJECT);
	nodeLastCreated.Add(KEY_ID_UPPER, VAL_ID_LAST_CREATED);
	nodeLastCreated.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist | iQobuzType_Mask_Playlist);
	nodeLastCreated.Add(KEY_COVER_ART, "");
	nodeLastCreated.Add(KEY_TOP, VAL_LAST_CREATED);
	nodeLastCreated.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeLastCreated.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist | iQobuzType_Mask_Playlist);
	nodeLastCreated.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodeEditorPicks(JSON_OBJECT);
	nodeEditorPicks.Add(KEY_ID_UPPER, VAL_ID_EDITOR_PICKS);
	nodeEditorPicks.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist | iQobuzType_Mask_Playlist);
	nodeEditorPicks.Add(KEY_COVER_ART, "");
	nodeEditorPicks.Add(KEY_TOP, VAL_EDITOR_PICKS);
	nodeEditorPicks.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeEditorPicks.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist | iQobuzType_Mask_Playlist);
	nodeEditorPicks.AddInt(KEY_MENU_DEPTH, 2);

	list.append(nodeLastCreated);
	list.append(nodeEditorPicks);
}

void IServiceWindow::SetRecommendGenre(QList<CJsonNode> &list, QString strID)
{
	CJsonNode nodeBestSellers(JSON_OBJECT);
	nodeBestSellers.Add(KEY_ID_UPPER, VAL_ID_BEST_SELLERS);
	nodeBestSellers.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeBestSellers.Add(KEY_COVER_ART, "");
	nodeBestSellers.Add(KEY_TOP, VAL_BEST_SELLERS);
	nodeBestSellers.Add(KEY_GENRE_ID, strID);
	nodeBestSellers.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeBestSellers.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Album);
	nodeBestSellers.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodeMostStreamd(JSON_OBJECT);
	nodeMostStreamd.Add(KEY_ID_UPPER, VAL_ID_MOST_STREAMED);
	nodeMostStreamd.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeMostStreamd.Add(KEY_COVER_ART, "");
	nodeMostStreamd.Add(KEY_TOP, VAL_MOST_STREAMED);
	nodeMostStreamd.Add(KEY_GENRE_ID, strID);
	nodeMostStreamd.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeMostStreamd.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Album);
	nodeMostStreamd.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodeNewReleases(JSON_OBJECT);
	nodeNewReleases.Add(KEY_ID_UPPER, VAL_ID_NEW_RELEASES);
	nodeNewReleases.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeNewReleases.Add(KEY_COVER_ART, "");
	nodeNewReleases.Add(KEY_TOP, VAL_NEW_RELEASES);
	nodeNewReleases.Add(KEY_GENRE_ID, strID);
	nodeNewReleases.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeNewReleases.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Album);
	nodeNewReleases.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodePressAward(JSON_OBJECT);
	nodePressAward.Add(KEY_ID_UPPER, VAL_ID_PRESS_AWARDS);
	nodePressAward.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodePressAward.Add(KEY_COVER_ART, "");
	nodePressAward.Add(KEY_TOP, VAL_PRESS_AWARDS);
	nodePressAward.Add(KEY_GENRE_ID, strID);
	nodePressAward.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodePressAward.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Album);
	nodePressAward.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodeEditorPicks(JSON_OBJECT);
	nodeEditorPicks.Add(KEY_ID_UPPER, VAL_ID_EDITOR_PICKS);
	nodeEditorPicks.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeEditorPicks.Add(KEY_COVER_ART, "");
	nodeEditorPicks.Add(KEY_TOP, VAL_EDITOR_PICKS);
	nodeEditorPicks.Add(KEY_GENRE_ID, strID);
	nodeEditorPicks.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeEditorPicks.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Album);
	nodeEditorPicks.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodeMostFeatured(JSON_OBJECT);
	nodeMostFeatured.Add(KEY_ID_UPPER, VAL_ID_MOST_FEATURED);
	nodeMostFeatured.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeMostFeatured.Add(KEY_COVER_ART, "");
	nodeMostFeatured.Add(KEY_TOP, VAL_MOST_FEATURED);
	nodeMostFeatured.Add(KEY_GENRE_ID, strID);
	nodeMostFeatured.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeMostFeatured.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Album);
	nodeMostFeatured.AddInt(KEY_MENU_DEPTH, 2);

	list.append(nodeBestSellers);
	list.append(nodeMostStreamd);
	list.append(nodeNewReleases);
	list.append(nodePressAward);
	list.append(nodeEditorPicks);
	list.append(nodeMostFeatured);
}

void IServiceWindow::ThreadStartList()
{
	ThreadTerminateList();

	m_pListThread->start();
}

void IServiceWindow::ThreadTerminateList()
{
	if (m_pListThread->isRunning())
	{
		m_pListThread->terminate();
	}
}
