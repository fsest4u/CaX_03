#include <QtWebEngineWidgets/QWebEngineView>

#include "iservicewindow.h"
#include "ui_iservicewindow.h"

#include "dialog/addqobuzplaylist.h"
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
	m_Menu(new QMenu(this)),
	m_WebURL(""),
	m_InternetType(-1),
	m_Type(-1),
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

	disconnect(m_Menu, SIGNAL(triggered(QAction*)));
	if (m_Menu)
	{
		delete m_Menu;
		m_Menu = nullptr;
	}

	delete ui;

}

void IServiceWindow::IServiceHome(QList<CJsonNode> list)
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pIconService);

	SetIServiceHome(list);

	m_pInfoService->SetTitle(STR_ISERVICE);
	m_pIconService->SetNodeList(list, IconService::ICON_SERVICE_ISERVICE);
}

void IServiceWindow::RequestIServiceURL(QString url)
{
//	LogDebug("call url [%s]", url.toUtf8().data());
	m_pAirableMgr->RequestURL(m_InternetType, url);
}

void IServiceWindow::RequestIServicePlay(QMap<int, CJsonNode> nodeMap, int nWhere)
{
	m_pAirableMgr->RequestPlay(m_InternetType, nodeMap, nWhere);
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

void IServiceWindow::AddWidgetItem(bool playAll, bool playRandom, bool menu)
{
	ui->gridLayoutTop->addWidget(m_pInfoBrowser);
	ui->gridLayoutBottom->addWidget(m_pListBrowser);

	m_pInfoBrowser->GetFormPlay()->ShowPlayAll(playAll);
	m_pInfoBrowser->GetFormPlay()->ShowMenu(menu);

	m_pInfoBrowser->SetCoverArt(UtilNovatron::GetCoverArtIcon(SIDEMENU_ISERVICE, m_InternetType));

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

int IServiceWindow::GetInternetType() const
{
	return m_InternetType;
}

void IServiceWindow::SetInternetType(int internetType)
{
	m_InternetType = internetType;
}

int IServiceWindow::GetType() const
{
	return m_Type;
}

void IServiceWindow::SetType(int Type)
{
	m_Type = Type;
}

int IServiceWindow::GetTypeParent() const
{
	return m_TypeParent;
}

void IServiceWindow::SetTypeParent(int TypeParent)
{
	m_TypeParent = TypeParent;
}

bool IServiceWindow::GetGenre() const
{
	return m_bGenre;
}

void IServiceWindow::SetGenre(bool bGenre)
{
	m_bGenre = bGenre;
}

QString IServiceWindow::GetPlaylistID() const
{
	return m_PlaylistID;
}

void IServiceWindow::SetPlaylistID(const QString &PlaylistID)
{
	m_PlaylistID = PlaylistID;
}

CJsonNode IServiceWindow::GetNode() const
{
	return m_Node;
}

void IServiceWindow::SetNode(const CJsonNode &Node)
{
	m_Node = Node;
//	LogDebug("node [%s]", m_Node.ToCompactByteArray().data());
}

void IServiceWindow::SlotAddWidget(QWidget *widget, QString title)
{
	emit SigAddWidget(widget, STR_ISERVICE);		// recursive
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
	if (iIServiceType_Qobuz == m_InternetType)
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

void IServiceWindow::SlotSelectMenu(const QModelIndex &modelIndex, QPoint point)
{
	m_ModelIndex = modelIndex;
	int type = qvariant_cast<int>(m_ModelIndex.data(ListBrowserDelegate::LIST_BROWSER_TYPE));
//	LogDebug("type [%d] x [%d] y [%d]", type, point.x(), point.y());

	if (iIServiceType_Qobuz == m_InternetType)
	{
		SetOptionMenu(type);
	}
	else
	{
		QString rawData = qvariant_cast<QString>(m_ModelIndex.data(ListBrowserDelegate::LIST_BROWSER_RAW));
		CJsonNode node;
		if (!node.SetContent(rawData))
		{
			LogCritical("invalid json");
			return;
		}
		CJsonNode acts = node.GetArray(KEY_ACTS);
		if (acts.ArraySize() <= 0)
		{
			LogCritical("array node is empty");
			return;
		}
		CJsonNode act = acts.GetArrayAt(0);
		QString name = act.GetString(KEY_NAME_CAP);
		if (name.isEmpty())
		{
			LogCritical("name is empty");
			return;
		}
		SetOptionMenu(type, name);
	}

	m_Menu->clear();

	QMap<int, QString>::iterator i;
	for (i = m_OptionMenuMap.begin(); i != m_OptionMenuMap.end(); i++)
	{
		QIcon icon = UtilNovatron::GetMenuIcon(i.value());
		QAction *action = new QAction(icon, i.value(), this);
		action->setData(i.key());
		m_Menu->addAction(action);
	}

	m_Menu->popup(m_pListBrowser->GetListView()->viewport()->mapToGlobal(point));
}

void IServiceWindow::SlotMenuAction(QAction *action)
{
	int type = qvariant_cast<int>(m_ModelIndex.data(ListBrowserDelegate::LIST_BROWSER_TYPE));

	if (iIServiceType_Qobuz == m_InternetType)
	{
		int id = qvariant_cast<int>(m_ModelIndex.data(ListBrowserDelegate::LIST_BROWSER_ID));
		SlotOptionMenuAction(QString::number(id), type, action->data().toInt());
	}
	else
	{
		QString rawData = qvariant_cast<QString>(m_ModelIndex.data(ListBrowserDelegate::LIST_BROWSER_RAW));
		CJsonNode node;
		if (!node.SetContent(rawData))
		{
			LogCritical("invalid json");
			return;
		}
		CJsonNode acts = node.GetArray(KEY_ACTS);
		if (acts.ArraySize() <= 0)
		{
			LogCritical("array node is empty");
			return;
		}

		CJsonNode act = acts.GetArrayAt(0);
		QString url = act.GetString(KEY_URL);
		if (url.isEmpty())
		{
			LogCritical("url is empty");
			return;
		}

		SlotOptionMenuAction(url, type, action->data().toInt());
	}


}

void IServiceWindow::SlotReqCoverArt(QString url, int index)
{
	m_pAirableMgr->RequestCoverArt(url, index, QListView::ListMode);
}

void IServiceWindow::SlotAppendList()
{
//	LogDebug("SlotAppendList node [%s]", m_Node.ToCompactByteArray().data());
	if (iIServiceType_Qobuz == m_InternetType)
	{
		if (iQobuzType_Mask_Search & m_TypeParent)
		{
			AppendSearchForQobuz();
		}
		else if (iQobuzType_Mask_Recommend & m_TypeParent)
		{
			AppendRecommendForQobuz();
		}
		else if (iQobuzType_Mask_Favorite & m_TypeParent)
		{
			AppendFavoriteForQobuz();
		}
		else if ((iQobuzType_Mask_UserPlaylist & m_TypeParent))
		{
			AppendUserPlaylistForQobuz();
		}
	}
	else
	{
		AppendTitleForAirable();
	}
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

void IServiceWindow::SlotRespQobuzList(QList<CJsonNode> list, bool genre, int start)
{
	m_StartCount = start;

	int nType =  list[0].GetInt(KEY_TYPE);
	UtilNovatron::DebugTypeForQobuz("SlotRespQobuzList", nType);

	if (m_Refresh)
	{
		m_Refresh = false;

		GetListBrowser()->ClearNodeList();
		nType = GetListBrowser()->SetNodeList(list, -1);
		SetType(nType);
		SetGenre(genre);
	}
	else
	{
		if (iQobuzType_Mask_Track & nType)
		{
			AddWidgetItem(true, true, true);
		}
		// search
		else if ((iQobuzType_Mask_Search | iQobuzType_Mask_Artist) == nType
				  || (iQobuzType_Mask_Search | iQobuzType_Mask_Album) == nType
				  || (iQobuzType_Mask_Search | iQobuzType_Mask_Playlist) == nType)
		{
			AddWidgetItem(false, false, true);
		}
		// recommend
		else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Album) == nType)
		{
			AddWidgetItem(false, false, true);
		}
		// favorite
		else if ((iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist | iQobuzType_Mask_Artist) == nType
					 || (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist | iQobuzType_Mask_Album) == nType
					 || (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Album) == nType)
		{
			AddWidgetItem(false, false, true);
		}
		// playlist
		else if ((iQobuzType_Mask_UserPlaylist | iQobuzType_Mask_Playlist) == nType)
		{
			AddWidgetItem(false, false, true);
		}
		else
		{
			AddWidgetItem();
		}

//		LogDebug("SlotRespQobuzList node [%s]", m_Node.ToCompactByteArray().data());

		GetInfoBrowser()->SetTitle(QOBUZ_TITLE);
		GetListBrowser()->SetNodeInfo(m_Node);
//		GetListBrowser()->ClearNodeList();
		nType = GetListBrowser()->SetNodeList(list, -1);
		SetType(nType);
		SetGenre(genre);

		if (!m_PlaylistID.isEmpty())
		{
			SetPlaylistID(m_PlaylistID);
		}
	}
}

void IServiceWindow::SlotListUpdate()
{
	m_Refresh = true;
	m_PlaylistID = "";

	if ((iQobuzType_Mask_UserPlaylist | iQobuzType_Mask_Playlist) == m_Type)
	{
		RequestQobuzPlaylist(QOBUZ_START, QOBUZ_COUNT);
	}
	else if ((iQobuzType_Mask_UserPlaylist | iQobuzType_Mask_Track) == m_Type)
	{
		m_PlaylistID = m_Node.GetString(KEY_ID_UPPER);

		RequestQobuzCategory(m_Type, m_PlaylistID, QOBUZ_START, QOBUZ_COUNT);
	}
}

void IServiceWindow::SlotRespAuth(int nServiceType)
{
	m_InternetType = nServiceType;

	IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
	emit SigAddWidget(widget, STR_ISERVICE);
	widget->SetInternetType(m_InternetType);
	widget->AddWidgetItem();
	widget->RequestIServiceURL();
}

void IServiceWindow::SlotRespURL(int nServiceType, QList<CJsonNode> list, QString title, QString nextUrl)
{
//	int count = list.count() - 1;
//	int nType =  list[count].GetInt(KEY_TYPE);
	m_Node.Add(KEY_NEXT, nextUrl);
//	LogDebug("SlotRespURL node [%s]", m_Node.ToCompactByteArray().data());

	GetInfoBrowser()->SetTitle(title);
	GetListBrowser()->SetNodeInfo(m_Node);
	//	widget->GetListBrowser()->ClearNodeList();
	int nType = GetListBrowser()->SetNodeList(list, SIDEMENU_ISERVICE);
	SetInternetType(nServiceType);
	SetType(nType);
}

void IServiceWindow::SlotRespForm(int nServiceType, CJsonNode node)
{
	m_InternetType = nServiceType;

	SearchDialog dialog;
	if (dialog.exec() == QDialog::Accepted)
	{
		QString keyword = dialog.GetKeyword();
		QString url = node.GetString(KEY_URL);
		url = url + "?q=" + keyword;

		IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
		emit SigAddWidget(widget, STR_ISERVICE);
		widget->SetInternetType(m_InternetType);
		widget->AddWidgetItem();
		widget->SetNode(node);
		widget->RequestIServiceURL(url);
	}
}

void IServiceWindow::SlotCoverArtUpdate(QString fileName, int nIndex, int mode)
{
	Q_UNUSED(mode);

	QModelIndex modelIndex = m_pListBrowser->GetModel()->index(nIndex, 0);
	m_pListBrowser->GetModel()->setData(modelIndex, fileName, ListBrowserDelegate::LIST_BROWSER_COVER);
}

void IServiceWindow::SlotPlayAll(int where)
{
	m_SelectMap = m_pListBrowser->GetSelectMapIService();

	if (m_SelectMap.count() <= 0)
	{
		m_pListBrowser->SetAllSelectMap();
		m_SelectMap = m_pListBrowser->GetSelectMapIService();
	}

	if (iIServiceType_Qobuz == m_InternetType)
	{
		RequestQobuzPlay(m_SelectMap, where);
	}
	else
	{
		RequestIServicePlay(m_SelectMap, where);
	}
}

void IServiceWindow::SlotPlayRandom()
{
	if (iIServiceType_Qobuz == m_InternetType)
	{
		m_pQobuzMgr->RequestRandom();
	}
	else
	{
		m_pAirableMgr->RequestRandom();
	}

}

void IServiceWindow::SlotTopMenu()
{
	if (iIServiceType_Qobuz == m_InternetType)
	{
		m_SelectMap = m_pListBrowser->GetSelectMapIService();
	}
	else
	{
		m_SelectMap = m_pListBrowser->GetSelectMapIService();
	}

	if (m_SelectMap.count() > 0)
	{
		SetSelectOnTopMenu();
	}
	else
	{
		SetSelectOffTopMenu();
	}
}

void IServiceWindow::SlotTopMenuAction(int menuID)
{
	switch (menuID) {
	case TOP_MENU_PLAY_NOW:
		DoTopMenuPlay(PLAY_NOW);
		break;
	case TOP_MENU_PLAY_LAST:
		DoTopMenuPlay(PLAY_LAST);
		break;
	case TOP_MENU_PLAY_NEXT:
		DoTopMenuPlay(PLAY_NEXT);
		break;
	case TOP_MENU_PLAY_CLEAR:
		DoTopMenuPlay(PLAY_CLEAR);
		break;
	case TOP_MENU_SELECT_ALL:
		DoTopMenuSelectAll();
		break;
	case TOP_MENU_CLEAR_ALL:
		DoTopMenuClearAll();
		break;
	case TOP_MENU_ADD_TO_FAVORITE:
		DoTopMenuAddToFavorite();
		break;
	case TOP_MENU_DELETE_TO_FAVORITE:
		DoTopMenuDeleteToFavorite();
		break;
	case TOP_MENU_ADD_TO_PLAYLIST:
		// todo-dylee
		break;
	case TOP_MENU_ADD_PLAYLIST:
		DoTopMenuAddPlaylist();
		break;
	case TOP_MENU_DELETE_PLAYLIST:
		DoTopMenuDeletePlaylist();
		break;
	case TOP_MENU_DELETE:
		DoTopMenuDeleteTrack();
		break;
	}
}

//void IServiceWindow::SlotOptionMenu(int index, int type)
//{
//	// qobuz
//	SetOptionMenu(type);

//	m_pListBrowser->SetEditor(index);

//}

//void IServiceWindow::SlotOptionMenu(int index, int type, QString menuName)
//{
//	SetOptionMenu(type, menuName);

//	m_pListBrowser->SetEditor(index);
//}

void IServiceWindow::SlotOptionMenuAction(QString url, int type, int menuID)
{
//	LogDebug("click option menu [%s] [%d] [%d]", url.toUtf8().data(), type, menuID);
	switch (menuID) {
	case OPTION_MENU_DELETE_PLAYLIST:
		m_pQobuzMgr->RequestDeletePlaylist(url);
		break;
	case OPTION_MENU_ADD_FAVORITE:
	case OPTION_MENU_DELETE_FAVORITE:
		m_pAirableMgr->RequestActionUrl(m_InternetType, url);
		break;
	}
}

void IServiceWindow::ConnectSigToSlot()
{
	connect(this, SIGNAL(SigAddWidget(QWidget*, QString)), parent(), SLOT(SlotAddWidget(QWidget*, QString)));		// recursive
	connect(this, SIGNAL(SigRespLogout()), parent(), SLOT(SlotRespAirableLogout()));

	connect(m_pQobuzMgr, SIGNAL(SigRespLoginFail(CJsonNode)), this, SLOT(SlotRespQobuzLoginFail(CJsonNode)));
	connect(m_pQobuzMgr, SIGNAL(SigRespLoginSuccess()), this, SLOT(SlotRespQobuzLoginSuccess()));
	connect(m_pQobuzMgr, SIGNAL(SigRespList(QList<CJsonNode>, bool, int)), this, SLOT(SlotRespQobuzList(QList<CJsonNode>, bool, int)));
	connect(m_pQobuzMgr, SIGNAL(SigListUpdate()), this, SLOT(SlotListUpdate()));

	connect(m_pAirableMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pAirableMgr, SIGNAL(SigRespLogout()), parent(), SLOT(SlotRespAirableLogout()));
	connect(m_pAirableMgr, SIGNAL(SigRespLoginFail(CJsonNode)), this, SLOT(SlotRespAirableLoginFail(CJsonNode)));
	connect(m_pAirableMgr, SIGNAL(SigRespLoginSuccess(int, bool)), this, SLOT(SlotRespAirableLoginSuccess(int, bool)));

	connect(m_pAirableMgr, SIGNAL(SigRespAuth(int)), this, SLOT(SlotRespAuth(int)));
	connect(m_pAirableMgr, SIGNAL(SigRespURL(int, QList<CJsonNode>, QString, QString)), this, SLOT(SlotRespURL(int, QList<CJsonNode>, QString, QString)));
	connect(m_pAirableMgr, SIGNAL(SigRespForm(int, CJsonNode)), this, SLOT(SlotRespForm(int, CJsonNode)));
	connect(m_pAirableMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));

	connect(m_pInfoBrowser->GetFormPlay(), SIGNAL(SigPlayAll()), this, SLOT(SlotPlayAll()));
	connect(m_pInfoBrowser->GetFormPlay(), SIGNAL(SigPlayRandom()), this, SLOT(SlotPlayRandom()));
	connect(m_pInfoBrowser->GetFormPlay(), SIGNAL(SigMenu()), this, SLOT(SlotTopMenu()));
	connect(m_pInfoBrowser->GetFormPlay(), SIGNAL(SigMenuAction(int)), this, SLOT(SlotTopMenuAction(int)));

//	connect(m_pInfoService->GetFormSort(), SIGNAL(SigResize()), this, SLOT(SlotResize()));

//	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectPlay(int)), this, SLOT(SlotSelectPlay(int)));
	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int)), this, SLOT(SlotSelectIconTitle(int)));
