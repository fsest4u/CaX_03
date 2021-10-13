#include <QtWebEngineWidgets/QWebEngineView>

#include "iservicewindow.h"
#include "ui_iservicewindow.h"

#include "manager/airablemanager.h"
#include "manager/qobuzmanager.h"

#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"
#include "widget/airable.h"
#include "widget/qobuz.h"

#include "formTop/infoservice.h"
#include "formBottom/iconservice.h"
#include "formBottom/iconservicedelegate.h"
#include "formBottom/listservice.h"
#include "formBottom/listservicedelegate.h"

#include "dialog/logindialog.h"
#include "dialog/webengineviewdialog.h"
#include "dialog/searchdialog.h"

#define ISERVICE_TITLE	"Internet Service"
#define QOBUZ_TITLE		"Qobuz"



IServiceWindow::IServiceWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pAirableMgr(new AirableManager),
	m_pQobuzMgr(new QobuzManager),
	m_pInfoService(new InfoService(this)),
	m_pIconService(new IconService(this)),
	m_pListService(new ListService(this)),
	m_pLoading(new Loading(parent)),
	m_WebURL(""),
	m_ServiceType(-1),
	ui(new Ui::IServiceWindow)
{
	ui->setupUi(this);

	m_pAirableMgr->SetAddr(addr);
	m_pQobuzMgr->SetAddr(addr);

	ConnectSigToSlot();

	m_pInfoService->SetSubmenuIService();

	m_bGenreSubmenu = false;
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

	if (m_pInfoService)
	{
		delete m_pInfoService;
		m_pInfoService = nullptr;
	}

	if (m_pIconService)
	{
		delete m_pIconService;
		m_pIconService = nullptr;
	}

	if (m_pListService)
	{
		delete m_pListService;
		m_pListService = nullptr;
	}

	if (m_pLoading)
	{
		delete m_pLoading;
		m_pLoading = nullptr;
	}

}

void IServiceWindow::IServiceHome(QList<CJsonNode> list)
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pIconService);

	SetIServiceHome(list);

	m_pInfoService->SetSubtitle(ISERVICE_TITLE);
	m_pIconService->SetNodeList(list, IconService::ICON_SERVICE_ISERVICE);
}

void IServiceWindow::RequestIServiceURL(int nServiceType, QString url)
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pListService);

	m_pAirableMgr->RequestURL(nServiceType, url);
}

void IServiceWindow::RequestQobuzSearch(int nType, QString keyword, int nStart, int nCount)
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pListService);

	m_pQobuzMgr->RequestSearch(nType, keyword, nStart, nCount);
}

void IServiceWindow::RequestQobuzGenre(QString strID)
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pListService);

	m_pQobuzMgr->RequestGenre(strID);
}

void IServiceWindow::RequestQobuzRecommend(int nType, QString strID, int nStart, int nCount, QString strGenreID)
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pListService);

	m_pQobuzMgr->RequestRecommend(nType, strID, nStart, nCount, strGenreID);
}

void IServiceWindow::RequestQobuzFavorite(int nType, int nStart, int nCount)
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pListService);

	m_pQobuzMgr->RequestFavorite(nType, nStart, nCount);
}

void IServiceWindow::RequestQobuzPlaylist(int nStart, int nCount)
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pListService);

	m_pQobuzMgr->RequestPlaylist(nStart, nCount);
}

void IServiceWindow::RequestQobuzCategory(int nType, QString strID, int nStart, int nCount)
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pListService);

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

void IServiceWindow::SlotAddWidget(QWidget *widget, QString title)
{
	emit SigAddWidget(widget, title);		// recursive
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

void IServiceWindow::SlotSelectTitle(int nType)
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
		emit SigAddWidget(widget, tr("Internet Service"));
		widget->DoQobuzSearch();
	}
	else if (iQobuzType_Mask_Recommend == nType)
	{
		IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
		emit SigAddWidget(widget, tr("Internet Service"));
		widget->DoQobuzRecommend();
	}
	else if (iQobuzType_Mask_Favorite == nType)
	{
		IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
		emit SigAddWidget(widget, tr("Internet Service"));
		widget->DoQobuzFavorite();
	}
	else if (iQobuzType_Mask_UserPlaylist == nType)
	{
		IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
		emit SigAddWidget(widget, tr("Internet Service"));
		widget->RequestQobuzPlaylist(0, 50);
	}
}

