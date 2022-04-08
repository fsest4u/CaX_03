#include "playlistwindow.h"
#include "ui_playlistwindow.h"

#include "dialog/commondialog.h"
#include "dialog/inputnamedialog.h"

#include "manager/playlistmanager.h"
#include "manager/sqlmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/caxtranslate.h"
#include "util/log.h"
#include "util/settingio.h"
#include "util/utilnovatron.h"

#include "widget/form/formplay.h"
#include "widget/form/formsort.h"
#include "widget/formTop/infoservice.h"
#include "widget/formTop/infotracks.h"
#include "widget/formBottom/icontracks.h"
#include "widget/formBottom/icontracksdelegate.h"
#include "widget/formBottom/listtracks.h"
#include "widget/formBottom/listtracksdelegate.h"
#include "widget/musicdbwindow.h"

const QString SETTINGS_GROUP = "Playlist";

PlaylistWindow::PlaylistWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new PlaylistManager),
	m_pInfoService(new InfoService(this)),
	m_pInfoTracks(new InfoTracks(this)),
	m_pIconTracks(new IconTracks(this)),
	m_pListTracks(new ListTracks(this)),
	m_Menu(new QMenu(this)),
	ui(new Ui::PlaylistWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
	ReadSettings();
	Initialize();

}

PlaylistWindow::~PlaylistWindow()
{
//	WriteSettings();

	if (m_pMgr)
	{
		delete m_pMgr;
		m_pMgr = nullptr;
	}

	if (m_pInfoService)
	{
		delete m_pInfoService;
		m_pInfoService = nullptr;
	}

	if (m_pInfoTracks)
	{
		delete m_pInfoTracks;
		m_pInfoTracks = nullptr;
	}

	if (m_pIconTracks)
	{
		delete m_pIconTracks;
		m_pIconTracks = nullptr;
	}

	if (m_pListTracks)
	{
		delete m_pListTracks;
		m_pListTracks = nullptr;
	}

	disconnect(m_Menu, SIGNAL(triggered(QAction*)));
	if (m_Menu)
	{
		delete m_Menu;
		m_Menu = nullptr;
	}

	delete ui;

}

void PlaylistWindow::AddWidgetItem(int typeMode)
{
	m_pInfoService->SetTitle(STR_PLAYLIST);
	m_ListMode = GetListModeFromResize(m_ResizeItem);
	m_TypeMode = typeMode;

	ui->gridLayoutTop->addWidget(m_pInfoService);
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pInfoService->GetFormSort()->SetResize(m_ResizeItem);
		ui->gridLayoutBottom->addWidget(m_pIconTracks);
	}
	else
	{
		m_pInfoService->GetFormSort()->SetResize(m_ResizeItem);
		ui->gridLayoutBottom->addWidget(m_pListTracks);
	}

	if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		m_pInfoService->GetFormPlay()->ShowPlayAll(false);
		m_pInfoService->GetFormSort()->ShowResize(false);
	}
}

void PlaylistWindow::AddWidgetTrack()
{
	m_ListMode = GetListModeFromResize(m_ResizeTrack);
	m_TypeMode = TYPE_MODE_TRACK;

	ui->gridLayoutTop->addWidget(m_pInfoTracks);
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pInfoTracks->GetFormSort()->SetResize(m_ResizeTrack);
		ui->gridLayoutBottom->addWidget(m_pIconTracks);
	}
	else
	{
		m_pInfoTracks->GetFormSort()->SetResize(m_ResizeTrack);
		ui->gridLayoutBottom->addWidget(m_pListTracks);
	}
}

void PlaylistWindow::RequestPlaylist()
{
	m_Loading = UtilNovatron::LoadingStart(parentWidget());

	m_pMgr->RequestPlaylist();
}

void PlaylistWindow::RequestPlaylistInfo(int id, QString coverArt)
{
	m_ID = id;
	m_pInfoTracks->SetCoverArt(coverArt);
	m_pMgr->RequestPlaylistInfo(id);
}

void PlaylistWindow::RequestTrackList(int id)
{
	m_Loading = UtilNovatron::LoadingStart(parentWidget());

	m_pMgr->RequestTrackList(id);
}

void PlaylistWindow::SlotAddWidget(QWidget *widget, QString title)
{
	emit SigAddWidget(widget, title);		// recursive
}

void PlaylistWindow::SlotRemoveWidget(QWidget *widget)
{
	emit SigRemoveWidget(widget);
}

void PlaylistWindow::SlotRespError(QString errMsg)
{
	if (m_Loading)
	{
		UtilNovatron::LoadingStop(m_Loading);
		delete m_Loading;
		m_Loading = nullptr;
	}

	if (!errMsg.isEmpty())
	{
		CommonDialog dialog(this, STR_WARNING, errMsg);
		dialog.exec();
	}
}

