#include <QStackedWidget>
#include <QMessageBox>

#include "musicdbwindow.h"
#include "ui_musicdbwindow.h"

#include "dialog/inputnamedialog.h"
#include "dialog/limitcountdialog.h"
#include "dialog/trackinfodialog.h"

#include "manager/musicdbmanager.h"
#include "manager/sqlmanager.h"

#include "util/caxkeyvalue.h"
#include "util/caxtranslate.h"
#include "util/log.h"

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

MusicDBWindow::MusicDBWindow(QWidget *parent, const QString &addr, const int &eventID) :
	QWidget(parent),
	m_pMgr(new MusicDBManager),
	m_pInfoHome(new InfoHome(this)),
	m_pInfoTracks(new InfoTracks(this)),
	m_pIconTracks(new IconTracks(this)),
	m_pListTracks(new ListTracks(this)),
	m_EventID(eventID),
	m_nCategory(SQLManager::CATEGORY_ALBUM),
	m_nSortCategory(SQLManager::SORT_NAME),
	m_nSortTrack(SQLManager::SORT_NAME),
	m_bIncreaseCategory(true),
	m_bIncreaseTrack(true),
	m_nFavorite(0),
	m_nRating(0),
	m_ArtistID(""),
	m_GenreID(""),
	m_ComposerID(""),
	m_LimitCount(100),
	m_TotalPage(0),
	m_CurPage(0),
	m_nID(-1),
	m_nOptionID(-1),
//	m_pCatThread(new QThread),
//	m_pSongThread(new QThread),
	ui(new Ui::MusicDBWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
	Initialize();

}

MusicDBWindow::~MusicDBWindow()
{
	delete ui;

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

//	if (m_pCatThread)
//	{
//		delete m_pCatThread;
//		m_pCatThread = nullptr;
//	}

//	if (m_pSongThread)
//	{
//		delete m_pSongThread;
//		m_pSongThread = nullptr;
//	}
}

void MusicDBWindow::AddWidgetItem(int typeMode)
{
	AddSortMusicDBHome();
	m_ListMode = VIEW_MODE_ICON;
	m_TypeMode = typeMode;
	m_pIconTracks->GetDelegate()->SetTypeMode(m_TypeMode);

	ui->gridLayoutTop->addWidget(m_pInfoHome);
//	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pInfoHome->GetFormSort()->SetResize(ICON_HEIGHT_MAX);
		ui->gridLayoutBottom->addWidget(m_pIconTracks);
	}
//	else
//	{
//		m_pInfoHome->GetFormSort()->SetResize(LIST_HEIGHT_MIN);
//		ui->gridLayoutBottom->addWidget(m_pListTracks);
//	}

	if (m_TypeMode == TYPE_MODE_ADD_ITEM)
	{
		m_pInfoHome->GetFormPlay()->ShowPlayAll(false);
		m_pInfoHome->GetFormPlay()->ShowPlayRandom(false);
	}
}

void MusicDBWindow::AddWidgetTrack(int typeMode)
{
	AddSortCategoryHome();
	m_ListMode = VIEW_MODE_LIST;
	m_TypeMode = typeMode;
	m_pIconTracks->GetDelegate()->SetTypeMode(m_TypeMode);

	ui->gridLayoutTop->addWidget(m_pInfoTracks);
//	if (m_ListMode == VIEW_MODE_ICON)
//	{
//		m_pInfoTracks->GetFormSort()->SetResize(ICON_HEIGHT_MAX);
//		ui->gridLayoutBottom->addWidget(m_pIconTracks);
//	}
//	else
	{
		m_pInfoTracks->GetFormSort()->SetResize(LIST_HEIGHT_MIN);
		ui->gridLayoutBottom->addWidget(m_pListTracks);
	}

	if (m_TypeMode == TYPE_MODE_ADD_TRACK)
	{
		m_pInfoTracks->GetFormPlay()->ShowPlayAll(false);
		m_pInfoTracks->GetFormPlay()->ShowPlayRandom(false);
		m_pInfoTracks->GetFormPlay()->ShowFavorite(false);
		m_pInfoTracks->GetFormPlay()->ShowRating(false);
	}
}

void MusicDBWindow::RequestCategoryList()
{
	m_pInfoHome->SetTitle(m_nCategory);
	m_pMgr->RequestMusicDBOverView();
	m_pMgr->RequestCategoryList(m_nCategory,
								m_nSortCategory,
								m_bIncreaseCategory,
								m_ArtistID,
								m_GenreID,
								m_ComposerID,
								m_nFavorite,
								m_nRating,
								m_LimitCount * m_CurPage,
								m_LimitCount);
}