void IServiceWindow::SlotSelectTitle(int nType, QString rawData)
{
	CJsonNode node;
	if (!node.SetContent(rawData))
	{
		LogCritical("invalid json");
		return;
	}
	QString title = node.GetString(KEY_NAME);

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
			emit SigAddWidget(widget, tr("Internet Service"));
			widget->RequestQobuzSearch(nType, keyword, 0, 50);
			widget->SetInfoTitle(title);
		}

	}
	else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album) == nType)
	{
		IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
		emit SigAddWidget(widget, tr("Internet Service"));
		widget->DoRecommendAlbum();
		widget->SetInfoTitle(title);
	}
	else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist) == nType)
	{
		IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
		emit SigAddWidget(widget, tr("Internet Service"));
		widget->DoRecommendPlaylist();
		widget->SetInfoTitle(title);
	}
	else if ((iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist) == nType
			|| (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Album) == nType
			|| (iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Track) == nType)
	{
		LogDebug("favorite ");
		IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
		emit SigAddWidget(widget, tr("Internet Service"));
		widget->RequestQobuzFavorite(nType, 0, 50);
		widget->SetInfoTitle(title);
	}
	else if ((iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre) == nType
			 || (iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist | iQobuzType_Mask_Playlist) == nType)
	{
		IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
		emit SigAddWidget(widget, tr("Internet Service"));
		widget->SetInfoTitle(title);

		QString strID = node.GetString(KEY_ID_UPPER);
		if (!strID.compare(VAL_GENRE))
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
			m_pQobuzMgr->RequestPlay(node);

		}
		else if (m_bGenreSubmenu)
		{
			LogDebug("fixed genre submenu display...");
			IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
			emit SigAddWidget(widget, tr("Internet Service"));
			widget->DoRecommendGenre(nType, strID);
		}
		else if (nType & iQobuzType_Mask_Album)
		{
			IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
			emit SigAddWidget(widget, tr("Internet Service"));
			widget->RequestQobuzCategory(nType, strID, 0, 50);
		}
		else if (nType & iQobuzType_Mask_Menu_Genre)
		{
			int nItemType = node.GetInt(KEY_ITEM_TYPE);
			if (nItemType & iQobuzType_Mask_Album)
			{
				QString strGenreID = node.GetString(KEY_GENRE_ID);

				IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
				emit SigAddWidget(widget, tr("Internet Service"));
				widget->RequestQobuzRecommend(nType, strID, 0, 50, strGenreID);
			}
			else
			{
				IServiceWindow *widget = new IServiceWindow(this, m_pQobuzMgr->GetAddr());
				emit SigAddWidget(widget, tr("Internet Service"));
				widget->RequestQobuzGenre(strID);
			}
		}
		else
		{
			LogDebug("not supported");
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
			emit SigAddWidget(widget, tr("Internet Service"));
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
	emit SigAddWidget(widget, tr("Internet Service"));
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
	m_pListService->SetNodeList(list);

}

void IServiceWindow::SlotRespGenreSubList(QList<CJsonNode> list)
{
	m_pListService->SetNodeList(list);
	m_bGenreSubmenu = true;
}

void IServiceWindow::SlotRespAuth(int nServiceType)
{
	IServiceWindow *widget = new IServiceWindow(this, m_pAirableMgr->GetAddr());
	emit SigAddWidget(widget, tr("Internet Service"));
	widget->RequestIServiceURL(nServiceType);
}


void IServiceWindow::SlotRespURL(int nServiceType, QString title, QList<CJsonNode> list)
{
	m_ServiceType = nServiceType;
	m_pInfoService->SetSubtitle(title);
	m_pListService->SetNodeList(list);
}