//	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));

	connect(m_pListBrowser, SIGNAL(SigReqCoverArt(QString, int)), this, SLOT(SlotReqCoverArt(QString, int)));
	connect(m_pListBrowser, SIGNAL(SigAppendList()), this, SLOT(SlotAppendList()));
	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigSelectPlay(int, CJsonNode)), this, SLOT(SlotSelectTitle(int, CJsonNode)));
	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigSelectTitle(int, CJsonNode)), this, SLOT(SlotSelectTitle(int, CJsonNode)));
//	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigMenu(int, int)), this, SLOT(SlotOptionMenu(int, int)));
//	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigMenu(int, int, QString)), this, SLOT(SlotOptionMenu(int, int, QString)));
//	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigMenuAction(QString, int, int)), this, SLOT(SlotOptionMenuAction(QString, int, int)));
	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigSelectMenu(const QModelIndex&, QPoint)), this, SLOT(SlotSelectMenu(const QModelIndex&, QPoint)));
	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigSelectMenu(const QModelIndex&, QPoint)), this, SLOT(SlotSelectMenu(const QModelIndex&, QPoint)));

	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuAction(QAction*)));
}

void IServiceWindow::Initialize()
{
//	m_pInfoService->GetFormSort()->ShowResize();

	m_TopMenuMap.clear();
	m_OptionMenuMap.clear();
	m_SelectMap.clear();

	m_PlaylistID = "";
	m_Refresh = false;

	m_StartCount = QOBUZ_START;

	QString style = QString("QMenu::icon {	\
								padding: 0px 0px 0px 20px;	\
							}	\
							QMenu::item {	\
								width: 260px;	\
								height: 40px;	\
								color: rgb(90, 91, 94);	\
								font-size: 14pt;	\
								padding: 0px 20px 0px 20px;	\
							}	\
							QMenu::item:selected {	\
								background: rgba(201,237,248,255);	\
							}");

	m_Menu->setStyleSheet(style);
}

