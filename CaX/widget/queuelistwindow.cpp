#include "queuelistwindow.h"
#include "ui_queuelistwindow.h"

#include "dialog/commondialog.h"

#include "manager/queuelistmanager.h"

#include "util/utilnovatron.h"

#include "widget/form/formcoverart.h"
#include "widget/form/formtitle.h"
#include "widget/formQueue/queuetrack.h"
#include "widget/formQueue/queuetrackdelegate.h"
#include "widget/formQueue/queuelyrics.h"
#include "widget/formQueue/queueartist.h"

QueuelistWindow::QueuelistWindow(QWidget *parent, const QString &addr, const int &eventID) :
	QWidget(parent),
	m_pMgr(new QueuelistManager),
	m_Track(new QueueTrack(this)),
	m_Lyrics(new QueueLyrics(this)),
	m_Artist(new QueueArtist(this)),
	m_pFormCoverArt(new FormCoverArt(this)),
	m_pFormTitle(new FormTitle(this)),
	m_EventID(eventID),
	ui(new Ui::QueuelistWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
	Initialize();
}

QueuelistWindow::~QueuelistWindow()
{

	if (m_pMgr)
	{
		delete m_pMgr;
		m_pMgr = nullptr;
	}

	if (m_Track)
	{
		delete m_Track;
		m_Track = nullptr;
	}

	if (m_Lyrics)
	{
		delete m_Lyrics;
		m_Lyrics = nullptr;
	}

	if (m_Artist)
	{
		delete m_Artist;
		m_Artist = nullptr;
	}

	if (m_pFormCoverArt)
	{
		delete m_pFormCoverArt;
		m_pFormCoverArt = nullptr;
	}

	if (m_pFormTitle)
	{
		delete m_pFormTitle;
		m_pFormTitle = nullptr;
	}

	delete ui;
}

void QueuelistWindow::RequestQueuelist(QList<CJsonNode> list, QString src)
{
	LogWarning("====================================");
	LogWarning("RequestQueuelist [%s]", src.toUtf8().data());
	LogWarning("====================================");

	m_Src = src;

	CJsonNode track = list.at(0);
	LogDebug("track [%s]", track.ToTabedByteArray().data());

	if (!m_Src.compare(SRC_MUSIC_DB))
	{
		int id = track.GetString(KEY_SONG).toInt();
		m_pMgr->RequestCategoryInfo(id);
	}
	else if (!m_Src.compare(SRC_BROWSER))
	{
		m_AlbumName = track.GetString(KEY_TOP);
		RequestCoverArtBrowser(track.GetString(KEY_FILE));
	}
	else if (!m_Src.compare(SRC_AUDIO_CD))
	{
		m_AlbumName = track.GetString(KEY_TOP);
		RequestCoverArtAudioCD(track.GetString(KEY_COVER_ART));
	}

	m_Track->ClearNodeList();
	m_Track->SetNodeList(list);
}

void QueuelistWindow::SlotClickBtnTrack()
{
	m_pFormCoverArt->SetCoverArt(m_AlbumCoverArt);
	m_pFormTitle->SetTitle(m_AlbumName);

	ui->stackedQueue->setCurrentWidget(m_Track);
}

void QueuelistWindow::SlotClickBtnLyrics()
{
	m_pFormCoverArt->SetCoverArt(m_AlbumCoverArt);
	m_pFormTitle->SetTitle(m_AlbumName);

	ui->stackedQueue->setCurrentWidget(m_Lyrics);
}

void QueuelistWindow::SlotClickBtnArtist()
{
//	m_pFormCoverArt->SetCoverArt(m_ArtistCoverArt);
//	m_pFormTitle->SetTitle(m_ArtistName);

//	ui->stackedQueue->setCurrentWidget(m_Artist);
}

void QueuelistWindow::SlotClickBtnClose()
{
	emit SigRemoveQueueWidget();
}

void QueuelistWindow::SlotRespError(QString errMsg)
{
	CommonDialog dialog(this, STR_WARNING, errMsg);
	dialog.exec();
}