void PlaylistWindow::SlotRefresh(CJsonNode node)
{
	Q_UNUSED(node)

	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pIconTracks->ClearNodeList();
	}
	else
	{
		m_pListTracks->ClearNodeList();
	}

	if (m_TypeMode == TYPE_MODE_TRACK)
	{
		RequestTrackList(m_ID);
	}
	else
	{
		RequestPlaylist();
	}
}

void PlaylistWindow::SlotRespPlaylist(QList<CJsonNode> list)
{
	m_RespList.clear();
	m_RespList = list;

	SetOptionMenu();

	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pIconTracks->ClearNodeList();
		m_pIconTracks->SetNodeList(m_RespList, SIDEMENU_PLAYLIST);
	}
	else
	{
		m_pListTracks->ClearNodeList();
		m_pListTracks->SetNodeList(m_RespList, SIDEMENU_PLAYLIST);
	}

	if (m_Loading)
	{
		UtilNovatron::LoadingStop(m_Loading);
		delete m_Loading;
		m_Loading = nullptr;
	}
}

void PlaylistWindow::SlotRespPlaylistInfo(CJsonNode node)
{
//	LogDebug("playlist info [%s]", node.ToCompactByteArray().data());

	QString count = node.GetString(KEY_COUNT);
	QString total = node.GetString(KEY_TOTAL);
	QString extension = node.GetString(KEY_EXTENSION);
	QString samplerate = node.GetString(KEY_SAMPLERATE);
	QString bps = node.GetString(KEY_BPS);

	QString info = count + " | " + total + " | " + extension + " | " + samplerate + " | " + bps;

	m_pInfoTracks->SetTitle(node.GetString(KEY_TITLE));
	m_pInfoTracks->SetSubtitle(QString("%1 : %2").arg(STR_ARTIST).arg(node.GetString(KEY_ARTIST)));
//	m_pInfoTracks->SetInfo(info);
}

void PlaylistWindow::SlotRespTrackList(QList<CJsonNode> list)
{
	m_RespList.clear();
	m_RespList = list;

	SetOptionMenu();

	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pIconTracks->ClearNodeList();
		m_pIconTracks->SetNodeList(m_RespList, SIDEMENU_PLAYLIST);
	}
	else
	{
		m_pListTracks->ClearNodeList();
		m_pListTracks->SetNodeList(m_RespList, SIDEMENU_PLAYLIST);
	}

	if (m_Loading)
	{
		UtilNovatron::LoadingStop(m_Loading);
		delete m_Loading;
		m_Loading = nullptr;
	}
}

void PlaylistWindow::SlotSelectMenu(const QModelIndex &modelIndex, QPoint point)
{
//	if (m_TypeMode == TYPE_MODE_ITEM_TRACK)
//	{
////		m_ID = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_ID));
//		m_ModelIndex = modelIndex;
//	}
//	else if (m_TypeMode == TYPE_MODE_TRACK)
//	{
////		m_TrackID = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_ID));
////		m_TrackCover = qvariant_cast<QString>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_COVER));
////		m_TrackAlbumID = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_ALBUM_ID));
////		m_TrackArtistID = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_ARTIST_ID));
////		m_TrackGenreID = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_GENRE_ID));
////		m_TrackFavorite = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_FAVORITE));
////		m_TrackIndex = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_INDEX));
//		m_ModelIndex = modelIndex;
//	}

	m_ModelIndex = modelIndex;

//	LogDebug("id [%d] x [%d] y [%d]", m_ID, point.x(), point.y());
//	LogDebug("index [%d] favorite [%d] album [%d] artist [%d] genre [%d]", m_TrackIndex, m_TrackFavorite, m_TrackAlbumID, m_TrackArtistID, m_TrackGenreID);

	SetOptionMenu();

	m_Menu->clear();

	QMap<int, QString>::iterator i;
	for (i = m_OptionMenuMap.begin(); i != m_OptionMenuMap.end(); i++)
	{
		QIcon icon = UtilNovatron::GetMenuIcon(i.value());
		QAction *action = new QAction(icon, i.value(), this);
		action->setData(i.key());
		m_Menu->addAction(action);
	}

	m_Menu->popup(m_pListTracks->GetListView()->viewport()->mapToGlobal(point));
}

void PlaylistWindow::SlotMenuAction(QAction *action)
{
//	if (m_TypeMode == TYPE_MODE_ITEM_TRACK)
//	{
//		SlotOptionMenuAction(m_ModelIndex, action->data().toInt());
//	}
//	else if (m_TypeMode == TYPE_MODE_TRACK)
//	{
//		SlotOptionMenuAction(m_ModelIndex, action->data().toInt());
//	}

	SlotOptionMenuAction(m_ModelIndex, action->data().toInt());
}

