#include <QStackedWidget>
#include <QMessageBox>

#include "musicdbwindow.h"
#include "ui_musicdbwindow.h"

#include "dialog/limitcountdialog.h"

#include "manager/musicdbmanager.h"
#include "manager/sqlmanager.h"

#include "util/caxconstants.h"
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

MusicDBWindow::MusicDBWindow(QWidget *parent, const QString &addr, const int &eventID) :
	QWidget(parent),
	m_pMgr(new MusicDBManager),
	m_pInfoHome(new InfoHome(this)),
	m_pInfoTracks(new InfoTracks(this)),
	m_pIconTracks(new IconTracks(this)),
	m_pListTracks(new ListTracks(this)),
	m_EventID(-1),
	m_nCategory(SQLManager::CATEGORY_ALBUM),
	m_nID(-1),
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
//	m_pCatThread(new QThread),
//	m_pSongThread(new QThread),
	ui(new Ui::MusicDBWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);
	m_EventID = eventID;

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

void MusicDBWindow::RequestMusicDBHome()
{
	m_pIconTracks->ClearNodeList();
	m_pInfoHome->SetTitle(m_nCategory);
	m_pMgr->RequestMusicDBInfo();
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

void MusicDBWindow::RequestCategoryHome(int nID, int nCategory, int nSort, bool bIncrease)
{
	m_pListTracks->ClearNodeList();
	m_nID = nID;
	m_nCategory = nCategory;
	m_nSortTrack = nSort;
	m_bIncreaseTrack = bIncrease;

	m_pMgr->RequestCategoryInfo(m_nID, m_nCategory);
	m_pMgr->RequestSongsOfCategory(m_nID,
								   m_nCategory,
								   m_nSortTrack,
								   m_bIncreaseTrack,
								   m_LimitCount * m_CurPage,
								   m_LimitCount);
}

void MusicDBWindow::AddWidgetMusicDBHome()
{
	ui->gridLayoutTop->addWidget(m_pInfoHome);
	ui->gridLayoutBottom->addWidget(m_pIconTracks);

	AddSortMusicDBHome();
}

void MusicDBWindow::AddWidgetCategoryHome()
{
	ui->gridLayoutTop->addWidget(m_pInfoTracks);
	ui->gridLayoutBottom->addWidget(m_pListTracks);

	AddSortCategoryHome();
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

void MusicDBWindow::SlotRespError(QString errMsg)
{
	QMessageBox::warning(this, "Warning", errMsg);
}

void MusicDBWindow::SlotRespMusicInfo(CJsonNode node)
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
	m_pIconTracks->SetNodeList(list, IconTracks::ICON_TRACKS_MUSIC_DB);
//	m_pCatThread->start();
}

void MusicDBWindow::SlotRespCategoryInfo(CJsonNode node)
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

void MusicDBWindow::SlotRespSongsOfCategory(QList<CJsonNode> list)
{
//	m_pListTracks->SetBackgroundTask(m_pSongThread);
	m_pListTracks->SetNodeList(list);
//	m_pSongThread->start();
}

void MusicDBWindow::SlotCoverArtUpdate(QString fileName, int nIndex, int mode)
{
	if (QListView::IconMode == mode)
	{
		QStandardItem *item = m_pIconTracks->GetModel()->item(nIndex);
		item->setData(fileName, IconTracksDelegate::ICON_TRACKS_COVER);
		m_pIconTracks->GetModel()->setItem(nIndex, item);
	}
	else
	{
		QStandardItem *item = m_pListTracks->GetModel()->item(nIndex);
		item->setData(fileName, ListTracksDelegate::LIST_TRACKS_COVER);
		m_pListTracks->GetModel()->setItem(nIndex, item);
	}

}

void MusicDBWindow::SlotPlayAll()
{
//	m_pMgr->RequestPlayCategoryItems(PLAY_CLEAR, m_nCategory);
	m_SelectItem = m_pIconTracks->GetSelectMap();
	if (m_SelectItem.count() > 0)
	{
		m_pMgr->RequestManageCategory(VAL_PLAY, m_SelectItem, PLAY_CLEAR, m_nCategory);
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
	m_SelectItem = m_pIconTracks->GetSelectMap();
	if (m_SelectItem.count() > 0)
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
	case TOP_MENU_UNSELECT:
		DoTopMenuUnselect();
		break;
	case TOP_MENU_GAIN_SET:
		DoTopMenuGainSet();
		break;
	case TOP_MENU_GAIN_CLEAR:
		DoTopMenuGainClear();
		break;
	case TOP_MENU_ADD_TO_PLAYLIST:
		DoTopMenuAddPlaylist();
		break;
	}

}

void MusicDBWindow::SlotSort(int sort)
{
	m_nSortCategory = sort;
	RequestMusicDBHome();

}

void MusicDBWindow::SlotIncDec(bool bIncrease)
{
	m_bIncreaseCategory = bIncrease;
	RequestMusicDBHome();
}

void MusicDBWindow::SlotResize()
{
	LogDebug("good choice resize");

}

void MusicDBWindow::SlotGenreList()
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetMusicDBHome();
	emit SigAddWidget(widget, STR_GENRE);

	widget->SetCategory(SQLManager::CATEGORY_GENRE);
	widget->RequestMusicDBHome();
}

