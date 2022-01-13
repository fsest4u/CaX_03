#include <QStackedWidget>
#include <QThread>

#include "musicdbwindow.h"
#include "ui_musicdbwindow.h"

#include "dialog/commondialog.h"
#include "dialog/edittagdialog.h"
#include "dialog/inputnamedialog.h"
#include "dialog/limitcountdialog.h"
#include "dialog/searchcoverartdialog.h"
#include "dialog/searchcoverartresultdialog.h"
#include "dialog/setcolumndialog.h"
#include "dialog/trackinfodialog.h"
#include "dialog/trackinfo.h"

#include "manager/musicdbmanager.h"
#include "manager/sqlmanager.h"

#include "util/caxkeyvalue.h"
#include "util/caxtranslate.h"
#include "util/log.h"
#include "util/settingio.h"
#include "util/utilnovatron.h"

#include "widget/form/formplay.h"
#include "widget/form/formsort.h"
#include "widget/form/formclassify.h"

#include "widget/formTop/infohome.h"
#include "widget/formTop/infotracks.h"
#include "widget/formBottom/icontracks.h"
#include "widget/formBottom/icontracksdelegate.h"
#include "widget/formBottom/listtracks.h"
#include "widget/formBottom/listtracksdelegate.h"

#include "widget/playlistwindow.h"

const QString SETTINGS_GROUP = "MusicDB";

MusicDBWindow::MusicDBWindow(QWidget *parent, const QString &addr, const int &eventID) :
	QWidget(parent),
	m_pMgr(new MusicDBManager),
	m_pInfoHome(new InfoHome(this)),
	m_pInfoTracks(new InfoTracks(this)),
	m_pIconTracks(new IconTracks(this)),
	m_pListTracks(new ListTracks(this)),
	m_pIconThread(new QThread),
	m_pListThread(new QThread),
	m_EventID(eventID),
	m_nCategory(SQLManager::CATEGORY_ALBUM),
	m_nSortCategory(SQLManager::SORT_IMPORTED_DATE),
	m_nSortTrack(SQLManager::SORT_IMPORTED_DATE),
	m_bIncreaseCategory(true),
	m_bIncreaseTrack(true),
	m_nFavorite(0),
	m_nRating(0),
	m_ArtistID(""),
	m_GenreID(""),
	m_ComposerID(""),
	m_LimitCount(100),
	m_TotalPage(0),
	m_CurIndex(0),
	m_nID(-1),
	m_nOptionID(-1),
	ui(new Ui::MusicDBWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
	ReadSettings();
	Initialize();
}

MusicDBWindow::~MusicDBWindow()
{
//	WriteSettings();

	if (m_pMgr)
	{
		delete m_pMgr;
		m_pMgr = nullptr;
	}

	if (m_pInfoHome)
	{
		delete m_pInfoHome;
		m_pInfoHome = nullptr;
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

	if (m_pIconThread)
	{
		ThreadTerminateIcon();
		delete m_pIconThread;
		m_pIconThread = nullptr;
	}

	if (m_pListThread)
	{
		ThreadTerminateList();
		delete m_pListThread;
		m_pListThread = nullptr;
	}

	delete ui;

}

void MusicDBWindow::AddWidgetItem(int typeMode, int category)
{
	m_ListMode = GetListModeFromResize(m_ResizeItem);
	m_TypeMode = typeMode;
	m_nCategory = category;

	SetSortMenu(category);

	m_pIconTracks->GetDelegate()->SetTypeMode(m_TypeMode);

	ui->gridLayoutTop->addWidget(m_pInfoHome);
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pInfoHome->GetFormSort()->SetResize(m_ResizeItem);
		ui->gridLayoutBottom->addWidget(m_pIconTracks);
	}
	else
	{
		m_pInfoHome->GetFormSort()->SetResize(m_ResizeItem);
		ui->gridLayoutBottom->addWidget(m_pListTracks);
	}

	if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		m_pInfoHome->GetFormPlay()->ShowPlayAll(false);
		m_pInfoHome->GetFormPlay()->ShowPlayRandom(false);
	}

	SetColumn(typeMode);
}

void MusicDBWindow::AddWidgetTrack(int typeMode, int category)
{
	m_ListMode = GetListModeFromResize(m_ResizeTrack);
	m_TypeMode = typeMode;
	m_nCategory = category;

	SetSortMenu(SQLManager::CATEGORY_TRACK);

	m_pIconTracks->GetDelegate()->SetTypeMode(m_TypeMode);

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

	if (m_TypeMode == TYPE_MODE_TRACK_ADD)
	{
		m_pInfoTracks->GetFormPlay()->ShowPlayAll(false);
		m_pInfoTracks->GetFormPlay()->ShowPlayRandom(false);
	}
//	m_pInfoTracks->GetFormPlay()->ShowFavorite(false);
//	m_pInfoTracks->GetFormPlay()->ShowRating(false);

	m_pListTracks->SetLineEditReadOnly(false);

	SetColumn(typeMode);
}

void MusicDBWindow::RequestCategoryList(int catID, int catID2)
{
	LogDebug("m_TypeMode [%d], category [%d], catID [%d], catID2 [%d]", m_TypeMode, m_nCategory, catID, catID2);

	QString title;
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK)
	{
		title = UtilNovatron::GetCategoryTitleName(m_nCategory);
		m_pMgr->RequestCategoryList(m_nCategory,
									m_nSortCategory,
									m_bIncreaseCategory,
									m_ArtistID,
									m_GenreID,
									m_ComposerID,
									m_nFavorite,
									m_nRating,
									m_LimitCount * m_CurIndex,
									m_LimitCount);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ALBUM)
	{
		m_nCatID = catID;
		QString strCatID = (m_nCatID < 0) ? "" : QString("%1").arg(m_nCatID);

		title = UtilNovatron::GetCategoryTitleName(m_nCategory) + " / " + KEY_ALBUM;
		m_pMgr->RequestAlbumOfCategoryList(m_nCategory,
										   m_nSortCategory,
										   m_bIncreaseCategory,
										   m_ArtistID,
										   m_GenreID,
										   m_ComposerID,
										   m_nFavorite,
										   m_nRating,
										   m_LimitCount * m_CurIndex,
										   m_LimitCount,
										   strCatID);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ARTIST)
	{
		m_nCatID = catID;
		QString strCatID = (m_nCatID < 0) ? "" : QString("%1").arg(m_nCatID);

		title = UtilNovatron::GetCategoryTitleName(m_nCategory) + " / " + KEY_ARTIST;
		m_pMgr->RequestArtistOfCategoryList(m_nCategory,
										   m_nSortCategory,
										   m_bIncreaseCategory,
										   m_ArtistID,
										   m_GenreID,
										   m_ComposerID,
										   m_nFavorite,
										   m_nRating,
										   m_LimitCount * m_CurIndex,
										   m_LimitCount,
										   strCatID);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		m_nCatID = catID;
		m_nCatID2 = catID2;
		QString strCatID = (catID < 0) ? "" : QString("%1").arg(catID);
		QString strCatID2 = (catID2 < 0) ? "" : QString("%1").arg(catID2);

		title = UtilNovatron::GetCategoryTitleName(m_nCategory) + " / " + KEY_ARTIST + " / " + KEY_ALBUM;
		m_pMgr->RequestAlbumOfArtistOfCategoryList(m_nCategory,
												   m_nSortCategory,
												   m_bIncreaseCategory,
												   m_ArtistID,
												   m_GenreID,
												   m_ComposerID,
												   m_nFavorite,
												   m_nRating,
												   m_LimitCount * m_CurIndex,
												   m_LimitCount,
												   strCatID,
												   strCatID2);
	}

	m_pInfoHome->SetTitle(title);
	m_pMgr->RequestMusicDBOverView();
}

void MusicDBWindow::RequestTrackList(int nID, int catID, int catID2)
{
	LogDebug("m_TypeMode [%d], category [%d], catID [%d], catID2 [%d]", m_TypeMode, m_nCategory, catID, catID2);

	m_nID = nID;

	if (m_nCategory == SQLManager::CATEGORY_TRACK)
	{
		m_pInfoTracks->SetTitle(KEY_TRACK);
		m_pMgr->RequestMusicDBOverView();
	}
	else
	{
		m_pMgr->RequestCategoryOverview(m_nID, m_nCategory);
	}

	if (m_TypeMode == TYPE_MODE_TRACK_ALBUM)
	{
		QString strCatID = (catID < 0) ? "" : QString("%1").arg(catID);

		m_pMgr->RequestTrackListOfAlbum(m_nID,
										m_nCategory,
										m_nSortTrack,
										m_bIncreaseTrack,
										m_LimitCount * m_CurIndex,
										m_LimitCount,
										strCatID);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		QString strCatID = (catID < 0) ? "" : QString("%1").arg(catID);
		QString strCatID2 = (catID2 < 0) ? "" : QString("%1").arg(catID2);

		m_pMgr->RequestTrackListOfAlbumOfArtist(m_nID,
												m_nCategory,
												m_nSortTrack,
												m_bIncreaseTrack,
												m_LimitCount * m_CurIndex,
												m_LimitCount,
												strCatID,
												strCatID2);
	}
	else
	{
		m_pMgr->RequestTrackList(m_nID,
								 m_nCategory,
								 m_nSortTrack,
								 m_bIncreaseTrack,
								 m_LimitCount * m_CurIndex,
								 m_LimitCount);
	}
}

//int MusicDBWindow::GetCategory() const
//{
//	return m_nCategory;
//}

//void MusicDBWindow::SetCategory(int nCategory)
//{
//	m_nCategory = nCategory;
//}

void MusicDBWindow::SlotAddWidget(QWidget *widget, QString title)
{
	emit SigAddWidget(widget, STR_MUSIC_DB);	// recursive
}

void MusicDBWindow::SlotRemoveWidget(QWidget *widget)
{
	emit SigRemoveWidget(widget);
}

void MusicDBWindow::SlotRespError(QString errMsg)
{
	CommonDialog dialog(this, STR_WARNING, errMsg);
	dialog.exec();
}