void PlaylistWindow::SlotReqCoverArt(const QModelIndex &modelIndex, int mode)
{
	int id;
	int index;

	if (m_ListMode == VIEW_MODE_ICON)
	{
		id = qvariant_cast<int>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_ID));
		index = qvariant_cast<int>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_INDEX));
	}
	else
	{
		id = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_ID));
		index = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_INDEX));
	}


	QString strCat;
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK || m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		strCat = KEY_PLS;
	}
	else
	{
		strCat = KEY_SONG;
	}

	QStringList lsAddr = m_pMgr->GetAddr().split(":");
	QString fullpath = QString("%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(strCat).arg(id);

	m_pMgr->RequestCoverArt(fullpath, index, mode);
}

void PlaylistWindow::SlotCoverArtUpdate(QString coverArt, int index, int mode)
{
	if (QListView::IconMode == mode)
	{
		QModelIndex modelIndex = m_pIconTracks->GetModel()->index(index, 0);
		m_pIconTracks->GetModel()->setData(modelIndex, coverArt, IconTracksDelegate::ICON_TRACKS_COVER);
	}
	else
	{
		QModelIndex modelIndex = m_pListTracks->GetModel()->index(index, 0);
		m_pListTracks->GetModel()->setData(modelIndex, coverArt, ListTracksDelegate::LIST_TRACKS_COVER);
	}
}

void PlaylistWindow::SlotSelectTitle(const QModelIndex &modelIndex)
{
	int id;
	QString cover;

	if (m_ListMode == VIEW_MODE_ICON)
	{
		id = qvariant_cast<int>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_ID));
		cover = qvariant_cast<QString>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_COVER));
	}
	else
	{
		id = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_ID));
		cover = qvariant_cast<QString>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_COVER));
	}

	if (m_TypeMode == TYPE_MODE_ITEM_TRACK)
	{
		PlaylistWindow *widget = new PlaylistWindow(this, m_pMgr->GetAddr());
		widget->AddWidgetTrack();
		emit widget->SigAddWidget(widget, STR_PLAYLIST);
		widget->RequestPlaylistInfo(id, cover);
		widget->RequestTrackList(id);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		emit SigAddToPlaylist(id);
		emit SigRemoveWidget(this);
	}
}

void PlaylistWindow::SlotSelectPlay(const QModelIndex &modelIndex, int playType)
{
	int id;
	int index;

	if (m_ListMode == VIEW_MODE_ICON)
	{
		id = qvariant_cast<int>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_ID));
		index = qvariant_cast<int>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_INDEX));
	}
	else
	{
		id = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_ID));
		index = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_INDEX));
	}

	QMap<int, int> map;
	map.insert(index, id);
	m_pMgr->RequestPlayPlaylist(map, playType);
}

void PlaylistWindow::SlotSelectTrackPlay(const QModelIndex &modelIndex, int playType)
{
	int id;
	int index;

	if (m_ListMode == VIEW_MODE_ICON)
	{
		id = qvariant_cast<int>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_ID));
		index = qvariant_cast<int>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_INDEX));
	}
	else
	{
		id = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_ID));
		index = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_INDEX));
	}

	QMap<int, int> map;
	map.insert(index, id);
	m_pMgr->RequestPlayTrack(map, playType);
}

void PlaylistWindow::SlotPlayAll()
{
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pIconTracks->SetAllSelectMap();
		m_SelectMap = m_pIconTracks->GetSelectMap();
	}
	else
	{
		m_pListTracks->SetAllSelectMap();
		m_SelectMap = m_pListTracks->GetSelectMap();
	}

	m_pMgr->RequestPlayPlaylist(m_SelectMap, PLAY_CLEAR);
}

void PlaylistWindow::SlotPlayRandom()
{
	m_pMgr->RequestRandom();
}

void PlaylistWindow::SlotTopMenu()
{
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_SelectMap = m_pIconTracks->GetSelectMap();
	}
	else
	{
		m_SelectMap = m_pListTracks->GetSelectMap();
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

void PlaylistWindow::SlotTopMenuAction(int menuID)
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
	case TOP_MENU_MAKE_PLAYLIST:
		DoTopMenuMakePlaylist();
		break;
	case TOP_MENU_DELETE:
		DoTopMenuDelete();
		break;
	case TOP_MENU_RENAME:
		DoTopMenuRename();
		break;
	case TOP_MENU_ADD_TO_PLAYLIST:
		DoTopMenuAddToPlaylist();
		break;
	}

}

void PlaylistWindow::SlotResize(int resize)
{
	if (m_TypeMode <= TYPE_MODE_ITEM_ADD)
	{
		m_ResizeItem = resize;
	}
	else
	{
		m_ResizeTrack = resize;
	}
	WriteSettings();

	int listMode = GetListModeFromResize(resize);
	if (listMode != m_ListMode)
	{
		m_ListMode = listMode;
		if (m_ListMode == VIEW_MODE_ICON)
		{
//			LogDebug("icon~~~~~~~~");
			if (m_pIconTracks->GetNodeList().count() != m_RespList.count())
			{
				m_pIconTracks->ClearNodeList();
				m_pIconTracks->SetNodeList(m_RespList, SIDEMENU_PLAYLIST);
			}

			m_pListTracks->hide();
			m_pIconTracks->show();
			ui->gridLayoutBottom->replaceWidget(m_pListTracks, m_pIconTracks);
		}
		else
		{
//			LogDebug("list~~~~~~~~");
			if (m_pListTracks->GetNodeList().count() != m_RespList.count())
			{
				m_pListTracks->ClearNodeList();
				m_pListTracks->SetNodeList(m_RespList, SIDEMENU_PLAYLIST);
			}

			m_pIconTracks->hide();
			m_pListTracks->show();
			ui->gridLayoutBottom->replaceWidget(m_pIconTracks, m_pListTracks);
		}
	}

	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pIconTracks->SetResize(resize);
	}
	else
	{
		m_pListTracks->SetResize(resize);
	}
}

