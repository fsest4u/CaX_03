#include <QMessageBox>

#include "playlistwindow.h"
#include "ui_playlistwindow.h"

#include "base/infoservice.h"
#include "base/infotracks.h"
#include "base/icontracks.h"
#include "base/icontracksdelegate.h"
#include "base/listtracks.h"
#include "base/listtracksdelegate.h"

#include "manager/playlistmanager.h"
#include "manager/sqlmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"

PlaylistWindow::PlaylistWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new PlaylistManager),
	m_pInfoService(new InfoService(this)),
	m_pInfoTracks(new InfoTracks(this)),
	m_pIconTracks(new IconTracks(this)),
	m_pListTracks(new ListTracks(this)),
	m_pLoading(new Loading(this)),
	ui(new Ui::PlaylistWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
}

PlaylistWindow::~PlaylistWindow()
{
	delete ui;

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

	if (m_pLoading)
	{
		delete m_pLoading;
		m_pLoading = nullptr;
	}
}

void PlaylistWindow::Playlist()
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pIconTracks);

	m_pInfoService->SetTitle(tr("Playlist"));

	m_pMgr->RequestPlaylist();
}

void PlaylistWindow::PlaylistInfo(int id, QString coverArt)
{
	ui->gridLayoutTop->addWidget(m_pInfoTracks);
	m_pInfoTracks->SetCoverArt(coverArt);
	m_pMgr->RequestPlaylistInfo(id);
}

void PlaylistWindow::TrackList(int id)
{
	ui->gridLayoutBottom->addWidget(m_pListTracks);
	m_pMgr->RequestTrackList(id);
}

void PlaylistWindow::PlayTrack(int id)
{
	m_pMgr->RequestPlayTrack(id);
	// temp_code, dylee
//	QList<int> ids;
//	ids.append(id);
//	m_pMgr->RequestPlayTracks(ids);
}

void PlaylistWindow::SlotAddWidget(QWidget *widget)
{
	emit SigAddWidget(widget);		// recursive
}

void PlaylistWindow::SlotRespError(QString message)
{
	QMessageBox::warning(this, "Warning", message);
}

void PlaylistWindow::SlotRespPlaylist(QList<CJsonNode> list)
{
	m_pIconTracks->SetNodeList(list, IconTracks::ICON_TRACKS_PLAYLIST);
}

void PlaylistWindow::SlotRespPlaylistInfo(CJsonNode node)
{
	LogDebug("playlist info [%s]", node.ToCompactByteArray().data());

	QString count = node.GetString(KEY_COUNT);
	QString total = node.GetString(KEY_TOTAL);
	QString extension = node.GetString(KEY_EXTENSION);
	QString samplerate = node.GetString(KEY_SAMPLERATE);
	QString bps = node.GetString(KEY_BPS);

	QString info = count + " | " + total + " | " + extension + " | " + samplerate + " | " + bps;

	m_pInfoTracks->SetTitle(node.GetString(KEY_TITLE));
	m_pInfoTracks->SetSubtitle("Artist : " + node.GetString(KEY_ARTIST));
	m_pInfoTracks->SetInfo(info);
}

void PlaylistWindow::SlotRespTrackList(QList<CJsonNode> list)
{
	m_pListTracks->SetNodeList(list);
}

void PlaylistWindow::SlotReqCoverArt(int id, int index, int mode)
{
	QString strCat;
	if (QListView::IconMode == mode)
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
		QStandardItem *item = m_pIconTracks->GetModel()->item(index);
		item->setData(coverArt, IconTracksDelegate::ICON_TRACKS_COVER);
		m_pIconTracks->GetModel()->setItem(index, item);
	}
	else
	{
		QStandardItem *item = m_pListTracks->GetModel()->item(index);
		item->setData(coverArt, ListTracksDelegate::LIST_TRACKS_COVER);
		m_pListTracks->GetModel()->setItem(index, item);
	}
}

void PlaylistWindow::SlotSelectCoverArt(int id, QString coverArt)
{
	LogDebug("click id [%d]", id);
	PlaylistWindow *widget = new PlaylistWindow(this, m_pMgr->GetAddr());
	emit SigAddWidget(widget);
	widget->PlaylistInfo(id, coverArt);
	widget->TrackList(id);
}

void PlaylistWindow::SlotSelectPlay(int id)
{
	PlayTrack(id);
}

void PlaylistWindow::ConnectSigToSlot()
{
	connect(this, SIGNAL(SigAddWidget(QWidget*)), parent(), SLOT(SlotAddWidget(QWidget*)));		// recursive

	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));

	connect(m_pMgr, SIGNAL(SigRespPlaylist(QList<CJsonNode>)), this, SLOT(SlotRespPlaylist(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespPlaylistInfo(CJsonNode)), this, SLOT(SlotRespPlaylistInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespTrackList(QList<CJsonNode>)), this, SLOT(SlotRespTrackList(QList<CJsonNode>)));
	connect(m_pIconTracks, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));
	connect(m_pListTracks, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));

	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectCoverArt(int, QString)), this, SLOT(SlotSelectCoverArt(int, QString)));
	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectPlay(int)), this, SLOT(SlotSelectPlay(int)));

}
