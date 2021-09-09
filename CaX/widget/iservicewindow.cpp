#include <QtWebEngineWidgets/QWebEngineView>

#include "iservicewindow.h"
#include "ui_iservicewindow.h"

#include "manager/airablemanager.h"
#include "manager/qobuzmanager.h"

#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"
#include "util/airable.h"
#include "util/qobuz.h"

#include "base/menuinfo.h"
#include "base/menuicon.h"
#include "base/menuicondelegate.h"
#include "base/menulist.h"
#include "base/menulistdelegate.h"

#include "dialog/logindialog.h"
#include "dialog/webengineviewdialog.h"
#include "dialog/searchdialog.h"

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

	m_pMenuInfo->SetSubmenuIService();

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

void IServiceWindow::IServiceHome(QList<CJsonNode> list)
{
	ui->gridLayoutTop->addWidget(m_pMenuInfo);
	ui->gridLayoutBottom->addWidget(m_pMenuIcon);

	SetIServiceHome(list);

	m_pMenuInfo->SetTitle(ISERVICE_TITLE);
	m_pMenuIcon->SetNodeList(list, MenuIcon::MENU_ISERVICE);
}

void IServiceWindow::RequestIServiceURL(int nServiceType, QString url)
{
	ui->gridLayoutTop->addWidget(m_pMenuInfo);
	ui->gridLayoutBottom->addWidget(m_pMenuList);

	m_pAirableMgr->RequestURL(nServiceType, url);
}

void IServiceWindow::RequestQobuzSearch(int nType, QString keyword, int nStart, int nCount)
{
	ui->gridLayoutTop->addWidget(m_pMenuInfo);
	ui->gridLayoutBottom->addWidget(m_pMenuList);

	m_pQobuzMgr->RequestSearch(nType, keyword, nStart, nCount);
}

void IServiceWindow::RequestQobuzGenre()
{
	ui->gridLayoutTop->addWidget(m_pMenuInfo);
	ui->gridLayoutBottom->addWidget(m_pMenuList);

	m_pQobuzMgr->RequestGenre();
}

void IServiceWindow::RequestQobuzRecommend(int nType, QString strID, int nStart, int nCount)
{
	ui->gridLayoutTop->addWidget(m_pMenuInfo);
	ui->gridLayoutBottom->addWidget(m_pMenuList);

	m_pQobuzMgr->RequestRecommend(nType, strID, nStart, nCount);
}

void IServiceWindow::RequestQobuzFavorite(int nType, int nStart, int nCount)
{
	ui->gridLayoutTop->addWidget(m_pMenuInfo);
	ui->gridLayoutBottom->addWidget(m_pMenuList);

	m_pQobuzMgr->RequestFavorite(nType, nStart, nCount);
}

void IServiceWindow::RequestQobuzPlaylist(int nStart, int nCount)
{
	ui->gridLayoutTop->addWidget(m_pMenuInfo);
	ui->gridLayoutBottom->addWidget(m_pMenuList);

	m_pQobuzMgr->RequestPlaylist(nStart, nCount);
}