void PlaylistWindow::SlotItemPlayAll()
{
	m_pListTracks->SetAllSelectMap();
	m_SelectMap = m_pListTracks->GetSelectMap();
	m_pMgr->RequestPlayTrack(m_SelectMap, PLAY_CLEAR);
}

void PlaylistWindow::SlotItemPlayRandom()
{
	m_pMgr->RequestRandom();
}

void PlaylistWindow::SlotItemTopMenu()
{
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_SelectMap = m_pIconTracks->GetSelectMap();
	}
	else
	{
		m_SelectMap = m_pListTracks->GetSelectMap();
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

void PlaylistWindow::SlotItemTopMenuAction(int menuID)
{
	switch (menuID) {
	case TOP_MENU_PLAY_NOW:
		DoTopMenuItemPlay(PLAY_NOW);
		break;
	case TOP_MENU_PLAY_LAST:
		DoTopMenuItemPlay(PLAY_LAST);
		break;
	case TOP_MENU_PLAY_NEXT:
		DoTopMenuItemPlay(PLAY_NEXT);
		break;
	case TOP_MENU_PLAY_CLEAR:
		DoTopMenuItemPlay(PLAY_CLEAR);
		break;
	case TOP_MENU_SELECT_ALL:
		DoTopMenuItemSelectAll();
		break;
	case TOP_MENU_CLEAR_ALL:
		DoTopMenuItemClearAll();
		break;
	case TOP_MENU_ADD_TO_PLAYLIST:
		DoTopMenuItemAddToPlaylist();
		break;
	case TOP_MENU_DELETE_FROM_PLAYLIST:
		DoTopMenuItemDeleteFromPlaylist();
		break;
	}
}

void PlaylistWindow::SlotOptionMenuAction(const QModelIndex &modelIndex, int menuID)
{
	switch (menuID) {
	case OPTION_MENU_PLAY_NOW:
		DoOptionMenuPlay(modelIndex, PLAY_NOW);
		break;
	case OPTION_MENU_PLAY_LAST:
		DoOptionMenuPlay(modelIndex, PLAY_LAST);
		break;
	case OPTION_MENU_PLAY_NEXT:
		DoOptionMenuPlay(modelIndex, PLAY_NEXT);
		break;
	case OPTION_MENU_PLAY_CLEAR:
		DoOptionMenuPlay(modelIndex, PLAY_CLEAR);
		break;
	case OPTION_MENU_RENAME:
		DoOptionMenuRename(modelIndex);
		break;
	case OPTION_MENU_DELETE:
	case OPTION_MENU_DELETE_FROM_PLAYLIST:
		DoOptionMenuDelete(modelIndex);
		break;
	case OPTION_MENU_ADD_TO_PLAYLIST:
		DoOptionMenuAddToPlaylist(modelIndex);
		break;
	case OPTION_MENU_FAVORITE:
		DoOptionMenuFavorite();
		break;
	case OPTION_MENU_GO_TO_ALBUM:
		DoOptionMenuGoToAlbum();
		break;
	case OPTION_MENU_GO_TO_ARTIST:
		DoOptionMenuGoToArtist();
		break;
	}
}

void PlaylistWindow::SlotAddCategoryFromPlaylist(int category, QMap<int, int> idMap)
{
//	LogDebug("playlist id [%d] cat [%d] id [%d]", m_ID, category);
	m_pMgr->RequestAddCategoryFromPlaylist(m_ID, idMap, category);
}

void PlaylistWindow::SlotAddTrackFromPlaylist(QMap<int, int> idMap)
{
//	LogDebug("playlist id [%d] track [%d]", m_ID);
	m_pMgr->RequestAddTrackFromPlaylist(m_ID, idMap);
}

void PlaylistWindow::ReadSettings()
{
	SettingIO settings;
	settings.beginGroup(SETTINGS_GROUP);

	m_ResizeItem = settings.value("resize_item_value").toInt();
	m_ResizeTrack = settings.value("resize_track_value").toInt();

	settings.endGroup();

	if (m_ResizeItem <= 0)
	{
		m_ResizeItem = ICON_HEIGHT_MAX;
	}
	if (m_ResizeTrack <= 0)
	{
		m_ResizeTrack = LIST_HEIGHT_MIN;
	}
//	LogDebug("read resize item [%d] resize track [%d]", m_ResizeItem, m_ResizeTrack);
}

void PlaylistWindow::WriteSettings()
{
	SettingIO settings;
	settings.beginGroup(SETTINGS_GROUP);

	settings.setValue("resize_item_value", m_ResizeItem);
	settings.setValue("resize_track_value", m_ResizeTrack);

	settings.endGroup();

//	LogDebug("write resize item [%d] resize track [%d]", m_ResizeItem, m_ResizeTrack);
}

void PlaylistWindow::ConnectSigToSlot()
{
	connect(this, SIGNAL(SigAddWidget(QWidget*, QString)), parent(), SLOT(SlotAddWidget(QWidget*, QString)));		// recursive
	connect(this, SIGNAL(SigRemoveWidget(QWidget*)), parent(), SLOT(SlotRemoveWidget(QWidget*)));

	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRefresh(CJsonNode)), this, SLOT(SlotRefresh(CJsonNode)));

	connect(m_pMgr, SIGNAL(SigRespPlaylist(QList<CJsonNode>)), this, SLOT(SlotRespPlaylist(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespPlaylistInfo(CJsonNode)), this, SLOT(SlotRespPlaylistInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespTrackList(QList<CJsonNode>)), this, SLOT(SlotRespTrackList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));

	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigPlayAll()), this, SLOT(SlotPlayAll()));
	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigPlayRandom()), this, SLOT(SlotPlayRandom()));
	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigMenu()), this, SLOT(SlotTopMenu()));
	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigMenuAction(int)), this, SLOT(SlotTopMenuAction(int)));
	connect(m_pInfoService->GetFormSort(), SIGNAL(SigResize(int)), this, SLOT(SlotResize(int)));

	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigPlayAll()), this, SLOT(SlotItemPlayAll()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigPlayRandom()), this, SLOT(SlotItemPlayRandom()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigMenu()), this, SLOT(SlotItemTopMenu()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigMenuAction(int)), this, SLOT(SlotItemTopMenuAction(int)));
	connect(m_pInfoTracks->GetFormSort(), SIGNAL(SigResize(int)), this, SLOT(SlotResize(int)));

	connect(m_pIconTracks, SIGNAL(SigReqCoverArt(const QModelIndex&, int)), this, SLOT(SlotReqCoverArt(const QModelIndex&, int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectPlay(const QModelIndex&, int)), this, SLOT(SlotSelectPlay(const QModelIndex&, int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectTitle(const QModelIndex&)), this, SLOT(SlotSelectTitle(const QModelIndex&)));
//	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectSubtitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));

	connect(m_pListTracks, SIGNAL(SigReqCoverArt(const QModelIndex&, int)), this, SLOT(SlotReqCoverArt(const QModelIndex&, int)));
	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectPlay(const QModelIndex&, int)), this, SLOT(SlotSelectTrackPlay(const QModelIndex&, int)));
	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectTitle(const QModelIndex&)), this, SLOT(SlotSelectTitle(const QModelIndex&)));
	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectMenu(const QModelIndex&, QPoint)), this, SLOT(SlotSelectMenu(const QModelIndex&, QPoint)));


	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuAction(QAction*)));

}

