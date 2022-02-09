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
	m_pMgr->RequestTrackList(id);
}

void PlaylistWindow::SlotAddWidget(QWidget *widget, QString title)
{
	emit SigAddWidget(widget, STR_PLAYLIST);		// recursive
}

void PlaylistWindow::SlotRemoveWidget(QWidget *widget)
{
	emit SigRemoveWidget(widget);
}

void PlaylistWindow::SlotRespError(QString errMsg)
{
	CommonDialog dialog(this, STR_WARNING, errMsg);
	dialog.exec();
}

void PlaylistWindow::SlotRespPlaylist(QList<CJsonNode> list)
{
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
}

void PlaylistWindow::SlotSelectMenu(const QModelIndex &modelIndex, QPoint point)
{
	m_ID = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_ID));
	m_TrackCover = qvariant_cast<QString>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_COVER));
	m_TrackAlbumID = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_ALBUM_ID));
	m_TrackArtistID = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_ARTIST_ID));
	m_TrackGenreID = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_GENRE_ID));
	m_TrackFavorite = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_FAVORITE));
	m_TrackIndex = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_INDEX));

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
	SlotOptionMenuAction(m_ID, action->data().toInt());
}

void PlaylistWindow::SlotReqCoverArt(int id, int index, int mode)
{
	QString strCat;
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK)
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

void PlaylistWindow::SlotSelectTitle(int id, QString coverArt)
{
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK)
	{
		PlaylistWindow *widget = new PlaylistWindow(this, m_pMgr->GetAddr());
		widget->AddWidgetTrack();
		emit widget->SigAddWidget(widget, STR_PLAYLIST);

		widget->RequestPlaylistInfo(id, coverArt);
		widget->RequestTrackList(id);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		emit SigAddToPlaylist(id);
		emit SigRemoveWidget(this);
	}
}

void PlaylistWindow::SlotSelectPlay(int id, int playType)
{
//	LogDebug("click Count [%d]", id);
	QMap<int, bool> map;
	map.insert(id, true);
	m_pMgr->RequestPlayPlaylist(map, playType);
}

void PlaylistWindow::SlotSelectTrackPlay(int id, int playType)
{
	QMap<int, bool> map;
	map.insert(id, true);
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
	case TOP_MENU_ADD:
		DoTopMenuAdd();
		break;
	case TOP_MENU_DELETE:
		DoTopMenuDelete();
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
			LogDebug("icon~~~~~~~~");
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
			LogDebug("list~~~~~~~~");
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
	case TOP_MENU_DELETE_TO_PLAYLIST:
		DoTopMenuItemDeleteToPlaylist();
		break;
	}
}

void PlaylistWindow::SlotOptionMenuAction(int nID, int menuID)
{
	switch (menuID) {
	case OPTION_MENU_PLAY_NOW:
		DoOptionMenuPlay(nID, PLAY_NOW);
		break;
	case OPTION_MENU_PLAY_LAST:
		DoOptionMenuPlay(nID, PLAY_LAST);
		break;
	case OPTION_MENU_PLAY_NEXT:
		DoOptionMenuPlay(nID, PLAY_NEXT);
		break;
	case OPTION_MENU_PLAY_CLEAR:
		DoOptionMenuPlay(nID, PLAY_CLEAR);
		break;
	case OPTION_MENU_RENAME:
		DoOptionMenuRename(nID);
		break;
	case OPTION_MENU_DELETE:
	case OPTION_MENU_DELETE_TO_PLAYLIST:
		DoOptionMenuDelete(nID);
		break;
	case OPTION_MENU_ADD_TO_PLAYLIST:
		DoOptionMenuAddToPlaylist(nID);
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

void PlaylistWindow::SlotAddCategoryFromPlaylist(int category, QMap<int, bool> idMap)
{
//	LogDebug("playlist id [%d] cat [%d] id [%d]", m_ID, category);
	// add category
	m_pMgr->RequestAddCategoryFromPlaylist(m_ID, idMap, category);
	// refresh
	if (m_TypeMode == TYPE_MODE_TRACK)
	{
		m_pMgr->RequestTrackList(m_ID);
	}
}

void PlaylistWindow::SlotAddTrackFromPlaylist(QMap<int, bool> idMap)
{
//	LogDebug("playlist id [%d] track [%d]", m_ID);
	// add track
	m_pMgr->RequestAddTrackFromPlaylist(m_ID, idMap);
	// refresh
	if (m_TypeMode == TYPE_MODE_TRACK)
	{
		m_pMgr->RequestTrackList(m_ID);
	}
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

	connect(m_pIconTracks, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectPlay(int, int)), this, SLOT(SlotSelectPlay(int, int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectSubtitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));

	connect(m_pListTracks, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));
	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectPlay(int, int)), this, SLOT(SlotSelectTrackPlay(int, int)));
	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));
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

	m_ID = -1;
	m_TrackAlbumID = -1;
	m_TrackArtistID = -1;
	m_TrackGenreID = -1;
	m_TrackFavorite = -1;
	m_TrackIndex = -1;
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
		m_TopMenuMap.insert(TOP_MENU_ADD, STR_ADD);

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
		m_TopMenuMap.insert(TOP_MENU_DELETE, STR_DELETE);
//		m_TopMenuMap.insert(TOP_MENU_EXPORT_TRACK, STR_EXPORT_TRACK);

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
		m_TopMenuMap.insert(TOP_MENU_DELETE_TO_PLAYLIST, STR_DELETE_TO_PLAYLIST);

		m_pInfoTracks->GetFormPlay()->ClearMenu();
		m_pInfoTracks->GetFormPlay()->SetMenu(m_TopMenuMap);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		m_TopMenuMap.insert(TOP_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);

		m_pInfoService->GetFormPlay()->ClearMenu();
		m_pInfoService->GetFormPlay()->SetMenu(m_TopMenuMap);
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