void MusicDBWindow::RequestTrackList(int nID, int nCategory, int nSort, bool bIncrease)
{
	m_pIconTracks->ClearNodeList();
	m_pListTracks->ClearNodeList();

	m_nID = nID;
	m_nCategory = nCategory;
	m_nSortTrack = nSort;
	m_bIncreaseTrack = bIncrease;

	m_pMgr->RequestCategoryOverview(m_nID, m_nCategory);
	m_pMgr->RequestTrackList(m_nID,
								   m_nCategory,
								   m_nSortTrack,
								   m_bIncreaseTrack,
								   m_LimitCount * m_CurPage,
								   m_LimitCount);
}

int MusicDBWindow::GetCategory() const
{
	return m_nCategory;
}

void MusicDBWindow::SetCategory(int nCategory)
{
	m_nCategory = nCategory;
}

void MusicDBWindow::SlotAddWidget(QWidget *widget, QString title)
{
	emit SigAddWidget(widget, title);	// recursive
}

void MusicDBWindow::SlotRemoveWidget(QWidget *widget)
{
	emit SigRemoveWidget(widget);
}

void MusicDBWindow::SlotRespError(QString errMsg)
{
	QMessageBox::warning(this, "Warning", errMsg);
}

void MusicDBWindow::SlotRespMusicOverview(CJsonNode node)
{
	LogDebug("node [%s]", node.ToCompactByteArray().data());
	int totalCount = GetTotalCount(node);
	CalculatePage(totalCount);

	QString strAlbumCnt = node.GetString(KEY_ALBUM);
	QString strArtistCnt = node.GetString(KEY_ARTIST);
	QString strGenreCnt = node.GetString(KEY_GENRE);
	QString strTrackCnt = node.GetString(KEY_SONG);

	if (strAlbumCnt.toInt() > 0)
	{
		m_pInfoHome->SetAlbumCnt(strAlbumCnt);
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
	if (strTrackCnt.toInt() > 0)
	{
		m_pInfoHome->SetTrackCnt(node.GetString(KEY_SONG));
		m_pMgr->RequestClassifyList(SQLManager::CATEGORY_COMPOSER);
	}

}

void MusicDBWindow::SlotRespCategoryList(QList<CJsonNode> list)
{
//	m_pIconTracks->SetBackgroundTask(m_pCatThread);

	SetOptionMenu();

	m_pIconTracks->ClearNodeList();
	m_pListTracks->ClearNodeList();

	m_pIconTracks->SetNodeList(list, IconTracks::ICON_TRACKS_MUSIC_DB);
	m_pListTracks->SetNodeList(list, ListTracks::LIST_TRACKS_MUSIC_DB);

//	m_pCatThread->start();
}

void MusicDBWindow::SlotRespCategoryOverview(CJsonNode node)
{
	LogDebug("node [%s]", node.ToCompactByteArray().data());
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
		title = "Track";
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
//	m_pListTracks->SetBackgroundTask(m_pSongThread);

	SetOptionMenu();

	m_pIconTracks->SetNodeList(list, IconTracks::ICON_TRACKS_MUSIC_DB);
	m_pListTracks->SetNodeList(list, ListTracks::LIST_TRACKS_MUSIC_DB);

//	m_pSongThread->start();
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
	m_nSortCategory = menuID;
	RequestCategoryList();

}

void MusicDBWindow::SlotIncDec(bool bIncrease)
{
	m_bIncreaseCategory = bIncrease;
	RequestCategoryList();
}

void MusicDBWindow::SlotResize(int resize)
{
	int listMode = VIEW_MODE_ICON;
	if (resize > 130)
	{
		listMode = VIEW_MODE_ICON;
	}
	else
	{
		listMode = VIEW_MODE_LIST;
	}

	if (listMode != m_ListMode)
	{
		m_ListMode = listMode;
		if (m_ListMode == VIEW_MODE_ICON)
		{
			LogDebug("icon~~~~~~~~");
			m_pListTracks->hide();
			m_pIconTracks->show();
			ui->gridLayoutBottom->replaceWidget(m_pListTracks, m_pIconTracks);
		}
		else
		{
			LogDebug("list~~~~~~~~");
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

void MusicDBWindow::SlotGenreList()
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetItem(m_TypeMode);
	emit widget->SigAddWidget(widget, STR_GENRE);

	widget->SetCategory(SQLManager::CATEGORY_GENRE);
	widget->RequestCategoryList();
}

void MusicDBWindow::SlotAlbumList()
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetItem(m_TypeMode);
	emit widget->SigAddWidget(widget, STR_ALBUM);

	widget->SetCategory(SQLManager::CATEGORY_ALBUM);
	widget->RequestCategoryList();
}

void MusicDBWindow::SlotArtistList()
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetItem(m_TypeMode);
	emit widget->SigAddWidget(widget, STR_ARTIST);

	widget->SetCategory(SQLManager::CATEGORY_ARTIST);
	widget->RequestCategoryList();
}

void MusicDBWindow::SlotTrackList()
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetItem(m_TypeMode);
	emit widget->SigAddWidget(widget, STR_TRACK);

	widget->RequestTrackList(-1, SQLManager::CATEGORY_TRACK);
	widget->SetCoverArt("");
}