void IServiceWindow::SlotCoverArtUpdate(QString fileName, int nIndex, int mode)
{
	if (QListView::IconMode == mode)
	{
		QStandardItem *item = m_pIconService->GetModel()->item(nIndex);
		item->setData(fileName, IconServiceDelegate::ICON_SERVICE_COVER);
		m_pIconService->GetModel()->setItem(nIndex, item);
	}
	else
	{
		QStandardItem *item = m_pListService->GetModel()->item(nIndex);
		item->setData(fileName, ListServiceDelegate::LIST_SERVICE_COVER_ART);
		m_pListService->GetModel()->setItem(nIndex, item);
	}
}

void IServiceWindow::ConnectSigToSlot()
{
	connect(this, SIGNAL(SigAddWidget(QWidget*, QString)), parent(), SLOT(SlotAddWidget(QWidget*, QString)));		// recursive

	connect(this, SIGNAL(SigRespLogout()), parent(), SLOT(SlotRespAirableLogout()));

	connect(m_pInfoService, SIGNAL(SigPlayAll()), this, SLOT(SlotPlayAll()));
	connect(m_pInfoService, SIGNAL(SigPlayRandom()), this, SLOT(SlotPlayRandom()));
	connect(m_pInfoService, SIGNAL(SigSubmenu(int)), this, SLOT(SlotSubmenu(int)));
	connect(m_pInfoService, SIGNAL(SigSort()), this, SLOT(SlotSort()));

//	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectPlay(int)), this, SLOT(SlotSelectPlay(int)));
	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int)), this, SLOT(SlotSelectTitle(int)));
	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));

	connect(m_pListService->GetDelegate(), SIGNAL(SigSelectCoverArt(QString)), this, SLOT(SlotSelectURL(QString)));
	connect(m_pListService->GetDelegate(), SIGNAL(SigSelectTitle(QString)), this, SLOT(SlotSelectURL(QString)));
	connect(m_pListService, SIGNAL(SigReqArt(QString, int)), this, SLOT(SlotReqArt(QString, int)));

	connect(m_pQobuzMgr, SIGNAL(SigRespLoginFail(CJsonNode)), this, SLOT(SlotRespQobuzLoginFail(CJsonNode)));
	connect(m_pQobuzMgr, SIGNAL(SigRespLoginSuccess()), this, SLOT(SlotRespQobuzLoginSuccess()));
	connect(m_pQobuzMgr, SIGNAL(SigRespList(QList<CJsonNode>)), this, SLOT(SlotRespList(QList<CJsonNode>)));
	connect(m_pQobuzMgr, SIGNAL(SigRespGenreSubList(QList<CJsonNode>)), this, SLOT(SlotRespGenreSubList(QList<CJsonNode>)));


	connect(m_pAirableMgr, SIGNAL(SigRespLogout()), parent(), SLOT(SlotRespAirableLogout()));
	connect(m_pAirableMgr, SIGNAL(SigRespLoginFail(CJsonNode)), this, SLOT(SlotRespAirableLoginFail(CJsonNode)));
	connect(m_pAirableMgr, SIGNAL(SigRespLoginSuccess(int, bool)), this, SLOT(SlotRespAirableLoginSuccess(int, bool)));

	connect(m_pAirableMgr, SIGNAL(SigRespAuth(int)), this, SLOT(SlotRespAuth(int)));
	connect(m_pAirableMgr, SIGNAL(SigRespURL(int, QString, QList<CJsonNode>)), this, SLOT(SlotRespURL(int, QString, QList<CJsonNode>)));
	connect(m_pAirableMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));

}


void IServiceWindow::DoQobuzHome()
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pIconService);

	QList<CJsonNode> list;
	SetQobuzHome(list);

	m_pInfoService->SetSubtitle(QOBUZ_TITLE);
	m_pIconService->SetNodeList(list, IconService::ICON_SERVICE_ISERVICE);
}

void IServiceWindow::DoQobuzSearch()
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pIconService);

	QList<CJsonNode> list;
	SetQobuzSearch(list);

	m_pInfoService->SetSubtitle(tr("Search"));
	m_pIconService->SetNodeList(list, IconService::ICON_SERVICE_ISERVICE);
}

void IServiceWindow::DoQobuzRecommend()
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pIconService);

	QList<CJsonNode> list;
	SetQobuzRecommend(list);

	m_pInfoService->SetSubtitle(tr("Recommend"));
	m_pIconService->SetNodeList(list, IconService::ICON_SERVICE_ISERVICE);
}

