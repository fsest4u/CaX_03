#include <QMessageBox>

#include "audiocdwindow.h"
#include "ui_audiocdwindow.h"

#include "manager/audiocdmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/caxtranslate.h"
#include "util/log.h"

#include "widget/form/formplay.h"
#include "widget/form/formsort.h"

#include "widget/formTop/infotracks.h"
#include "widget/formBottom/icontracks.h"
#include "widget/formBottom/icontracksdelegate.h"
#include "widget/formBottom/listtracks.h"
#include "widget/formBottom/listtracksdelegate.h"

#include "dialog/cdripinfodialog.h"
#include "dialog/trackinfodialog.h"

AudioCDWindow::AudioCDWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new AudioCDManager),
	m_pInfoTracks(new InfoTracks(this)),
	m_pIconTracks(new IconTracks(this)),
	m_pListTracks(new ListTracks(this)),
	m_TotalCount(""),
	m_TotalTime(""),
	m_Format(""),
	m_Date(""),
	ui(new Ui::AudioCDWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
	Initialize();
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

}

void AudioCDWindow::AddWidgetAudioCDHome()
{
	ui->gridLayoutTop->addWidget(m_pInfoTracks);
	ui->gridLayoutBottom->addWidget(m_pIconTracks);
//	ui->gridLayoutBottom->addWidget(m_pListTracks);
}


void AudioCDWindow::TrackList()
{
	QList<int> list;
	list.append(0);
	m_pMgr->RequestCDRipInfo(-1, list);

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

void AudioCDWindow::CDRip(CJsonNode node, QList<CJsonNode> list)
{
	// todo-dylee
	m_pMgr->RequestCDRip(node);
}

void AudioCDWindow::SlotRespTrackList(QList<CJsonNode> list)
{	
	m_pIconTracks->ClearNodeList();
	m_pIconTracks->SetNodeList(list, IconTracks::ICON_TRACKS_AUDIO_CD);
//	m_pListTracks->SetNodeList(list, ListTracks::LIST_TRACKS_AUDIO_CD);

	m_TotalCount = QString("%1 songs").arg(list.count());
//	m_pInfoTracks->SetInfo( MakeInfo() );

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
//	m_pInfoTracks->SetInfo(MakeInfo());

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

void AudioCDWindow::SlotRespError(QString errMsg)
{
	QMessageBox::warning(this, "Warning", errMsg);
}

void AudioCDWindow::SlotSelectTitle(int id, QString coverArt)
{
	Q_UNUSED(coverArt)

	TrackPlay(id);
}

void AudioCDWindow::SlotCalcTotalTime(int time)
{
	m_TotalTime = QString("%1").arg(time);
	//	m_pInfoTracks->SetInfo( MakeInfo() );
}

void AudioCDWindow::SlotSelectPlay(int id, int playType)
{
	Q_UNUSED(playType)

	TrackPlay(id);
}

void AudioCDWindow::SlotTopMenu()
{
	m_SelectMap = m_pIconTracks->GetSelectMap();
	if (m_SelectMap.count() > 0)
	{
		SetSelectOnTopMenu();
	}
	else
	{
		SetSelectOffTopMenu();
	}

}

void AudioCDWindow::SlotTopMenuAction(int menuID)
{
	switch (menuID) {
	case TOP_MENU_SELECT_ALL:
		DoTopMenuSelectAll();
		break;
	case TOP_MENU_CLEAR_ALL:
		DoTopMenuClearAll();
		break;
	case TOP_MENU_CD_RIPPING:
		DoTopMenuCDRipping();
		break;
	case TOP_MENU_EJECT_CD:
		DoTopMenuEjectCD();
		break;
	}
}

void AudioCDWindow::SlotResize(int resize)
{
	LogDebug("click resize [%d]", resize);
	m_pIconTracks->SetResize(resize);
}

void AudioCDWindow::ConnectSigToSlot()
{
	connect(this, SIGNAL(SigRemoveWidget(QWidget*)), parent(), SLOT(SlotRemoveWidget(QWidget*)));

	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespTrackList(QList<CJsonNode>)), this, SLOT(SlotRespTrackList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespTrackInfo(CJsonNode)), this, SLOT(SlotRespTrackInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespCDRipInfo(CJsonNode)), this, SLOT(SlotRespCDRipInfo(CJsonNode)));

	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigMenu()), this, SLOT(SlotTopMenu()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigMenuAction(int)), this, SLOT(SlotTopMenuAction(int)));
	connect(m_pInfoTracks->GetFormSort(), SIGNAL(SigResize(int)), this, SLOT(SlotResize(int)));

	connect(m_pIconTracks, SIGNAL(SigCalcTotalTime(int)), this, SLOT(SlotCalcTotalTime(int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectPlay(int, int)), this, SLOT(SlotSelectPlay(int, int)));

//	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectPlay(int, int)), this, SLOT(SlotSelectPlay(int, int)));

}

void AudioCDWindow::Initialize()
{
	m_pInfoTracks->GetFormPlay()->ShowMenu();
	m_pInfoTracks->GetFormSort()->ShowResize();
	m_pInfoTracks->GetFormSort()->SetResize(ICON_HEIGHT_MAX);

	m_TopMenuMap.clear();
	m_SelectMap.clear();
}

void AudioCDWindow::SetSelectOffTopMenu()
{
	m_TopMenuMap.clear();

	m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
//	m_TopMenuMap.insert(TOP_MENU_CD_RIPPING, STR_CD_RIPPING);
	m_TopMenuMap.insert(TOP_MENU_EJECT_CD, STR_EJECT_CD);

	m_pInfoTracks->GetFormPlay()->ClearMenu();
	m_pInfoTracks->GetFormPlay()->SetMenu(m_TopMenuMap);
}

void AudioCDWindow::SetSelectOnTopMenu()
{
	m_TopMenuMap.clear();

	m_TopMenuMap.insert(TOP_MENU_CLEAR_ALL, STR_CLEAR_ALL);
//	m_TopMenuMap.insert(TOP_MENU_CD_RIPPING, STR_CD_RIPPING);

	m_pInfoTracks->GetFormPlay()->ClearMenu();
	m_pInfoTracks->GetFormPlay()->SetMenu(m_TopMenuMap);
}

void AudioCDWindow::DoTopMenuSelectAll()
{
	m_pIconTracks->SetAllSelectMap();
}

void AudioCDWindow::DoTopMenuClearAll()
{
	m_pIconTracks->ClearSelectMap();
}

void AudioCDWindow::DoTopMenuCDRipping()
{
	// list does not contain track id. list contains index information
	//	// temp_code, dylee

	if (m_SelectMap.count() > 0)
	{

	}
	else
	{

	}
//	QList<int> list;
//	list.append(id - 1);
//	m_pMgr->RequestCDRipInfo(index, list);
}

void AudioCDWindow::DoTopMenuEjectCD()
{
	m_pMgr->RequestEject();

	emit SigRemoveWidget(this);
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