void QueuelistWindow::SlotRespCategoryInfo(CJsonNode node)
{
	LogDebug("node [%s]", node.ToTabedByteArray().data());

	m_AlbumName = node.GetString(KEY_ALBUM);
//	m_ArtistName = node.GetString(KEY_ARTIST);

	RequestCoverArtMusicDB(node.GetInt(KEY_ALBUM_ID), 0);
//	RequestCoverArtMusicDB(node.GetInt(KEY_ARTIST_ID), 1);
}

void QueuelistWindow::SlotCoverArtUpdate(QString fileName, int nIndex, int mode)
{
	Q_UNUSED(mode)

	LogDebug("filename [%s]", fileName.toUtf8().data());
	if (!m_Src.compare(SRC_MUSIC_DB))
	{
	//	if (nIndex == 0)
		{
			m_AlbumCoverArt = fileName;
		}
	//	else
	//	{
	//		m_ArtistCoverArt = fileName;
	//	}
	}
	else if (!m_Src.compare(SRC_BROWSER))
	{
		m_AlbumCoverArt = fileName;
	}
	else if (!m_Src.compare(SRC_AUDIO_CD))
	{
		m_AlbumCoverArt = fileName;
	}

	m_pFormCoverArt->SetCoverArt(m_AlbumCoverArt);
	m_pFormTitle->SetTitle(m_AlbumName);

}

void QueuelistWindow::SlotSelectPlay(int index, int playType)
{
	Q_UNUSED(playType)

	m_pMgr->RequestTrackPlay(index);
}

void QueuelistWindow::ConnectSigToSlot()
{
	connect(this, SIGNAL(SigRemoveQueueWidget()), parent(), SLOT(SlotRemoveQueueWidget()));

	connect(ui->btnTrack, SIGNAL(clicked()), this, SLOT(SlotClickBtnTrack()));
	connect(ui->btnLyrics, SIGNAL(clicked()), this, SLOT(SlotClickBtnLyrics()));
	connect(ui->btnArtist, SIGNAL(clicked()), this, SLOT(SlotClickBtnArtist()));
	connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(SlotClickBtnClose()));

	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespCategoryInfo(CJsonNode)), this, SLOT(SlotRespCategoryInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));

	connect(m_Track->GetDelegate(), SIGNAL(SigSelectPlay(int, int)), this, SLOT(SlotSelectPlay(int, int)));

}

void QueuelistWindow::Initialize()
{
	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);
	ui->gridLayoutFormTitle->addWidget(m_pFormTitle);

	ui->stackedQueue->addWidget(m_Track);
	ui->stackedQueue->addWidget(m_Lyrics);
	ui->stackedQueue->addWidget(m_Artist);

	// temp_code, dylee
	ui->btnLyrics->hide();
	ui->btnArtist->hide();

	m_pFormTitle->SetTitleFont(FONT_SIZE_QUEUE_TITLE, FONT_COLOR_WHITE, FONT_WEIGHT_NORMAL);

	m_Src = "";

}

void QueuelistWindow::RequestCoverArtMusicDB(int id, int index)
{
	QString strCat;
	if (index == 0)
	{
		strCat = UtilNovatron::GetCategoryName(SQLManager::CATEGORY_ALBUM);
	}
	else
	{
		strCat = UtilNovatron::GetCategoryName(SQLManager::CATEGORY_ARTIST);
	}

	QStringList lsAddr = m_pMgr->GetAddr().split(":");
	QString fullpath = QString("%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(strCat).arg(id);

//	LogDebug("path [%s] index [%d]", fullpath.toUtf8().data(), index);
	m_pMgr->RequestCoverArt(fullpath, index, -1);
}

void QueuelistWindow::RequestCoverArtBrowser(QString path)
{
	QStringList lsAddr = m_pMgr->GetAddr().split(":");
	QString fullpath = QString("%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(SRC_BROWSER).arg(path);

	m_pMgr->RequestCoverArt(fullpath, -1, -1);
}

void QueuelistWindow::RequestCoverArtAudioCD(QString fullpath)
{
	m_pMgr->RequestCoverArt(fullpath, -1, -1);

}