void IServiceWindow::DoQobuzFavorite()
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pIconService);

	QList<CJsonNode> list;
	SetQobuzFavorite(list);

	m_pInfoService->SetSubtitle(tr("Favorite"));
	m_pIconService->SetNodeList(list, IconService::ICON_SERVICE_ISERVICE);
}

void IServiceWindow::DoRecommendAlbum()
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pIconService);

	QList<CJsonNode> list;
	SetRecommendAlbum(list);

	m_pInfoService->SetSubtitle(tr("Album"));
	m_pIconService->SetNodeList(list, IconService::ICON_SERVICE_ISERVICE);
}

void IServiceWindow::DoRecommendPlaylist()
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pIconService);

	QList<CJsonNode> list;
	SetRecommendPlaylist(list);

	m_pInfoService->SetSubtitle(tr("Playlist"));
	m_pIconService->SetNodeList(list, IconService::ICON_SERVICE_ISERVICE);
}

void IServiceWindow::DoRecommendGenre(int nType, QString strID)
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pListService);

	QList<CJsonNode> list;
	SetRecommendGenre(list, strID);

	m_pInfoService->SetSubtitle(tr("Genre"));
	m_pListService->SetNodeList(list);
}
void IServiceWindow::SetIServiceHome(QList<CJsonNode> &list)
{
	QList<CJsonNode> tempList;
	QString strCover = "";
	foreach (CJsonNode node, list)
	{
		if (node.GetInt(KEY_TYPE) == iIServiceType_Qobuz)
		{
			strCover = ":/resource/internets-img160-qobuz-n@3x.png";
		}
		else if (node.GetInt(KEY_TYPE) == iIServiceType_Radios)
		{
			strCover = ":/resource/internets-img160-airable-n@3x.png";
		}
		else if (node.GetInt(KEY_TYPE) == iIServiceType_Podcasts)
		{
			strCover = ":/resource/playlist-img160-albumart-h@3x.png";
		}
		else if (node.GetInt(KEY_TYPE) == iIServiceType_Tidal)
		{
			strCover = ":/resource/internets-img160-tidal-n@3x.png";
		}
		else if (node.GetInt(KEY_TYPE) == iIServiceType_Deezer)
		{
			strCover = ":/resource/internets-img160-deezer-n@3x.png";
		}
		else if (node.GetInt(KEY_TYPE) == iIServiceType_Napster)
		{
			strCover = ":/resource/internets-img160-napster-n@3x.png";
		}
		else if (node.GetInt(KEY_TYPE) == iIServiceType_HighResAudio)
		{
			strCover = ":/resource/playlist-img160-albumart-h@3x.png";
		}
		else if (node.GetInt(KEY_TYPE) == iIServiceType_AmazonMusic)
		{
			strCover = ":/resource/playlist-img160-albumart-h@3x.png";
		}
		else
		{
			LogDebug("not found type");
			continue;
		}
		node.Add(KEY_COVER_ART, strCover);
		tempList.append(node);
	}
	list = tempList;
}

void IServiceWindow::SetQobuzHome(QList<CJsonNode> &list)
{
	CJsonNode nodeSearch(JSON_OBJECT);
	nodeSearch.AddInt(KEY_TYPE, iQobuzType_Mask_Search);
	nodeSearch.AddString(KEY_COVER_ART, ":/resource/playlist-img160-albumart-h@3x.png");
	nodeSearch.AddString(KEY_NAME, "Search");

	CJsonNode nodeRecommend(JSON_OBJECT);
	nodeRecommend.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend);
	nodeRecommend.AddString(KEY_COVER_ART, ":/resource/playlist-img160-albumart-h@3x.png");
	nodeRecommend.AddString(KEY_NAME, "Recommend");

	CJsonNode nodeFavorite(JSON_OBJECT);
	nodeFavorite.AddInt(KEY_TYPE, iQobuzType_Mask_Favorite);
	nodeFavorite.AddString(KEY_COVER_ART, ":/resource/playlist-img160-albumart-h@3x.png");
	nodeFavorite.AddString(KEY_NAME, "Favorite");

	CJsonNode nodePlaylist(JSON_OBJECT);
	nodePlaylist.AddInt(KEY_TYPE, iQobuzType_Mask_UserPlaylist);
	nodePlaylist.AddString(KEY_COVER_ART, ":/resource/playlist-img160-albumart-h@3x.png");
	nodePlaylist.AddString(KEY_NAME, "Playlist");

	list.append(nodeSearch);
	list.append(nodeRecommend);
	list.append(nodeFavorite);
	list.append(nodePlaylist);

}