void MusicDBWindow::SlotRespMusicOverview(CJsonNode node)
{
//	LogDebug("node [%s]", node.ToCompactByteArray().data());
	int totalCount = UtilNovatron::GetCategoryCount(m_nCategory, node);
	CalculatePage(totalCount);

	QString strAlbumCnt = node.GetString(KEY_ALBUM);
	QString strAlbumArtistCnt = node.GetString(KEY_ALBUM_ARTIST);
	QString strArtistCnt = node.GetString(KEY_ARTIST);
	QString strGenreCnt = node.GetString(KEY_GENRE);

	if (m_nCategory != SQLManager::CATEGORY_TRACK)
	{
		if (strAlbumCnt.toInt() > 0)
		{
			m_pInfoHome->SetAlbumCnt(strAlbumCnt);
		}
		if (strAlbumArtistCnt.toInt() > 0)
		{
			m_pInfoHome->SetAlbumArtistCnt(node.GetString(KEY_ALBUM_ARTIST));
			m_pMgr->RequestClassifyList(SQLManager::CATEGORY_COMPOSER);
		}
		if (strArtistCnt.toInt() > 0)
		{
			m_pInfoHome->SetArtistCnt(node.GetString(KEY_ARTIST));
			m_pMgr->RequestClassifyList(SQLManager::CATEGORY_ARTIST);
		}
		if (strGenreCnt.toInt() > 0)
		{
			m_pInfoHome->SetGenreCnt(node.GetString(KEY_GENRE));
			m_pMgr->RequestClassifyList(SQLManager::CATEGORY_GENRE);
		}
	}
}

void MusicDBWindow::SlotRespCategoryList(QList<CJsonNode> list)
{

	m_RespList.append(list);

	SetOptionMenu();

	int service = SIDEMENU_MUSIC_DB;
	if (m_nCategory == SQLManager::CATEGORY_YEAR)
	{
		service = IconTracks::ICON_TRACKS_MUSIC_DB_YEAR;
	}
	else if (m_nCategory == SQLManager::CATEGORY_TRACK)
	{
		service = IconTracks::ICON_TRACKS_MUSIC_DB_TRACK;
	}

	if (m_ListMode == VIEW_MODE_ICON)
	{
//		m_pIconTracks->ClearNodeList();
		m_pIconTracks->SetNodeList(list, service);
		ThreadStartIcon();
	}
	else
	{
//		m_pListTracks->ClearNodeList();
		m_pListTracks->SetNodeList(list, service);
		ThreadStartList();
	}

}

void MusicDBWindow::SlotRespCategoryOverview(CJsonNode node)
{
//	LogDebug("node [%s]", node.ToCompactByteArray().data());
	QString title = node.GetString(KEY_TITLE);
	QString artist = node.GetString(KEY_ARTIST);
	QString favorite = node.GetString(KEY_FAVORITE);
	QString rating = node.GetString(KEY_RATING);
	QString count = node.GetString(KEY_COUNT);
	QString total = node.GetString(KEY_TOTAL);
	QString extension = node.GetString(KEY_EXTENSION);
	QString samplerate = node.GetString(KEY_SAMPLERATE);
	QString bps = node.GetString(KEY_BPS);

	QString info = count + " | " + total + " | " + extension + " | " + samplerate + " | " + bps;

	if (title.isEmpty())
	{
		title = STR_TRACK;
	}

	CalculatePage(count.toInt());

	m_pInfoTracks->SetTitle(title);
	m_pInfoTracks->SetSubtitle(artist);
//	m_pInfoTracks->SetInfo(info);
	m_pInfoTracks->GetFormPlay()->SetFavorite(favorite.toInt());
	m_pInfoTracks->GetFormPlay()->SetRating(rating.toInt());

}

void MusicDBWindow::SlotRespTrackList(QList<CJsonNode> list)
{
	m_RespList.append(list);

	SetOptionMenu();

	int service = SIDEMENU_MUSIC_DB;
	if (m_nCategory == SQLManager::CATEGORY_YEAR)
	{
		service = ListTracks::LIST_TRACKS_MUSIC_DB_YEAR;
	}
	else if (m_nCategory == SQLManager::CATEGORY_TRACK)
	{
		service = ListTracks::LIST_TRACKS_MUSIC_DB_TRACK;
	}

	if (m_ListMode == VIEW_MODE_ICON)
	{
//		m_pIconTracks->ClearNodeList();
		m_pIconTracks->SetNodeList(list, service);
		ThreadStartIcon();
	}
	else
	{
//		m_pListTracks->ClearNodeList();
		m_pListTracks->SetNodeList(list, service);
		ThreadStartList();
	}
}

void MusicDBWindow::SlotRespTrackListForEditTag(QList<CJsonNode> list)
{
	EditTagDialog dialog;
	dialog.SetAlbumList(m_AlbumList);
	dialog.SetAlbumArtistList(m_AlbumArtistList);
	dialog.SetArtistList(m_ArtistList);
	dialog.SetGenreList(m_GenreList);
	dialog.SetComposerList(m_ComposerList);
	dialog.SetMoodList(m_MoodList);
	dialog.SetNodeList(list);
	if (dialog.exec() == QDialog::Accepted)
	{
		m_UpdateModel = dialog.GetModel();
		m_UpdateMap = dialog.GetUpdateCell();

//		DoInsertIgnoreCategoryAll();
		m_pMgr->RequestSetTrackInfoAll(m_UpdateMap, m_UpdateModel, m_EventID);
	}
	else
	{
//		m_UpdateModel->clear();
//		m_UpdateMap.clear();
	}
}

void MusicDBWindow::SlotCoverArtUpdate(QString fileName, int nIndex, int mode)
{
	if (QListView::IconMode == mode)
	{
		QStandardItem *itemIcon = m_pIconTracks->GetModel()->item(nIndex);
		itemIcon->setData(fileName, IconTracksDelegate::ICON_TRACKS_COVER);
		m_pIconTracks->GetModel()->setItem(nIndex, itemIcon);
	}
	else
	{
		QStandardItem *itemList = m_pListTracks->GetModel()->item(nIndex);
		itemList->setData(fileName, ListTracksDelegate::LIST_TRACKS_COVER);
		m_pListTracks->GetModel()->setItem(nIndex, itemList);
	}
}

void MusicDBWindow::SlotPlayAll()
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
		m_pMgr->RequestManageCategory(VAL_PLAY, m_SelectMap, PLAY_CLEAR, m_nCategory);
	}
	else
	{
		QMap<int, bool> map;
		m_pMgr->RequestManageCategory(VAL_PLAY, map, PLAY_CLEAR, m_nCategory);
	}
}

void MusicDBWindow::SlotPlayRandom()
{
	m_pMgr->RequestRandom();
}

void MusicDBWindow::SlotTopMenu()
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

void MusicDBWindow::SlotTopMenuAction(int menuID)
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
	case TOP_MENU_RELOAD:
		DoTopMenuReload();
		break;
	case TOP_MENU_LOAD_COUNT:
		DoTopMenuSetLimitCount(m_LimitCount);
		break;
	case TOP_MENU_SELECT_ALL:
		DoTopMenuSelectAll();
		break;
	case TOP_MENU_CLEAR_ALL:
		DoTopMenuClearAll();
		break;
	case TOP_MENU_GAIN_SET:
		DoTopMenuGainSet();
		break;
	case TOP_MENU_GAIN_CLEAR:
		DoTopMenuGainClear();
		break;
	case TOP_MENU_ADD_TO_PLAYLIST:
		DoTopMenuAddToPlaylist();
		break;

	}

}

void MusicDBWindow::SlotSortMenu(int menuID)
{
	if (SQLManager::SORT_IMPORTED_DATE == menuID
			|| SQLManager::SORT_ALPHABET == menuID
			|| SQLManager::SORT_FAVORITE == menuID
			|| SQLManager::SORT_RATING == menuID
			|| SQLManager::DISP_MODE_TRACK == menuID)
	{
		m_DispMode = menuID;
		m_nSortCategory = menuID;
		if (SQLManager::DISP_MODE_TRACK != menuID)
		{
			WriteSettings();
		}

		RequestCategoryList(m_nCatID, m_nCatID2);

	}
	else if (SQLManager::DISP_MODE_ALBUM == menuID)
	{
		m_DispMode = menuID;
//		m_nSortCategory = SQLManager::SORT_IMPORTED_DATE;
//		RequestCategoryList();
		QString title = UtilNovatron::GetCategoryTitleName(m_nCategory) + " / " + KEY_ALBUM;
		m_pInfoHome->SetTitle(title);
	}
	else if (SQLManager::DISP_MODE_ARTIST == menuID)
	{
		m_DispMode = menuID;
//		m_nSortCategory = SQLManager::SORT_IMPORTED_DATE;
//		RequestCategoryList();
		QString title = UtilNovatron::GetCategoryTitleName(m_nCategory) + " / " + KEY_ARTIST;
		m_pInfoHome->SetTitle(title);
	}
}

void MusicDBWindow::SlotIncDec(bool bIncrease)
{
	m_bIncreaseCategory = bIncrease;
	WriteSettings();

	RequestCategoryList(m_nCatID, m_nCatID2);
}

void MusicDBWindow::SlotResize(int resize)
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
			ui->gridLayoutBottom->replaceWidget(m_pListTracks, m_pIconTracks);

			int service = SIDEMENU_MUSIC_DB;
			if (m_nCategory == SQLManager::CATEGORY_YEAR)
			{
				service = IconTracks::ICON_TRACKS_MUSIC_DB_YEAR;
			}
			else if (m_nCategory == SQLManager::CATEGORY_TRACK)
			{
				service = IconTracks::ICON_TRACKS_MUSIC_DB_TRACK;
			}

			LogDebug("icon~~~~~~~~[%d][%d]", m_pIconTracks->GetNodeList().count(), m_RespList.count());