void IServiceWindow::SetSelectOffTopMenu()
{
	m_TopMenuMap.clear();

	if (iIServiceType_Qobuz == m_InternetType)
	{
		int nType = m_Type;
		UtilNovatron::DebugTypeForQobuz("SetSelectOffTopMenu", nType);

		if (iQobuzType_Mask_Track & nType)
		{
			m_TopMenuMap.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
			m_TopMenuMap.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
			m_TopMenuMap.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
			m_TopMenuMap.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		}
		// playlist
		else if ((iQobuzType_Mask_UserPlaylist | iQobuzType_Mask_Playlist) == nType)
		{
			m_TopMenuMap.insert(TOP_MENU_ADD_PLAYLIST, STR_ADD_PLAYLIST);
		}

		m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
	}
	else
	{
		m_TopMenuMap.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_TopMenuMap.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_TopMenuMap.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_TopMenuMap.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
	}

	m_pInfoBrowser->GetFormPlay()->ClearMenu();
	m_pInfoBrowser->GetFormPlay()->SetMenu(m_TopMenuMap);

}

void IServiceWindow::SetSelectOnTopMenu()
{
	m_TopMenuMap.clear();

	if (iIServiceType_Qobuz == m_InternetType)
	{
		int nType = m_Type;
		UtilNovatron::DebugTypeForQobuz("SetSelectOnTopMenu", nType);

		if (iQobuzType_Mask_Track & nType)
		{
			m_TopMenuMap.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
			m_TopMenuMap.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
			m_TopMenuMap.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
			m_TopMenuMap.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);

			// search
			if (iQobuzType_Mask_Search & nType)
			{
				m_TopMenuMap.insert(TOP_MENU_ADD_TO_FAVORITE, STR_ADD_TO_FAVORITE);
//				m_TopMenuMap.insert(TOP_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
			}
			// recommend
			else if (iQobuzType_Mask_Recommend & nType)
			{
				m_TopMenuMap.insert(TOP_MENU_ADD_TO_FAVORITE, STR_ADD_TO_FAVORITE);
//				m_TopMenuMap.insert(TOP_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
			}
			// favorite
			else if (iQobuzType_Mask_Favorite & nType)
			{
//				m_TopMenuMap.insert(TOP_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
				m_TopMenuMap.insert(TOP_MENU_DELETE_TO_FAVORITE, STR_DELETE_TO_FAVORITE);
			}
			// playlist
			else if (iQobuzType_Mask_UserPlaylist & nType)
			{
				m_TopMenuMap.insert(TOP_MENU_ADD_TO_FAVORITE, STR_ADD_TO_FAVORITE);
//				m_TopMenuMap.insert(TOP_MENU_DELETE, STR_DELETE);		// is not working
			}
		}
		// search
		else if ((iQobuzType_Mask_Search | iQobuzType_Mask_Artist) == nType
				  || (iQobuzType_Mask_Search | iQobuzType_Mask_Album) == nType
				  || (iQobuzType_Mask_Search | iQobuzType_Mask_Playlist) == nType)
		{
			m_TopMenuMap.insert(TOP_MENU_ADD_TO_FAVORITE, STR_ADD_TO_FAVORITE);
		}
		// recommend
		else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Album) == nType)
		{
			m_TopMenuMap.insert(TOP_MENU_ADD_TO_FAVORITE, STR_ADD_TO_FAVORITE);
		}
		// favorite
		else if ((iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist | iQobuzType_Mask_Artist) == nType
					 || (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist | iQobuzType_Mask_Album) == nType
					 || (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Album) == nType)
		{
			m_TopMenuMap.insert(TOP_MENU_DELETE_TO_FAVORITE, STR_DELETE_TO_FAVORITE);
		}
		// playlist
		else if ((iQobuzType_Mask_UserPlaylist | iQobuzType_Mask_Playlist) == nType)
		{
//			m_TopMenuMap.insert(TOP_MENU_DELETE_PLAYLIST, STR_DELETE_PLAYLIST);
		}
		m_TopMenuMap.insert(TOP_MENU_CLEAR_ALL, STR_CLEAR_ALL);
	}
	else
	{
		m_TopMenuMap.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_TopMenuMap.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_TopMenuMap.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_TopMenuMap.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_TopMenuMap.insert(TOP_MENU_CLEAR_ALL, STR_CLEAR_ALL);
	}

	m_pInfoBrowser->GetFormPlay()->ClearMenu();
	m_pInfoBrowser->GetFormPlay()->SetMenu(m_TopMenuMap);
}