void IServiceWindow::RequestQobuzCategory(int nType, QString strID, int nStart, int nCount)
{
	ui->gridLayoutTop->addWidget(m_pMenuInfo);
	ui->gridLayoutBottom->addWidget(m_pMenuList);

	m_pQobuzMgr->RequestCategory(nType, strID, nStart, nCount);
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

void IServiceWindow::SlotSubmenu(int index)
{
	LogDebug("click submenu");
}

void IServiceWindow::SlotSort()
{
	LogDebug("click sort");
}

void IServiceWindow::SlotSelectCoverArt(int nType)
{
	if (iIServiceType_Qobuz == nType)
	{
		m_pQobuzMgr->RequestLogin();
	}
	else if (iIServiceType_Radios <= nType && nType <= iIServiceType_AmazonMusic)
	{
		m_pAirableMgr->RequestAuth(nType);
	}
	else if (iQobuzType_Mask_Search == nType)
	{
		IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
		emit SigAddWidget(widget);
		widget->DoQobuzSearch();
	}
	else if (iQobuzType_Mask_Recommend == nType)
	{
		IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
		emit SigAddWidget(widget);
		widget->DoQobuzRecommend();
	}
	else if (iQobuzType_Mask_Favorite == nType)
	{
		IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
		emit SigAddWidget(widget);
		widget->DoQobuzFavorite();
	}
	else if (iQobuzType_Mask_UserPlaylist == nType)
	{
		IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
		emit SigAddWidget(widget);
		widget->RequestQobuzPlaylist(0, 50);
	}
}

void IServiceWindow::SlotSelectCoverArt(int nType, QString rawData)
{
	if ((iQobuzType_Mask_Search | iQobuzType_Mask_Artist) == nType
			 || (iQobuzType_Mask_Search | iQobuzType_Mask_Album) == nType
			 || (iQobuzType_Mask_Search | iQobuzType_Mask_Track) == nType
			 || (iQobuzType_Mask_Search | iQobuzType_Mask_Playlist) == nType)
	{
		LogDebug("search ");
		SearchDialog dialog;
		if (dialog.exec() == QDialog::Accepted)
		{
			QString keyword = dialog.GetKeyword();
			IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
			emit SigAddWidget(widget);
			widget->RequestQobuzSearch(nType, keyword, 0, 50);
		}

	}
	else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album) == nType)
	{
		IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
		emit SigAddWidget(widget);
		widget->DoRecommendAlbum();
	}
	else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist) == nType)
	{
		IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
		emit SigAddWidget(widget);
		widget->DoRecommendPlaylist();

	}
	else if ((iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist) == nType
			|| (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Album) == nType
			|| (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Track) == nType)
	{
		LogDebug("favorite ");
		IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
		emit SigAddWidget(widget);
		widget->RequestQobuzFavorite(nType, 0, 50);

	}
	else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre) == nType
			 || (iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist | iQobuzType_Mask_Playlist) == nType)
	{
		CJsonNode node;
		if (!node.SetContent(rawData))
		{
			LogCritical("invalid json");
			return;
		}

		IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
		emit SigAddWidget(widget);

		QString strID = node.GetString(KEY_ID_UPPER);
		if (!strID.compare("Genre"))
		{
			widget->RequestQobuzGenre();
		}
		else
		{
			widget->RequestQobuzRecommend(nType, strID, 0, 50);
		}
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

	if (m_ServiceType == iIServiceType_Qobuz || m_ServiceType == -1)
	{
		int nType = node.GetInt(KEY_TYPE);
		QString strID = node.GetString(KEY_ID_UPPER);

		if (nType & iQobuzType_Mask_Track)
		{
			LogDebug("click qobuz track");
			m_pQobuzMgr->RequestPlay(node);

		}
		else
		{
			IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
			emit SigAddWidget(widget);
			widget->RequestQobuzCategory(nType, strID, 0, 50);
		}
	}
	else
	{
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
	widget->DoQobuzHome();
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

void IServiceWindow::SlotRespList(QList<CJsonNode> list)
{
	m_pMenuList->SetNodeList(list);

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
	connect(m_pMenuInfo, SIGNAL(SigSubmenu(int)), this, SLOT(SlotSubmenu(int)));
	connect(m_pMenuInfo, SIGNAL(SigSort()), this, SLOT(SlotSort()));

	connect(m_pMenuIcon->GetDelegate(), SIGNAL(SigSelectCoverArt(int)), this, SLOT(SlotSelectCoverArt(int)));
	connect(m_pMenuIcon->GetDelegate(), SIGNAL(SigSelectCoverArt(int, QString)), this, SLOT(SlotSelectCoverArt(int, QString)));
	connect(m_pMenuList->GetDelegate(), SIGNAL(SigSelectIcon(QString)), this, SLOT(SlotSelectURL(QString)));
	connect(m_pMenuList->GetDelegate(), SIGNAL(SigSelectTitle(QString)), this, SLOT(SlotSelectURL(QString)));
	connect(m_pMenuList, SIGNAL(SigReqArt(QString, int)), this, SLOT(SlotReqArt(QString, int)));

	connect(m_pQobuzMgr, SIGNAL(SigRespLoginFail(CJsonNode)), this, SLOT(SlotRespQobuzLoginFail(CJsonNode)));
	connect(m_pQobuzMgr, SIGNAL(SigRespLoginSuccess()), this, SLOT(SlotRespQobuzLoginSuccess()));
	connect(m_pQobuzMgr, SIGNAL(SigRespList(QList<CJsonNode>)), this, SLOT(SlotRespList(QList<CJsonNode>)));


	connect(m_pAirableMgr, SIGNAL(SigRespLogout()), parent(), SLOT(SlotRespAirableLogout()));
	connect(m_pAirableMgr, SIGNAL(SigRespLoginFail(CJsonNode)), this, SLOT(SlotRespAirableLoginFail(CJsonNode)));
	connect(m_pAirableMgr, SIGNAL(SigRespLoginSuccess(int, bool)), this, SLOT(SlotRespAirableLoginSuccess(int, bool)));

	connect(m_pAirableMgr, SIGNAL(SigRespAuth(int)), this, SLOT(SlotRespAuth(int)));
	connect(m_pAirableMgr, SIGNAL(SigRespURL(int, QList<CJsonNode>)), this, SLOT(SlotRespURL(int, QList<CJsonNode>)));
	connect(m_pAirableMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));

}


void IServiceWindow::DoQobuzHome()
{
	ui->gridLayoutTop->addWidget(m_pMenuInfo);
	ui->gridLayoutBottom->addWidget(m_pMenuIcon);

	QList<CJsonNode> list;
	SetQobuzHome(list);

	m_pMenuInfo->SetTitle(QOBUZ_TITLE);
	m_pMenuIcon->SetNodeList(list, MenuIcon::MENU_ISERVICE);
}

void IServiceWindow::DoQobuzSearch()
{
	ui->gridLayoutTop->addWidget(m_pMenuInfo);
	ui->gridLayoutBottom->addWidget(m_pMenuIcon);

	QList<CJsonNode> list;
	SetQobuzSearch(list);

	m_pMenuInfo->SetTitle(QOBUZ_TITLE);
	m_pMenuIcon->SetNodeList(list, MenuIcon::MENU_ISERVICE);
}

void IServiceWindow::DoQobuzRecommend()
{
	ui->gridLayoutTop->addWidget(m_pMenuInfo);
	ui->gridLayoutBottom->addWidget(m_pMenuIcon);

	QList<CJsonNode> list;
	SetQobuzRecommend(list);

	m_pMenuInfo->SetTitle(QOBUZ_TITLE);
	m_pMenuIcon->SetNodeList(list, MenuIcon::MENU_ISERVICE);
}

void IServiceWindow::DoQobuzFavorite()
{
	ui->gridLayoutTop->addWidget(m_pMenuInfo);
	ui->gridLayoutBottom->addWidget(m_pMenuIcon);

	QList<CJsonNode> list;
	SetQobuzFavorite(list);

	m_pMenuInfo->SetTitle(QOBUZ_TITLE);
	m_pMenuIcon->SetNodeList(list, MenuIcon::MENU_ISERVICE);
}


void IServiceWindow::DoRecommendAlbum()
{
	ui->gridLayoutTop->addWidget(m_pMenuInfo);
	ui->gridLayoutBottom->addWidget(m_pMenuIcon);

	QList<CJsonNode> list;
	SetRecommendAlbum(list);

	m_pMenuInfo->SetTitle(QOBUZ_TITLE);
	m_pMenuIcon->SetNodeList(list, MenuIcon::MENU_ISERVICE);
}

void IServiceWindow::DoRecommendPlaylist()
{
	ui->gridLayoutTop->addWidget(m_pMenuInfo);
	ui->gridLayoutBottom->addWidget(m_pMenuIcon);

	QList<CJsonNode> list;
	SetRecommendPlaylist(list);

	m_pMenuInfo->SetTitle(QOBUZ_TITLE);
	m_pMenuIcon->SetNodeList(list, MenuIcon::MENU_ISERVICE);
}


void IServiceWindow::SetIServiceHome(QList<CJsonNode> &srclist)
{
	QList<CJsonNode> dstList;
	QString strCover = "";
	foreach (CJsonNode node, srclist)
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
	srclist = dstList;
}

void IServiceWindow::SetQobuzHome(QList<CJsonNode> &srclist)
{
	CJsonNode nodeSearch(JSON_OBJECT);
	nodeSearch.AddInt(KEY_TYPE, iQobuzType_Mask_Search);
	nodeSearch.AddString(KEY_COVER_ART, ":/resource/baseline_search_black_24dp.png");
	nodeSearch.AddString(KEY_NAME, "Search");

	CJsonNode nodeRecommend(JSON_OBJECT);
	nodeRecommend.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend);
	nodeRecommend.AddString(KEY_COVER_ART, ":/resource/baseline_search_black_24dp.png");
	nodeRecommend.AddString(KEY_NAME, "Recommend");

	CJsonNode nodeFavorite(JSON_OBJECT);
	nodeFavorite.AddInt(KEY_TYPE, iQobuzType_Mask_Favorite);
	nodeFavorite.AddString(KEY_COVER_ART, ":/resource/baseline_search_black_24dp.png");
	nodeFavorite.AddString(KEY_NAME, "Favorite");

	CJsonNode nodePlaylist(JSON_OBJECT);
	nodePlaylist.AddInt(KEY_TYPE, iQobuzType_Mask_UserPlaylist);
	nodePlaylist.AddString(KEY_COVER_ART, ":/resource/baseline_search_black_24dp.png");
	nodePlaylist.AddString(KEY_NAME, "Playlist");

	srclist.append(nodeSearch);
	srclist.append(nodeRecommend);
	srclist.append(nodeFavorite);
	srclist.append(nodePlaylist);

}