//			if (m_pIconTracks->GetNodeList().count() != m_RespList.count())
			{
				m_pIconTracks->ClearNodeList();
				m_pIconTracks->SetNodeList(m_RespList, service);
				ThreadStartIcon();
			}

			m_pListTracks->hide();
			m_pIconTracks->show();
		}
		else
		{
			ui->gridLayoutBottom->replaceWidget(m_pIconTracks, m_pListTracks);

			int service = SIDEMENU_MUSIC_DB;
			if (m_nCategory == SQLManager::CATEGORY_YEAR)
			{
				service = ListTracks::LIST_TRACKS_MUSIC_DB_YEAR;
			}
			else if (m_nCategory == SQLManager::CATEGORY_TRACK)
			{
				service = ListTracks::LIST_TRACKS_MUSIC_DB_TRACK;
			}

			LogDebug("list~~~~~~~~[%d][%d]", m_pListTracks->GetNodeList().count(), m_RespList.count());
//			if (m_pListTracks->GetNodeList().count() != m_RespList.count())
			{
				m_pListTracks->ClearNodeList();
				m_pListTracks->SetNodeList(m_RespList, service);
				ThreadStartList();
			}

			if (m_TypeMode == TYPE_MODE_ITEM_TRACK
					|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
					|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
					|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
			{
				m_pListTracks->SetHeaderTitle(STR_NAME);
			}
			else if (m_TypeMode == TYPE_MODE_TRACK
					 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
					 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
			{
				m_pListTracks->SetHeaderTitle(STR_TITLE);
			}

			m_pIconTracks->hide();
			m_pListTracks->show();
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

void MusicDBWindow::SlotAlbumList()
{
	ThreadTerminateIcon();
	ThreadTerminateList();

	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetItem(TYPE_MODE_ITEM_TRACK, SQLManager::CATEGORY_ALBUM);
	emit widget->SigAddWidget(widget, STR_MUSIC_DB);

	widget->RequestCategoryList();
}

void MusicDBWindow::SlotAlbumArtistList()
{
	ThreadTerminateIcon();
	ThreadTerminateList();

	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetItem(TYPE_MODE_ITEM_TRACK, SQLManager::CATEGORY_ALBUM_ARTIST);
	emit widget->SigAddWidget(widget, STR_MUSIC_DB);

	widget->RequestCategoryList();
}

void MusicDBWindow::SlotArtistList()
{
	ThreadTerminateIcon();
	ThreadTerminateList();

	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetItem(TYPE_MODE_ITEM_TRACK, SQLManager::CATEGORY_ARTIST);
	emit widget->SigAddWidget(widget, STR_MUSIC_DB);

	widget->RequestCategoryList();
}

void MusicDBWindow::SlotGenreList()
{
	ThreadTerminateIcon();
	ThreadTerminateList();

	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetItem(TYPE_MODE_ITEM_TRACK, SQLManager::CATEGORY_GENRE);
	emit widget->SigAddWidget(widget, STR_MUSIC_DB);

	widget->RequestCategoryList();
}

void MusicDBWindow::SlotCategoryMenu()
{
	m_CategoryMenuMap.clear();
	m_CategoryMenuMap.insert(SQLManager::CATEGORY_COMPOSER, STR_COMPOSER);
	m_CategoryMenuMap.insert(SQLManager::CATEGORY_MOOD, STR_MOOD);
	m_CategoryMenuMap.insert(SQLManager::CATEGORY_FOLDER, STR_FOLDER);
	m_CategoryMenuMap.insert(SQLManager::CATEGORY_TRACK, STR_TRACK);
	m_CategoryMenuMap.insert(SQLManager::CATEGORY_YEAR, STR_YEAR);

	m_pInfoHome->ClearMenu();
	m_pInfoHome->SetMenu(m_CategoryMenuMap);
}

void MusicDBWindow::SlotCategoryMenuAction(int nCategory, QString title)
{
	ThreadTerminateIcon();
	ThreadTerminateList();

	if (nCategory == SQLManager::CATEGORY_TRACK)
	{
		MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
		widget->AddWidgetTrack(TYPE_MODE_TRACK, nCategory);
		emit widget->SigAddWidget(widget, STR_MUSIC_DB);

		widget->RequestTrackList(-1);
		widget->SetCoverArt("");
	}
	else
	{
		MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
		widget->AddWidgetItem(TYPE_MODE_ITEM_TRACK, nCategory);
		emit widget->SigAddWidget(widget, STR_MUSIC_DB);

		widget->RequestCategoryList();
	}
}

void MusicDBWindow::SlotItemPlayAll()
{
//	m_pMgr->RequestPlayCategoryItem(m_nID, PLAY_CLEAR, m_nCategory);
	m_SelectMap = m_pListTracks->GetSelectMap();
	if (m_SelectMap.count() > 0)
	{
		m_pMgr->RequestManageCategory(VAL_PLAY, m_SelectMap, PLAY_CLEAR, SQLManager::CATEGORY_TRACK);
	}
	else
	{
		QMap<int, bool> map;
		map.insert(m_nID, true);
		m_pMgr->RequestManageCategory(VAL_PLAY, map, PLAY_CLEAR, m_nCategory);
	}
}

void MusicDBWindow::SlotItemPlayRandom()
{
	m_pMgr->RequestRandom();
}

void MusicDBWindow::SlotItemTopMenu()
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

void MusicDBWindow::SlotItemTopMenuAction(int menuID)
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
	case TOP_MENU_GAIN_SET:
		DoTopMenuItemGainSet();
		break;
	case TOP_MENU_GAIN_CLEAR:
		DoTopMenuItemGainClear();
		break;
	case TOP_MENU_ADD_TO_PLAYLIST:
		DoTopMenuItemAddToPlaylist();
		break;
	case TOP_MENU_SHOW_COLUMNS:
		DoTopMenuItemShowColumns();
		break;
	}

}

void MusicDBWindow::SlotItemFavorite(int nFavorite)
{
	m_pMgr->RequestUpdateFavorite(m_nID, nFavorite, m_nCategory);
}

void MusicDBWindow::SlotItemRating(int nRating)
{
	m_pMgr->RequestUpdateRating(m_nID, nRating, m_nCategory);
}

void MusicDBWindow::SlotItemSortMenu(int sort)
{
	m_nSortTrack = sort;
	WriteSettings();

	DoTopMenuReload();
}

void MusicDBWindow::SlotItemIncDec(bool bIncrease)
{
	m_bIncreaseTrack = bIncrease;
	WriteSettings();

	DoTopMenuReload();
}

void MusicDBWindow::SlotSelectPlay(int nID, int where)
{
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		SlotSelectCategoryPlay(nID, where);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		SlotSelectTrackPlay(nID, where);
	}
}

void MusicDBWindow::SlotSelectTitle(int nID, QString coverArt)
{
	LogDebug("id [%d] cover art [%s]", nID, coverArt.toUtf8().data());
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK)
	{
		if (m_DispMode == SQLManager::DISP_MODE_ALBUM)
		{
			LogDebug("### Album mode");
			MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
			widget->AddWidgetItem(TYPE_MODE_ITEM_ALBUM, m_nCategory);
			emit widget->SigAddWidget(widget, STR_MUSIC_DB);

			widget->RequestCategoryList(nID);
		}
		else if (m_DispMode == SQLManager::DISP_MODE_ARTIST)
		{
			LogDebug("### Artist mode");
			MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
			widget->AddWidgetItem(TYPE_MODE_ITEM_ARTIST, m_nCategory);
			emit widget->SigAddWidget(widget, STR_MUSIC_DB);

			widget->RequestCategoryList(nID);
		}
		else
		{
			LogDebug("### Track mode");
			ThreadTerminateIcon();
			ThreadTerminateList();

			MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
			widget->AddWidgetTrack(TYPE_MODE_TRACK, m_nCategory);
			emit widget->SigAddWidget(widget, STR_MUSIC_DB);

			widget->RequestTrackList(nID);
			widget->SetCoverArt(coverArt);
		}
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ALBUM)
	{
		LogDebug("### Track mode");
		ThreadTerminateIcon();
		ThreadTerminateList();

		MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
		widget->AddWidgetTrack(TYPE_MODE_TRACK_ALBUM, m_nCategory);
		emit widget->SigAddWidget(widget, STR_MUSIC_DB);

		widget->RequestTrackList(nID, m_nCatID);
		widget->SetCoverArt(coverArt);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ARTIST)
	{
		LogDebug("### Album mode");
		MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
		widget->AddWidgetItem(TYPE_MODE_ITEM_ARTIST_ALBUM, m_nCategory);
		emit widget->SigAddWidget(widget, STR_MUSIC_DB);

		widget->RequestCategoryList(m_nCatID, nID);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		LogDebug("### Track mode");
		ThreadTerminateIcon();
		ThreadTerminateList();

		MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
		widget->AddWidgetTrack(TYPE_MODE_TRACK_ALBUM_ARTIST, m_nCategory);
		emit widget->SigAddWidget(widget, STR_MUSIC_DB);

		widget->RequestTrackList(nID, m_nCatID, m_nCatID2);
		widget->SetCoverArt(coverArt);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		ThreadTerminateIcon();
		ThreadTerminateList();

		MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
		widget->AddWidgetTrack(TYPE_MODE_TRACK_ADD, m_nCategory);
		emit widget->SigAddWidget(widget, STR_MUSIC_DB);

		widget->RequestTrackList(nID);
		widget->SetCoverArt(coverArt);

		connect(widget, SIGNAL(SigAddTrackFromPlaylist(QMap<int, bool>)), this, SLOT(SlotAddTrackFromPlaylist(QMap<int, bool>)));

	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		SlotSelectTrackPlay(nID, PLAY_CLEAR);
	}

}

void MusicDBWindow::SlotSelectCategoryPlay(int nID, int where)
{
	QMap<int, bool> map;
	map.insert(nID, true);
	m_pMgr->RequestManageCategory(VAL_PLAY, map, where, m_nCategory);
}

void MusicDBWindow::SlotSelectFavorite(int nID, int nFavorite)
{
	m_pMgr->RequestUpdateFavorite(nID, nFavorite, m_nCategory);
}

void MusicDBWindow::SlotSelectRating(int nID, int nRating)
{
	m_pMgr->RequestUpdateRating(nID, nRating, m_nCategory);
}

void MusicDBWindow::SlotReqCoverArt(int id, int index, int mode)
{
	QString strCat;
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		strCat = UtilNovatron::GetCategoryName(m_nCategory);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ALBUM
			 || m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		strCat = KEY_ALBUM;
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ARTIST)
	{
		strCat = KEY_ARTIST;
	}
	else
	{
		strCat = UtilNovatron::GetCategoryName(-1);
	}

	QStringList lsAddr = m_pMgr->GetAddr().split(":");
	QString fullpath = QString("%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(strCat).arg(id);

//	LogDebug("path [%s] index [%d] typeMode [%d]", fullpath.toUtf8().data(), index, m_TypeMode);
	m_pMgr->RequestCoverArt(fullpath, index, mode);
}