void MusicDBWindow::SlotAlbumList()
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetMusicDBHome();
	emit SigAddWidget(widget, STR_ALBUM);

	widget->SetCategory(SQLManager::CATEGORY_ALBUM);
	widget->RequestMusicDBHome();
}

void MusicDBWindow::SlotArtistList()
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetMusicDBHome();
	emit SigAddWidget(widget, STR_ARTIST);

	widget->SetCategory(SQLManager::CATEGORY_ARTIST);
	widget->RequestMusicDBHome();
}

void MusicDBWindow::SlotTrackList()
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetCategoryHome();
	emit SigAddWidget(widget, STR_TRACK);

	widget->RequestCategoryHome(-1, SQLManager::CATEGORY_TRACK);
	widget->SetCoverArt("");
}

void MusicDBWindow::SlotSubmenu2()
{
	LogDebug("click Submenu2");
}

void MusicDBWindow::SlotItemPlayAll()
{
//	m_pMgr->RequestPlayCategoryItem(m_nID, PLAY_CLEAR, m_nCategory);
	m_SelectItem = m_pListTracks->GetSelectMap();
	if (m_SelectItem.count() > 0)
	{
		m_pMgr->RequestManageCategory(VAL_PLAY, m_SelectItem, PLAY_CLEAR, SQLManager::CATEGORY_TRACK);
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
	m_SelectItem = m_pListTracks->GetSelectMap();
	if (m_SelectItem.count() > 0)
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
		DoItemTopMenuPlay(PLAY_NOW);
		break;
	case TOP_MENU_PLAY_LAST:
		DoItemTopMenuPlay(PLAY_LAST);
		break;
	case TOP_MENU_PLAY_NEXT:
		DoItemTopMenuPlay(PLAY_NEXT);
		break;
	case TOP_MENU_PLAY_CLEAR:
		DoItemTopMenuPlay(PLAY_CLEAR);
		break;
	case TOP_MENU_RELOAD:
		break;
	case TOP_MENU_LOAD_COUNT:
		break;
	case TOP_MENU_SELECT_ALL:
		break;
	case TOP_MENU_UNSELECT:
		break;
	case TOP_MENU_GAIN_SET:
		break;
	case TOP_MENU_GAIN_CLEAR:
		break;
	case TOP_MENU_ADD_TO_PLAYLIST:
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

void MusicDBWindow::SlotItemSort(int sort)
{
	RequestCategoryHome(m_nID, m_nCategory, sort, m_bIncreaseTrack);
}

void MusicDBWindow::SlotItemIncDec(bool bIncrease)
{
	RequestCategoryHome(m_nID, m_nCategory, m_nSortCategory, bIncrease);
}

void MusicDBWindow::SlotItemResize()
{
	LogDebug("album resize");

}

void MusicDBWindow::SlotSelectTitle(int nID, QString coverArt)
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetCategoryHome();
	emit SigAddWidget(widget, STR_MUSIC_DB);

	widget->RequestCategoryHome(nID, m_nCategory);
	widget->SetCoverArt(coverArt);
}

void MusicDBWindow::SlotSelectPlay(int nID)
{
	LogDebug("click Play");
//	m_pMgr->RequestPlaySong();
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
	if (QListView::IconMode == mode)
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
		m_pMgr->RequestSongsOfCategory(m_nID,
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

void MusicDBWindow::SlotSelectTrackPlay(int nID)
{
//	m_pMgr->RequestPlaySong(nID);
	QMap<int, bool> map;
	map.insert(nID, true);
	m_pMgr->RequestManageCategory(VAL_PLAY, map, PLAY_NOW, SQLManager::CATEGORY_TRACK);
}

void MusicDBWindow::SlotSelectTrackMore(int nID)
{
	Q_UNUSED(nID)

	LogDebug("click select more - temp code");
	QListView::ViewMode mode = m_pListTracks->GetViewMode() == QListView::IconMode ? QListView::ListMode : QListView::IconMode;
	m_pListTracks->SetViewMode(mode);
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
	RequestMusicDBHome();
}

void MusicDBWindow::SlotClassifyRating(int nRating)
{
	m_nRating = nRating;
	RequestMusicDBHome();
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
	m_pMgr->RequestCategoryList(m_nCategory, m_nSortCategory, m_bIncreaseCategory, m_ArtistID, m_GenreID, m_ComposerID);
}

void MusicDBWindow::ConnectSigToSlot()
{
	// recursive
	connect(this, SIGNAL(SigAddWidget(QWidget*, QString)), parent(), SLOT(SlotAddWidget(QWidget*, QString)));

	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespMusicInfo(CJsonNode)), this, SLOT(SlotRespMusicInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespCategoryList(QList<CJsonNode>)), this, SLOT(SlotRespCategoryList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespCategoryInfo(CJsonNode)), this, SLOT(SlotRespCategoryInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespSongsOfCategory(QList<CJsonNode>)), this, SLOT(SlotRespSongsOfCategory(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));
	connect(m_pMgr, SIGNAL(SigRespClassifyArtist(QList<CJsonNode>)), this, SLOT(SlotRespClassifyArtist(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespClassifyGenre(QList<CJsonNode>)), this, SLOT(SlotRespClassifyGenre(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespClassifyComposer(QList<CJsonNode>)), this, SLOT(SlotRespClassifyComposer(QList<CJsonNode>)));

	connect(m_pInfoHome->GetFormPlay(), SIGNAL(SigPlayAll()), this, SLOT(SlotPlayAll()));
	connect(m_pInfoHome->GetFormPlay(), SIGNAL(SigPlayRandom()), this, SLOT(SlotPlayRandom()));
	connect(m_pInfoHome->GetFormPlay(), SIGNAL(SigTopMenu()), this, SLOT(SlotTopMenu()));
	connect(m_pInfoHome->GetFormPlay(), SIGNAL(SigTopMenuAction(int)), this, SLOT(SlotTopMenuAction(int)));

	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigClassifyFavorite(int)), this, SLOT(SlotClassifyFavorite(int)));
	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigClassifyRating(int)), this, SLOT(SlotClassifyRating(int)));
	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigClassifyArtist(bool, QString)), this, SLOT(SlotClassifyArtist(bool, QString)));
	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigClassifyGenre(bool, QString)), this, SLOT(SlotClassifyGenre(bool, QString)));
	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigClassifyComposer(bool, QString)), this, SLOT(SlotClassifyComposer(bool, QString)));

	connect(m_pInfoHome->GetFormSort(), SIGNAL(SigSort(int)), this, SLOT(SlotSort(int)));
	connect(m_pInfoHome->GetFormSort(), SIGNAL(SigIncDec(bool)), this, SLOT(SlotIncDec(bool)));
	connect(m_pInfoHome->GetFormSort(), SIGNAL(SigResize()), this, SLOT(SlotResize()));

	connect(m_pInfoHome, SIGNAL(SigGenreList()), this, SLOT(SlotGenreList()));
	connect(m_pInfoHome, SIGNAL(SigAlbumList()), this, SLOT(SlotAlbumList()));
	connect(m_pInfoHome, SIGNAL(SigArtistList()), this, SLOT(SlotArtistList()));
	connect(m_pInfoHome, SIGNAL(SigTrackList()), this, SLOT(SlotTrackList()));
	connect(m_pInfoHome, SIGNAL(SigSubmenu2()), this, SLOT(SlotSubmenu2()));

	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigPlayAll()), this, SLOT(SlotItemPlayAll()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigPlayRandom()), this, SLOT(SlotItemPlayRandom()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigFavorite(int)), this, SLOT(SlotItemFavorite(int)));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigRating(int)), this, SLOT(SlotItemRating(int)));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigTopMenu()), this, SLOT(SlotItemTopMenu()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigTopMenuAction(int)), this, SLOT(SlotItemTopMenuAction(int)));

	connect(m_pInfoTracks->GetFormSort(), SIGNAL(SigSort(int)), this, SLOT(SlotItemSort(int)));
	connect(m_pInfoTracks->GetFormSort(), SIGNAL(SigIncDec(bool)), this, SLOT(SlotItemIncDec(bool)));
	connect(m_pInfoTracks->GetFormSort(), SIGNAL(SigResize()), this, SLOT(SlotItemResize()));

	connect(m_pIconTracks, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));
	connect(m_pIconTracks, SIGNAL(SigAppendIconList()), this, SLOT(SlotAppendIconList()));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectPlay(int)), this, SLOT(SlotSelectPlay(int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectFavorite(int, int)), this, SLOT(SlotSelectFavorite(int, int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectRating(int, int)), this, SLOT(SlotSelectRating(int, int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectSubtitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));

	connect(m_pListTracks, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));
	connect(m_pListTracks, SIGNAL(SigAppendList()), this, SLOT(SlotAppendList()));
	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectPlay(int)), this, SLOT(SlotSelectTrackPlay(int)));
	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectMore(int)), this, SLOT(SlotSelectTrackMore(int)));
	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectFavorite(int, int)), this, SLOT(SlotSelectTrackFavorite(int, int)));


}