void IServiceWindow::SetQobuzSearch(QList<CJsonNode> &srclist)
{
	CJsonNode nodeArtist(JSON_OBJECT);
	nodeArtist.AddInt(KEY_TYPE, iQobuzType_Mask_Search | iQobuzType_Mask_Artist);
	nodeArtist.Add(KEY_NAME, "Artist");
	nodeArtist.Add(KEY_CMD1, "m_SearchArtist");
	nodeArtist.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
	nodeArtist.AddInt("QobuzMenu", iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeArtist.AddInt("ItemType", iQobuzType_Mask_Search | iQobuzType_Mask_Artist);
	nodeArtist.AddInt("MenuDepth", 1);

	CJsonNode nodeAlbum(JSON_OBJECT);
	nodeAlbum.AddInt(KEY_TYPE, iQobuzType_Mask_Search | iQobuzType_Mask_Album);
	nodeAlbum.Add(KEY_NAME, "Album");
	nodeAlbum.Add(KEY_CMD1, "m_SearchAlbum");
	nodeAlbum.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
	nodeAlbum.AddInt("QobuzMenu", iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeAlbum.AddInt("ItemType", iQobuzType_Mask_Search | iQobuzType_Mask_Album);
	nodeAlbum.AddInt("MenuDepth", 1);

	CJsonNode nodeTrack(JSON_OBJECT);
	nodeTrack.AddInt(KEY_TYPE, iQobuzType_Mask_Search | iQobuzType_Mask_Track);
	nodeTrack.Add(KEY_NAME, "Track");
	nodeTrack.Add(KEY_CMD1, "m_SearchTrack");
	nodeTrack.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
	nodeTrack.AddInt("QobuzMenu", iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeTrack.AddInt("ItemType", iQobuzType_Mask_Search | iQobuzType_Mask_Track);
	nodeTrack.AddInt("MenuDepth", 1);

	CJsonNode nodePlaylist(JSON_OBJECT);
	nodePlaylist.AddInt(KEY_TYPE, iQobuzType_Mask_Search | iQobuzType_Mask_Playlist);
	nodePlaylist.Add(KEY_NAME, "Playlist");
	nodePlaylist.Add(KEY_CMD1, "m_SearchPlaylist");
	nodePlaylist.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
	nodePlaylist.AddInt("QobuzMenu", iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodePlaylist.AddInt("ItemType", iQobuzType_Mask_Search | iQobuzType_Mask_Playlist);
	nodePlaylist.AddInt("MenuDepth", 1);

	srclist.append(nodeArtist);
	srclist.append(nodeAlbum);
	srclist.append(nodeTrack);
	srclist.append(nodePlaylist);
}

void IServiceWindow::SetQobuzRecommend(QList<CJsonNode> &srclist)
{
	CJsonNode nodeAlbum(JSON_OBJECT);
	nodeAlbum.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album);
	nodeAlbum.Add(KEY_NAME, "Album");
	nodeAlbum.Add(KEY_CMD1, "m_RecommendAlbum");
	nodeAlbum.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
	nodeAlbum.AddInt("QobuzMenu", iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeAlbum.AddInt("ItemType", iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album);
	nodeAlbum.AddInt("MenuDepth", 1);

	CJsonNode nodePlaylist(JSON_OBJECT);
	nodePlaylist.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist);
	nodePlaylist.Add(KEY_NAME, "Playlist");
	nodePlaylist.Add(KEY_CMD1, "m_RecommendPlaylist");
	nodePlaylist.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
	nodePlaylist.AddInt("QobuzMenu", iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodePlaylist.AddInt("ItemType", iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist);
	nodePlaylist.AddInt("MenuDepth", 1);

	srclist.append(nodeAlbum);
	srclist.append(nodePlaylist);
}

void IServiceWindow::SetQobuzFavorite(QList<CJsonNode> &srclist)
{
	// m_QobuzFavorite
	CJsonNode nodeArtist(JSON_OBJECT);
	nodeArtist.AddInt(KEY_TYPE, iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist);
	nodeArtist.Add(KEY_NAME, "Artist");
	nodeArtist.Add(KEY_CMD1, "m_FavoriteArtist");
	nodeArtist.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
	nodeArtist.AddInt("QobuzMenu", iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeArtist.AddInt("ItemType", iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist);
	nodeArtist.AddInt("MenuDepth", 1);

	CJsonNode nodeAlbum(JSON_OBJECT);
	nodeAlbum.AddInt(KEY_TYPE, iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Album);
	nodeAlbum.Add(KEY_NAME, "Album");
	nodeAlbum.Add(KEY_CMD1, "m_FavoriteAlbum");
	nodeAlbum.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
	nodeAlbum.AddInt("QobuzMenu", iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeAlbum.AddInt("ItemType", iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Album);
	nodeAlbum.AddInt("MenuDepth", 1);

	CJsonNode nodeTrack(JSON_OBJECT);
	nodeTrack.AddInt(KEY_TYPE, iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Track);
	nodeTrack.Add(KEY_NAME, "Track");
	nodeTrack.Add(KEY_CMD1, "m_FavoriteTrack");
	nodeTrack.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
	nodeTrack.AddInt("QobuzMenu", iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeTrack.AddInt("ItemType", iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Track);
	nodeTrack.AddInt("MenuDepth", 1);

	srclist.append(nodeArtist);
	srclist.append(nodeAlbum);
	srclist.append(nodeTrack);

}

void IServiceWindow::SetRecommendAlbum(QList<CJsonNode> &srclist)
{
	CJsonNode nodeGenre(JSON_OBJECT);
	nodeGenre.Add(KEY_ID_UPPER, "Genre");
	nodeGenre.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeGenre.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
	nodeGenre.Add(KEY_NAME, "Genre");
	nodeGenre.AddInt("QobuzMenu", iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeGenre.AddInt("ItemType", iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Album);
	nodeGenre.AddInt("MenuDepth", 2);

	CJsonNode nodeBestSellers(JSON_OBJECT);
	nodeBestSellers.Add(KEY_ID_UPPER, "best-sellers");
	nodeBestSellers.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeBestSellers.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
	nodeBestSellers.Add(KEY_NAME, "BestSellers");
	nodeBestSellers.AddInt("QobuzMenu", iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeBestSellers.AddInt("ItemType", iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Album);
	nodeBestSellers.AddInt("MenuDepth", 2);

	CJsonNode nodeMostStreamd(JSON_OBJECT);
	nodeMostStreamd.Add(KEY_ID_UPPER, "most-streamed");
	nodeMostStreamd.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeMostStreamd.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
	nodeMostStreamd.Add(KEY_NAME, "MostStreamd");
	nodeMostStreamd.AddInt("QobuzMenu", iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeMostStreamd.AddInt("ItemType", iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Album);
	nodeMostStreamd.AddInt("MenuDepth", 2);

	CJsonNode nodeNewReleases(JSON_OBJECT);
	nodeNewReleases.Add(KEY_ID_UPPER, "new-releases");
	nodeNewReleases.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeNewReleases.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
	nodeNewReleases.Add(KEY_NAME, "NewReleases");
	nodeNewReleases.AddInt("QobuzMenu", iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeNewReleases.AddInt("ItemType", iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Album);
	nodeNewReleases.AddInt("MenuDepth", 2);

	CJsonNode nodePressAward(JSON_OBJECT);
	nodePressAward.Add(KEY_ID_UPPER, "press-awards");
	nodePressAward.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodePressAward.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
	nodePressAward.Add(KEY_NAME, "PressAward");
	nodePressAward.AddInt("QobuzMenu", iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodePressAward.AddInt("ItemType", iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Album);
	nodePressAward.AddInt("MenuDepth", 2);

	CJsonNode nodeEditorPicks(JSON_OBJECT);
	nodeEditorPicks.Add(KEY_ID_UPPER, "editor-picks");
	nodeEditorPicks.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeEditorPicks.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
	nodeEditorPicks.Add(KEY_NAME, "EditorPicks");
	nodeEditorPicks.AddInt("QobuzMenu", iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeEditorPicks.AddInt("ItemType", iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Album);
	nodeEditorPicks.AddInt("MenuDepth", 2);

	CJsonNode nodeMostFeatured(JSON_OBJECT);
	nodeMostFeatured.Add(KEY_ID_UPPER, "most-featured");
	nodeMostFeatured.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeMostFeatured.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
	nodeMostFeatured.Add(KEY_NAME, "MostFeatured");
	nodeMostFeatured.AddInt("QobuzMenu", iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeMostFeatured.AddInt("ItemType", iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Album);
	nodeMostFeatured.AddInt("MenuDepth", 2);

	srclist.append(nodeGenre);
	srclist.append(nodeBestSellers);
	srclist.append(nodeMostStreamd);
	srclist.append(nodeNewReleases);
	srclist.append(nodePressAward);
	srclist.append(nodeEditorPicks);
	srclist.append(nodeMostFeatured);
}

void IServiceWindow::SetRecommendPlaylist(QList<CJsonNode> &srclist)
{
	CJsonNode nodeLastCreated(JSON_OBJECT);
	nodeLastCreated.Add(KEY_ID_UPPER, "last-created");
	nodeLastCreated.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist | iQobuzType_Mask_Playlist);
	nodeLastCreated.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
	nodeLastCreated.Add(KEY_NAME, "LastCreated");
	nodeLastCreated.AddInt("QobuzMenu", iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeLastCreated.AddInt("ItemType", iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist | iQobuzType_Mask_Playlist);
	nodeLastCreated.AddInt("MenuDepth", 2);

	CJsonNode nodeEditorPicks(JSON_OBJECT);
	nodeEditorPicks.Add(KEY_ID_UPPER, "editor-picks");
	nodeEditorPicks.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist | iQobuzType_Mask_Playlist);
	nodeEditorPicks.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
	nodeEditorPicks.Add(KEY_NAME, "EditorPicks");
	nodeEditorPicks.AddInt("QobuzMenu", iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeEditorPicks.AddInt("ItemType", iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist | iQobuzType_Mask_Playlist);
	nodeEditorPicks.AddInt("MenuDepth", 2);

	srclist.append(nodeLastCreated);
	srclist.append(nodeEditorPicks);
}