void PlaylistWindow::DoTopMenuAdd()
{
	InputNameDialog dialog;
	if (dialog.exec() == QDialog::Accepted)
	{
		QString name = dialog.GetName();
		m_pMgr->RequestAddPlaylist(name);

		//refresh
		RequestPlaylist();
	}
}

void PlaylistWindow::DoTopMenuDelete()
{
	if (m_SelectMap.count() > 0)
	{
		m_pMgr->RequestDeletePlaylist(m_SelectMap);

		//refresh
		RequestPlaylist();
	}
}

void PlaylistWindow::DoTopMenuAddToPlaylist()
{
	if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		if (m_SelectMap.count() <= 0)
		{
			CommonDialog dialog(this, STR_WARNING, tr("no select"));
			dialog.exec();
		}
		else if (m_SelectMap.count() > 1)
		{
			CommonDialog dialog(this, STR_WARNING, tr("select only one"));
			dialog.exec();
		}
		else if (m_SelectMap.count() == 1)
		{
			int id = 0;
			QMap<int, bool>::iterator i;
			for (i = m_SelectMap.begin(); i!= m_SelectMap.end(); i++)
			{
				id = i.key();
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

		connect(widget, SIGNAL(SigAddCategoryFromPlaylist(int, QMap<int, bool>)), this, SLOT(SlotAddCategoryFromPlaylist(int, QMap<int, bool>)));
		connect(widget, SIGNAL(SigAddTrackFromPlaylist(QMap<int, bool>)), this, SLOT(SlotAddTrackFromPlaylist(QMap<int, bool>)));
	}
}

void PlaylistWindow::DoTopMenuItemDeleteToPlaylist()
{
	if (m_SelectMap.count() > 0)
	{
		m_pMgr->RequestDelTrack(m_ID, m_SelectMap);

		//refresh
		m_pMgr->RequestTrackList(m_ID);
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
		m_OptionMenuMap.insert(OPTION_MENU_DELETE_TO_PLAYLIST, STR_DELETE_TO_PLAYLIST);

		if (m_TrackFavorite == 1)
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

void PlaylistWindow::DoOptionMenuPlay(int nID, int where)
{
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK)
	{
		QMap<int, bool> map;
		map.insert(nID, true);
		m_pMgr->RequestPlayPlaylist(map, where);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK)
	{
		SlotSelectPlay(nID, where);
	}
}

void PlaylistWindow::DoOptionMenuRename(int nID)
{
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK)
	{
		InputNameDialog dialog;
		if (dialog.exec() == QDialog::Accepted)
		{
			QString name = dialog.GetName();
			m_pMgr->RequestRenamePlaylist(nID, name);

			//refresh
			RequestPlaylist();
		}
	}
	else if (m_TypeMode == TYPE_MODE_TRACK)
	{
		LogWarning("invalid type");
	}
}

void PlaylistWindow::DoOptionMenuDelete(int nID)
{
	QMap<int, bool> map;
	map.insert(nID, true);

	if (m_TypeMode == TYPE_MODE_ITEM_TRACK)
	{
		m_pMgr->RequestDeletePlaylist(map);

		// refresh
		RequestPlaylist();
	}
	else if (m_TypeMode == TYPE_MODE_TRACK)
	{

		m_pMgr->RequestDelTrack(m_ID, map);

		//refresh
		RequestTrackList(m_ID);
	}
}

void PlaylistWindow::DoOptionMenuAddToPlaylist(int nID)
{
	m_ID = nID;
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK)
	{
		MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), -1);
		widget->AddWidgetItem(TYPE_MODE_ITEM_ADD);
		emit widget->SigAddWidget(widget, STR_MUSIC_DB);
		widget->RequestCategoryList();

		connect(widget, SIGNAL(SigAddCategoryFromPlaylist(int, QMap<int, bool>)), this, SLOT(SlotAddCategoryFromPlaylist(int, QMap<int, bool>)));
		connect(widget, SIGNAL(SigAddTrackFromPlaylist(QMap<int, bool>)), this, SLOT(SlotAddTrackFromPlaylist(QMap<int, bool>)));
	}
}


void PlaylistWindow::DoOptionMenuFavorite()
{
	m_TrackFavorite = m_TrackFavorite == 0 ? 1 : 0;
//	LogDebug("DoTopMenuItemFavorite favorite [%d]", m_TrackFavorite);
	QModelIndex modelIndex = m_pListTracks->GetModel()->index(m_TrackIndex, 0);
	m_pListTracks->GetModel()->setData(modelIndex, m_TrackFavorite, ListTracksDelegate::LIST_TRACKS_FAVORITE);

	m_pMgr->RequestUpdateTrackFavorite(m_ID, m_TrackFavorite);

}

void PlaylistWindow::DoOptionMenuGoToAlbum()
{
//	LogDebug("DoOptionMenuGoToAlbum id [%d]", m_TrackAlbumID);
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), -1);
	widget->AddWidgetTrack(TYPE_MODE_TRACK, SQLManager::CATEGORY_ALBUM);
	emit widget->SigAddWidget(widget, STR_MUSIC_DB);

	widget->RequestTrackList(m_TrackAlbumID);
	widget->SetCoverArt(m_TrackCover);

}

void PlaylistWindow::DoOptionMenuGoToArtist()
{
//	LogDebug("DoOptionMenuGoToArtist id [%d]", m_TrackArtistID);
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), -1);
	widget->AddWidgetItem(TYPE_MODE_ITEM_ALBUM, SQLManager::CATEGORY_ARTIST);
	emit widget->SigAddWidget(widget, STR_MUSIC_DB);

	widget->RequestCategoryList(m_TrackArtistID);
}