void MusicDBWindow::SlotCategoryMenu()
{
	m_CategoryMenuMap.clear();
	m_CategoryMenuMap.insert(SQLManager::CATEGORY_ALBUMARTIST, STR_ALBUM_ARTIST);
	m_CategoryMenuMap.insert(SQLManager::CATEGORY_COMPOSER, STR_COMPOSER);
	m_CategoryMenuMap.insert(SQLManager::CATEGORY_MOOD, STR_MOOD);
	m_CategoryMenuMap.insert(SQLManager::CATEGORY_FOLDER, STR_FOLDER);
	m_CategoryMenuMap.insert(SQLManager::CATEGORY_YEAR, STR_YEAR);

	m_pInfoHome->ClearMenu();
	m_pInfoHome->SetMenu(m_CategoryMenuMap);
}

void MusicDBWindow::SlotCategoryMenuAction(int nCategory, QString title)
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetItem(m_TypeMode);
	emit widget->SigAddWidget(widget, title);

	widget->SetCategory(nCategory);
	widget->RequestCategoryList();
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
	RequestTrackList(m_nID, m_nCategory, sort, m_bIncreaseTrack);
}

void MusicDBWindow::SlotItemIncDec(bool bIncrease)
{
	RequestTrackList(m_nID, m_nCategory, m_nSortCategory, bIncrease);
}

//void MusicDBWindow::SlotItemResize(int resize)
//{
//	int listMode = VIEW_MODE_ICON;
//	if (resize > 130)
//	{
//		listMode = VIEW_MODE_ICON;
//	}
//	else
//	{
//		listMode = VIEW_MODE_LIST;
//	}

//	if (listMode != m_ListMode)
//	{
//		m_ListMode = listMode;
//		if (m_ListMode == VIEW_MODE_ICON)
//		{
//			LogDebug("icon~~~~~~~~");
//			ui->gridLayoutBottom->replaceWidget(m_pListTracks, m_pIconTracks);
//			m_pListTracks->hide();
//			m_pIconTracks->show();

//		}
//		else
//		{
//			LogDebug("list~~~~~~~~");
//			ui->gridLayoutBottom->replaceWidget(m_pIconTracks, m_pListTracks);
//			m_pIconTracks->hide();
//			m_pListTracks->show();
//		}
//	}

//	if (m_ListMode == VIEW_MODE_ICON)
//	{
//		m_pIconTracks->SetResize(resize);
//	}
//	else
//	{
//		m_pListTracks->SetResize(resize);
//	}
//}

void MusicDBWindow::SlotSelectTitle(int nID, QString coverArt)
{
	LogDebug("id [%d] cover art [%s]", nID, coverArt.toUtf8().data());
	if (m_TypeMode == TYPE_MODE_ITEM)
	{
		MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
		widget->AddWidgetTrack();
		emit widget->SigAddWidget(widget, STR_MUSIC_DB);

		widget->RequestTrackList(nID, m_nCategory);
		widget->SetCoverArt(coverArt);
	}
	else if (m_TypeMode == TYPE_MODE_ADD_ITEM)
	{
		MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
		widget->AddWidgetTrack(TYPE_MODE_ADD_TRACK);
		emit widget->SigAddWidget(widget, STR_MUSIC_DB);

		widget->RequestTrackList(nID, m_nCategory);
		widget->SetCoverArt(coverArt);

		connect(widget, SIGNAL(SigAddTrackFromPlaylist(QMap<int, bool>)), this, SLOT(SlotAddTrackFromPlaylist(QMap<int, bool>)));

	}

}