void PlaylistWindow::Initialize()
{
	m_pInfoService->GetFormPlay()->ShowPlayAll();
	m_pInfoService->GetFormPlay()->ShowMenu();
	m_pInfoService->GetFormSort()->ShowResize();
	m_pInfoService->GetFormSort()->SetSliderMinimum(LIST_HEIGHT_MIN);

	m_pInfoTracks->GetFormPlay()->ShowPlayAll();
	m_pInfoTracks->GetFormPlay()->ShowMenu();
	m_pInfoTracks->GetFormSort()->ShowResize();
	m_pInfoTracks->GetFormSort()->SetSliderMinimum(LIST_HEIGHT_MIN);

	m_TopMenuMap.clear();
	m_SelectMap.clear();

//	m_ListMode = GetListModeFromResize(m_Resize);

	m_Loading = nullptr;

	m_ID = -1;
//	m_TrackID = -1;
//	m_TrackAlbumID = -1;
//	m_TrackArtistID = -1;
//	m_TrackGenreID = -1;
//	m_TrackFavorite = -1;
//	m_TrackIndex = -1;
	m_TrackCover.clear();

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

int PlaylistWindow::GetListModeFromResize(int resize)
{
	if (resize > ICON_HEIGHT_MID)
	{
		return VIEW_MODE_ICON;
	}
	else
	{
		return VIEW_MODE_LIST;
	}
}

void PlaylistWindow::SetSelectOffTopMenu()
{
	m_TopMenuMap.clear();

	if (m_TypeMode == TYPE_MODE_ITEM_TRACK)
	{
		m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
		m_TopMenuMap.insert(TOP_MENU_MAKE_PLAYLIST, STR_MAKE_PLAYLIST);

		m_pInfoService->GetFormPlay()->ClearMenu();
		m_pInfoService->GetFormPlay()->SetMenu(m_TopMenuMap);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		m_TopMenuMap.insert(TOP_MENU_MAKE_PLAYLIST, STR_MAKE_PLAYLIST);

		m_pInfoService->GetFormPlay()->ClearMenu();
		m_pInfoService->GetFormPlay()->SetMenu(m_TopMenuMap);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK)
	{
		m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
		m_TopMenuMap.insert(TOP_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);

		m_pInfoTracks->GetFormPlay()->ClearMenu();
		m_pInfoTracks->GetFormPlay()->SetMenu(m_TopMenuMap);
	}

}

void PlaylistWindow::SetSelectOnTopMenu()
{
	m_TopMenuMap.clear();

	if (m_TypeMode == TYPE_MODE_ITEM_TRACK)
	{
		m_TopMenuMap.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_TopMenuMap.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_TopMenuMap.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_TopMenuMap.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_TopMenuMap.insert(TOP_MENU_CLEAR_ALL, STR_CLEAR_ALL);
		m_TopMenuMap.insert(TOP_MENU_RENAME, STR_RENAME);
		m_TopMenuMap.insert(TOP_MENU_DELETE, STR_DELETE);
//		m_TopMenuMap.insert(TOP_MENU_EXPORT_TRACK, STR_EXPORT_TRACK);

		m_pInfoService->GetFormPlay()->ClearMenu();
		m_pInfoService->GetFormPlay()->SetMenu(m_TopMenuMap);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		m_TopMenuMap.insert(TOP_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);

		m_pInfoService->GetFormPlay()->ClearMenu();
		m_pInfoService->GetFormPlay()->SetMenu(m_TopMenuMap);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK)
	{
		m_TopMenuMap.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_TopMenuMap.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_TopMenuMap.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_TopMenuMap.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_TopMenuMap.insert(TOP_MENU_CLEAR_ALL, STR_CLEAR_ALL);
		m_TopMenuMap.insert(TOP_MENU_DELETE_FROM_PLAYLIST, STR_DELETE_FROM_PLAYLIST);

		m_pInfoTracks->GetFormPlay()->ClearMenu();
		m_pInfoTracks->GetFormPlay()->SetMenu(m_TopMenuMap);
	}
}

void PlaylistWindow::DoTopMenuPlay(int nWhere)
{
	if (m_SelectMap.count() > 0)
	{
		m_pMgr->RequestPlayPlaylist(m_SelectMap, nWhere);
	}
}

void PlaylistWindow::DoTopMenuSelectAll()
{
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pIconTracks->SetAllSelectMap();
	}
	else
	{
		m_pListTracks->SetAllSelectMap();
	}
}