void MusicDBWindow::SlotAppendList()
{
	m_CurIndex++;
	if (m_TotalPage > m_CurIndex)
	{
		if (m_TypeMode == TYPE_MODE_ITEM_TRACK
				|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
				|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
				|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
		{
			RequestCategoryList(m_nCatID, m_nCatID2);
		}
		else if (m_TypeMode == TYPE_MODE_TRACK
				 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
				 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
		{
			RequestTrackList(m_nID, m_nCatID, m_nCatID2);
		}
	}
	else
	{
		m_CurIndex = m_TotalPage - 1;
	}
}

void MusicDBWindow::SlotSelectTrackPlay(int nID, int where)
{
//	m_pMgr->RequestPlaySong(nID);
	QMap<int, bool> map;
	map.insert(nID, true);
	m_pMgr->RequestManageCategory(VAL_PLAY, map, where, SQLManager::CATEGORY_TRACK);
}

void MusicDBWindow::SlotSelectTrackFavorite(int nID, int nFavorite)
{
	m_pMgr->RequestUpdateTrackFavorite(nID, nFavorite);
}

void MusicDBWindow::SlotRespClassifyArtist(QList<CJsonNode> list)
{
	m_pInfoHome->GetFormClassify()->ClearClassifyArtistMenu();
	m_pInfoHome->GetFormClassify()->SetClassifyArtistMenu(list);
}

void MusicDBWindow::SlotRespClassifyGenre(QList<CJsonNode> list)
{
	m_pInfoHome->GetFormClassify()->ClearClassifyGenreMenu();
	m_pInfoHome->GetFormClassify()->SetClassifyGenreMenu(list);
}

void MusicDBWindow::SlotRespClassifyComposer(QList<CJsonNode> list)
{
	m_pInfoHome->GetFormClassify()->ClearClassifyComposerMenu();
	m_pInfoHome->GetFormClassify()->SetClassifyComposerMenu(list);
}

void MusicDBWindow::SlotClassifyFavorite(int nFavorite)
{
	m_nFavorite = nFavorite;
	RequestCategoryList(m_nCatID, m_nCatID2);
}

void MusicDBWindow::SlotClassifyRating(int nRating)
{
	m_nRating = nRating;
	RequestCategoryList(m_nCatID, m_nCatID2);
}

void MusicDBWindow::SlotClassifyArtist(bool bAdd, QString id)
{
	LogDebug("artist add [%d] id [%s]", bAdd, id.toUtf8().data());
	if (bAdd)
	{
		m_ArtistID = id;
	}
	else
	{
		m_ArtistID = "";
	}

//	m_pMgr->RequestCategoryList(m_nCategory, m_nSortCategory, m_bIncreaseCategory, m_ArtistID, m_GenreID, m_ComposerID);
	m_nFavorite = 0;
	m_nRating = 0;
	m_CurIndex = 0;
	RequestCategoryList(m_nCatID, m_nCatID2);

}

void MusicDBWindow::SlotClassifyGenre(bool bAdd, QString id)
{
	LogDebug("genre add [%d] id [%s]", bAdd, id.toUtf8().data());
	if (bAdd)
	{
		m_GenreID = id;
	}
	else
	{
		m_GenreID = "";
	}

//	m_pMgr->RequestCategoryList(m_nCategory, m_nSortCategory, m_bIncreaseCategory, m_ArtistID, m_GenreID, m_ComposerID);
	m_nFavorite = 0;
	m_nRating = 0;
	m_CurIndex = 0;
	RequestCategoryList(m_nCatID, m_nCatID2);
}

void MusicDBWindow::SlotClassifyComposer(bool bAdd, QString id)
{
	LogDebug("composer add [%d] id [%s]", bAdd, id.toUtf8().data());
	if (bAdd)
	{
		m_ComposerID = id;
	}
	else
	{
		m_ComposerID = "";
	}

//	m_pMgr->RequestCategoryList(m_nCategory, m_nSortCategory, m_bIncreaseCategory, m_ArtistID, m_GenreID, m_ComposerID);
	m_nFavorite = 0;
	m_nRating = 0;
	m_CurIndex = 0;
	RequestCategoryList(m_nCatID, m_nCatID2);
}

void MusicDBWindow::SlotRespCategoryInfo(CJsonNode node)
{
	TrackInfoDialog dialog;
	dialog.SetWindowTitle("Album info");
	dialog.SetAddr(m_pMgr->GetAddr());
	dialog.SetAlbumList(m_AlbumList);
	dialog.SetAlbumArtistList(m_AlbumArtistList);
//	dialog.SetArtistList(m_ArtistList);
//	dialog.SetGenreList(m_GenreList);
//	dialog.SetComposerList(m_ComposerList);
//	dialog.SetMoodList(m_MoodList);
	dialog.SetBtnEdit(true);
	dialog.SetMode(TrackInfo::TRACK_INFO_MODE_VIEW);
	dialog.SetInfoData(node);
	if (dialog.exec() == QDialog::Accepted)
	{
		node = dialog.GetInfoData();
		m_pMgr->RequestSetCategoryInfo(m_nOptionID, m_EventID, node);
	}
}

void MusicDBWindow::SlotRespCategoryInfoList(QList<CJsonNode> list)
{
	SetCategoryList(list);
}

void MusicDBWindow::SlotRespTrackInfo(CJsonNode node)
{
	TrackInfoDialog dialog;
	dialog.SetWindowTitle("Track info");
	dialog.SetAddr(m_pMgr->GetAddr());
	dialog.SetAlbumList(m_AlbumList);
	dialog.SetAlbumArtistList(m_AlbumArtistList);
	dialog.SetArtistList(m_ArtistList);
	dialog.SetGenreList(m_GenreList);
	dialog.SetComposerList(m_ComposerList);
	dialog.SetMoodList(m_MoodList);
	dialog.SetBtnEdit(true);
	dialog.SetMode(TrackInfo::TRACK_INFO_MODE_VIEW);
	dialog.SetInfoData(node);
	if (dialog.exec() == QDialog::Accepted)
	{
		node = dialog.GetInfoData();
		m_pMgr->RequestSetTrackInfo(m_nOptionID, m_EventID, node);
	}
}

void MusicDBWindow::SlotRespSetCoverArt(int id, int category)
{
	// temp_code, dylee
	LogDebug("id [%d] category [%d]", id, category);
}

//void MusicDBWindow::SlotRespUpdateCategory(int updateId)
//{
//	if (updateId >= 0)
//	{
//		m_nID = updateId;
//	}
//	DoTopMenuReload();
//}

//void MusicDBWindow::SlotRespInsertReplaceCategoryAll()
//{
//	m_pMgr->RequestUpdateCategoryAll(m_UpdateMap, m_UpdateModel);
//}

void MusicDBWindow::SlotRespRefresh()
{
	DoTopMenuReload();
}

void MusicDBWindow::SlotOptionMenuAction(int nID, int menuID)
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
	case OPTION_MENU_ADD_TO_PLAYLIST:
		DoOptionMenuAddToPlaylist(nID);
		break;
	case OPTION_MENU_TAG_EDIT:
		DoOptionMenuInfo(nID);
		break;
	case OPTION_MENU_ADD_COVER_ART:
		DoOptionMenuAddCoverArt(nID);
		break;
	case OPTION_MENU_RENAME:
		DoOptionMenuRename(nID);
		break;
	case OPTION_MENU_GAIN_SET:
		DoOptionMenuGain(nID, VAL_GAIN_SET);
		break;
	case OPTION_MENU_GAIN_CLEAR:
		DoOptionMenuGain(nID, VAL_GAIN_CLEAR);
		break;

	}
}

void MusicDBWindow::SlotTopMenuAddToPlaylist(int id)
{
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		m_pMgr->RequestAddToPlaylist(id, m_SelectMap, m_nCategory);

	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		m_pMgr->RequestAddToPlaylist(id, m_SelectMap, -1);

	}
}

void MusicDBWindow::SlotOptionMenuAddToPlaylist(int id)
{
	QMap<int, bool> map;
	map.insert(m_nOptionID, true);

	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		m_pMgr->RequestAddToPlaylist(id, map, m_nCategory);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		m_pMgr->RequestAddToPlaylist(id, map, -1);
	}
}

void MusicDBWindow::SlotAddTrackFromPlaylist(QMap<int, bool> idMap)
{
	emit SigAddTrackFromPlaylist(idMap);	// recursive
	emit SigRemoveWidget(this);
}

void MusicDBWindow::SlotContextMenu(QPoint point)
{
	QModelIndex modelIndex = m_pIconTracks->GetListView()->indexAt(point);
	QStandardItem *item = m_pIconTracks->GetModel()->itemFromIndex(modelIndex);
	if (!item)
	{
		return;
	}
	m_nID = qvariant_cast<int>(item->data(IconTracksDelegate::ICON_TRACKS_ID));
	int index = modelIndex.row();

	LogDebug("index [%d] id [%d]", index, m_nID);

	QAction *actionPlayNow = new QAction(STR_PLAY_NOW, this);
	QAction *actionPlayLast = new QAction(STR_PLAY_LAST, this);
	QAction *actionPlayNext = new QAction(STR_PLAY_NEXT, this);
	QAction *actionPlayClear = new QAction(STR_PLAY_CLEAR, this);
	QAction *actionTagEdit = new QAction(STR_TAG_EDIT, this);

	connect(actionPlayNow, SIGNAL(triggered()), this, SLOT(SlotContextMenuPlayNow()));
	connect(actionPlayLast, SIGNAL(triggered()), this, SLOT(SlotContextMenuPlayLast()));
	connect(actionPlayNext, SIGNAL(triggered()), this, SLOT(SlotContextMenuPlayNext()));
	connect(actionPlayClear, SIGNAL(triggered()), this, SLOT(SlotContextMenuPlayClear()));
	connect(actionTagEdit, SIGNAL(triggered()), this, SLOT(SlotContextMenuTagEdit()));

	QMenu *menu=new QMenu(this);
	menu->addAction(actionPlayNow);
	menu->addAction(actionPlayLast);
	menu->addAction(actionPlayNext);
	menu->addAction(actionPlayClear);
	menu->addAction(actionTagEdit);

	menu->popup(m_pIconTracks->GetListView()->viewport()->mapToGlobal(point));

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

	menu->setStyleSheet(style);

}

void MusicDBWindow::SlotContextMenuPlayNow()
{
	DoOptionMenuPlay(m_nID, PLAY_NOW);
}

void MusicDBWindow::SlotContextMenuPlayLast()
{
	DoOptionMenuPlay(m_nID, PLAY_LAST);
}

void MusicDBWindow::SlotContextMenuPlayNext()
{
	DoOptionMenuPlay(m_nID, PLAY_NEXT);
}

void MusicDBWindow::SlotContextMenuPlayClear()
{
	DoOptionMenuPlay(m_nID, PLAY_CLEAR);
}

void MusicDBWindow::SlotContextMenuTagEdit()
{
	m_pMgr->RequestTrackListForEditTag(m_nID, m_nCategory);
}