void MusicDBWindow::SlotSelectPlay(int nID, int playType)
{
	QMap<int, bool> map;
	map.insert(nID, true);
	m_pMgr->RequestManageCategory(VAL_PLAY, map, playType, m_nCategory);
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
	if (m_TypeMode == TYPE_MODE_ITEM || m_TypeMode == TYPE_MODE_ADD_ITEM)
	{
		strCat = m_pMgr->GetSqlMgr()->GetCategoryName(m_nCategory);
	}
	else
	{
		strCat = m_pMgr->GetSqlMgr()->GetCategoryName(-1);
	}

	QStringList lsAddr = m_pMgr->GetAddr().split(":");
	QString fullpath = QString("%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(strCat).arg(id);

	LogDebug("path [%s] index [%d]", fullpath.toUtf8().data(), index);
	m_pMgr->RequestCoverArt(fullpath, index, mode);
}

void MusicDBWindow::SlotAppendIconList()
{
	m_CurPage++;
	if (m_TotalPage > m_CurPage)
	{
		m_pMgr->RequestCategoryList(m_nCategory,
									m_nSortCategory,
									m_bIncreaseCategory,
									m_ArtistID,
									m_GenreID,
									m_ComposerID,
									m_nFavorite,
									m_nRating,
									m_LimitCount * m_CurPage,
									m_LimitCount);
	}
	else
	{
		m_CurPage = m_TotalPage - 1;
	}
}

void MusicDBWindow::SlotAppendList()
{
	m_CurPage++;
	if (m_TotalPage > m_CurPage)
	{
		m_pMgr->RequestTrackList(m_nID,
									   m_nCategory,
									   m_nSortTrack,
									   m_bIncreaseTrack,
									   m_LimitCount * m_CurPage,
									   m_LimitCount);
	}
	else
	{
		m_CurPage = m_TotalPage - 1;
	}
}

void MusicDBWindow::SlotSelectTrackPlay(int nID, int playType)
{
//	m_pMgr->RequestPlaySong(nID);
	QMap<int, bool> map;
	map.insert(nID, true);
	m_pMgr->RequestManageCategory(VAL_PLAY, map, playType, SQLManager::CATEGORY_TRACK);
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
	RequestCategoryList();
}

void MusicDBWindow::SlotClassifyRating(int nRating)
{
	m_nRating = nRating;
	RequestCategoryList();
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

	m_pIconTracks->ClearNodeList();
	m_pListTracks->ClearNodeList();
	m_pMgr->RequestCategoryList(m_nCategory, m_nSortCategory, m_bIncreaseCategory, m_ArtistID, m_GenreID, m_ComposerID);
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

	m_pIconTracks->ClearNodeList();
	m_pListTracks->ClearNodeList();
	m_pMgr->RequestCategoryList(m_nCategory, m_nSortCategory, m_bIncreaseCategory, m_ArtistID, m_GenreID, m_ComposerID);
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

	m_pIconTracks->ClearNodeList();
	m_pListTracks->ClearNodeList();
	m_pMgr->RequestCategoryList(m_nCategory, m_nSortCategory, m_bIncreaseCategory, m_ArtistID, m_GenreID, m_ComposerID);
}

void MusicDBWindow::SlotRespCategoryInfo(CJsonNode node)
{
	TrackInfoDialog dialog;
	dialog.SetTitle(node.GetString(KEY_ALBUM));
	dialog.SetAlbum(node.GetString(KEY_ALBUM));
	dialog.SetAlbumArtist(node.GetString(KEY_ALBUM_ARTIST));
	dialog.SetCDYear(node.GetString(KEY_CDYEAR));
	dialog.SetCDNumber(QString("%1").arg(node.GetInt(KEY_CDNUMBER)));
	dialog.SetCDTotal(QString("%1").arg(node.GetInt(KEY_CDTOTAL)));
	dialog.exec();
}

void MusicDBWindow::SlotRespTrackInfo(CJsonNode node)
{
	TrackInfoDialog dialog;
	dialog.SetTitle(node.GetString(KEY_TITLE_CAP));
	dialog.SetAlbum(node.GetString(KEY_ALBUM));
	dialog.SetArtist(node.GetString(KEY_ARTIST));
	dialog.SetGenre(node.GetString(KEY_GENRE));
	dialog.SetAlbumArtist(node.GetString(KEY_ALBUM_ARTIST));
	dialog.SetComposer(node.GetString(KEY_COMPOSER));
	dialog.SetMood(node.GetString(KEY_MOOD));
	dialog.SetTempo(node.GetString(KEY_TEMPO));
	dialog.SetTrack(node.GetString(KEY_TRACK));
	dialog.SetPath(node.GetString(KEY_PATH));
	dialog.SetBitrate(node.GetString(KEY_BITRATE));
	dialog.SetDuration(node.GetString(KEY_DURATION));
	dialog.SetSampleRate(node.GetString(KEY_SAMPLERATE_CAP));
	dialog.SetChannel(node.GetString(KEY_CHANNEL));
	dialog.SetFormat(node.GetString(KEY_FORMAT));
	dialog.SetYear(node.GetString(KEY_YEAR));
	dialog.SetCDYear(node.GetString(KEY_CDYEAR));
	dialog.SetCDNumber(QString("%1").arg(node.GetInt(KEY_CDNUMBER)));
	dialog.SetCDTotal(QString("%1").arg(node.GetInt(KEY_CDTOTAL)));
	dialog.exec();
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
	case OPTION_MENU_INFO:
		DoOptionMenuInfo(nID);
		break;
	case OPTION_MENU_SEARCH_COVERART:
		DoOptionMenuSearchCoverArt(nID);
		break;
	case OPTION_MENU_RENAME_ITEM:
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
	if (m_TypeMode == TYPE_MODE_ITEM)
	{
		m_pMgr->RequestAddToPlaylist(id, m_SelectMap, m_nCategory);

	}
	else if (m_TypeMode == TYPE_MODE_TRACK)
	{
		m_pMgr->RequestAddToPlaylist(id, m_SelectMap, -1);

	}
}

void MusicDBWindow::SlotOptionMenuAddToPlaylist(int id)
{
	QMap<int, bool> map;
	map.insert(m_nOptionID, true);

	if (m_TypeMode == TYPE_MODE_ITEM)
	{
		m_pMgr->RequestAddToPlaylist(id, map, m_nCategory);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK)
	{
		m_pMgr->RequestAddToPlaylist(id, map, -1);
	}
}

void MusicDBWindow::SlotAddTrackFromPlaylist(QMap<int, bool> idMap)
{
	emit SigAddTrackFromPlaylist(idMap);	// recursive
	emit SigRemoveWidget(this);
}

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
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));
	connect(m_pMgr, SIGNAL(SigRespClassifyArtist(QList<CJsonNode>)), this, SLOT(SlotRespClassifyArtist(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespClassifyGenre(QList<CJsonNode>)), this, SLOT(SlotRespClassifyGenre(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespClassifyComposer(QList<CJsonNode>)), this, SLOT(SlotRespClassifyComposer(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespCategoryInfo(CJsonNode)), this, SLOT(SlotRespCategoryInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespTrackInfo(CJsonNode)), this, SLOT(SlotRespTrackInfo(CJsonNode)));

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

	connect(m_pInfoHome, SIGNAL(SigGenreList()), this, SLOT(SlotGenreList()));
	connect(m_pInfoHome, SIGNAL(SigAlbumList()), this, SLOT(SlotAlbumList()));
	connect(m_pInfoHome, SIGNAL(SigArtistList()), this, SLOT(SlotArtistList()));
	connect(m_pInfoHome, SIGNAL(SigTrackList()), this, SLOT(SlotTrackList()));
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
	connect(m_pIconTracks, SIGNAL(SigAppendIconList()), this, SLOT(SlotAppendIconList()));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectPlay(int, int)), this, SLOT(SlotSelectPlay(int, int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectFavorite(int, int)), this, SLOT(SlotSelectFavorite(int, int)));
	// todo-dylee, check to change list mode
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectRating(int, int)), this, SLOT(SlotSelectRating(int, int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectSubtitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));

	connect(m_pListTracks, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));
	connect(m_pListTracks, SIGNAL(SigAppendList()), this, SLOT(SlotAppendList()));
	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectPlay(int, int)), this, SLOT(SlotSelectTrackPlay(int, int)));
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
	m_pInfoHome->GetFormSort()->ShowIncDec();
	m_pInfoHome->GetFormSort()->ShowResize();