void PlaylistWindow::DoTopMenuClearAll()
{
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pIconTracks->ClearSelectMap();
	}
	else
	{
		m_pListTracks->ClearSelectMap();
	}
}

void PlaylistWindow::DoTopMenuMakePlaylist()
{
	InputNameDialog dialog;
	if (dialog.exec() == QDialog::Accepted)
	{
		QString name = dialog.GetName();
		m_pMgr->RequestAddPlaylist(name);
	}
}

void PlaylistWindow::DoTopMenuDelete()
{
	if (m_SelectMap.count() <= 0)
	{
		return;
	}

	int idAutoPlay = -1;
	foreach (CJsonNode tempNode, m_RespList)
	{
		if (tempNode.GetString(KEY_TITLE) == STR_AUTO_PLAY)
		{
			idAutoPlay = tempNode.GetInt(KEY_ID_LOWER);
			break;
		}
	}

	bool bAutoPlay = false;
	QMap<int, int>::iterator i;
	for (i = m_SelectMap.begin(); i!= m_SelectMap.end(); i++)
	{
		if ((int64_t)i.value() == idAutoPlay)
		{
			bAutoPlay = true;
			break;
		}
	}

	if(bAutoPlay)
	{
		CommonDialog dialog(this, STR_WARNING, STR_CANNOT_DELETE_AUTO_PLAY);
		dialog.exec();
	}
	else
	{
		m_pMgr->RequestDeletePlaylist(m_SelectMap);
	}
}