void MusicDBWindow::Initialize()
{
	m_pInfoHome->GetFormPlay()->ShowPlayAll();
	m_pInfoHome->GetFormPlay()->ShowPlayRandom();
	m_pInfoHome->GetFormPlay()->ShowTopMenu();

	m_pInfoHome->GetFormSort()->ShowSort();
	m_pInfoHome->GetFormSort()->ShowIncDec();
	m_pInfoHome->GetFormSort()->ShowResize();
	m_pInfoHome->GetFormSort()->SetIncrease(m_bIncreaseCategory);

	m_pInfoTracks->GetFormPlay()->ShowPlayAll();
	m_pInfoTracks->GetFormPlay()->ShowPlayRandom();
	m_pInfoTracks->GetFormPlay()->ShowFavorite();
	m_pInfoTracks->GetFormPlay()->ShowRating();
	m_pInfoTracks->GetFormPlay()->ShowTopMenu();

	m_pInfoTracks->GetFormSort()->ShowSort();
	m_pInfoTracks->GetFormSort()->ShowIncDec();
	m_pInfoTracks->GetFormSort()->ShowResize();
	m_pInfoTracks->GetFormSort()->SetIncrease(m_bIncreaseTrack);

	m_TopMenu.clear();
	m_SelectItem.clear();

}