//	m_pInfoHome->GetFormSort()->SetResize(ICON_HEIGHT_MAX);
	m_pInfoHome->GetFormSort()->SetIncrease(m_bIncreaseCategory);

	m_pInfoTracks->GetFormPlay()->ShowPlayAll();
	m_pInfoTracks->GetFormPlay()->ShowPlayRandom();
	m_pInfoTracks->GetFormPlay()->ShowFavorite();
	m_pInfoTracks->GetFormPlay()->ShowRating();
	m_pInfoTracks->GetFormPlay()->ShowMenu();

	m_pInfoTracks->GetFormSort()->ShowMenu();
	m_pInfoTracks->GetFormSort()->ShowIncDec();
	m_pInfoTracks->GetFormSort()->ShowResize();
//	m_pInfoTracks->GetFormSort()->SetResize(LIST_HEIGHT_MIN);
	m_pInfoTracks->GetFormSort()->SetIncrease(m_bIncreaseTrack);

	m_TopMenuMap.clear();
	m_CategoryMenuMap.clear();
	m_SelectMap.clear();

}

void MusicDBWindow::SetSelectOffTopMenu()
{
	m_TopMenuMap.clear();
	if (m_TypeMode == TYPE_MODE_ITEM)
	{
		m_TopMenuMap.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_TopMenuMap.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_TopMenuMap.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_TopMenuMap.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_TopMenuMap.insert(TOP_MENU_RELOAD, STR_RELOAD);
		m_TopMenuMap.insert(TOP_MENU_LOAD_COUNT, QString("%1 - %2").arg(STR_LOAD_COUNT).arg(m_LimitCount));
		m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK)
	{
		m_TopMenuMap.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_TopMenuMap.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_TopMenuMap.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_TopMenuMap.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
	}
	else if (m_TypeMode == TYPE_MODE_ADD_ITEM)
	{
		m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
	}
	else if (m_TypeMode == TYPE_MODE_ADD_TRACK)
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
	if (m_TypeMode == TYPE_MODE_ITEM)
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
	else if (m_TypeMode == TYPE_MODE_TRACK)
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
	else if (m_TypeMode == TYPE_MODE_ADD_ITEM)
	{
		m_TopMenuMap.insert(TOP_MENU_CLEAR_ALL, STR_CLEAR_ALL);
		m_TopMenuMap.insert(TOP_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
	}
	else if (m_TypeMode == TYPE_MODE_ADD_TRACK)
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
	RequestCategoryList();
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
	if (m_TypeMode == TYPE_MODE_ITEM)
	{
		PlaylistWindow *widget = new PlaylistWindow(this, m_pMgr->GetAddr());
		widget->AddWidgetItem(TYPE_MODE_ADD_ITEM);
		emit widget->SigAddWidget(widget, STR_ADD_TO_PLAYLIST);
		widget->RequestPlaylist();

		connect(widget, SIGNAL(SigAddToPlaylist(int)), this, SLOT(SlotTopMenuAddToPlaylist(int)));
	}
	else if (m_TypeMode == TYPE_MODE_ADD_ITEM)
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
		PlaylistWindow *widget = new PlaylistWindow(this, m_pMgr->GetAddr());
		widget->AddWidgetItem(TYPE_MODE_ADD_ITEM);
		emit widget->SigAddWidget(widget, STR_ADD_TO_PLAYLIST);
		widget->RequestPlaylist();

		connect(widget, SIGNAL(SigAddToPlaylist(int)), this, SLOT(SlotTopMenuAddToPlaylist(int)));
	}
	else if (m_TypeMode == TYPE_MODE_ADD_TRACK)
	{
		emit SigAddTrackFromPlaylist(m_SelectMap);
		emit SigRemoveWidget(this);
	}

}