void PlaylistWindow::DoTopMenuRename()
{
	if (m_SelectMap.count() > 1)
	{
		CommonDialog dialog(this, STR_WARNING, STR_SELECT_ONLY_ONE_ITEM);
		dialog.exec();
	}
	else if (m_SelectMap.count() == 1)
	{
		int id = -1;
		QMap<int, int>::iterator i;
		for (i = m_SelectMap.begin(); i!= m_SelectMap.end(); i++)
		{
			id = i.value();
			break;
		}

		CJsonNode node;
		foreach (CJsonNode tempNode, m_RespList)
		{
			if (tempNode.GetInt(KEY_ID_LOWER) == id)
			{
				node = tempNode;
//				LogDebug("node [%s]", node.ToCompactByteArray().data());
				break;
			}
		}

		if (node.GetString(KEY_TITLE) == STR_AUTO_PLAY)
		{
			CommonDialog dialog(this, STR_WARNING, STR_CANNOT_RENAME_AUTO_PLAY);
			dialog.exec();
		}
		else
		{
			InputNameDialog dialog;
			dialog.SetName(node.GetString(KEY_TITLE));
			if (dialog.exec() == QDialog::Accepted)
			{
				QString name = dialog.GetName();
				m_pMgr->RequestRenamePlaylist(id, name);
			}
		}
	}
}

void PlaylistWindow::DoTopMenuAddToPlaylist()
{
	if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		if (m_SelectMap.count() <= 0)
		{
			CommonDialog dialog(this, STR_WARNING, STR_NO_SELECT);
			dialog.exec();
		}
		else if (m_SelectMap.count() > 1)
		{
			CommonDialog dialog(this, STR_WARNING, STR_SELECT_ONLY_ONE_ITEM);
			dialog.exec();
		}
		else if (m_SelectMap.count() == 1)
		{
			int id = -1;
			QMap<int, int>::iterator i;
			for (i = m_SelectMap.begin(); i!= m_SelectMap.end(); i++)
			{
				id = i.value();
				break;
			}
			emit SigAddToPlaylist(id);
			emit SigRemoveWidget(this);
		}
	}
	else
	{
		LogWarning("bad choice");
	}
}

void PlaylistWindow::DoTopMenuItemPlay(int nWhere)
{
	if (m_SelectMap.count() > 0)
	{
		m_pMgr->RequestPlayTrack(m_SelectMap, nWhere);
	}

}

void PlaylistWindow::DoTopMenuItemSelectAll()
{
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pIconTracks->SetAllSelectMap();
	}
	else
	{
		m_pListTracks->SetAllSelectMap();
	}
}

void PlaylistWindow::DoTopMenuItemClearAll()
{
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pIconTracks->ClearSelectMap();
	}
	else
	{
		m_pListTracks->ClearSelectMap();
	}
}

void PlaylistWindow::DoTopMenuItemAddToPlaylist()
{
	if (m_TypeMode == TYPE_MODE_TRACK)
	{
		MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), -1);
		widget->AddWidgetItem(TYPE_MODE_ITEM_ADD);
		emit widget->SigAddWidget(widget, STR_MUSIC_DB);
		widget->RequestCategoryList();

		connect(widget, SIGNAL(SigAddCategoryFromPlaylist(int, QMap<int, int>)), this, SLOT(SlotAddCategoryFromPlaylist(int, QMap<int, int>)));
		connect(widget, SIGNAL(SigAddTrackFromPlaylist(QMap<int, int>)), this, SLOT(SlotAddTrackFromPlaylist(QMap<int, int>)));
	}
}

void PlaylistWindow::DoTopMenuItemDeleteFromPlaylist()
{
	if (m_SelectMap.count() > 0)
	{
		m_pMgr->RequestDelTrack(m_ID, m_SelectMap);
	}
}

void PlaylistWindow::SetOptionMenu()
{
	m_OptionMenuMap.clear();
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK)
	{
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_OptionMenuMap.insert(OPTION_MENU_RENAME, STR_RENAME);
		m_OptionMenuMap.insert(OPTION_MENU_DELETE, STR_DELETE);
		m_OptionMenuMap.insert(OPTION_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK)
	{
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_OptionMenuMap.insert(OPTION_MENU_DELETE_FROM_PLAYLIST, STR_DELETE_FROM_PLAYLIST);

		int favorite = qvariant_cast<int>(m_ModelIndex.data(ListTracksDelegate::LIST_TRACKS_FAVORITE));
		if (favorite == 1)
		{
			m_OptionMenuMap.insert(OPTION_MENU_FAVORITE, STR_DELETE_TO_FAVORITE);
		}
		else
		{
			m_OptionMenuMap.insert(OPTION_MENU_FAVORITE, STR_ADD_TO_FAVORITE);
		}

		m_OptionMenuMap.insert(OPTION_MENU_GO_TO_ALBUM, STR_GO_TO_ALBUM);
		m_OptionMenuMap.insert(OPTION_MENU_GO_TO_ARTIST, STR_GO_TO_ARTIST);
	}

}

void PlaylistWindow::DoOptionMenuPlay(const QModelIndex &modelIndex, int where)
{
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK)
	{
		int id = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_ID));
		int index = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_INDEX));

		QMap<int, int> map;
		map.insert(index, id);
		m_pMgr->RequestPlayPlaylist(map, where);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK)
	{
		SlotSelectPlay(modelIndex, where);
	}
}

