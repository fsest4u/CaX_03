#include <QStackedWidget>
#include <QMessageBox>

#include "musicdbwindow.h"
#include "ui_musicdbwindow.h"

#include "manager/musicdbmanager.h"
#include "manager/sqlmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
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

MusicDBWindow::MusicDBWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new MusicDBManager),
	m_pInfoHome(new InfoHome(this)),
	m_pInfoTracks(new InfoTracks(this)),
	m_pIconTracks(new IconTracks(this)),
	m_pListTracks(new ListTracks(this)),
	m_nCategory(SQLManager::CATEGORY_ALBUM),
	m_nID(-1),
	m_nSortCategory(SQLManager::SORT_NAME),
	m_nSortTrack(SQLManager::SORT_NAME),
	m_bIncreaseCategory(true),
	m_bIncreaseTrack(true),
	m_ArtistID(""),
	m_GenreID(""),
	m_ComposerID(""),
//	m_pCatThread(new QThread),
//	m_pSongThread(new QThread),
	ui(new Ui::MusicDBWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();

	m_pInfoHome->GetFormPlay()->ShowPlayAll();
	m_pInfoHome->GetFormPlay()->ShowPlayRandom();
	m_pInfoHome->GetFormPlay()->ShowSubmenu();

	m_pInfoHome->GetFormSort()->ShowSort();
	m_pInfoHome->GetFormSort()->ShowIncDec();
	m_pInfoHome->GetFormSort()->ShowResize();
	m_pInfoHome->GetFormSort()->SetIncrease(m_bIncreaseCategory);

	m_pInfoTracks->GetFormPlay()->ShowPlayAll();
	m_pInfoTracks->GetFormPlay()->ShowPlayRandom();
	m_pInfoTracks->GetFormPlay()->ShowSubmenu();
	m_pInfoTracks->GetFormPlay()->ShowFavorite();
	m_pInfoTracks->GetFormPlay()->ShowRating();

	m_pInfoTracks->GetFormSort()->ShowSort();
	m_pInfoTracks->GetFormSort()->ShowIncDec();
	m_pInfoTracks->GetFormSort()->ShowResize();
	m_pInfoTracks->GetFormSort()->SetIncrease(m_bIncreaseTrack);

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

void MusicDBWindow::RequestMusicDBHome(int nCategory, int nSort, bool bIncrease)
{
	m_nCategory = nCategory;
	m_nSortCategory = nSort;
	m_bIncreaseCategory = bIncrease;

	m_pInfoHome->SetTitle(m_nCategory);
	m_pMgr->RequestMusicDBInfo();
	m_pMgr->RequestCategoryList(m_nCategory, m_nSortCategory, m_bIncreaseCategory);

}

void MusicDBWindow::RequestCategoryHome(int nID, int nCategory, int nSort, bool bIncrease)
{
	m_nID = nID;
	m_nCategory = nCategory;
	m_nSortTrack = nSort;
	m_bIncreaseTrack = bIncrease;

	m_pMgr->RequestCategoryInfo(nID, m_nCategory);
	m_pMgr->RequestSongsOfCategory(nID, m_nCategory, m_nSortTrack, m_bIncreaseTrack);
}

void MusicDBWindow::AddWidgetMusicDBHome()
{
	ui->gridLayoutTop->addWidget(m_pInfoHome);
	ui->gridLayoutBottom->addWidget(m_pIconTracks);
}

void MusicDBWindow::AddWidgetCategoryHome()
{
	ui->gridLayoutTop->addWidget(m_pInfoTracks);
	ui->gridLayoutBottom->addWidget(m_pListTracks);
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
	m_pIconTracks->ClearNodeList();
	m_pIconTracks->SetNodeList(list, IconTracks::ICON_TRACKS_MUSIC_DB);
//	m_pCatThread->start();
}

void MusicDBWindow::SlotRespCategoryInfo(CJsonNode node)
{
	QString count = node.GetString(KEY_COUNT);
	QString total = node.GetString(KEY_TOTAL);
	QString extension = node.GetString(KEY_EXTENSION);
	QString samplerate = node.GetString(KEY_SAMPLERATE);
	QString bps = node.GetString(KEY_BPS);

	QString info = count + " | " + total + " | " + extension + " | " + samplerate + " | " + bps;

	QString title = node.GetString(KEY_TITLE);
	if (title.isEmpty())
	{
		title = "Track";
	}
	m_pInfoTracks->SetTitle(title);
	m_pInfoTracks->SetSubtitle(node.GetString(KEY_ARTIST));
//	m_pInfoTracks->SetInfo(info);
}

void MusicDBWindow::SlotRespSongsOfCategory(QList<CJsonNode> list)
{
//	m_pListTracks->SetBackgroundTask(m_pSongThread);
	m_pListTracks->ClearNodeList();
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
	m_pMgr->RequestPlayCategoryItems(PLAY_CLEAR, m_nCategory);
}

void MusicDBWindow::SlotPlayRandom()
{
	m_pMgr->RequestRandom();
}

void MusicDBWindow::SlotFavorite()
{
	LogDebug("music favorite");

}

void MusicDBWindow::SlotRating()
{
	LogDebug("music rating");

}

void MusicDBWindow::SlotSubmenu()
{
	LogDebug("music Submenu");

}

void MusicDBWindow::SlotSort()
{
	LogDebug("good choice sort");

}

void MusicDBWindow::SlotIncDec(bool bIncrease)
{
	RequestMusicDBHome(m_nCategory, m_nSortCategory, bIncrease);
}

void MusicDBWindow::SlotResize()
{
	LogDebug("good choice resize");

}

void MusicDBWindow::SlotGenreList()
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr());
	widget->AddWidgetMusicDBHome();
	emit SigAddWidget(widget, tr("Genre"));

	widget->RequestMusicDBHome(SQLManager::CATEGORY_GENRE);
}