void MusicDBWindow::SetOptionMenu()
{
	m_OptionMenuMap.clear();
	if (m_TypeMode == TYPE_MODE_ITEM)
	{
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_OptionMenuMap.insert(OPTION_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
		m_OptionMenuMap.insert(OPTION_MENU_INFO, STR_INFO);
		m_OptionMenuMap.insert(OPTION_MENU_SEARCH_COVERART, STR_SEARCH_COVERART);
		m_OptionMenuMap.insert(OPTION_MENU_RENAME_ITEM, STR_RENAME_ITEM);
		m_OptionMenuMap.insert(OPTION_MENU_GAIN_SET, STR_GAIN_SET);
		m_OptionMenuMap.insert(OPTION_MENU_GAIN_CLEAR, STR_GAIN_CLEAR);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK)
	{
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_OptionMenuMap.insert(OPTION_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
		m_OptionMenuMap.insert(OPTION_MENU_INFO, STR_INFO);
		m_OptionMenuMap.insert(OPTION_MENU_SEARCH_COVERART, STR_SEARCH_COVERART);
		m_OptionMenuMap.insert(OPTION_MENU_RENAME_ITEM, STR_RENAME_ITEM);
		m_OptionMenuMap.insert(OPTION_MENU_GAIN_SET, STR_GAIN_SET);
		m_OptionMenuMap.insert(OPTION_MENU_GAIN_CLEAR, STR_GAIN_CLEAR);
	}
	else if (m_TypeMode == TYPE_MODE_ADD_ITEM)
	{
		m_OptionMenuMap.insert(OPTION_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
	}
	else if (m_TypeMode == TYPE_MODE_ADD_TRACK)
	{
		m_OptionMenuMap.insert(OPTION_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
	}

	m_pListTracks->GetDelegate()->SetOptionMenuMap(m_OptionMenuMap);

}

void MusicDBWindow::DoOptionMenuPlay(int nID, int where)
{
	if (m_TypeMode == TYPE_MODE_ITEM)
	{
		SlotSelectPlay(nID, where);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK)
	{
		SlotSelectTrackPlay(nID, where);
	}
}

void MusicDBWindow::DoOptionMenuAddToPlaylist(int nID)
{
	m_nOptionID = nID;

	if (m_TypeMode == TYPE_MODE_ITEM)
	{
		PlaylistWindow *widget = new PlaylistWindow(this, m_pMgr->GetAddr());
		widget->AddWidgetItem(TYPE_MODE_ADD_ITEM);
		emit widget->SigAddWidget(widget, STR_ADD_TO_PLAYLIST);
		widget->RequestPlaylist();

		connect(widget, SIGNAL(SigAddToPlaylist(int)), this, SLOT(SlotOptionMenuAddToPlaylist(int)));
	}
	else if (m_TypeMode == TYPE_MODE_TRACK)
	{
		PlaylistWindow *widget = new PlaylistWindow(this, m_pMgr->GetAddr());
		widget->AddWidgetItem(TYPE_MODE_ADD_ITEM);
		emit widget->SigAddWidget(widget, STR_ADD_TO_PLAYLIST);
		widget->RequestPlaylist();

		connect(widget, SIGNAL(SigAddToPlaylist(int)), this, SLOT(SlotOptionMenuAddToPlaylist(int)));
	}
	else if (m_TypeMode == TYPE_MODE_ADD_ITEM)
	{
		QMap<int, bool> map;
		map.insert(nID, true);
		emit SigAddCategoryFromPlaylist(m_nCategory, map);
		emit SigRemoveWidget(this);
	}
	else if (m_TypeMode == TYPE_MODE_ADD_TRACK)
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
	if (m_TypeMode == TYPE_MODE_ITEM)
	{
		m_pMgr->RequestCategoryInfo(nID);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK)
	{
		m_pMgr->RequestTrackInfo(nID);
	}
}

void MusicDBWindow::DoOptionMenuSearchCoverArt(int nID)
{
	// todo-dylee

}

void MusicDBWindow::DoOptionMenuRename(int nID)
{
	InputNameDialog dialog;
	if (dialog.exec() == QDialog::Accepted)
	{
		QString name = dialog.GetName();
		if (m_TypeMode == TYPE_MODE_ITEM)
		{
			m_pMgr->RequestRenameCategory(nID, name, m_nCategory, m_EventID);
			RequestCategoryList();
		}
		else if (m_TypeMode == TYPE_MODE_TRACK)
		{
			m_pMgr->RequestRenameTrack(nID, name, m_EventID);
			RequestTrackList(m_nID, m_nCategory, m_nSortTrack, m_bIncreaseTrack);
		}
	}

}

void MusicDBWindow::DoOptionMenuGain(int nID, QString gainType)
{
	if (m_TypeMode == TYPE_MODE_ITEM)
	{
		QMap<int, bool> map;
		map.insert(nID, true);
		m_pMgr->RequestManageCategory(gainType,
									  map,
									  PLAY_NONE,
									  m_nCategory,
									  m_EventID);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK)
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


void MusicDBWindow::SetCoverArt(QString coverArt)
{
	m_pInfoTracks->SetCoverArt(coverArt);
}

void MusicDBWindow::AddSortMusicDBHome()
{
	QMap<int, QString> list;
	list.insert(SQLManager::SORT_NAME, STR_SORT_NAME);
	list.insert(SQLManager::SORT_ALBUM, STR_SORT_ALBUM);
	list.insert(SQLManager::SORT_ALBUM_ARTIST, STR_SORT_ALBUM_ARTIST);
	list.insert(SQLManager::SORT_ARTIST, STR_SORT_ARTIST);
	list.insert(SQLManager::SORT_GENRE, STR_SORT_GENRE);
	list.insert(SQLManager::SORT_COMPOSER, STR_SORT_COMPOSER);
	list.insert(SQLManager::SORT_MOOD, STR_SORT_MOOD);
	list.insert(SQLManager::SORT_FOLDER, STR_SORT_FOLDER);
	list.insert(SQLManager::SORT_YEAR, STR_SORT_YEAR);

	m_pInfoHome->GetFormSort()->SetMenu(list);
	m_pInfoHome->GetFormSort()->SetMenuTitle(STR_SORT_NAME);
}

void MusicDBWindow::AddSortCategoryHome()
{
	QMap<int, QString> list;
	list.insert(SQLManager::SORT_NAME, STR_SORT_NAME);
	list.insert(SQLManager::SORT_ALBUM, STR_SORT_ALBUM);
	list.insert(SQLManager::SORT_ALBUM_ARTIST, STR_SORT_ALBUM_ARTIST);
	list.insert(SQLManager::SORT_ARTIST, STR_SORT_ARTIST);
	list.insert(SQLManager::SORT_GENRE, STR_SORT_GENRE);
	list.insert(SQLManager::SORT_COMPOSER, STR_SORT_COMPOSER);
	list.insert(SQLManager::SORT_MOOD, STR_SORT_MOOD);
	list.insert(SQLManager::SORT_FOLDER, STR_SORT_FOLDER);
	list.insert(SQLManager::SORT_YEAR, STR_SORT_YEAR);

	m_pInfoTracks->GetFormSort()->SetMenu(list);
	m_pInfoTracks->GetFormSort()->SetMenuTitle(STR_SORT_NAME);

}

void MusicDBWindow::ClearClassifyMenu()
{
	m_pInfoHome->GetFormClassify()->ClearClassifyArtistMenu();
	m_pInfoHome->GetFormClassify()->ClearClassifyGenreMenu();
	m_pInfoHome->GetFormClassify()->ClearClassifyComposerMenu();
}

int MusicDBWindow::GetTotalCount(CJsonNode node)
{

	int totalCount = 0;

	switch (m_nCategory)
	{
	case SQLManager::CATEGORY_ALBUM:
		totalCount = node.GetString(KEY_ALBUM).toInt();
		break;
//	case SQLManager::CATEGORY_ALBUMARTIST:
//		totalCount = node.GetString(KEY_ALBUM_ARTIST).toInt();
//		break;
	case SQLManager::CATEGORY_ARTIST:
		totalCount = node.GetString(KEY_ARTIST).toInt();
		break;
	case SQLManager::CATEGORY_COMPOSER:
		totalCount = node.GetString(KEY_COMPOSER).toInt();
		break;
	case SQLManager::CATEGORY_GENRE:
		totalCount = node.GetString(KEY_GENRE).toInt();
		break;
	case SQLManager::CATEGORY_MOOD:
		totalCount = node.GetString(KEY_MOOD).toInt();
		break;
	case SQLManager::CATEGORY_FOLDER:
		totalCount = node.GetString(KEY_FOLDER).toInt();
		break;
	case SQLManager::CATEGORY_YEAR:
		totalCount = node.GetString(KEY_YEAR).toInt();
		break;
	case SQLManager::CATEGORY_TRACK:
		totalCount = node.GetString(KEY_SONG).toInt();
		break;
	default:
		break;
	}

	return totalCount;
}

void MusicDBWindow::CalculatePage(int totalCount)
{
	if (totalCount <= 0)
	{
		return;
	}

	m_TotalPage = totalCount / m_LimitCount;
	m_CurPage = 0;
	LogDebug("limit [%d] total [%d] current [%d]", m_LimitCount, m_TotalPage, m_CurPage);
}




