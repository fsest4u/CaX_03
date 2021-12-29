#include <QStackedWidget>
#include <QThread>

#include "musicdbwindow.h"
#include "ui_musicdbwindow.h"

#include "dialog/commondialog.h"
#include "dialog/inputnamedialog.h"
#include "dialog/limitcountdialog.h"
#include "dialog/searchcoverartdialog.h"
#include "dialog/searchcoverartresultdialog.h"
#include "dialog/trackinfodialog.h"
#include "dialog/trackinfo.h"

#include "manager/musicdbmanager.h"
#include "manager/sqlmanager.h"

#include "util/caxkeyvalue.h"
#include "util/caxtranslate.h"
#include "util/log.h"
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
	m_CurPage(0),
	m_nID(-1),
	m_nOptionID(-1),
	ui(new Ui::MusicDBWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
	Initialize();

}

MusicDBWindow::~MusicDBWindow()
{
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
//	AddSortMusicDBHome();
	SetSortMenu(category);

	m_TypeMode = typeMode;
	m_nCategory = category;

	m_ListMode = VIEW_MODE_ICON;
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

	if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		m_pInfoHome->GetFormPlay()->ShowPlayAll(false);
		m_pInfoHome->GetFormPlay()->ShowPlayRandom(false);
	}
}

void MusicDBWindow::AddWidgetTrack(int typeMode, int category)
{
//	AddSortCategoryHome();
	SetSortMenu(SQLManager::CATEGORY_TRACK);

	m_TypeMode = typeMode;
	m_nCategory = category;

	m_ListMode = VIEW_MODE_LIST;
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

	if (m_TypeMode == TYPE_MODE_TRACK_ADD)
	{
		m_pInfoTracks->GetFormPlay()->ShowPlayAll(false);
		m_pInfoTracks->GetFormPlay()->ShowPlayRandom(false);
		m_pInfoTracks->GetFormPlay()->ShowFavorite(false);
		m_pInfoTracks->GetFormPlay()->ShowRating(false);
	}

	m_pListTracks->SetLineEditReadOnly(false);

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

void MusicDBWindow::RequestTrackList(int nID, int nSort, bool bIncrease)
{
	m_nID = nID;
//	m_nCategory = nCategory;
	m_nSortTrack = nSort;
	m_bIncreaseTrack = bIncrease;

	if (m_nCategory == SQLManager::CATEGORY_TRACK)
	{
		m_pInfoTracks->SetTitle(KEY_TRACK);
		m_pMgr->RequestMusicDBOverView();
	}
	else
	{
		m_pMgr->RequestCategoryOverview(m_nID, m_nCategory);
	}

	m_pMgr->RequestTrackList(m_nID,
								   m_nCategory,
								   m_nSortTrack,
								   m_bIncreaseTrack,
								   m_LimitCount * m_CurPage,
								   m_LimitCount);
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
	LogDebug("node [%s]", node.ToCompactByteArray().data());
	int totalCount = GetTotalCount(node);
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

	m_RespList = list;

	SetOptionMenu();

	m_pIconTracks->ClearNodeList();
	m_pIconTracks->SetNodeList(m_RespList, IconTracks::ICON_TRACKS_MUSIC_DB);
	ThreadStartIcon();

//	m_pListTracks->ClearNodeList();
//	m_pListTracks->SetNodeList(m_RespList, ListTracks::LIST_TRACKS_MUSIC_DB);

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

	m_RespList = list;

	SetOptionMenu();

//	m_pIconTracks->ClearNodeList();
//	m_pIconTracks->SetNodeList(m_RespList, IconTracks::ICON_TRACKS_MUSIC_DB);
//	ThreadStartIcon();

	m_pListTracks->ClearNodeList();
	m_pListTracks->SetNodeList(m_RespList, ListTracks::LIST_TRACKS_MUSIC_DB);
	ThreadStartList();

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
			LogDebug("icon~~~~~~~~[%d][%d]", m_pIconTracks->GetNodeList().count(), m_RespList.count());
			if (m_pIconTracks->GetNodeList().count() != m_RespList.count())
			{
				m_pIconTracks->ClearNodeList();
				m_pIconTracks->SetNodeList(m_RespList, IconTracks::ICON_TRACKS_MUSIC_DB);
				ThreadStartIcon();
			}

			m_pListTracks->hide();
			m_pIconTracks->show();
			ui->gridLayoutBottom->replaceWidget(m_pListTracks, m_pIconTracks);
		}
		else
		{
			LogDebug("list~~~~~~~~", m_pListTracks->GetNodeList().count(), m_RespList.count());
			if (m_pListTracks->GetNodeList().count() != m_RespList.count())
			{
				m_pListTracks->ClearNodeList();
				m_pListTracks->SetNodeList(m_RespList, ListTracks::LIST_TRACKS_MUSIC_DB);
				ThreadStartList();
			}

			if (m_TypeMode == TYPE_MODE_ITEM)
			{
				m_pListTracks->SetHeaderTitle(STR_NAME);
			}
			else if (m_TypeMode == TYPE_MODE_TRACK)
			{
				m_pListTracks->SetHeaderTitle(STR_TITLE);
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

void MusicDBWindow::SlotAlbumList()
{
	ThreadTerminateIcon();
	ThreadTerminateList();

	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetItem(m_TypeMode, SQLManager::CATEGORY_ALBUM);
	emit widget->SigAddWidget(widget, STR_MUSIC_DB);

	widget->RequestCategoryList();
}

void MusicDBWindow::SlotAlbumArtistList()
{
	ThreadTerminateIcon();
	ThreadTerminateList();

	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetItem(m_TypeMode, SQLManager::CATEGORY_ALBUM_ARTIST);
	emit widget->SigAddWidget(widget, STR_MUSIC_DB);

	widget->RequestCategoryList();
}

void MusicDBWindow::SlotArtistList()
{
	ThreadTerminateIcon();
	ThreadTerminateList();

	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetItem(m_TypeMode, SQLManager::CATEGORY_ARTIST);
	emit widget->SigAddWidget(widget, STR_MUSIC_DB);

	widget->RequestCategoryList();
}

void MusicDBWindow::SlotGenreList()
{
	ThreadTerminateIcon();
	ThreadTerminateList();

	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetItem(m_TypeMode, SQLManager::CATEGORY_GENRE);
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
		widget->AddWidgetItem(m_TypeMode, nCategory);
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
	RequestTrackList(m_nID, sort, m_bIncreaseTrack);
}

void MusicDBWindow::SlotItemIncDec(bool bIncrease)
{
	RequestTrackList(m_nID, m_nSortTrack, bIncrease);
}

void MusicDBWindow::SlotSelectPlay(int nID, int where)
{
	if (m_TypeMode == TYPE_MODE_ITEM)
	{
		SlotSelectCategoryPlay(nID, where);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK)
	{
		SlotSelectTrackPlay(nID, where);
	}
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
		if (m_nCategory == SQLManager::CATEGORY_TRACK)
		{
			SlotSelectTrackPlay(nID, PLAY_CLEAR);
		}
		else
		{
			ThreadTerminateIcon();
			ThreadTerminateList();

			MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
			widget->AddWidgetTrack(TYPE_MODE_TRACK, m_nCategory);
			emit widget->SigAddWidget(widget, STR_MUSIC_DB);

			widget->RequestTrackList(nID);
			widget->SetCoverArt(coverArt);
		}

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
	if (m_TypeMode == TYPE_MODE_ITEM || m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		strCat = m_pMgr->GetSqlMgr()->GetCategoryName(m_nCategory);
	}
	else
	{
		strCat = m_pMgr->GetSqlMgr()->GetCategoryName(-1);
	}

	QStringList lsAddr = m_pMgr->GetAddr().split(":");
	QString fullpath = QString("%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(strCat).arg(id);

//	LogDebug("path [%s] index [%d]", fullpath.toUtf8().data(), index);
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

	m_pMgr->RequestCategoryList(m_nCategory, m_nSortCategory, m_bIncreaseCategory, m_ArtistID, m_GenreID, m_ComposerID);
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

void MusicDBWindow::SlotRespUpdateCategory(int updateId)
{
	if (updateId >= 0)
	{
		m_nID = updateId;
	}
	RequestTrackList(m_nID, m_nSortTrack, m_bIncreaseTrack);
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
	case OPTION_MENU_SEARCH_COVER_ART:
		DoOptionMenuSearchCoverArt(nID);
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

void MusicDBWindow::SlotEditAllArtist(QString value)
{
	LogDebug("edit artist [%s]", value.toUtf8().data());
	m_pMgr->RequestCheckCategory(m_nID, m_nCategory, SQLManager::CATEGORY_ARTIST, value);
}

void MusicDBWindow::SlotEditAllAlbum(QString value)
{
	LogDebug("edit album [%s]", value.toUtf8().data());
	m_pMgr->RequestCheckCategory(m_nID, m_nCategory, SQLManager::CATEGORY_ALBUM, value);
}

void MusicDBWindow::SlotEditAllGenre(QString value)
{
	LogDebug("edit genre [%s]", value.toUtf8().data());
	m_pMgr->RequestCheckCategory(m_nID, m_nCategory, SQLManager::CATEGORY_GENRE, value);
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
	connect(m_pMgr, SIGNAL(SigRespClassifyArtist(QList<CJsonNode>)), this, SLOT(SlotRespClassifyArtist(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespClassifyGenre(QList<CJsonNode>)), this, SLOT(SlotRespClassifyGenre(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespClassifyComposer(QList<CJsonNode>)), this, SLOT(SlotRespClassifyComposer(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespCategoryInfo(CJsonNode)), this, SLOT(SlotRespCategoryInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespCategoryInfoList(QList<CJsonNode>)), this, SLOT(SlotRespCategoryInfoList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespTrackInfo(CJsonNode)), this, SLOT(SlotRespTrackInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespUpdateCategory(int)), this, SLOT(SlotRespUpdateCategory(int)));
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
	connect(m_pIconTracks, SIGNAL(SigAppendIconList()), this, SLOT(SlotAppendIconList()));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectPlay(int, int)), this, SLOT(SlotSelectPlay(int, int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectFavorite(int, int)), this, SLOT(SlotSelectFavorite(int, int)));

	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectRating(int, int)), this, SLOT(SlotSelectRating(int, int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectSubtitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));

	connect(m_pListTracks, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));
	connect(m_pListTracks, SIGNAL(SigAppendList()), this, SLOT(SlotAppendList()));
	connect(m_pListTracks, SIGNAL(SigEditAllArtist(QString)), this, SLOT(SlotEditAllArtist(QString)));
	connect(m_pListTracks, SIGNAL(SigEditAllAlbum(QString)), this, SLOT(SlotEditAllAlbum(QString)));
	connect(m_pListTracks, SIGNAL(SigEditAllGenre(QString)), this, SLOT(SlotEditAllGenre(QString)));

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

	m_AlbumList.clear();
	m_AlbumArtistList.clear();
	m_ArtistList.clear();
	m_GenreList.clear();
	m_ComposerList.clear();
	m_MoodList.clear();

	m_pListTracks->SetLineEditReadOnly(true);

	m_pIconTracks->SetBackgroundTask(m_pIconThread);
	m_pListTracks->SetBackgroundTask(m_pListThread);
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

	if (m_nCategory == SQLManager::CATEGORY_TRACK)
	{
		RequestTrackList(m_nID, m_nSortTrack, m_bIncreaseTrack);
	}
	else
	{
		RequestCategoryList();
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
	if (m_TypeMode == TYPE_MODE_ITEM)
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
		m_OptionMenuMap.insert(OPTION_MENU_INFO, STR_TAG_EDIT);
		m_OptionMenuMap.insert(OPTION_MENU_SEARCH_COVER_ART, STR_SEARCH_COVERART);
		m_OptionMenuMap.insert(OPTION_MENU_RENAME, STR_RENAME);
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
		m_OptionMenuMap.insert(OPTION_MENU_INFO, STR_TAG_EDIT);
		m_OptionMenuMap.insert(OPTION_MENU_SEARCH_COVER_ART, STR_SEARCH_COVERART);
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

	if (m_TypeMode == TYPE_MODE_ITEM)
	{
		ThreadTerminateIcon();
		ThreadTerminateList();

		PlaylistWindow *widget = new PlaylistWindow(this, m_pMgr->GetAddr());
		widget->AddWidgetItem(TYPE_MODE_ITEM_ADD);
		emit widget->SigAddWidget(widget, STR_PLAYLIST);
		widget->RequestPlaylist();

		connect(widget, SIGNAL(SigAddToPlaylist(int)), this, SLOT(SlotOptionMenuAddToPlaylist(int)));
	}
	else if (m_TypeMode == TYPE_MODE_TRACK)
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

	if (m_TypeMode == TYPE_MODE_ITEM)
	{
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_ALBUM);
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_ALBUM_ARTIST);
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_ARTIST);
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_GENRE);
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_COMPOSER);
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_MOOD);

		m_pMgr->RequestCategoryInfo(nID);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK)
	{
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_ALBUM);
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_ALBUM_ARTIST);
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_ARTIST);
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_GENRE);
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_COMPOSER);
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_MOOD);

		m_pMgr->RequestTrackInfo(nID);
	}
}