void IServiceWindow::DoTopMenuSelectAll()
{
	m_pListBrowser->SetAllSelectMap();

}

void IServiceWindow::DoTopMenuClearAll()
{
	m_pListBrowser->ClearSelectMap();

}

void IServiceWindow::DoTopMenuPlay(int nWhere)
{
	SlotPlayAll(nWhere);
}

void IServiceWindow::DoTopMenuAddToFavorite()
{
	m_SelectMap = m_pListBrowser->GetSelectMapIService();
	if (m_SelectMap.count() > 0)
	{
		m_pQobuzMgr->RequestAddToFavorite(m_Type, m_SelectMap);
	}
}

void IServiceWindow::DoTopMenuDeleteToFavorite()
{
	m_SelectMap = m_pListBrowser->GetSelectMapIService();
	if (m_SelectMap.count() > 0)
	{
		m_pQobuzMgr->RequestDeleteToFavorite(m_Type, m_SelectMap);
	}
}

void IServiceWindow::DoTopMenuAddPlaylist()
{
	AddQobuzPlaylist dialog;
	if (dialog.exec() == QDialog::Accepted)
	{
		QString name = dialog.GetName();
		QString desc = dialog.GetDesc();
		m_pQobuzMgr->RequestAddPlaylist(name, desc);
	}
}