void MusicDBWindow::ReadSettings()
{
	SettingIO settings;
	settings.beginGroup(SETTINGS_GROUP);

	m_ShowFavorite = settings.value("show_favorite").toBool();
	m_ShowTime = settings.value("show_time").toBool();
	m_ShowArtist = settings.value("show_artist").toBool();
	m_ShowAlbum = settings.value("show_album").toBool();
	m_ShowGenre = settings.value("show_genre").toBool();
	m_ShowAlbumArtist = settings.value("show_album_artist").toBool();
	m_ShowComposer = settings.value("show_composer").toBool();
	m_ShowYear = settings.value("show_year").toBool();

	m_ShowMood = settings.value("show_mood").toBool();
	m_ShowTempo = settings.value("show_tempo").toBool();
	m_ShowFormat = settings.value("show_format").toBool();
	m_ShowSampleRate = settings.value("show_sample_rate").toBool();
	m_ShowBitDepth = settings.value("show_bit_depth").toBool();
	m_ShowRating = settings.value("show_rating").toBool();

	m_ResizeItem = settings.value("resize_item_value").toInt();
	m_ResizeTrack = settings.value("resize_track_value").toInt();

	m_nSortCategory = settings.value("sort_item_value").toInt();
	m_nSortTrack = settings.value("sort_track_value").toInt();

	m_bIncreaseCategory = settings.value("increase_item").toBool();
	m_bIncreaseTrack = settings.value("increase_track").toBool();

	settings.endGroup();

	if (m_ResizeItem <= 0)
	{
		m_ResizeItem = ICON_HEIGHT_MAX;
	}
	if (m_ResizeTrack <= 0)
	{
		m_ResizeTrack = LIST_HEIGHT_MIN;
	}
}

void MusicDBWindow::WriteSettings()
{
	SettingIO settings;
	settings.beginGroup(SETTINGS_GROUP);

	settings.setValue("show_favorite", m_ShowFavorite);
	settings.setValue("show_time", m_ShowTime);
	settings.setValue("show_artist", m_ShowArtist);
	settings.setValue("show_album", m_ShowAlbum);
	settings.setValue("show_genre", m_ShowGenre);
	settings.setValue("show_album_artist", m_ShowAlbumArtist);
	settings.setValue("show_composer", m_ShowComposer);
	settings.setValue("show_year", m_ShowYear);

	settings.setValue("show_mood", m_ShowMood);
	settings.setValue("show_tempo", m_ShowTempo);
	settings.setValue("show_format", m_ShowFormat);
	settings.setValue("show_sample_rate", m_ShowSampleRate);
	settings.setValue("show_bit_depth", m_ShowBitDepth);
	settings.setValue("show_rating", m_ShowRating);

	settings.setValue("resize_item_value", m_ResizeItem);
	settings.setValue("resize_track_value", m_ResizeTrack);

	settings.setValue("sort_item_value", m_nSortCategory);
	settings.setValue("sort_track_value", m_nSortTrack);

	settings.setValue("increase_item", m_bIncreaseCategory);
	settings.setValue("increase_track", m_bIncreaseTrack);


	settings.endGroup();
}

//void MusicDBWindow::SlotEditAllArtist(QString value)
//{
//	LogDebug("edit artist [%s]", value.toUtf8().data());
//	m_pMgr->RequestCheckCategory(m_nID, m_nCategory, SQLManager::CATEGORY_ARTIST, value);
//}

//void MusicDBWindow::SlotEditAllAlbum(QString value)
//{
//	LogDebug("edit album [%s]", value.toUtf8().data());
//	m_pMgr->RequestCheckCategory(m_nID, m_nCategory, SQLManager::CATEGORY_ALBUM, value);
//}

//void MusicDBWindow::SlotEditAllGenre(QString value)
//{
//	LogDebug("edit genre [%s]", value.toUtf8().data());
//	m_pMgr->RequestCheckCategory(m_nID, m_nCategory, SQLManager::CATEGORY_GENRE, value);
//}

void MusicDBWindow::ConnectSigToSlot()
{
	// recursive
	connect(this, SIGNAL(SigAddWidget(QWidget*, QString)), parent(), SLOT(SlotAddWidget(QWidget*, QString)));
	connect(this, SIGNAL(SigRemoveWidget(QWidget*)), parent(), SLOT(SlotRemoveWidget(QWidget*)));

	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespMusicOverview(CJsonNode)), this, SLOT(SlotRespMusicOverview(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespCategoryList(QList<CJsonNode>)), this, SLOT(SlotRespCategoryList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespCategoryOverview(CJsonNode)), this, SLOT(SlotRespCategoryOverview(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespTrackList(QList<CJsonNode>)), this, SLOT(SlotRespTrackList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespTrackListForEditTag(QList<CJsonNode>)), this, SLOT(SlotRespTrackListForEditTag(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespClassifyArtist(QList<CJsonNode>)), this, SLOT(SlotRespClassifyArtist(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespClassifyGenre(QList<CJsonNode>)), this, SLOT(SlotRespClassifyGenre(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespClassifyComposer(QList<CJsonNode>)), this, SLOT(SlotRespClassifyComposer(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespCategoryInfo(CJsonNode)), this, SLOT(SlotRespCategoryInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespCategoryInfoList(QList<CJsonNode>)), this, SLOT(SlotRespCategoryInfoList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespTrackInfo(CJsonNode)), this, SLOT(SlotRespTrackInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespSetCoverArt(int, int)), this, SLOT(SlotRespSetCoverArt(int, int)));
//	connect(m_pMgr, SIGNAL(SigRespUpdateCategory(int)), this, SLOT(SlotRespUpdateCategory(int)));
//	connect(m_pMgr, SIGNAL(SigRespInsertReplaceCategoryAll()), this, SLOT(SlotRespInsertReplaceCategoryAll()));
	connect(m_pMgr, SIGNAL(SigRespRefresh()), this, SLOT(SlotRespRefresh()));
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));

	connect(m_pInfoHome->GetFormPlay(), SIGNAL(SigPlayAll()), this, SLOT(SlotPlayAll()));
	connect(m_pInfoHome->GetFormPlay(), SIGNAL(SigPlayRandom()), this, SLOT(SlotPlayRandom()));
	connect(m_pInfoHome->GetFormPlay(), SIGNAL(SigMenu()), this, SLOT(SlotTopMenu()));
	connect(m_pInfoHome->GetFormPlay(), SIGNAL(SigMenuAction(int)), this, SLOT(SlotTopMenuAction(int)));

	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigClassifyFavorite(int)), this, SLOT(SlotClassifyFavorite(int)));
	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigClassifyRating(int)), this, SLOT(SlotClassifyRating(int)));
	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigClassifyArtist(bool, QString)), this, SLOT(SlotClassifyArtist(bool, QString)));
	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigClassifyGenre(bool, QString)), this, SLOT(SlotClassifyGenre(bool, QString)));
	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigClassifyComposer(bool, QString)), this, SLOT(SlotClassifyComposer(bool, QString)));

	connect(m_pInfoHome->GetFormSort(), SIGNAL(SigMenu(int)), this, SLOT(SlotSortMenu(int)));
	connect(m_pInfoHome->GetFormSort(), SIGNAL(SigIncDec(bool)), this, SLOT(SlotIncDec(bool)));
	connect(m_pInfoHome->GetFormSort(), SIGNAL(SigResize(int)), this, SLOT(SlotResize(int)));

	connect(m_pInfoHome, SIGNAL(SigAlbumList()), this, SLOT(SlotAlbumList()));
	connect(m_pInfoHome, SIGNAL(SigAlbumArtistList()), this, SLOT(SlotAlbumArtistList()));
	connect(m_pInfoHome, SIGNAL(SigArtistList()), this, SLOT(SlotArtistList()));
	connect(m_pInfoHome, SIGNAL(SigGenreList()), this, SLOT(SlotGenreList()));
	connect(m_pInfoHome, SIGNAL(SigMenu()), this, SLOT(SlotCategoryMenu()));
	connect(m_pInfoHome, SIGNAL(SigMenuAction(int, QString)), this, SLOT(SlotCategoryMenuAction(int, QString)));

	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigPlayAll()), this, SLOT(SlotItemPlayAll()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigPlayRandom()), this, SLOT(SlotItemPlayRandom()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigFavorite(int)), this, SLOT(SlotItemFavorite(int)));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigRating(int)), this, SLOT(SlotItemRating(int)));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigMenu()), this, SLOT(SlotItemTopMenu()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigMenuAction(int)), this, SLOT(SlotItemTopMenuAction(int)));

	connect(m_pInfoTracks->GetFormSort(), SIGNAL(SigMenu(int)), this, SLOT(SlotItemSortMenu(int)));
	connect(m_pInfoTracks->GetFormSort(), SIGNAL(SigIncDec(bool)), this, SLOT(SlotItemIncDec(bool)));
	connect(m_pInfoTracks->GetFormSort(), SIGNAL(SigResize(int)), this, SLOT(SlotResize(int)));

	connect(m_pIconTracks, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));
	connect(m_pIconTracks, SIGNAL(SigAppendList()), this, SLOT(SlotAppendList()));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectPlay(int, int)), this, SLOT(SlotSelectPlay(int, int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectFavorite(int, int)), this, SLOT(SlotSelectFavorite(int, int)));

	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectRating(int, int)), this, SLOT(SlotSelectRating(int, int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectSubtitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));
	connect(m_pIconTracks->GetListView(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(SlotContextMenu(QPoint)));

	connect(m_pListTracks, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));
	connect(m_pListTracks, SIGNAL(SigAppendList()), this, SLOT(SlotAppendList()));
//	connect(m_pListTracks, SIGNAL(SigEditAllArtist(QString)), this, SLOT(SlotEditAllArtist(QString)));
//	connect(m_pListTracks, SIGNAL(SigEditAllAlbum(QString)), this, SLOT(SlotEditAllAlbum(QString)));
//	connect(m_pListTracks, SIGNAL(SigEditAllGenre(QString)), this, SLOT(SlotEditAllGenre(QString)));

	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectPlay(int, int)), this, SLOT(SlotSelectPlay(int, int)));
	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));
	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectFavorite(int, int)), this, SLOT(SlotSelectTrackFavorite(int, int)));
	connect(m_pListTracks->GetDelegate(), SIGNAL(SigMenuAction(int, int)), this, SLOT(SlotOptionMenuAction(int, int)));



}

