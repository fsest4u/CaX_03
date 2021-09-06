#include <QStackedWidget>
#include <QMessageBox>

#include "mymusicwindow.h"
#include "ui_mymusicwindow.h"

#include "manager/mymusicmanager.h"
#include "manager/sqlmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"

#include "base/musicinfo.h"
#include "base/albuminfo.h"
#include "base/categoryconts.h"
#include "base/categorycontsdelegate.h"
#include "base/songconts.h"
#include "base/songcontsdelegate.h"


MyMusicWindow::MyMusicWindow(QWidget *parent, const QString &addr)
	: QWidget(parent)
	, m_pMgr(new MyMusicManager)
	, m_pMusicInfo(new MusicInfo(this))
	, m_pAlbumInfo(new AlbumInfo(this))
	, m_pCatConts(new CategoryConts(this))
	, m_pSongConts(new SongConts(this))
	, m_pLoading(new Loading(parent))
//	, m_pCatThread(new QThread)
//	, m_pSongThread(new QThread)
	, ui(new Ui::MyMusicWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();

	m_nCategory = SQLManager::CATEGORY_ALBUM;

}

MyMusicWindow::~MyMusicWindow()
{
	delete ui;

	if (m_pMgr)
	{
		delete m_pMgr;
		m_pMgr = nullptr;
	}

	if (m_pMusicInfo)
	{
		delete m_pMusicInfo;
		m_pMusicInfo = nullptr;
	}

	if (m_pAlbumInfo)
	{
		delete m_pAlbumInfo;
		m_pAlbumInfo = nullptr;
	}

	if (m_pCatConts)
	{
		delete m_pCatConts;
		m_pCatConts = nullptr;
	}

	if (m_pSongConts)
	{
		delete m_pSongConts;
		m_pSongConts = nullptr;
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

void MyMusicWindow::RequestMusicHome()
{
	ui->gridLayoutTop->addWidget(m_pMusicInfo);
	ui->gridLayoutBottom->addWidget(m_pCatConts);

	m_pLoading->Start();
	m_pMgr->RequestMusicInfo();
	m_pMgr->RequestCategoryList(m_nCategory);

}

void MyMusicWindow::RequestCategoryHome(int nID, int nCategory)
{
	m_nCategory = nCategory;
	ui->gridLayoutTop->addWidget(m_pAlbumInfo);
	ui->gridLayoutBottom->addWidget(m_pSongConts);

	m_pLoading->Start();
	m_pMgr->RequestCategoryInfo(nID, m_nCategory);
	m_pMgr->RequestSongsOfCategory(nID, m_nCategory);
}

void MyMusicWindow::SlotRespError(QString errMsg)
{
	m_pLoading->Stop();
	QMessageBox::warning(this, "Warning", errMsg);
}

void MyMusicWindow::SlotRespMusicInfo(CJsonNode node)
{
	m_pMusicInfo->SetAlbumCnt(node.GetString(KEY_ALBUM));
	m_pMusicInfo->SetArtistCnt(node.GetString(KEY_ARTIST));
	m_pMusicInfo->SetComposerCnt(node.GetString(KEY_COMPOSER));
	m_pMusicInfo->SetTrackCnt(node.GetString(KEY_SONG));

}

void MyMusicWindow::SlotRespCategoryList(QList<CJsonNode> nodeList)
{
//	m_pCatConts->SetBackgroundTask(m_pCatThread);
	m_pCatConts->SetContentList(nodeList);
	m_pLoading->Stop();
//	m_pCatThread->start();
}

void MyMusicWindow::SlotRespCategoryInfo(CJsonNode node)
{

	QString count = node.GetString(KEY_COUNT);
	QString total = node.GetString(KEY_TOTAL);
	QString extension = node.GetString(KEY_EXTENSION);
	QString samplerate = node.GetString(KEY_SAMPLERATE);
	QString bps = node.GetString(KEY_BPS);

	QString info = count + "|" + total + "|" + extension + "|" + samplerate + "|" + bps;

	m_pAlbumInfo->SetTitle(node.GetString(KEY_TITLE));
	m_pAlbumInfo->SetSubtitle("Artist : " + node.GetString(KEY_ARTIST));
	m_pAlbumInfo->SetInfo(info);

}

void MyMusicWindow::SlotRespSongsOfCategory(QList<CJsonNode> nodeList)
{
//	m_pSongConts->SetBackgroundTask(m_pSongThread);
	m_pSongConts->SetContentList(nodeList);
	m_pLoading->Stop();
//	m_pSongThread->start();
}

void MyMusicWindow::SlotCoverArtUpdate(QString fileName, int nIndex, int mode)
{
	if (QListView::IconMode == mode)
	{
		QStandardItem *item = m_pCatConts->GetModel()->item(nIndex);
		item->setData(fileName, CategoryContsDelegate::CATEGORY_ROLE_COVER);
		m_pCatConts->GetModel()->setItem(nIndex, item);
	}
	else
	{
		QStandardItem *item = m_pSongConts->GetModel()->item(nIndex);
		item->setData(fileName, SongContsDelegate::SONG_ROLE_COVER);
		m_pSongConts->GetModel()->setItem(nIndex, item);
	}

}

void MyMusicWindow::SlotMusicPlayAll()
{
	LogDebug("music PlayAll");

}

void MyMusicWindow::SlotMusicPlayRandom()
{
	LogDebug("music PlayRandom");

}

void MyMusicWindow::SlotMusicAlbum()
{
	LogDebug("music Album");

}

void MyMusicWindow::SlotMusicArtist()
{
	LogDebug("music Artist");

}

void MyMusicWindow::SlotMusicTrack()
{
	LogDebug("music Track");

}

void MyMusicWindow::SlotMusicComposer()
{
	LogDebug("music Composer");

}

void MyMusicWindow::SlotMusicSubmenu()
{
	LogDebug("music Submenu");

}

void MyMusicWindow::SlotMusicSubmenu2()
{
	LogDebug("music Submenu2");

}

void MyMusicWindow::SlotMusicDisplayMode()
{
	LogDebug("music Display mode");
}

void MyMusicWindow::SlotMusicSort()
{
	LogDebug("frame Sort");

}

void MyMusicWindow::SlotAlbumPlayAll()
{
	LogDebug("album play all");

}

void MyMusicWindow::SlotAlbumPlayRandom()
{
	LogDebug("album play random");

}

void MyMusicWindow::SlotAlbumSubmenu()
{
	LogDebug("album submenu");

}

void MyMusicWindow::SlotAlbumSort()
{
	LogDebug("album sort");

}

void MyMusicWindow::SlotReqCategoryCover(int nID, int nIndex)
{

	QString strCat = m_pMgr->GetCategoryName(m_nCategory);
	QStringList lsAddr = m_pMgr->GetAddr().split(":");
	QString fullpath = QString("%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(strCat).arg(nID);

	m_pMgr->RequestCoverArt(fullpath, nIndex, QListView::IconMode);
}

void MyMusicWindow::SlotSelectCategory(int nID)
{
	MyMusicWindow *widget = new MyMusicWindow(this, m_pMgr->GetAddr());
	widget->RequestCategoryHome(nID, m_nCategory);

	emit SigAddWidget(widget);
}

void MyMusicWindow::SlotSelectCount(int nID)
{
//	m_pMgr->RequestPlaySong();
}

void MyMusicWindow::SlotSelectFavorite(int nID, int nFavorite)
{
	m_pMgr->RequestFavorite(nID, nFavorite, m_nCategory);
}

void MyMusicWindow::SlotSelectRating(int nID, int nRating)
{
	m_pMgr->RequestRating(nID, nRating, m_nCategory);
}

void MyMusicWindow::SlotReqSongCover(int nID, int nIndex)
{
	QString strCat = m_pMgr->GetCategoryName(-1);
	QStringList lsAddr = m_pMgr->GetAddr().split(":");
	QString fullpath = QString("%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(strCat).arg(nID);

	m_pMgr->RequestCoverArt(fullpath, nIndex, QListView::ListMode);
}

void MyMusicWindow::SlotSelectPlay(int nID)
{
	m_pMgr->RequestPlaySong(nID);
}

void MyMusicWindow::SlotSelectMore(int nID)
{
	Q_UNUSED(nID)

	LogDebug("click select more - temp code");
	QListView::ViewMode mode = m_pSongConts->GetViewMode() == QListView::IconMode ? QListView::ListMode : QListView::IconMode;
	m_pSongConts->SetViewMode(mode);
}

void MyMusicWindow::ConnectSigToSlot()
{
	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespMusicInfo(CJsonNode)), this, SLOT(SlotRespMusicInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespCategoryList(QList<CJsonNode>)), this, SLOT(SlotRespCategoryList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespCategoryInfo(CJsonNode)), this, SLOT(SlotRespCategoryInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespSongsOfCategory(QList<CJsonNode>)), this, SLOT(SlotRespSongsOfCategory(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));

	connect(m_pMusicInfo, SIGNAL(SigPlayAll()), this, SLOT(SlotMusicPlayAll()));
	connect(m_pMusicInfo, SIGNAL(SigPlayRandom()), this, SLOT(SlotMusicPlayRandom()));
	connect(m_pMusicInfo, SIGNAL(SigAlbum()), this, SLOT(SlotMusicAlbum()));
	connect(m_pMusicInfo, SIGNAL(SigArtist()), this, SLOT(SlotMusicArtist()));
	connect(m_pMusicInfo, SIGNAL(SigTrack()), this, SLOT(SlotMusicTrack()));
	connect(m_pMusicInfo, SIGNAL(SigComposer()), this, SLOT(SlotMusicComposer()));
//	connect(m_pMusicInfo, SIGNAL(SigSubmenu()), this, SLOT(SlotMusicSubmenu()));
	connect(m_pMusicInfo, SIGNAL(SigSubmenu2()), this, SLOT(SlotMusicSubmenu2()));
	connect(m_pMusicInfo, SIGNAL(SigDisplayMode()), this, SLOT(SlotMusicDisplayMode()));
//	connect(m_pMusicInfo, SIGNAL(SigSort()), this, SLOT(SlotMusicSort()));

	connect(m_pAlbumInfo, SIGNAL(SigPlayAll()), this, SLOT(SlotAlbumPlayAll()));
	connect(m_pAlbumInfo, SIGNAL(SigPlayRandom()), this, SLOT(SlotAlbumPlayRandom()));
	connect(m_pAlbumInfo, SIGNAL(SigSubmenu()), this, SLOT(SlotAlbumSubmenu()));
	connect(m_pAlbumInfo, SIGNAL(SigSort()), this, SLOT(SlotAlbumSort()));

	connect(m_pCatConts, SIGNAL(SigReqCoverArt(int, int)), this, SLOT(SlotReqCategoryCover(int, int)));
	connect(m_pCatConts->GetDelegate(), SIGNAL(SigSelectCoverArt(int)), this, SLOT(SlotSelectCategory(int)));
	connect(m_pCatConts->GetDelegate(), SIGNAL(SigSelectCount(int)), this, SLOT(SlotSelectCount(int)));
	connect(m_pCatConts->GetDelegate(), SIGNAL(SigSelectFavorite(int, int)), this, SLOT(SlotSelectFavorite(int, int)));
	connect(m_pCatConts->GetDelegate(), SIGNAL(SigSelectRating(int, int)), this, SLOT(SlotSelectRating(int, int)));

	connect(m_pSongConts, SIGNAL(SigReqCoverArt(int, int)), this, SLOT(SlotReqSongCover(int, int)));
	connect(m_pSongConts->GetDelegate(), SIGNAL(SigSelectPlay(int)), this, SLOT(SlotSelectPlay(int)));
	connect(m_pSongConts->GetDelegate(), SIGNAL(SigSelectMore(int)), this, SLOT(SlotSelectMore(int)));


}