void IServiceWindow::DoTopMenuDeletePlaylist()
{
	m_SelectMap = m_pListBrowser->GetSelectMapIService();
	if (m_SelectMap.count() > 0)
	{
		m_pQobuzMgr->RequestDeletePlaylist("7801142");
	}
}

void IServiceWindow::DoTopMenuDeleteTrack()
{
	m_SelectMap = m_pListBrowser->GetSelectMapIService();
	if (m_SelectMap.count() > 0)
	{
		m_pQobuzMgr->RequestDeleteTrack(GetPlaylistID(), m_SelectMap);
	}
}

void IServiceWindow::SelectSearchForQobuz(int nType, CJsonNode node)
{
	UtilNovatron::DebugTypeForQobuz("SelectSearchForQobuz", nType);

	SetType(nType);
	SetNode(node);

//	LogDebug("node [%s]", m_Node.ToCompactByteArray().data());

	int itemType = m_Node.GetInt(KEY_ITEM_TYPE);

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
			m_Node.Add(KEY_KEYWORD, keyword);

			IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
			emit SigAddWidget(widget, STR_ISERVICE);
			widget->SetInternetType(iIServiceType_Qobuz);
//			widget->AddWidgetItem();
			widget->SetNode(m_Node);
			widget->SetTypeParent(m_Type);
			widget->RequestQobuzSearch(itemType, keyword, QOBUZ_START, QOBUZ_COUNT);
		}
	}
	else if ((iQobuzType_Mask_Search | iQobuzType_Mask_Artist) == nType
		  || (iQobuzType_Mask_Search | iQobuzType_Mask_Album) == nType
		  || (iQobuzType_Mask_Search | iQobuzType_Mask_Playlist) == nType)
	{
		QString strID = m_Node.GetString(KEY_ID_UPPER);

		IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
		emit SigAddWidget(widget, STR_ISERVICE);
		widget->SetInternetType(iIServiceType_Qobuz);
		widget->SetNode(m_Node);
		widget->SetTypeParent(m_Type);
		widget->RequestQobuzCategory(nType, strID, QOBUZ_START, QOBUZ_COUNT);
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

	SetType(nType);
	SetNode(node);

//	LogDebug("node [%s]", m_Node.ToCompactByteArray().data());

	if (iQobuzType_Mask_Recommend == nType)
	{
		DoQobuzRecommend();
	}
	else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album) == nType)
	{
		DoQobuzRecommendAlbum();
	}
	else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist) == nType)
	{
		DoQobuzRecommendPlaylist();
	}
	else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre) == nType)
	{
		QString strID = m_Node.GetString(KEY_ID_UPPER);
		if (!strID.compare(VAL_GENRE))
		{
			IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
			emit SigAddWidget(widget, STR_ISERVICE);
			widget->SetInternetType(iIServiceType_Qobuz);
			widget->SetNode(m_Node);
			widget->SetTypeParent(m_Type);
			widget->RequestQobuzGenre();
		}
		else if (!strID.compare(VAL_ID_BEST_SELLERS)
				 || !strID.compare(VAL_ID_MOST_STREAMED)
				 || !strID.compare(VAL_ID_NEW_RELEASES)
				 || !strID.compare(VAL_ID_PRESS_AWARDS)
				 || !strID.compare(VAL_ID_EDITOR_PICKS)
				 || !strID.compare(VAL_ID_MOST_FEATURED))
		{
			QString strGenreID = m_Node.GetString(KEY_GENRE_ID);

			IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
			emit SigAddWidget(widget, STR_ISERVICE);
			widget->SetInternetType(iIServiceType_Qobuz);
			widget->SetNode(m_Node);
			widget->SetTypeParent(m_Type);
			widget->RequestQobuzRecommend(nType, strID, QOBUZ_START, QOBUZ_COUNT, strGenreID);
		}
		else
		{
			if (m_bGenre)
			{
				m_bGenre = false;
				DoQobuzRecommendGenre(nType, strID);
			}
			else
			{
				IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
				emit SigAddWidget(widget, STR_ISERVICE);
				widget->SetInternetType(iIServiceType_Qobuz);
				widget->SetNode(m_Node);
				widget->SetTypeParent(m_Type);
				widget->RequestQobuzGenre(strID);
			}
		}
	}
	else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Album) == nType
			 || (iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist | iQobuzType_Mask_Playlist) == nType)
	{
		QString strID = m_Node.GetString(KEY_ID_UPPER);
		if (!strID.compare(VAL_ID_BEST_SELLERS)
				 || !strID.compare(VAL_ID_MOST_STREAMED)
				 || !strID.compare(VAL_ID_NEW_RELEASES)
				 || !strID.compare(VAL_ID_PRESS_AWARDS)
				 || !strID.compare(VAL_ID_EDITOR_PICKS)
				 || !strID.compare(VAL_ID_MOST_FEATURED)
				 || !strID.compare(VAL_ID_LAST_CREATED))
		{
			IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
			emit SigAddWidget(widget, STR_ISERVICE);
			widget->SetInternetType(iIServiceType_Qobuz);
			widget->SetNode(m_Node);
			widget->SetTypeParent(m_Type);
			widget->RequestQobuzRecommend(nType, strID, QOBUZ_START, QOBUZ_COUNT);
		}
		else
		{
			QString strID = m_Node.GetString(KEY_ID_UPPER);

			IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
			emit SigAddWidget(widget, STR_ISERVICE);
			widget->SetInternetType(iIServiceType_Qobuz);
			widget->SetNode(m_Node);
			widget->SetTypeParent(m_Type);
			widget->RequestQobuzCategory(nType, strID, QOBUZ_START, QOBUZ_COUNT);
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

	SetType(nType);
	SetNode(node);

//	LogDebug("node [%s]", m_Node.ToCompactByteArray().data());

	if (iQobuzType_Mask_Favorite == nType)
	{
		DoQobuzFavorite();
	}
	else if ((iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist) == nType
			 || (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Album) == nType
			 || (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Track) == nType)
	{
		IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
		emit SigAddWidget(widget, STR_ISERVICE);
		widget->SetInternetType(iIServiceType_Qobuz);
		widget->SetNode(m_Node);
		widget->SetTypeParent(m_Type);
		widget->RequestQobuzFavorite(nType, QOBUZ_START, QOBUZ_COUNT);
	}
	else if ((iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist | iQobuzType_Mask_Artist) == nType
			 || (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist | iQobuzType_Mask_Album) == nType
			 || (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Album) == nType)
	{
		QString strID = m_Node.GetString(KEY_ID_UPPER);

		IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
		emit SigAddWidget(widget, STR_ISERVICE);
		widget->SetInternetType(iIServiceType_Qobuz);
		widget->SetNode(m_Node);
		widget->SetTypeParent(m_Type);
		widget->RequestQobuzCategory(nType, strID, QOBUZ_START, QOBUZ_COUNT);
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

	SetType(nType);
	SetNode(node);

	m_PlaylistID = "";

//	LogDebug("node [%s]", m_Node.ToCompactByteArray().data());

	if (iQobuzType_Mask_UserPlaylist == nType)
	{
		IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
		emit SigAddWidget(widget, STR_ISERVICE);
		widget->SetInternetType(iIServiceType_Qobuz);
		widget->SetNode(m_Node);
		widget->SetTypeParent(m_Type);
		widget->RequestQobuzPlaylist(QOBUZ_START, QOBUZ_COUNT);
	}
	else if ((iQobuzType_Mask_UserPlaylist | iQobuzType_Mask_Playlist) == nType)
	{
		m_PlaylistID = m_Node.GetString(KEY_ID_UPPER);

		IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
		emit SigAddWidget(widget, STR_ISERVICE);
		widget->SetInternetType(iIServiceType_Qobuz);
		widget->SetNode(m_Node);
		widget->SetTypeParent(m_Type);
		widget->RequestQobuzCategory(nType, m_PlaylistID, QOBUZ_START, QOBUZ_COUNT);
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

	SetType(nType);
	SetNode(node);

//	LogDebug("node [%s]", m_Node.ToCompactByteArray().data());

	QString url = m_Node.GetString(KEY_URL);

	if (nType & iAirableType_Mask_Dir)
	{
		if (!url.isEmpty())
		{
			IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
			emit SigAddWidget(widget, STR_ISERVICE);
			widget->SetInternetType(m_InternetType);
			widget->AddWidgetItem();
			widget->SetNode(m_Node);
			widget->RequestIServiceURL(url);
		}
	}
	else if (nType & iAirableType_Mask_Track
			 || nType & iAirableType_Mask_Program
			 || nType & iAirableType_Mask_Radio
			 || nType & iAirableType_Mask_Feed
			 || nType & iAirableType_Mask_Play)
	{
		QMap<int, CJsonNode> map;
		map.insert(0, m_Node);
		RequestIServicePlay(map, PLAY_CLEAR);
	}
	else if (nType & iAirableType_Mask_Logout)
	{
		if (!url.isEmpty())
		{
			m_pAirableMgr->RequestLogout(m_InternetType, url);
		}
	}
}

void IServiceWindow::AppendSearchForQobuz()
{
	int nType = m_TypeParent;
	CJsonNode node = m_Node;

	UtilNovatron::DebugTypeForQobuz("AppendSearchForQobuz", nType);

//	LogDebug("node [%s]", node.ToCompactByteArray().data());

	int itemType = node.GetInt(KEY_ITEM_TYPE);

	if (iQobuzType_Mask_Search == nType && itemType < 0)
	{
//		DoQobuzSearch();
	}
	else if (iQobuzType_Mask_Search == nType
			 && ((iQobuzType_Mask_Search | iQobuzType_Mask_Artist) == itemType
				 || (iQobuzType_Mask_Search | iQobuzType_Mask_Album) == itemType
				 || (iQobuzType_Mask_Search | iQobuzType_Mask_Track) == itemType
				 || (iQobuzType_Mask_Search | iQobuzType_Mask_Playlist) == itemType))
	{
//		SearchDialog dialog;
//		if (dialog.exec() == QDialog::Accepted)
//		{
//			QString keyword = dialog.GetKeyword();

//			IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
//			emit SigAddWidget(widget, STR_ISERVICE);
//			widget->SetInternetType(iIServiceType_Qobuz);
////			widget->AddWidgetItem();
//			widget->SetNode(node);
//			widget->RequestQobuzSearch(itemType, keyword, QOBUZ_START, QOBUZ_COUNT);
//		}

		QString keyword = node.GetString(KEY_KEYWORD);

		RequestQobuzSearch(itemType, keyword, m_StartCount, QOBUZ_COUNT);

	}
	else if ((iQobuzType_Mask_Search | iQobuzType_Mask_Artist) == nType
		  || (iQobuzType_Mask_Search | iQobuzType_Mask_Album) == nType
		  || (iQobuzType_Mask_Search | iQobuzType_Mask_Playlist) == nType)
	{
//		QString strID = m_Node.GetString(KEY_ID_UPPER);

//		IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
//		emit SigAddWidget(widget, STR_ISERVICE);
//		widget->SetInternetType(iIServiceType_Qobuz);
//		widget->SetNode(node);
//		widget->RequestQobuzCategory(nType, strID, QOBUZ_START, QOBUZ_COUNT);

		QString strID = node.GetString(KEY_ID_UPPER);

		RequestQobuzCategory(nType, strID, m_StartCount, QOBUZ_COUNT);
	}
	else if ((iQobuzType_Mask_Search | iQobuzType_Mask_Track) == nType)
	{
//		QMap<int, CJsonNode> map;
//		map.insert(0, node);
//		RequestQobuzPlay(map, PLAY_CLEAR);
	}

}

void IServiceWindow::AppendRecommendForQobuz()
{
	int nType = m_TypeParent;
	CJsonNode node = m_Node;

	UtilNovatron::DebugTypeForQobuz("AppendRecommendForQobuz", nType);

//	LogDebug("node [%s]", node.ToCompactByteArray().data());

	if (iQobuzType_Mask_Recommend == nType)
	{
//		DoQobuzRecommend();
	}
	else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album) == nType)
	{
//		DoQobuzRecommendAlbum();
	}
	else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist) == nType)
	{
//		DoQobuzRecommendPlaylist();
	}
	else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre) == nType)
	{
		QString strID = node.GetString(KEY_ID_UPPER);
		if (!strID.compare(VAL_GENRE))
		{
//			IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
//			emit SigAddWidget(widget, STR_ISERVICE);
//			widget->SetInternetType(iIServiceType_Qobuz);
//			widget->SetNode(node);
//			widget->RequestQobuzGenre();
		}
		else if (!strID.compare(VAL_ID_BEST_SELLERS)
				 || !strID.compare(VAL_ID_MOST_STREAMED)
				 || !strID.compare(VAL_ID_NEW_RELEASES)
				 || !strID.compare(VAL_ID_PRESS_AWARDS)
				 || !strID.compare(VAL_ID_EDITOR_PICKS)
				 || !strID.compare(VAL_ID_MOST_FEATURED))
		{
//			QString strGenreID = m_Node.GetString(KEY_GENRE_ID);

//			IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
//			emit SigAddWidget(widget, STR_ISERVICE);
//			widget->SetInternetType(iIServiceType_Qobuz);
//			widget->SetNode(node);
//			widget->RequestQobuzRecommend(nType, strID, QOBUZ_START, QOBUZ_COUNT, strGenreID);

			QString strGenreID = node.GetString(KEY_GENRE_ID);

			RequestQobuzRecommend(nType, strID, m_StartCount, QOBUZ_COUNT, strGenreID);
		}
		else
		{
//			if (m_bGenre)
//			{
//				m_bGenre = false;
//				DoQobuzRecommendGenre(nType, strID);
//			}
//			else
//			{
//				IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
//				emit SigAddWidget(widget, STR_ISERVICE);
//				widget->SetInternetType(iIServiceType_Qobuz);
//				widget->SetNode(node);
//				widget->RequestQobuzGenre(strID);
//			}
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
//			IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
//			emit SigAddWidget(widget, STR_ISERVICE);
//			widget->SetInternetType(iIServiceType_Qobuz);
//			widget->SetNode(node);
//			widget->RequestQobuzRecommend(nType, strID, QOBUZ_START, QOBUZ_COUNT);

			RequestQobuzRecommend(nType, strID, m_StartCount, QOBUZ_COUNT);
		}
		else
		{
//			QString strID = m_Node.GetString(KEY_ID_UPPER);

//			IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
//			emit SigAddWidget(widget, STR_ISERVICE);
//			widget->SetInternetType(iIServiceType_Qobuz);
//			widget->SetNode(node);
//			widget->RequestQobuzCategory(nType, strID, QOBUZ_START, QOBUZ_COUNT);

			RequestQobuzCategory(nType, strID, m_StartCount, QOBUZ_COUNT);
		}
	}
	else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Track) == nType
			 || (iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Track) == nType
			 || (iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist | iQobuzType_Mask_Track) == nType)
	{
//		QMap<int, CJsonNode> map;
//		map.insert(0, m_Node);
//		RequestQobuzPlay(map, PLAY_CLEAR);
	}
}