void MusicDBWindow::SlotAlbumList()
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr());
	widget->AddWidgetMusicDBHome();
	emit SigAddWidget(widget, tr("Album"));

	widget->RequestMusicDBHome(SQLManager::CATEGORY_ALBUM);
}

void MusicDBWindow::SlotArtistList()
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr());
	widget->AddWidgetMusicDBHome();
	emit SigAddWidget(widget, tr("Artist"));

	widget->RequestMusicDBHome(SQLManager::CATEGORY_ARTIST);
}

void MusicDBWindow::SlotTrackList()
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr());
	widget->AddWidgetCategoryHome();
	emit SigAddWidget(widget, tr("Track"));

	widget->RequestCategoryHome(-1, SQLManager::CATEGORY_TRACK);
	widget->SetCoverArt("");
}

void MusicDBWindow::SlotMusicSubmenu2()
{
	LogDebug("music Submenu2");

}

void MusicDBWindow::SlotAlbumPlayAll()
{
	m_pMgr->RequestPlayCategoryItem(m_nID, PLAY_CLEAR, m_nCategory);
}

void MusicDBWindow::SlotAlbumPlayRandom()
{
	m_pMgr->RequestRandom();
}

void MusicDBWindow::SlotAlbumSubmenu()
{
	LogDebug("album submenu");

}

void MusicDBWindow::SlotAlbumFavorite()
{
	LogDebug("album favorite");

}

void MusicDBWindow::SlotAlbumRating()
{
	LogDebug("album rating");

}

void MusicDBWindow::SlotAlbumSort()
{
	LogDebug("album sort");

}

void MusicDBWindow::SlotAlbumIncDec(bool bIncrease)
{
	RequestCategoryHome(m_nID, m_nCategory, m_nSortCategory, bIncrease);
}

void MusicDBWindow::SlotAlbumResize()
{
	LogDebug("album resize");

}

void MusicDBWindow::SlotSelectTitle(int nID, QString coverArt)
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr());
	widget->AddWidgetCategoryHome();
	emit SigAddWidget(widget, tr("Music DB"));

	widget->RequestCategoryHome(nID, m_nCategory);
	widget->SetCoverArt(coverArt);
}

void MusicDBWindow::SlotSelectCount(int nID)
{
	LogDebug("click count");
//	m_pMgr->RequestPlaySong();
}

void MusicDBWindow::SlotSelectFavorite(int nID, int nFavorite)
{
	m_pMgr->RequestFavorite(nID, nFavorite, m_nCategory);
}

void MusicDBWindow::SlotSelectRating(int nID, int nRating)
{
	m_pMgr->RequestRating(nID, nRating, m_nCategory);
}