void MusicDBWindow::DoOptionMenuSearchCoverArt(int nID)
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
		m_pMgr->RequestSetCoverArt(nID,
								   SQLManager::CATEGORY_TRACK,
								   m_EventID,
								   resultDialog.GetImage(),
								   resultDialog.GetThumb());

		// refresh
		RequestTrackList(m_nID, m_nSortTrack, m_bIncreaseTrack);
	}
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
			RequestTrackList(m_nID, m_nSortTrack, m_bIncreaseTrack);
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

void MusicDBWindow::SetSortMenu(int category)
{
	QString title;
	QMap<int, QString> list;
	if (category == SQLManager::CATEGORY_TRACK)
	{
		title = STR_SORT_IMPORTED_DATE;
		list.insert(SQLManager::SORT_IMPORTED_DATE, STR_SORT_IMPORTED_DATE);
		list.insert(SQLManager::SORT_ALPHABET, STR_SORT_ALPHABET);
		list.insert(SQLManager::SORT_FAVORITE, STR_SORT_FAVORITE);

		m_pInfoTracks->GetFormSort()->ClearMenu();
		m_pInfoTracks->GetFormSort()->SetMenu(list);
		m_pInfoTracks->GetFormSort()->SetMenuTitle(title);
	}
	else if (category == SQLManager::CATEGORY_YEAR)
	{
		title = STR_SORT_ALPHABET;
		list.insert(SQLManager::SORT_ALPHABET, STR_SORT_ALPHABET);

		m_pInfoHome->GetFormSort()->ClearMenu();
		m_pInfoHome->GetFormSort()->SetMenu(list);
		m_pInfoHome->GetFormSort()->SetMenuTitle(title);
	}
	else
	{
		title = STR_SORT_IMPORTED_DATE;
		list.insert(SQLManager::SORT_IMPORTED_DATE, STR_SORT_IMPORTED_DATE);
		list.insert(SQLManager::SORT_ALPHABET, STR_SORT_ALPHABET);
		list.insert(SQLManager::SORT_FAVORITE, STR_SORT_FAVORITE);
		list.insert(SQLManager::SORT_RATING, STR_SORT_RATING);

		m_pInfoHome->GetFormSort()->ClearMenu();
		m_pInfoHome->GetFormSort()->SetMenu(list);
		m_pInfoHome->GetFormSort()->SetMenuTitle(title);
	}
}