void IServiceWindow::AppendFavoriteForQobuz()
{
	int nType = m_TypeParent;
	CJsonNode node = m_Node;

	UtilNovatron::DebugTypeForQobuz("AppendFavoriteForQobuz", nType);

//	LogDebug("node [%s]", node.ToCompactByteArray().data());

	if (iQobuzType_Mask_Favorite == nType)
	{
//		DoQobuzFavorite();
	}
	else if ((iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist) == nType
			 || (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Album) == nType
			 || (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Track) == nType)
	{
//		IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
//		emit SigAddWidget(widget, STR_ISERVICE);
//		widget->SetInternetType(iIServiceType_Qobuz);
//		widget->SetNode(node);
//		widget->RequestQobuzFavorite(nType, QOBUZ_START, QOBUZ_COUNT);

		RequestQobuzFavorite(nType, m_StartCount, QOBUZ_COUNT);
	}
	else if ((iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist | iQobuzType_Mask_Artist) == nType
			 || (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist | iQobuzType_Mask_Album) == nType
			 || (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Album) == nType)
	{
//		QString strID = m_Node.GetString(KEY_ID_UPPER);

//		IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
//		emit SigAddWidget(widget, STR_ISERVICE);
//		widget->SetInternetType(iIServiceType_Qobuz);
//		widget->SetNode(node);
//		widget->RequestQobuzCategory(nType, strID, QOBUZ_START, QOBUZ_COUNT);

		QString strID = node.GetString(KEY_ID_UPPER);

		RequestQobuzCategory(nType, strID, m_StartCount, QOBUZ_COUNT);
	}
	else if ((iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist | iQobuzType_Mask_Track) == nType
			 || (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Track) == nType
			 || (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Track | iQobuzType_Mask_Track) == nType)
	{
//		QMap<int, CJsonNode> map;
//		map.insert(0, node);
//		RequestQobuzPlay(map, PLAY_CLEAR);
	}
}