void IServiceWindow::SetQobuzSearch(QList<CJsonNode> &list)
{
	CJsonNode nodeArtist(JSON_OBJECT);
	nodeArtist.AddInt(KEY_TYPE, iQobuzType_Mask_Search | iQobuzType_Mask_Artist);
	nodeArtist.Add(KEY_NAME, VAL_ARTIST);
	nodeArtist.Add(KEY_CMD1, "m_SearchArtist");
	nodeArtist.Add(KEY_COVER_ART, ":/resource/playlist-img160-albumart-h@3x.png");
	nodeArtist.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeArtist.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Search | iQobuzType_Mask_Artist);
	nodeArtist.AddInt(KEY_MENU_DEPTH, 1);

	CJsonNode nodeAlbum(JSON_OBJECT);
	nodeAlbum.AddInt(KEY_TYPE, iQobuzType_Mask_Search | iQobuzType_Mask_Album);
	nodeAlbum.Add(KEY_NAME, VAL_ALBUM);
	nodeAlbum.Add(KEY_CMD1, "m_SearchAlbum");
	nodeAlbum.Add(KEY_COVER_ART, ":/resource/playlist-img160-albumart-h@3x.png");
	nodeAlbum.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeAlbum.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Search | iQobuzType_Mask_Album);
	nodeAlbum.AddInt(KEY_MENU_DEPTH, 1);

	CJsonNode nodeTrack(JSON_OBJECT);
	nodeTrack.AddInt(KEY_TYPE, iQobuzType_Mask_Search | iQobuzType_Mask_Track);
	nodeTrack.Add(KEY_NAME, VAL_TRACK);
	nodeTrack.Add(KEY_CMD1, "m_SearchTrack");
	nodeTrack.Add(KEY_COVER_ART, ":/resource/playlist-img160-albumart-h@3x.png");
	nodeTrack.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeTrack.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Search | iQobuzType_Mask_Track);
	nodeTrack.AddInt(KEY_MENU_DEPTH, 1);

	CJsonNode nodePlaylist(JSON_OBJECT);
	nodePlaylist.AddInt(KEY_TYPE, iQobuzType_Mask_Search | iQobuzType_Mask_Playlist);
	nodePlaylist.Add(KEY_NAME, "Playlist");
	nodePlaylist.Add(KEY_CMD1, "m_SearchPlaylist");
	nodePlaylist.Add(KEY_COVER_ART, ":/resource/playlist-img160-albumart-h@3x.png");
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
	nodeAlbum.Add(KEY_NAME, VAL_ALBUM);
	nodeAlbum.Add(KEY_CMD1, "m_RecommendAlbum");
	nodeAlbum.Add(KEY_COVER_ART, ":/resource/playlist-img160-albumart-h@3x.png");
	nodeAlbum.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeAlbum.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album);
	nodeAlbum.AddInt(KEY_MENU_DEPTH, 1);

	CJsonNode nodePlaylist(JSON_OBJECT);
	nodePlaylist.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist);
	nodePlaylist.Add(KEY_NAME, "Playlist");
	nodePlaylist.Add(KEY_CMD1, "m_RecommendPlaylist");
	nodePlaylist.Add(KEY_COVER_ART, ":/resource/playlist-img160-albumart-h@3x.png");
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
	nodeArtist.Add(KEY_NAME, VAL_ARTIST);
	nodeArtist.Add(KEY_CMD1, "m_FavoriteArtist");
	nodeArtist.Add(KEY_COVER_ART, ":/resource/playlist-img160-albumart-h@3x.png");
	nodeArtist.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeArtist.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist);
	nodeArtist.AddInt(KEY_MENU_DEPTH, 1);

	CJsonNode nodeAlbum(JSON_OBJECT);
	nodeAlbum.AddInt(KEY_TYPE, iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Album);
	nodeAlbum.Add(KEY_NAME, VAL_ALBUM);
	nodeAlbum.Add(KEY_CMD1, "m_FavoriteAlbum");
	nodeAlbum.Add(KEY_COVER_ART, ":/resource/playlist-img160-albumart-h@3x.png");
	nodeAlbum.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeAlbum.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Album);
	nodeAlbum.AddInt(KEY_MENU_DEPTH, 1);

	CJsonNode nodeTrack(JSON_OBJECT);
	nodeTrack.AddInt(KEY_TYPE, iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Track);
	nodeTrack.Add(KEY_NAME, VAL_TRACK);
	nodeTrack.Add(KEY_CMD1, "m_FavoriteTrack");
	nodeTrack.Add(KEY_COVER_ART, ":/resource/playlist-img160-albumart-h@3x.png");
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
	nodeGenre.Add(KEY_COVER_ART, ":/resource/playlist-img160-albumart-h@3x.png");
	nodeGenre.Add(KEY_NAME, VAL_GENRE);
	nodeGenre.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeGenre.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeGenre.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodeBestSellers(JSON_OBJECT);
	nodeBestSellers.Add(KEY_ID_UPPER, VAL_ID_BEST_SELLERS);
	nodeBestSellers.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeBestSellers.Add(KEY_COVER_ART, ":/resource/playlist-img160-albumart-h@3x.png");
	nodeBestSellers.Add(KEY_NAME, VAL_BEST_SELLERS);
	nodeBestSellers.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeBestSellers.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeBestSellers.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodeMostStreamd(JSON_OBJECT);
	nodeMostStreamd.Add(KEY_ID_UPPER, VAL_ID_MOST_STREAMED);
	nodeMostStreamd.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeMostStreamd.Add(KEY_COVER_ART, ":/resource/playlist-img160-albumart-h@3x.png");
	nodeMostStreamd.Add(KEY_NAME, VAL_MOST_STREAMED);
	nodeMostStreamd.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeMostStreamd.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeMostStreamd.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodeNewReleases(JSON_OBJECT);
	nodeNewReleases.Add(KEY_ID_UPPER, VAL_ID_NEW_RELEASES);
	nodeNewReleases.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeNewReleases.Add(KEY_COVER_ART, ":/resource/playlist-img160-albumart-h@3x.png");
	nodeNewReleases.Add(KEY_NAME, VAL_NEW_RELEASES);
	nodeNewReleases.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeNewReleases.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeNewReleases.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodePressAward(JSON_OBJECT);
	nodePressAward.Add(KEY_ID_UPPER, VAL_ID_PRESS_AWARDS);
	nodePressAward.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodePressAward.Add(KEY_COVER_ART, ":/resource/playlist-img160-albumart-h@3x.png");
	nodePressAward.Add(KEY_NAME, VAL_PRESS_AWARDS);
	nodePressAward.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodePressAward.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodePressAward.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodeEditorPicks(JSON_OBJECT);
	nodeEditorPicks.Add(KEY_ID_UPPER, VAL_ID_EDITOR_PICKS);
	nodeEditorPicks.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeEditorPicks.Add(KEY_COVER_ART, ":/resource/playlist-img160-albumart-h@3x.png");
	nodeEditorPicks.Add(KEY_NAME, VAL_EDITOR_PICKS);
	nodeEditorPicks.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeEditorPicks.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeEditorPicks.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodeMostFeatured(JSON_OBJECT);
	nodeMostFeatured.Add(KEY_ID_UPPER, VAL_ID_MOST_FEATURED);
	nodeMostFeatured.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeMostFeatured.Add(KEY_COVER_ART, ":/resource/playlist-img160-albumart-h@3x.png");
	nodeMostFeatured.Add(KEY_NAME, VAL_MOST_FEATURED);
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
	nodeLastCreated.Add(KEY_ID_UPPER, "last-created");
	nodeLastCreated.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist | iQobuzType_Mask_Playlist);
	nodeLastCreated.Add(KEY_COVER_ART, ":/resource/playlist-img160-albumart-h@3x.png");
	nodeLastCreated.Add(KEY_NAME, "LastCreated");
	nodeLastCreated.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeLastCreated.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist | iQobuzType_Mask_Playlist);
	nodeLastCreated.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodeEditorPicks(JSON_OBJECT);
	nodeEditorPicks.Add(KEY_ID_UPPER, VAL_ID_EDITOR_PICKS);
	nodeEditorPicks.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist | iQobuzType_Mask_Playlist);
	nodeEditorPicks.Add(KEY_COVER_ART, ":/resource/playlist-img160-albumart-h@3x.png");
	nodeEditorPicks.Add(KEY_NAME, VAL_EDITOR_PICKS);
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
	nodeBestSellers.Add(KEY_ICON, ":/resource/playlist-img160-albumart-h@3x.png");
	nodeBestSellers.Add(KEY_TOP, VAL_BEST_SELLERS);
	nodeBestSellers.Add(KEY_GENRE_ID, strID);
	nodeBestSellers.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeBestSellers.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Album);
	nodeBestSellers.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodeMostStreamd(JSON_OBJECT);
	nodeMostStreamd.Add(KEY_ID_UPPER, VAL_ID_MOST_STREAMED);
	nodeMostStreamd.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeMostStreamd.Add(KEY_ICON, ":/resource/playlist-img160-albumart-h@3x.png");
	nodeMostStreamd.Add(KEY_TOP, VAL_MOST_STREAMED);
	nodeMostStreamd.Add(KEY_GENRE_ID, strID);
	nodeMostStreamd.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeMostStreamd.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Album);
	nodeMostStreamd.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodeNewReleases(JSON_OBJECT);
	nodeNewReleases.Add(KEY_ID_UPPER, VAL_ID_NEW_RELEASES);
	nodeNewReleases.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeNewReleases.Add(KEY_ICON, ":/resource/playlist-img160-albumart-h@3x.png");
	nodeNewReleases.Add(KEY_TOP, VAL_NEW_RELEASES);
	nodeNewReleases.Add(KEY_GENRE_ID, strID);
	nodeNewReleases.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeNewReleases.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Album);
	nodeNewReleases.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodePressAward(JSON_OBJECT);
	nodePressAward.Add(KEY_ID_UPPER, VAL_ID_PRESS_AWARDS);
	nodePressAward.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodePressAward.Add(KEY_ICON, ":/resource/playlist-img160-albumart-h@3x.png");
	nodePressAward.Add(KEY_TOP, VAL_PRESS_AWARDS);
	nodePressAward.Add(KEY_GENRE_ID, strID);
	nodePressAward.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodePressAward.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Album);
	nodePressAward.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodeEditorPicks(JSON_OBJECT);
	nodeEditorPicks.Add(KEY_ID_UPPER, VAL_ID_EDITOR_PICKS);
	nodeEditorPicks.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeEditorPicks.Add(KEY_ICON, ":/resource/playlist-img160-albumart-h@3x.png");
	nodeEditorPicks.Add(KEY_TOP, VAL_EDITOR_PICKS);
	nodeEditorPicks.Add(KEY_GENRE_ID, strID);
	nodeEditorPicks.AddInt(KEY_QOBUZ_MENU, iQobuzMenu_CoverView | iQobuzMenu_AddFavorite);
	nodeEditorPicks.AddInt(KEY_ITEM_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre | iQobuzType_Mask_Album);
	nodeEditorPicks.AddInt(KEY_MENU_DEPTH, 2);

	CJsonNode nodeMostFeatured(JSON_OBJECT);
	nodeMostFeatured.Add(KEY_ID_UPPER, VAL_ID_MOST_FEATURED);
	nodeMostFeatured.AddInt(KEY_TYPE, iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre);
	nodeMostFeatured.Add(KEY_ICON, ":/resource/playlist-img160-albumart-h@3x.png");
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

void IServiceWindow::SetInfoTitle(QString title)
{
	m_pInfoService->SetSubtitle(title);
}