//void MusicDBWindow::AddSortMusicDBHome()
//{
//	QMap<int, QString> list;
//	list.insert(SQLManager::SORT_IMPORTED_DATE, STR_SORT_IMPORTED_DATE);
//	list.insert(SQLManager::SORT_ALPHABET, STR_SORT_ALPHABET);
//	list.insert(SQLManager::SORT_FAVORITE, STR_SORT_FAVORITE);
//	list.insert(SQLManager::SORT_RATING, STR_SORT_RATING);

//	m_pInfoHome->GetFormSort()->SetMenu(list);
//	m_pInfoHome->GetFormSort()->SetMenuTitle(STR_SORT_IMPORTED_DATE);
//}

//void MusicDBWindow::AddSortCategoryHome()
//{
////	QMap<int, QString> list;
////	list.insert(SQLManager::SORT_NAME, STR_SORT_NAME);
////	list.insert(SQLManager::SORT_ALBUM, STR_SORT_ALBUM);
////	list.insert(SQLManager::SORT_ALBUM_ARTIST, STR_SORT_ALBUM_ARTIST);
////	list.insert(SQLManager::SORT_ARTIST, STR_SORT_ARTIST);
////	list.insert(SQLManager::SORT_GENRE, STR_SORT_GENRE);
////	list.insert(SQLManager::SORT_COMPOSER, STR_SORT_COMPOSER);
////	list.insert(SQLManager::SORT_MOOD, STR_SORT_MOOD);
////	list.insert(SQLManager::SORT_FOLDER, STR_SORT_FOLDER);
////	list.insert(SQLManager::SORT_YEAR, STR_SORT_YEAR);

////	m_pInfoTracks->GetFormSort()->SetMenu(list);
////	m_pInfoTracks->GetFormSort()->SetMenuTitle(STR_SORT_NAME);

//}

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

int MusicDBWindow::GetTotalCount(CJsonNode node)
{

	int totalCount = 0;

	switch (m_nCategory)
	{
	case SQLManager::CATEGORY_ALBUM:
		totalCount = node.GetString(KEY_ALBUM).toInt();
		break;
//	case SQLManager::CATEGORY_ALBUM_ARTIST:
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