void IServiceWindow::AppendUserPlaylistForQobuz()
{
	int nType = m_TypeParent;
	CJsonNode node = m_Node;

	UtilNovatron::DebugTypeForQobuz("AppendUserPlaylistForQobuz", nType);

//	LogDebug("node [%s]", node.ToCompactByteArray().data());

	if (iQobuzType_Mask_UserPlaylist == nType)
	{
//		IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
//		emit SigAddWidget(widget, STR_ISERVICE);
//		widget->SetInternetType(iIServiceType_Qobuz);
//		widget->SetNode(node);
//		widget->RequestQobuzPlaylist(QOBUZ_START, QOBUZ_COUNT);

		RequestQobuzPlaylist(m_StartCount, QOBUZ_COUNT);
	}
	else if ((iQobuzType_Mask_UserPlaylist | iQobuzType_Mask_Playlist) == nType)
	{
		QString playlistID = node.GetString(KEY_ID_UPPER);

//		IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
//		emit SigAddWidget(widget, STR_ISERVICE);
//		widget->SetInternetType(iIServiceType_Qobuz);
//		widget->SetNode(node);
//		widget->RequestQobuzCategory(nType, m_PlaylistID, QOBUZ_START, QOBUZ_COUNT);

		RequestQobuzCategory(nType, playlistID, m_StartCount, QOBUZ_COUNT);
	}
	else if ((iQobuzType_Mask_UserPlaylist | iQobuzType_Mask_Track) == nType)
	{
//		QMap<int, CJsonNode> map;
//		map.insert(0, node);
//		RequestQobuzPlay(map, PLAY_CLEAR);
	}
}

