#include "audiocdwindow.h"
#include "ui_audiocdwindow.h"

#include "manager/audiocdmanager.h"

#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"

#include "base/infotracks.h"
#include "base/icontracks.h"
#include "base/icontracksdelegate.h"
#include "base/listtracks.h"
#include "base/listtracksdelegate.h"

#include "dialog/cdripinfodialog.h"
#include "dialog/trackinfodialog.h"

AudioCDWindow::AudioCDWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new AudioCDManager),
	m_pInfoTracks(new InfoTracks(this)),
	m_pIconTracks(new IconTracks(this)),
	m_pListTracks(new ListTracks(this)),
	m_pLoading(new Loading(this)),
	m_TotalCount(""),
	m_TotalTime(""),
	m_Format(""),
	m_Date(""),
	ui(new Ui::AudioCDWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
}

AudioCDWindow::~AudioCDWindow()
{
	delete ui;

	if (m_pMgr)
	{
		delete m_pMgr;
		m_pMgr = nullptr;
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

void AudioCDWindow::TrackList()
{
	ui->gridLayoutTop->addWidget(m_pInfoTracks);
	ui->gridLayoutBottom->addWidget(m_pIconTracks);

	QList<int> list;
	list.append(0);
	CDRipInfo(-1, list);

	m_pMgr->RequestTrackList();
}

void AudioCDWindow::TrackInfo(int index)
{
	m_pMgr->RequestTrackInfo(index);
}

void AudioCDWindow::TrackPlay(int index)
{
	m_pMgr->RequestTrackPlay(index);
}

void AudioCDWindow::Eject()
{
	m_pMgr->RequestEject();

	emit SigRemoveWidget(this);
}

void AudioCDWindow::CDRipInfo(int index, QList<int> list)
{
	// list does not contain track id. list contains index information
	m_pMgr->RequestCDRipInfo(index, list);
}

void AudioCDWindow::CDRip(CJsonNode node, QList<CJsonNode> list)
{
	// todo-dylee ui 미완성으로 아직 미구현
	m_pMgr->RequestCDRip(node);
}

void AudioCDWindow::SlotRespTrackList(QList<CJsonNode> list)
{	
	m_pIconTracks->SetNodeList(list, IconTracks::ICON_TRACKS_AUDIO_CD);

	m_TotalCount = QString("%1 songs").arg(list.count());
	m_pInfoTracks->SetInfo( MakeInfo() );

}

void AudioCDWindow::SlotRespTrackInfo(CJsonNode node)
{
	TrackInfoDialog dialog;
	dialog.SetTitle(node.GetString(KEY_TITLE));
	dialog.SetAlbum(node.GetString(KEY_ALBUM));
	dialog.SetArtist(node.GetString(KEY_ARTIST));
	dialog.SetGenre(node.GetString(KEY_GENRE));
	dialog.SetAlbumArtist(node.GetString(KEY_ALBUM_ARTIST));
	dialog.SetComposer(node.GetString(KEY_COMPOSER));
	dialog.SetMood(node.GetString(KEY_MOOD));
	dialog.SetTempo(node.GetString(KEY_TEMPO));
	dialog.SetTrack(node.GetString(KEY_TRACK));
	dialog.SetYear(node.GetString(KEY_YEAR));
	dialog.SetCDYear(node.GetString(KEY_CDYEAR));
	dialog.SetCDNumber(QString("%1").arg(node.GetInt(KEY_CDNUMBER)));
	dialog.exec();
}

void AudioCDWindow::SlotRespCDRipInfo(CJsonNode node)
{
	LogDebug("node [%s]", node.ToCompactByteArray().data());
	CJsonNode tracks = node.GetArray(KEY_TRACKS);
	CJsonNode track = tracks.GetArrayAt(0);

	QStringList formats = node.GetStringList(KEY_FORMATS);
	int index = node.GetInt(KEY_FORMAT);
	if (formats.count() > index)
	{
		m_Format = formats.at(index);
	}
	m_pInfoTracks->SetCoverArt(node.GetString(KEY_COVER_ART));
	m_pInfoTracks->SetTitle(node.GetString(KEY_ALBUM));
	m_pInfoTracks->SetSubtitle(node.GetString(KEY_ALBUM_ARTIST));
	m_pInfoTracks->SetInfo(MakeInfo());

//	CDRipInfoDialog dialog;
//	dialog.SetTitle(track.GetString(KEY_TITLE_CAP));
//	dialog.SetArtist(track.GetString(KEY_ARTIST));
//	dialog.SetGenre(track.GetString(KEY_GENRE));
//	dialog.SetComposer(track.GetString(KEY_COMPOSER));
//	dialog.SetMood(track.GetString(KEY_MOOD));
//	dialog.SetTempo(track.GetString(KEY_TEMPO));
//	dialog.SetYear(track.GetString(KEY_YEAR));
//	if (dialog.exec() == QDialog::Accepted)
//	{
////		CDRip(node, list);
//	}
}

void AudioCDWindow::SlotSelectCoverArt(int id, QString coverArt)
{
	Q_UNUSED(coverArt)

	TrackPlay(id);

//	// temp_code, dylee
//	QList<int> list;
//	list.append(id - 1);
//	CDRipInfo(-1, list);
}

void AudioCDWindow::SlotCalcTotalTime(int time)
{
	m_TotalTime = QString("%1").arg(time);
	m_pInfoTracks->SetInfo( MakeInfo() );
}

void AudioCDWindow::ConnectSigToSlot()
{
	connect(this, SIGNAL(SigRemoveWidget(QWidget*)), parent(), SLOT(SlotRemoveWidget(QWidget*)));

	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespTrackList(QList<CJsonNode>)), this, SLOT(SlotRespTrackList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespTrackInfo(CJsonNode)), this, SLOT(SlotRespTrackInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespCDRipInfo(CJsonNode)), this, SLOT(SlotRespCDRipInfo(CJsonNode)));

	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectCoverArt(int, QString)), this, SLOT(SlotSelectCoverArt(int, QString)));
	connect(m_pIconTracks, SIGNAL(SigCalcTotalTime(int)), this, SLOT(SlotCalcTotalTime(int)));

}

QString AudioCDWindow::MakeInfo()
{
	QString info = m_TotalCount;

	if (!m_TotalTime.isEmpty())
	{
		info += " | " + m_TotalTime;
	}
	if (!m_Format.isEmpty())
	{
		info += " | " + m_Format;
	}
	if (!m_Date.isEmpty())
	{
		info += " | " + m_Date;
	}

	return info;
}