void MusicDBWindow::SetSelectOffTopMenu()
{
	m_TopMenu.clear();
	m_TopMenu.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
	m_TopMenu.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
	m_TopMenu.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
	m_TopMenu.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
	m_TopMenu.insert(TOP_MENU_RELOAD, STR_RELOAD);
	m_TopMenu.insert(TOP_MENU_LOAD_COUNT, STR_LOAD_COUNT.arg(m_LimitCount));
	m_TopMenu.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);

	m_pInfoHome->GetFormPlay()->ClearTopMenu();
	m_pInfoHome->GetFormPlay()->SetTopMenu(m_TopMenu);

	m_pInfoTracks->GetFormPlay()->ClearTopMenu();
	m_pInfoTracks->GetFormPlay()->SetTopMenu(m_TopMenu);
}

void MusicDBWindow::SetSelectOnTopMenu()
{
	m_TopMenu.clear();
	m_TopMenu.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
	m_TopMenu.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
	m_TopMenu.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
	m_TopMenu.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
	m_TopMenu.insert(TOP_MENU_UNSELECT, STR_UNSELECT);
//	m_TopMenu.insert(TOP_MENU_ADD_TO_PLAYLIST, STR_ADD_PLAYLIST);	// todo-dylee
	m_TopMenu.insert(TOP_MENU_GAIN_SET, STR_GAIN_SET);
	m_TopMenu.insert(TOP_MENU_GAIN_CLEAR, STR_GAIN_CLEAR);

	m_pInfoHome->GetFormPlay()->ClearTopMenu();
	m_pInfoHome->GetFormPlay()->SetTopMenu(m_TopMenu);

	m_pInfoTracks->GetFormPlay()->ClearTopMenu();
	m_pInfoTracks->GetFormPlay()->SetTopMenu(m_TopMenu);
}