void IServiceWindow::AppendTitleForAirable()
{
	UtilNovatron::DebugTypeForAirable("AppendTitleForAirable", m_Type);
	QString nextUrl = m_Node.GetString(KEY_NEXT);
	if (!nextUrl.isEmpty())
	{
		RequestIServiceURL(nextUrl);
	}
}

void IServiceWindow::SetOptionMenu(int type, QString menuName)
{
	m_OptionMenuMap.clear();

	if (iIServiceType_Qobuz == m_InternetType)
	{
		UtilNovatron::DebugTypeForQobuz(menuName, type);
		if ((iQobuzType_Mask_UserPlaylist | iQobuzType_Mask_Playlist) == type)
		{
			m_OptionMenuMap.insert(OPTION_MENU_DELETE_PLAYLIST, STR_DELETE_PLAYLIST);
		}
	}
	else
	{
		UtilNovatron::DebugTypeForAirable(menuName, type);
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
	}

//	if (m_OptionMenuMap.count() > 0)
//	{
//		m_pListBrowser->GetDelegate()->SetOptionMenuMap(m_OptionMenuMap);
//	}

}


void IServiceWindow::DoQobuzHome()
{
	IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
	emit SigAddWidget(widget, STR_ISERVICE);
	widget->SetInternetType(iIServiceType_Qobuz);
	widget->AddWidgetItem();

	QList<CJsonNode> list;
	widget->SetQobuzHome(list);

	widget->GetInfoBrowser()->SetTitle(QOBUZ_TITLE);

	widget->GetListBrowser()->ClearNodeList();
	int nType = widget->GetListBrowser()->SetNodeList(list, -1);
}

void IServiceWindow::DoQobuzSearch()
{
	IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
	emit SigAddWidget(widget, STR_ISERVICE);
	widget->SetInternetType(iIServiceType_Qobuz);
	widget->AddWidgetItem();

	QList<CJsonNode> list;
	SetQobuzSearch(list);

	widget->GetInfoBrowser()->SetTitle(STR_SEARCH);

	widget->GetListBrowser()->ClearNodeList();
	int nType = widget->GetListBrowser()->SetNodeList(list, -1);
}

void IServiceWindow::DoQobuzRecommend()
{
	IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
	emit SigAddWidget(widget, STR_ISERVICE);
	widget->SetInternetType(iIServiceType_Qobuz);
	widget->AddWidgetItem();

	QList<CJsonNode> list;
	SetQobuzRecommend(list);

	widget->GetInfoBrowser()->SetTitle(STR_RECOMMEND);

	widget->GetListBrowser()->ClearNodeList();
	int nType = widget->GetListBrowser()->SetNodeList(list, -1);
}

void IServiceWindow::DoQobuzFavorite()
{
	IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
	emit SigAddWidget(widget, STR_ISERVICE);
	widget->SetInternetType(iIServiceType_Qobuz);
	widget->AddWidgetItem();

	QList<CJsonNode> list;
	SetQobuzFavorite(list);

	widget->GetInfoBrowser()->SetTitle(STR_FAVORITE);

	widget->GetListBrowser()->ClearNodeList();
	int nType = widget->GetListBrowser()->SetNodeList(list, -1);
}

void IServiceWindow::DoQobuzRecommendAlbum()
{
	IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
	emit SigAddWidget(widget, STR_ISERVICE);
	widget->SetInternetType(iIServiceType_Qobuz);
	widget->AddWidgetItem();

	QList<CJsonNode> list;
	SetRecommendAlbum(list);

	widget->GetInfoBrowser()->SetTitle(STR_ALBUM);

	widget->GetListBrowser()->ClearNodeList();
	int nType = widget->GetListBrowser()->SetNodeList(list, -1);
}

void IServiceWindow::DoQobuzRecommendPlaylist()
{
	IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
	emit SigAddWidget(widget, STR_ISERVICE);
	widget->SetInternetType(iIServiceType_Qobuz);
	widget->AddWidgetItem();

	QList<CJsonNode> list;
	SetRecommendPlaylist(list);

	widget->GetInfoBrowser()->SetTitle(STR_PLAYLIST);

	widget->GetListBrowser()->ClearNodeList();
	int nType = widget->GetListBrowser()->SetNodeList(list, -1);
}

void IServiceWindow::DoQobuzRecommendGenre(int nType, QString strID)
{
	IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
	emit SigAddWidget(widget, STR_ISERVICE);
	widget->SetInternetType(iIServiceType_Qobuz);
	widget->AddWidgetItem();

	QList<CJsonNode> list;
	SetRecommendGenre(list, strID);

	widget->GetInfoBrowser()->SetTitle(STR_GENRE);

	widget->GetListBrowser()->ClearNodeList();
	nType = widget->GetListBrowser()->SetNodeList(list, -1);
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