void MusicDBWindow::Initialize()
{
	m_pInfoHome->GetFormPlay()->ShowPlayAll();
	m_pInfoHome->GetFormPlay()->ShowPlayRandom();
	m_pInfoHome->GetFormPlay()->ShowMenu();

	m_pInfoHome->GetFormSort()->ShowMenu();
	m_pInfoHome->GetFormSort()->ShowResize();
	m_pInfoHome->GetFormSort()->ShowIncDec();
	m_pInfoHome->GetFormSort()->SetIncrease(m_bIncreaseCategory);

	m_pInfoTracks->GetFormPlay()->ShowPlayAll();
	m_pInfoTracks->GetFormPlay()->ShowPlayRandom();
//	m_pInfoTracks->GetFormPlay()->ShowFavorite();
//	m_pInfoTracks->GetFormPlay()->ShowRating();
	m_pInfoTracks->GetFormPlay()->ShowMenu();

	m_pInfoTracks->GetFormSort()->ShowMenu();
	m_pInfoTracks->GetFormSort()->ShowResize();
	m_pInfoTracks->GetFormSort()->ShowIncDec();
	m_pInfoTracks->GetFormSort()->SetIncrease(m_bIncreaseTrack);

	m_TopMenuMap.clear();
	m_CategoryMenuMap.clear();
	m_SelectMap.clear();

	m_AlbumList.clear();
	m_AlbumArtistList.clear();
	m_ArtistList.clear();
	m_GenreList.clear();
	m_ComposerList.clear();
	m_MoodList.clear();

//	m_ListMode = VIEW_MODE_ICON;
	m_TypeMode = TYPE_MODE_ITEM_TRACK;
	m_DispMode = SQLManager::DISP_MODE_TRACK;

	m_pListTracks->SetLineEditReadOnly(true);

	m_pIconTracks->SetBackgroundTask(m_pIconThread);
	m_pListTracks->SetBackgroundTask(m_pListThread);

	m_nID = -1;
	m_nCatID = -1;

	m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_ALBUM);
	m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_ALBUM_ARTIST);
	m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_ARTIST);
	m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_GENRE);
	m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_COMPOSER);
	m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_MOOD);
}

void MusicDBWindow::SetCategoryList(QList<CJsonNode> list)
{
	CJsonNode temp = list.at(0);
	if (!temp.GetString(KEY_ALBUM).isEmpty())
	{
		m_AlbumList.clear();
		foreach (CJsonNode node, list)
		{
			m_AlbumList.append(node.GetString(KEY_ALBUM));
		}
	}
	else if (!temp.GetString(KEY_ALBUM_ARTIST).isEmpty())
	{
		m_AlbumArtistList.clear();
		foreach (CJsonNode node, list)
		{
			m_AlbumArtistList.append(node.GetString(KEY_ALBUM_ARTIST));
		}
	}
	else if (!temp.GetString(KEY_ARTIST).isEmpty())
	{
		m_ArtistList.clear();
		foreach (CJsonNode node, list)
		{
			m_ArtistList.append(node.GetString(KEY_ARTIST));
		}
	}
	else if (!temp.GetString(KEY_GENRE).isEmpty())
	{
		m_GenreList.clear();
		foreach (CJsonNode node, list)
		{
			m_GenreList.append(node.GetString(KEY_GENRE));
		}
	}
	else if (!temp.GetString(KEY_COMPOSER).isEmpty())
	{
		m_ComposerList.clear();
		foreach (CJsonNode node, list)
		{
			m_ComposerList.append(node.GetString(KEY_COMPOSER));
		}
	}
	else if (!temp.GetString(KEY_MOOD).isEmpty())
	{
		m_MoodList.clear();
		foreach (CJsonNode node, list)
		{
			m_MoodList.append(node.GetString(KEY_MOOD));
		}
	}
}

void MusicDBWindow::SetSelectOffTopMenu()
{
	m_TopMenuMap.clear();
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		m_TopMenuMap.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_TopMenuMap.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_TopMenuMap.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_TopMenuMap.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_TopMenuMap.insert(TOP_MENU_RELOAD, STR_RELOAD);
		m_TopMenuMap.insert(TOP_MENU_LOAD_COUNT, QString("%1 - %2").arg(STR_LOAD_COUNT).arg(m_LimitCount));
		m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		m_TopMenuMap.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_TopMenuMap.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_TopMenuMap.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_TopMenuMap.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
		m_TopMenuMap.insert(TOP_MENU_SHOW_COLUMNS, STR_SHOW_COLUMNS);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK_ADD)
	{
		m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
	}

	m_pInfoHome->GetFormPlay()->ClearMenu();
	m_pInfoHome->GetFormPlay()->SetMenu(m_TopMenuMap);

	m_pInfoTracks->GetFormPlay()->ClearMenu();
	m_pInfoTracks->GetFormPlay()->SetMenu(m_TopMenuMap);
}