void MusicDBWindow::DoTopMenuPlay(int nWhere)
{
	m_pMgr->RequestManageCategory(VAL_PLAY, m_SelectItem, nWhere, m_nCategory);
}

void MusicDBWindow::DoTopMenuReload()
{
	RequestMusicDBHome();
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
	m_pIconTracks->SetAllSelectMap();
}

void MusicDBWindow::DoTopMenuUnselect()
{
	m_pIconTracks->ClearSelectMap();
}

void MusicDBWindow::DoTopMenuGainSet()
{
	m_pMgr->RequestManageCategory(VAL_GAIN_SET,
								  m_SelectItem,
								  PLAY_NONE,
								  m_nCategory,
								  m_EventID);
}

void MusicDBWindow::DoTopMenuGainClear()
{
	m_pMgr->RequestManageCategory(VAL_GAIN_CLEAR,
								  m_SelectItem,
								  PLAY_NONE,
								  m_nCategory,
								  m_EventID);

}

void MusicDBWindow::DoTopMenuAddPlaylist()
{
	m_pMgr->RequestManageCategory(VAL_ADD,
								  m_SelectItem,
								  PLAY_NONE,
								  m_nCategory);
}

void MusicDBWindow::DoItemTopMenuPlay(int nWhere)
{
	if (m_SelectItem.count() > 0)
	{
		m_pMgr->RequestManageCategory(VAL_PLAY,
									  m_SelectItem,
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

void MusicDBWindow::DoItemTopMenuSelectAll()
{
	m_pListTracks->SetAllSelectMap();

}

void MusicDBWindow::DoItemTopMenuUnselect()
{
	m_pListTracks->ClearSelectMap();

}

void MusicDBWindow::DoItemTopMenuGainSet()
{
	if (m_SelectItem.count() > 0)
	{
		m_pMgr->RequestManageCategory(VAL_GAIN_SET,
									  m_SelectItem,
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

void MusicDBWindow::DoItemTopMenuGainClear()
{
	if (m_SelectItem.count() > 0)
	{
		m_pMgr->RequestManageCategory(VAL_GAIN_CLEAR,
									  m_SelectItem,
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

void MusicDBWindow::DoItemTopMenuAddPlaylist()
{

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

	m_pInfoHome->GetFormSort()->SetSortMenu(list);
	m_pInfoHome->GetFormSort()->SetSortMenuTitle(STR_SORT_NAME);
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

	m_pInfoTracks->GetFormSort()->SetSortMenu(list);
	m_pInfoTracks->GetFormSort()->SetSortMenuTitle(STR_SORT_NAME);

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