void PlaylistWindow::DoOptionMenuRename(const QModelIndex &modelIndex)
{
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK)
	{
		int id = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_ID));

		CJsonNode node;
		foreach (CJsonNode tempNode, m_RespList)
		{
			if (tempNode.GetInt(KEY_ID_LOWER) == id)
			{
				node = tempNode;
//				LogDebug("node [%s]", node.ToCompactByteArray().data());
				break;
			}
		}

		if (node.GetString(KEY_TITLE) == STR_AUTO_PLAY)
		{
			CommonDialog dialog(this, STR_WARNING, STR_CANNOT_RENAME_AUTO_PLAY);
			dialog.exec();
		}
		else
		{
			InputNameDialog dialog;
			dialog.SetName(node.GetString(KEY_TITLE));
			if (dialog.exec() == QDialog::Accepted)
			{
				QString name = dialog.GetName();
				m_pMgr->RequestRenamePlaylist(id, name);
			}
		}

	}
	else if (m_TypeMode == TYPE_MODE_TRACK)
	{
		LogWarning("invalid type");
	}
}

void PlaylistWindow::DoOptionMenuDelete(const QModelIndex &modelIndex)
{
	int id = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_ID));
	int index = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_INDEX));

	QMap<int, int> map;
	map.insert(index, id);

	if (m_TypeMode == TYPE_MODE_ITEM_TRACK)
	{
		CJsonNode node;
		foreach (CJsonNode tempNode, m_RespList)
		{
			if (tempNode.GetInt(KEY_ID_LOWER) == id)
			{
				node = tempNode;
//				LogDebug("node [%s]", node.ToCompactByteArray().data());
				break;
			}
		}

		if (node.GetString(KEY_TITLE) == STR_AUTO_PLAY)
		{
			CommonDialog dialog(this, STR_WARNING, STR_CANNOT_DELETE_AUTO_PLAY);
			dialog.exec();
		}
		else
		{
			m_pMgr->RequestDeletePlaylist(map);
		}
	}
	else if (m_TypeMode == TYPE_MODE_TRACK)
	{
		m_pMgr->RequestDelTrack(m_ID, map);
	}
}

void PlaylistWindow::DoOptionMenuAddToPlaylist(const QModelIndex &modelIndex)
{
	m_ID = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_ID));

	if (m_TypeMode == TYPE_MODE_ITEM_TRACK)
	{
		MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), -1);
		widget->AddWidgetItem(TYPE_MODE_ITEM_ADD);
		emit widget->SigAddWidget(widget, STR_MUSIC_DB);
		widget->RequestCategoryList();

		connect(widget, SIGNAL(SigAddCategoryFromPlaylist(int, QMap<int, int>)), this, SLOT(SlotAddCategoryFromPlaylist(int, QMap<int, int>)));
		connect(widget, SIGNAL(SigAddTrackFromPlaylist(QMap<int, int>)), this, SLOT(SlotAddTrackFromPlaylist(QMap<int, int>)));
	}
}


void PlaylistWindow::DoOptionMenuFavorite()
{
	int favorite = qvariant_cast<int>(m_ModelIndex.data(ListTracksDelegate::LIST_TRACKS_FAVORITE));
	int index = qvariant_cast<int>(m_ModelIndex.data(ListTracksDelegate::LIST_TRACKS_INDEX));

	favorite = favorite == 0 ? 1 : 0;
//	LogDebug("DoTopMenuItemFavorite favorite [%d]", m_TrackFavorite);
	QModelIndex modelIndex = m_pListTracks->GetModel()->index(index, 0);
	m_pListTracks->GetModel()->setData(modelIndex, favorite, ListTracksDelegate::LIST_TRACKS_FAVORITE);

	m_pMgr->RequestUpdateTrackFavorite(m_ID, favorite);

}

void PlaylistWindow::DoOptionMenuGoToAlbum()
{
	int albumID = qvariant_cast<int>(m_ModelIndex.data(ListTracksDelegate::LIST_TRACKS_ALBUM_ID));
	QString cover = qvariant_cast<QString>(m_ModelIndex.data(ListTracksDelegate::LIST_TRACKS_COVER));
//	LogDebug("DoOptionMenuGoToAlbum id [%d]", albumID);
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), -1);
	widget->AddWidgetTrack(TYPE_MODE_TRACK, SQLManager::CATEGORY_ALBUM);
	emit widget->SigAddWidget(widget, STR_MUSIC_DB);
	widget->RequestTrackList(albumID);
	widget->SetCoverArt(cover);
}

void PlaylistWindow::DoOptionMenuGoToArtist()
{
	int artistID = qvariant_cast<int>(m_ModelIndex.data(ListTracksDelegate::LIST_TRACKS_ARTIST_ID));

//	LogDebug("DoOptionMenuGoToArtist id [%d]", artistID);
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), -1);
	widget->AddWidgetItem(TYPE_MODE_ITEM_ALBUM, SQLManager::CATEGORY_ARTIST);
	emit widget->SigAddWidget(widget, STR_MUSIC_DB);
	widget->RequestCategoryList(artistID);
}