void MusicDBWindow::SetSelectOnTopMenu()
{
	m_TopMenuMap.clear();
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		m_TopMenuMap.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_TopMenuMap.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_TopMenuMap.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_TopMenuMap.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_TopMenuMap.insert(TOP_MENU_CLEAR_ALL, STR_CLEAR_ALL);
		m_TopMenuMap.insert(TOP_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
		m_TopMenuMap.insert(TOP_MENU_GAIN_SET, STR_GAIN_SET);
		m_TopMenuMap.insert(TOP_MENU_GAIN_CLEAR, STR_GAIN_CLEAR);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		m_TopMenuMap.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_TopMenuMap.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_TopMenuMap.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_TopMenuMap.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_TopMenuMap.insert(TOP_MENU_CLEAR_ALL, STR_CLEAR_ALL);
		m_TopMenuMap.insert(TOP_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
		m_TopMenuMap.insert(TOP_MENU_GAIN_SET, STR_GAIN_SET);
		m_TopMenuMap.insert(TOP_MENU_GAIN_CLEAR, STR_GAIN_CLEAR);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		m_TopMenuMap.insert(TOP_MENU_CLEAR_ALL, STR_CLEAR_ALL);
		m_TopMenuMap.insert(TOP_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK_ADD)
	{
		m_TopMenuMap.insert(TOP_MENU_CLEAR_ALL, STR_CLEAR_ALL);
		m_TopMenuMap.insert(TOP_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
	}

	m_pInfoHome->GetFormPlay()->ClearMenu();
	m_pInfoHome->GetFormPlay()->SetMenu(m_TopMenuMap);

	m_pInfoTracks->GetFormPlay()->ClearMenu();
	m_pInfoTracks->GetFormPlay()->SetMenu(m_TopMenuMap);
}

void MusicDBWindow::DoTopMenuPlay(int nWhere)
{
	m_pMgr->RequestManageCategory(VAL_PLAY, m_SelectMap, nWhere, m_nCategory);
}

void MusicDBWindow::DoTopMenuReload()
{
	UtilNovatron::RemoveTempDirectory();
	UtilNovatron::CreateTempDirectory();

	m_pIconTracks->ClearNodeList();
	m_pListTracks->ClearNodeList();

	m_CurIndex = 0;

//	if (m_nCategory == SQLManager::CATEGORY_TRACK)
//	{
//		RequestTrackList(m_nID, m_nCatID, m_nCatID2);
//	}
//	else
//	{
//		RequestCategoryList(m_nCatID, m_nCatID2);
//	}

	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		RequestCategoryList(m_nCatID, m_nCatID2);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		RequestTrackList(m_nID, m_nCatID, m_nCatID2);
	}
}

void MusicDBWindow::DoTopMenuSetLimitCount(int count)
{
	LimitCountDialog dialog;
	dialog.SetLimitCount(count);
	if (dialog.exec() == QDialog::Accepted)
	{
		m_LimitCount = dialog.GetLimitCount();
	}
}

void MusicDBWindow::DoTopMenuSelectAll()
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

void MusicDBWindow::DoTopMenuClearAll()
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

void MusicDBWindow::DoTopMenuGainSet()
{
	m_pMgr->RequestManageCategory(VAL_GAIN_SET,
								  m_SelectMap,
								  PLAY_NONE,
								  m_nCategory,
								  m_EventID);
}

void MusicDBWindow::DoTopMenuGainClear()
{
	m_pMgr->RequestManageCategory(VAL_GAIN_CLEAR,
								  m_SelectMap,
								  PLAY_NONE,
								  m_nCategory,
								  m_EventID);

}

void MusicDBWindow::DoTopMenuAddToPlaylist()
{
	if (m_TypeMode == TYPE_MODE_TRACK
			|| m_TypeMode == TYPE_MODE_TRACK_ALBUM
			|| m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		ThreadTerminateIcon();
		ThreadTerminateList();

		PlaylistWindow *widget = new PlaylistWindow(this, m_pMgr->GetAddr());
		widget->AddWidgetItem(TYPE_MODE_ITEM_ADD);
		emit widget->SigAddWidget(widget, STR_PLAYLIST);
		widget->RequestPlaylist();

		connect(widget, SIGNAL(SigAddToPlaylist(int)), this, SLOT(SlotTopMenuAddToPlaylist(int)));
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		emit SigAddCategoryFromPlaylist(m_nCategory, m_SelectMap);
		emit SigRemoveWidget(this);
	}

//	m_pMgr->RequestManageCategory(VAL_ADD,
//								  m_SelectMap,
//								  PLAY_NONE,
//								  m_nCategory);
}


void MusicDBWindow::DoTopMenuItemPlay(int nWhere)
{
	if (m_SelectMap.count() > 0)
	{
		m_pMgr->RequestManageCategory(VAL_PLAY,
									  m_SelectMap,
									  nWhere,
									  SQLManager::CATEGORY_TRACK);
	}
	else
	{
		QMap<int, bool> map;
		map.insert(m_nID, true);
		m_pMgr->RequestManageCategory(VAL_PLAY,
									  map,
									  nWhere,
									  m_nCategory);
	}

}

void MusicDBWindow::DoTopMenuItemSelectAll()
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

void MusicDBWindow::DoTopMenuItemClearAll()
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

void MusicDBWindow::DoTopMenuItemGainSet()
{
	if (m_SelectMap.count() > 0)
	{
		m_pMgr->RequestManageCategory(VAL_GAIN_SET,
									  m_SelectMap,
									  PLAY_NONE,
									  SQLManager::CATEGORY_TRACK,
									  m_EventID);
	}
	else
	{
		QMap<int, bool> map;
		map.insert(m_nID, true);
		m_pMgr->RequestManageCategory(VAL_GAIN_SET,
									  map,
									  PLAY_NONE,
									  m_nCategory,
									  m_EventID);
	}
}

void MusicDBWindow::DoTopMenuItemGainClear()
{
	if (m_SelectMap.count() > 0)
	{
		m_pMgr->RequestManageCategory(VAL_GAIN_CLEAR,
									  m_SelectMap,
									  PLAY_NONE,
									  SQLManager::CATEGORY_TRACK,
									  m_EventID);
	}
	else
	{
		QMap<int, bool> map;
		map.insert(m_nID, true);
		m_pMgr->RequestManageCategory(VAL_GAIN_CLEAR,
									  map,
									  PLAY_NONE,
									  m_nCategory,
									  m_EventID);
	}
}

void MusicDBWindow::DoTopMenuItemAddToPlaylist()
{
	if (m_TypeMode == TYPE_MODE_TRACK)
	{
		ThreadTerminateIcon();
		ThreadTerminateList();

		PlaylistWindow *widget = new PlaylistWindow(this, m_pMgr->GetAddr());
		widget->AddWidgetItem(TYPE_MODE_ITEM_ADD);
		emit widget->SigAddWidget(widget, STR_PLAYLIST);
		widget->RequestPlaylist();

		connect(widget, SIGNAL(SigAddToPlaylist(int)), this, SLOT(SlotTopMenuAddToPlaylist(int)));
	}
	else if (m_TypeMode == TYPE_MODE_TRACK_ADD)
	{
		emit SigAddTrackFromPlaylist(m_SelectMap);
		emit SigRemoveWidget(this);
	}

}

void MusicDBWindow::DoTopMenuItemShowColumns()
{
	SetColumnDialog dialog;
	dialog.SetCBFavorite(m_pListTracks->GetDelegate()->GetShowFavorite());
	dialog.SetCBTime(m_pListTracks->GetDelegate()->GetShowTime());
	dialog.SetCBArtist(m_pListTracks->GetDelegate()->GetShowArtist());
	dialog.SetCBAlbum(m_pListTracks->GetDelegate()->GetShowAlbum());
	dialog.SetCBGenre(m_pListTracks->GetDelegate()->GetShowGenre());
	dialog.SetCBAlbumArtist(m_pListTracks->GetDelegate()->GetShowAlbumArtist());
	dialog.SetCBComposer(m_pListTracks->GetDelegate()->GetShowComposer());
	dialog.SetCBYear(m_pListTracks->GetDelegate()->GetShowYear());

	dialog.SetCBMood(m_pListTracks->GetDelegate()->GetShowMood());
	dialog.SetCBTempo(m_pListTracks->GetDelegate()->GetShowTempo());
	dialog.SetCBFormat(m_pListTracks->GetDelegate()->GetShowFormat());
	dialog.SetCBSampleRate(m_pListTracks->GetDelegate()->GetShowSampleRate());
	dialog.SetCBBitDepth(m_pListTracks->GetDelegate()->GetShowBitDepth());
	dialog.SetCBRating(m_pListTracks->GetDelegate()->GetShowRating());

	if (dialog.exec() == QDialog::Accepted)
	{
		m_ShowFavorite = dialog.GetCBFavorite();
		m_ShowTime = dialog.GetCBTime();
		m_ShowArtist = dialog.GetCBArtist();
		m_ShowAlbum = dialog.GetCBAlbum();
		m_ShowGenre = dialog.GetCBGenre();
		m_ShowAlbumArtist = dialog.GetCBAlbumArtist();
		m_ShowComposer = dialog.GetCBComposer();
		m_ShowYear = dialog.GetCBYear();

		m_ShowMood = dialog.GetCBMood();
		m_ShowTempo = dialog.GetCBTempo();
		m_ShowFormat = dialog.GetCBFormat();
		m_ShowSampleRate = dialog.GetCBSampleRate();
		m_ShowBitDepth = dialog.GetCBBitDepth();
		m_ShowRating = dialog.GetCBRating();

		WriteSettings();

		m_pListTracks->ShowHeaderFavorite(m_ShowFavorite);
		m_pListTracks->ShowHeaderTime(m_ShowTime);
		m_pListTracks->ShowHeaderArtist(m_ShowArtist);
		m_pListTracks->ShowHeaderAlbum(m_ShowAlbum);
		m_pListTracks->ShowHeaderGenre(m_ShowGenre);
		m_pListTracks->ShowHeaderAlbumArtist(m_ShowAlbumArtist);
		m_pListTracks->ShowHeaderComposer(m_ShowComposer);
		m_pListTracks->ShowHeaderYear(m_ShowYear);

		m_pListTracks->ShowHeaderMood(m_ShowMood);
		m_pListTracks->ShowHeaderTempo(m_ShowTempo);
		m_pListTracks->ShowHeaderFormat(m_ShowFormat);
		m_pListTracks->ShowHeaderSampleRating(m_ShowSampleRate);
		m_pListTracks->ShowHeaderBitDepth(m_ShowBitDepth);
		m_pListTracks->ShowHeaderRating(m_ShowRating);

		m_pListTracks->GetDelegate()->SetShowFavorite(m_ShowFavorite);
		m_pListTracks->GetDelegate()->SetShowTime(m_ShowTime);
		m_pListTracks->GetDelegate()->SetShowArtist(m_ShowArtist);
		m_pListTracks->GetDelegate()->SetShowAlbum(m_ShowAlbum);
		m_pListTracks->GetDelegate()->SetShowGenre(m_ShowGenre);
		m_pListTracks->GetDelegate()->SetShowAlbumArtist(m_ShowAlbumArtist);
		m_pListTracks->GetDelegate()->SetShowComposer(m_ShowComposer);
		m_pListTracks->GetDelegate()->SetShowYear(m_ShowYear);

		m_pListTracks->GetDelegate()->SetShowMood(m_ShowMood);
		m_pListTracks->GetDelegate()->SetShowTempo(m_ShowTempo);
		m_pListTracks->GetDelegate()->SetShowFormat(m_ShowFormat);
		m_pListTracks->GetDelegate()->SetShowSampleRate(m_ShowSampleRate);
		m_pListTracks->GetDelegate()->SetShowBitDepth(m_ShowBitDepth);
		m_pListTracks->GetDelegate()->SetShowRating(m_ShowRating);

		// change value
		{
			int count = m_pListTracks->GetModel()->rowCount();
			for (int i = 0; i < count; i++)
			{
				m_pListTracks->SetEditor(i);
			}
		}
	}
}


void MusicDBWindow::SetOptionMenu()
{
	m_OptionMenuMap.clear();
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_OptionMenuMap.insert(OPTION_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
		m_OptionMenuMap.insert(OPTION_MENU_TAG_EDIT, STR_TAG_EDIT);
		m_OptionMenuMap.insert(OPTION_MENU_ADD_COVER_ART, STR_ADD_COVERART);
		m_OptionMenuMap.insert(OPTION_MENU_RENAME, STR_RENAME);
		m_OptionMenuMap.insert(OPTION_MENU_GAIN_SET, STR_GAIN_SET);
		m_OptionMenuMap.insert(OPTION_MENU_GAIN_CLEAR, STR_GAIN_CLEAR);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_OptionMenuMap.insert(OPTION_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
		m_OptionMenuMap.insert(OPTION_MENU_TAG_EDIT, STR_TAG_EDIT);
		m_OptionMenuMap.insert(OPTION_MENU_ADD_COVER_ART, STR_ADD_COVERART);
		m_OptionMenuMap.insert(OPTION_MENU_RENAME, STR_RENAME);
		m_OptionMenuMap.insert(OPTION_MENU_GAIN_SET, STR_GAIN_SET);
		m_OptionMenuMap.insert(OPTION_MENU_GAIN_CLEAR, STR_GAIN_CLEAR);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		m_OptionMenuMap.insert(OPTION_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK_ADD)
	{
		m_OptionMenuMap.insert(OPTION_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
	}

	m_pListTracks->GetDelegate()->SetOptionMenuMap(m_OptionMenuMap);

}

void MusicDBWindow::DoOptionMenuPlay(int nID, int where)
{
	SlotSelectPlay(nID, where);
}

void MusicDBWindow::DoOptionMenuAddToPlaylist(int nID)
{
	m_nOptionID = nID;

	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		ThreadTerminateIcon();
		ThreadTerminateList();

		PlaylistWindow *widget = new PlaylistWindow(this, m_pMgr->GetAddr());
		widget->AddWidgetItem(TYPE_MODE_ITEM_ADD);
		emit widget->SigAddWidget(widget, STR_PLAYLIST);
		widget->RequestPlaylist();

		connect(widget, SIGNAL(SigAddToPlaylist(int)), this, SLOT(SlotOptionMenuAddToPlaylist(int)));
	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		ThreadTerminateIcon();
		ThreadTerminateList();

		PlaylistWindow *widget = new PlaylistWindow(this, m_pMgr->GetAddr());
		widget->AddWidgetItem(TYPE_MODE_ITEM_ADD);
		emit widget->SigAddWidget(widget, STR_PLAYLIST);
		widget->RequestPlaylist();

		connect(widget, SIGNAL(SigAddToPlaylist(int)), this, SLOT(SlotOptionMenuAddToPlaylist(int)));
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		QMap<int, bool> map;
		map.insert(nID, true);
		emit SigAddCategoryFromPlaylist(m_nCategory, map);
		emit SigRemoveWidget(this);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK_ADD)
	{
		QMap<int, bool> map;
		map.insert(nID, true);
		emit SigAddTrackFromPlaylist(map);
		emit SigRemoveWidget(this);
//		emit SigRemoveWidget(this);
	}

}

void MusicDBWindow::DoOptionMenuInfo(int nID)
{
	m_nOptionID = nID;

	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		m_pMgr->RequestCategoryInfo(nID);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		m_pMgr->RequestTrackInfo(nID);
	}
}

void MusicDBWindow::DoOptionMenuAddCoverArt(int nID)
{
	QString site;
	QString keyword;
	QString artist;

	SearchCoverArtDialog searchDialog;
	if (searchDialog.exec() == QDialog::Accepted)
	{
		site = searchDialog.GetSite();
		keyword = searchDialog.GetKeyword();
		artist = searchDialog.GetArtist();
	}
	else
	{
		return;
	}

	SearchCoverArtResultDialog resultDialog;
	resultDialog.SetAddr(m_pMgr->GetAddr());
	resultDialog.RequestCoverArtList(site, keyword, artist);
	if (resultDialog.exec() == QDialog::Accepted)
	{
		QString image = resultDialog.GetImage();
		QString thumb = resultDialog.GetThumb();
		if (m_TypeMode == TYPE_MODE_ITEM_TRACK
				|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
				|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
				|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
		{
			m_pMgr->RequestSetCategoryCoverArt(nID,
									   m_nCategory,
									   m_EventID,
									   image,
									   thumb);
		}
		else if (m_TypeMode == TYPE_MODE_TRACK
				 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
				 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
		{
			m_pMgr->RequestSetTrackCoverArt(nID,
									   SQLManager::CATEGORY_TRACK,
									   m_EventID,
									   image,
									   thumb);
		}
//		DoTopMenuReload();
	}
}

void MusicDBWindow::DoOptionMenuRename(int nID)
{
	InputNameDialog dialog;
	if (dialog.exec() == QDialog::Accepted)
	{
		QString name = dialog.GetName();
		if (m_TypeMode == TYPE_MODE_ITEM_TRACK
				|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
				|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
				|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
		{
			m_pMgr->RequestRenameCategory(nID, name, m_nCategory, m_EventID);
		}
		else if (m_TypeMode == TYPE_MODE_TRACK
				 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
				 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
		{
			m_pMgr->RequestRenameTrack(nID, name, m_EventID);
		}
//		DoTopMenuReload();
	}

}

void MusicDBWindow::DoOptionMenuGain(int nID, QString gainType)
{
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		QMap<int, bool> map;
		map.insert(nID, true);
		m_pMgr->RequestManageCategory(gainType,
									  map,
									  PLAY_NONE,
									  m_nCategory,
									  m_EventID);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		QMap<int, bool> map;
		map.insert(nID, true);
		m_pMgr->RequestManageCategory(gainType,
									  map,
									  PLAY_NONE,
									  SQLManager::CATEGORY_TRACK,
									  m_EventID);
	}
}

//void MusicDBWindow::DoInsertIgnoreCategoryAll()
//{
//	m_pMgr->RequestInsertIgnoreCategoryAll(m_UpdateMap);
//}

void MusicDBWindow::SetCoverArt(QString coverArt)
{
	m_pInfoTracks->SetCoverArt(coverArt);
}

void MusicDBWindow::SetSortMenu(int category)
{
	QString title;
	QMap<int, QString> list;
	if (category == SQLManager::CATEGORY_TRACK)
	{
		title = GetTitleSortMenu(m_nSortTrack);

		list.insert(SQLManager::SORT_IMPORTED_DATE, STR_SORT_IMPORTED_DATE);
		list.insert(SQLManager::SORT_ALPHABET, STR_SORT_ALPHABET);
		list.insert(SQLManager::SORT_FAVORITE, STR_SORT_FAVORITE);

		m_pInfoTracks->GetFormSort()->ClearMenu();
		m_pInfoTracks->GetFormSort()->SetMenu(list);
		m_pInfoTracks->GetFormSort()->SetMenuTitle(title);
	}
	else if (category == SQLManager::CATEGORY_ARTIST
			 || category == SQLManager::CATEGORY_COMPOSER
			 || category == SQLManager::CATEGORY_MOOD
			 || category == SQLManager::CATEGORY_GENRE)
	{
		title = GetTitleSortMenu(m_nSortCategory);

		list.insert(SQLManager::SORT_IMPORTED_DATE, STR_SORT_IMPORTED_DATE);
		list.insert(SQLManager::SORT_ALPHABET, STR_SORT_ALPHABET);
		list.insert(SQLManager::SORT_FAVORITE, STR_SORT_FAVORITE);
		list.insert(SQLManager::SORT_RATING, STR_SORT_RATING);

		if (m_TypeMode == TYPE_MODE_ITEM_TRACK)
		{
			list.insert(SQLManager::DISP_MODE_TRACK, STR_DISP_MODE_TRACK);
			list.insert(SQLManager::DISP_MODE_ALBUM, STR_DISP_MODE_ALBUM);

			if (category == SQLManager::CATEGORY_GENRE)
			{
				list.insert(SQLManager::DISP_MODE_ARTIST, STR_DISP_MODE_ARTIST);
			}
		}
		else if (m_TypeMode == TYPE_MODE_ITEM_ALBUM
				 || m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
		{

		}
		else if (m_TypeMode == TYPE_MODE_ITEM_ARTIST)
		{
			list.insert(SQLManager::DISP_MODE_ALBUM, STR_DISP_MODE_ALBUM);

		}

		m_pInfoHome->GetFormSort()->ClearMenu();
		m_pInfoHome->GetFormSort()->SetMenu(list);
		m_pInfoHome->GetFormSort()->SetMenuTitle(title);
	}
	else
	{
		title = GetTitleSortMenu(m_nSortCategory);

		if (category == SQLManager::CATEGORY_YEAR)
		{
			title = STR_SORT_ALPHABET;
			list.insert(SQLManager::SORT_ALPHABET, STR_SORT_ALPHABET);

		}
		else
		{
			list.insert(SQLManager::SORT_IMPORTED_DATE, STR_SORT_IMPORTED_DATE);
			list.insert(SQLManager::SORT_ALPHABET, STR_SORT_ALPHABET);
			list.insert(SQLManager::SORT_FAVORITE, STR_SORT_FAVORITE);
			list.insert(SQLManager::SORT_RATING, STR_SORT_RATING);
		}

		m_pInfoHome->GetFormSort()->ClearMenu();
		m_pInfoHome->GetFormSort()->SetMenu(list);
		m_pInfoHome->GetFormSort()->SetMenuTitle(title);
	}
}

void MusicDBWindow::ClearClassifyMenu()
{
	m_pInfoHome->GetFormClassify()->ClearClassifyArtistMenu();
	m_pInfoHome->GetFormClassify()->ClearClassifyGenreMenu();
	m_pInfoHome->GetFormClassify()->ClearClassifyComposerMenu();
}

void MusicDBWindow::ThreadStartIcon()
{
	ThreadTerminateIcon();

	m_pIconThread->start();
}

void MusicDBWindow::ThreadStartList()
{
	ThreadTerminateList();

	m_pListThread->start();
}

void MusicDBWindow::ThreadTerminateIcon()
{
	if (m_pIconThread->isRunning())
	{
		m_pIconThread->terminate();
	}
}

void MusicDBWindow::ThreadTerminateList()
{
	if (m_pListThread->isRunning())
	{
		m_pListThread->terminate();
	}
}


void MusicDBWindow::CalculatePage(int totalCount)
{
	if (totalCount <= 0)
	{
		return;
	}

	m_TotalPage = ceil((float) totalCount / m_LimitCount);

//	LogDebug("limit [%d] total [%d] current [%d]", m_LimitCount, m_TotalPage, m_CurIndex);
}

void MusicDBWindow::SetColumn(int typeMode)
{
	if (typeMode <= TYPE_MODE_ITEM_ADD)
	{
		m_pListTracks->ShowHeaderFavorite(true);
//		m_pListTracks->ShowHeaderTime(true);
		m_pListTracks->ShowHeaderArtist(true);
//		m_pListTracks->ShowHeaderAlbum(true);
//		m_pListTracks->ShowHeaderGenre(true);

		m_pListTracks->GetDelegate()->SetShowFavorite(true);
//		m_pListTracks->GetDelegate()->SetShowTime(true);
		m_pListTracks->GetDelegate()->SetShowArtist(true);
//		m_pListTracks->GetDelegate()->SetShowAlbum(true);
//		m_pListTracks->GetDelegate()->SetShowGenre(true);
	}
	else
	{
		m_pListTracks->ShowHeaderFavorite(m_ShowFavorite);
		m_pListTracks->ShowHeaderTime(m_ShowTime);
		m_pListTracks->ShowHeaderArtist(m_ShowArtist);
		m_pListTracks->ShowHeaderAlbum(m_ShowAlbum);
		m_pListTracks->ShowHeaderGenre(m_ShowGenre);
		m_pListTracks->ShowHeaderAlbumArtist(m_ShowAlbumArtist);
		m_pListTracks->ShowHeaderComposer(m_ShowComposer);
		m_pListTracks->ShowHeaderYear(m_ShowYear);

		m_pListTracks->ShowHeaderMood(m_ShowMood);
		m_pListTracks->ShowHeaderTempo(m_ShowTempo);
		m_pListTracks->ShowHeaderFormat(m_ShowFormat);
		m_pListTracks->ShowHeaderSampleRating(m_ShowSampleRate);
		m_pListTracks->ShowHeaderBitDepth(m_ShowBitDepth);
		m_pListTracks->ShowHeaderRating(m_ShowRating);

		m_pListTracks->GetDelegate()->SetShowFavorite(m_ShowFavorite);
		m_pListTracks->GetDelegate()->SetShowTime(m_ShowTime);
		m_pListTracks->GetDelegate()->SetShowArtist(m_ShowArtist);
		m_pListTracks->GetDelegate()->SetShowAlbum(m_ShowAlbum);
		m_pListTracks->GetDelegate()->SetShowGenre(m_ShowGenre);
		m_pListTracks->GetDelegate()->SetShowAlbumArtist(m_ShowAlbumArtist);
		m_pListTracks->GetDelegate()->SetShowComposer(m_ShowComposer);
		m_pListTracks->GetDelegate()->SetShowYear(m_ShowYear);

		m_pListTracks->GetDelegate()->SetShowMood(m_ShowMood);
		m_pListTracks->GetDelegate()->SetShowTempo(m_ShowTempo);
		m_pListTracks->GetDelegate()->SetShowFormat(m_ShowFormat);
		m_pListTracks->GetDelegate()->SetShowSampleRate(m_ShowSampleRate);
		m_pListTracks->GetDelegate()->SetShowBitDepth(m_ShowBitDepth);
		m_pListTracks->GetDelegate()->SetShowRating(m_ShowRating);
	}
}

int MusicDBWindow::GetListModeFromResize(int resize)
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

QString MusicDBWindow::GetTitleSortMenu(int sort)
{
	QString title;

	switch (sort)
	{
	case SQLManager::SORT_IMPORTED_DATE:
		title = STR_SORT_IMPORTED_DATE;
		break;
	case SQLManager::SORT_ALPHABET:
		title = STR_SORT_ALPHABET;
		break;
	case SQLManager::SORT_FAVORITE:
		title = STR_SORT_FAVORITE;
		break;
	case SQLManager::SORT_RATING:
		title = STR_SORT_RATING;
		break;
//	case SQLManager::DISP_MODE_TRACK:
//		title = STR_DISP_MODE_TRACK;
//		break;
//	case SQLManager::DISP_MODE_ALBUM:
//		title = STR_DISP_MODE_ALBUM;
//		break;
//	case SQLManager::DISP_MODE_ARTIST:
//		title = STR_DISP_MODE_ARTIST;
//		break;
//	case SQLManager::DISP_MODE_ARTIST_ALBUM:
//		title = STR_DISP_MODE_ALBUM;
//		break;
	}

	return title;
}




