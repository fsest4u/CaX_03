#include <QStackedWidget>
#include <QMessageBox>

#include "musicdbwindow.h"
#include "ui_musicdbwindow.h"

#include "manager/musicdbmanager.h"
#include "manager/sqlmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"

#include "base/infohome.h"
#include "base/infotracks.h"
#include "base/icontracks.h"
#include "base/icontracksdelegate.h"
#include "base/listtracks.h"
#include "base/listtracksdelegate.h"


MusicDBWindow::MusicDBWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new MusicDBManager),
	m_pInfoHome(new InfoHome(this)),
	m_pInfoTracks(new InfoTracks(this)),
	m_pIconTracks(new IconTracks(this)),
	m_pListTracks(new ListTracks(this)),
	m_pLoading(new Loading(this)),
//	m_pCatThread(new QThread),
//	m_pSongThread(new QThread),
	ui(new Ui::MusicDBWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();

	m_nCategory = SQLManager::CATEGORY_ALBUM;

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

	if (m_pLoading)
	{
		delete m_pLoading;
		m_pLoading = nullptr;
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

void MusicDBWindow::RequestMusicHome()
{
	ui->gridLayoutTop->addWidget(m_pInfoHome);
	ui->gridLayoutBottom->addWidget(m_pIconTracks);

	m_pLoading->Start();
	m_pMgr->RequestMusicInfo();
	m_pMgr->RequestCategoryList(m_nCategory);

}

void MusicDBWindow::RequestCategoryHome(int nID, int nCategory)
{
	m_nCategory = nCategory;
	ui->gridLayoutTop->addWidget(m_pInfoTracks);
	ui->gridLayoutBottom->addWidget(m_pListTracks);

	m_pLoading->Start();
	m_pMgr->RequestCategoryInfo(nID, m_nCategory);
	m_pMgr->RequestSongsOfCategory(nID, m_nCategory);
}

void MusicDBWindow::SlotAddWidget(QWidget *widget)
{
	emit SigAddWidget(widget);	// recursive
}

void MusicDBWindow::SlotRespError(QString errMsg)
{
	m_pLoading->Stop();
	QMessageBox::warning(this, "Warning", errMsg);
}

void MusicDBWindow::SlotRespMusicInfo(CJsonNode node)
{
	m_pInfoHome->SetAlbumCnt(node.GetString(KEY_ALBUM));
	m_pInfoHome->SetArtistCnt(node.GetString(KEY_ARTIST));
	m_pInfoHome->SetComposerCnt(node.GetString(KEY_COMPOSER));
	m_pInfoHome->SetTrackCnt(node.GetString(KEY_SONG));

}

void MusicDBWindow::SlotRespCategoryList(QList<CJsonNode> list)
{
//	m_pIconTracks->SetBackgroundTask(m_pCatThread);
	m_pIconTracks->SetNodeList(list, IconTracks::ICON_TRACKS_MUSIC_DB);
	m_pLoading->Stop();
//	m_pCatThread->start();
}

void MusicDBWindow::SlotRespCategoryInfo(CJsonNode node)
{

	QString count = node.GetString(KEY_COUNT);
	QString total = node.GetString(KEY_TOTAL);
	QString extension = node.GetString(KEY_EXTENSION);
	QString samplerate = node.GetString(KEY_SAMPLERATE);
	QString bps = node.GetString(KEY_BPS);

	QString info = count + "|" + total + "|" + extension + "|" + samplerate + "|" + bps;

	m_pInfoTracks->SetTitle(node.GetString(KEY_TITLE));
	m_pInfoTracks->SetSubtitle("Artist : " + node.GetString(KEY_ARTIST));
	m_pInfoTracks->SetInfo(info);

}

void MusicDBWindow::SlotRespSongsOfCategory(QList<CJsonNode> list)
{
//	m_pListTracks->SetBackgroundTask(m_pSongThread);
	m_pListTracks->SetContentList(list);
	m_pLoading->Stop();
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

void MusicDBWindow::SlotMusicPlayAll()
{
	LogDebug("music PlayAll");

}

void MusicDBWindow::SlotMusicPlayRandom()
{
	LogDebug("music PlayRandom");

}

void MusicDBWindow::SlotMusicAlbum()
{
	LogDebug("music Album");

}

void MusicDBWindow::SlotMusicArtist()
{
	LogDebug("music Artist");

}

void MusicDBWindow::SlotMusicTrack()
{
	LogDebug("music Track");

}

void MusicDBWindow::SlotMusicComposer()
{
	LogDebug("music Composer");

}

void MusicDBWindow::SlotMusicSubmenu()
{
	LogDebug("music Submenu");

}

void MusicDBWindow::SlotMusicSubmenu2()
{
	LogDebug("music Submenu2");

}

void MusicDBWindow::SlotMusicDisplayMode()
{
	LogDebug("music Display mode");
}

void MusicDBWindow::SlotMusicSort()
{
	LogDebug("frame Sort");

}

void MusicDBWindow::SlotAlbumPlayAll()
{
	LogDebug("album play all");

}

void MusicDBWindow::SlotAlbumPlayRandom()
{
	LogDebug("album play random");

}

void MusicDBWindow::SlotAlbumSubmenu()
{
	LogDebug("album submenu");

}

void MusicDBWindow::SlotAlbumSort()
{
	LogDebug("album sort");

}

void MusicDBWindow::SlotReqCategoryCover(int nID, int nIndex)
{
	QString strCat = m_pMgr->GetCategoryName(m_nCategory);
	QStringList lsAddr = m_pMgr->GetAddr().split(":");
	QString fullpath = QString("%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(strCat).arg(nID);

	m_pMgr->RequestCoverArt(fullpath, nIndex, QListView::IconMode);
}

void MusicDBWindow::SlotSelectCategory(int nID)
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr());
	widget->RequestCategoryHome(nID, m_nCategory);

	emit SigAddWidget(widget);
}

void MusicDBWindow::SlotSelectCount(int nID)
{
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

void MusicDBWindow::SlotReqSongCover(int nID, int nIndex)
{
	QString strCat = m_pMgr->GetCategoryName(-1);
	QStringList lsAddr = m_pMgr->GetAddr().split(":");
	QString fullpath = QString("%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(strCat).arg(nID);

	m_pMgr->RequestCoverArt(fullpath, nIndex, QListView::ListMode);
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

void MusicDBWindow::ConnectSigToSlot()
{
	// recursive
	connect(this, SIGNAL(SigAddWidget(QWidget*)), parent(), SLOT(SlotAddWidget(QWidget*)));

	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespMusicInfo(CJsonNode)), this, SLOT(SlotRespMusicInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespCategoryList(QList<CJsonNode>)), this, SLOT(SlotRespCategoryList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespCategoryInfo(CJsonNode)), this, SLOT(SlotRespCategoryInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespSongsOfCategory(QList<CJsonNode>)), this, SLOT(SlotRespSongsOfCategory(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));

	connect(m_pInfoHome, SIGNAL(SigPlayAll()), this, SLOT(SlotMusicPlayAll()));
	connect(m_pInfoHome, SIGNAL(SigPlayRandom()), this, SLOT(SlotMusicPlayRandom()));
	connect(m_pInfoHome, SIGNAL(SigAlbum()), this, SLOT(SlotMusicAlbum()));
	connect(m_pInfoHome, SIGNAL(SigArtist()), this, SLOT(SlotMusicArtist()));
	connect(m_pInfoHome, SIGNAL(SigTrack()), this, SLOT(SlotMusicTrack()));
	connect(m_pInfoHome, SIGNAL(SigComposer()), this, SLOT(SlotMusicComposer()));
//	connect(m_pInfoHome, SIGNAL(SigSubmenu()), this, SLOT(SlotMusicSubmenu()));
	connect(m_pInfoHome, SIGNAL(SigSubmenu2()), this, SLOT(SlotMusicSubmenu2()));
	connect(m_pInfoHome, SIGNAL(SigDisplayMode()), this, SLOT(SlotMusicDisplayMode()));
//	connect(m_pInfoHome, SIGNAL(SigSort()), this, SLOT(SlotMusicSort()));

	connect(m_pInfoTracks, SIGNAL(SigPlayAll()), this, SLOT(SlotAlbumPlayAll()));
	connect(m_pInfoTracks, SIGNAL(SigPlayRandom()), this, SLOT(SlotAlbumPlayRandom()));
	connect(m_pInfoTracks, SIGNAL(SigSubmenu()), this, SLOT(SlotAlbumSubmenu()));
	connect(m_pInfoTracks, SIGNAL(SigSort()), this, SLOT(SlotAlbumSort()));

	connect(m_pIconTracks, SIGNAL(SigReqCoverArt(int, int)), this, SLOT(SlotReqCategoryCover(int, int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectCoverArt(int)), this, SLOT(SlotSelectCategory(int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectCount(int)), this, SLOT(SlotSelectCount(int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectFavorite(int, int)), this, SLOT(SlotSelectFavorite(int, int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectRating(int, int)), this, SLOT(SlotSelectRating(int, int)));

	connect(m_pListTracks, SIGNAL(SigReqCoverArt(int, int)), this, SLOT(SlotReqSongCover(int, int)));
	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectPlay(int)), this, SLOT(SlotSelectPlay(int)));
	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectMore(int)), this, SLOT(SlotSelectMore(int)));


}