void MusicDBWindow::SlotReqCoverArt(int id, int index, int mode)
{
	QString strCat;
	if (QListView::IconMode == mode)
	{
		strCat = m_pMgr->GetCategoryName(m_nCategory);
	}
	else
	{
		strCat = m_pMgr->GetCategoryName(-1);
	}

	QStringList lsAddr = m_pMgr->GetAddr().split(":");
	QString fullpath = QString("%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(strCat).arg(id);

	m_pMgr->RequestCoverArt(fullpath, index, mode);
}

void MusicDBWindow::SlotSelectPlay(int nID)
{
	m_pMgr->RequestPlaySong(nID);
}

void MusicDBWindow::SlotSelectMore(int nID)
{
	Q_UNUSED(nID)

	LogDebug("click select more - temp code");
	QListView::ViewMode mode = m_pListTracks->GetViewMode() == QListView::IconMode ? QListView::ListMode : QListView::IconMode;
	m_pListTracks->SetViewMode(mode);
}

void MusicDBWindow::SlotRespClassifyArtist(QList<CJsonNode> list)
{
	m_pInfoHome->GetFormClassify()->SetClassifyArtistMenu(list);
}

void MusicDBWindow::SlotRespClassifyGenre(QList<CJsonNode> list)
{
	m_pInfoHome->GetFormClassify()->SetClassifyGenreMenu(list);
}

void MusicDBWindow::SlotRespClassifyComposer(QList<CJsonNode> list)
{
	m_pInfoHome->GetFormClassify()->SetClassifyComposerMenu(list);
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
	connect(m_pInfoHome->GetFormPlay(), SIGNAL(SigSubmenu()), this, SLOT(SlotSubmenu()));

	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigClassify()), this, SLOT(SlotFilterClassify()));
	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigFavorite()), this, SLOT(SlotFilterFavorite()));
	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigRating()), this, SLOT(SlotFilterRating()));
	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigClassifyArtist(bool, QString)), this, SLOT(SlotClassifyArtist(bool, QString)));
	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigClassifyGenre(bool, QString)), this, SLOT(SlotClassifyGenre(bool, QString)));
	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigClassifyComposer(bool, QString)), this, SLOT(SlotClassifyComposer(bool, QString)));

	connect(m_pInfoHome->GetFormSort(), SIGNAL(SigSort()), this, SLOT(SlotSort()));
	connect(m_pInfoHome->GetFormSort(), SIGNAL(SigIncDec(bool)), this, SLOT(SlotIncDec(bool)));
	connect(m_pInfoHome->GetFormSort(), SIGNAL(SigResize()), this, SLOT(SlotResize()));

	connect(m_pInfoHome, SIGNAL(SigGenreList()), this, SLOT(SlotGenreList()));
	connect(m_pInfoHome, SIGNAL(SigAlbumList()), this, SLOT(SlotAlbumList()));
	connect(m_pInfoHome, SIGNAL(SigArtistList()), this, SLOT(SlotArtistList()));
	connect(m_pInfoHome, SIGNAL(SigTrackList()), this, SLOT(SlotTrackList()));
	connect(m_pInfoHome, SIGNAL(SigSubmenu2()), this, SLOT(SlotMusicSubmenu2()));

	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigPlayAll()), this, SLOT(SlotAlbumPlayAll()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigPlayRandom()), this, SLOT(SlotAlbumPlayRandom()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigSubmenu()), this, SLOT(SlotAlbumSubmenu()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigFavorite()), this, SLOT(SlotAlbumFavorite()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigRating()), this, SLOT(SlotAlbumRating()));

	connect(m_pInfoTracks, SIGNAL(SigSort()), this, SLOT(SlotAlbumSort()));
	connect(m_pInfoTracks->GetFormSort(), SIGNAL(SigSort()), this, SLOT(SlotAlbumSort()));
	connect(m_pInfoTracks->GetFormSort(), SIGNAL(SigIncDec(bool)), this, SLOT(SlotAlbumIncDec(bool)));
	connect(m_pInfoTracks->GetFormSort(), SIGNAL(SigResize()), this, SLOT(SlotAlbumResize()));

	connect(m_pIconTracks, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectCount(int)), this, SLOT(SlotSelectCount(int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectFavorite(int, int)), this, SLOT(SlotSelectFavorite(int, int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectRating(int, int)), this, SLOT(SlotSelectRating(int, int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectSubtitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));

	connect(m_pListTracks, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));
	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectPlay(int)), this, SLOT(SlotSelectPlay(int)));
	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectMore(int)), this, SLOT(SlotSelectMore(int)));


}

void MusicDBWindow::SetCoverArt(QString coverArt)
{
	m_pInfoTracks->SetCoverArt(coverArt);
}

