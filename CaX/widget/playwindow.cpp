#include <QMouseEvent>
#include <QTimer>
#include <QWidgetAction>

#include "playwindow.h"
#include "ui_playwindow.h"

#include "dialog/commondialog.h"
#include "dialog/volumedialog.h"

#include "manager/playmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/StringLib.h"
#include "util/utilnovatron.h"

#include "widget/form/formtitle.h"
#include "widget/form/formcoverart.h"

const QString SETTINGS_GROUP = "Setup";

PlayWindow::PlayWindow(QWidget *parent)	:
	QWidget(parent),
	m_pMgr(new PlayManager),
	m_pFormCoverArt(new FormCoverArt(this)),
	m_pFormTitle(new FormTitle(this)),
	m_VolumeMenu(new QMenu(this)),
	m_Timer(nullptr),
	ui(new Ui::PlayWindow)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();
	InitVariable();
}

PlayWindow::~PlayWindow()
{

	if (m_pMgr)
	{
		delete m_pMgr;
		m_pMgr = nullptr;
	}

	if (m_Timer)
	{
		m_Timer->stop();
		delete m_Timer;
		m_Timer = nullptr;
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

	if (m_VolumeMenu)
	{
		delete m_VolumeMenu;
		m_VolumeMenu = nullptr;
	}

	delete ui;

}

void PlayWindow::SetAddr(const QString &addr)
{
	m_pMgr->SetAddr(addr);
}

void PlayWindow::SlotClickCoverArt(int index)
{
	Q_UNUSED(index)

	SlotBtnInfo();

}

void PlayWindow::SlotBtnInfo()
{
	//	m_pMgr->RequestSongInfo(m_ID);
	if (!m_Src.compare(SRC_MUSIC_DB)
			|| !m_Src.compare(SRC_BROWSER)
			|| !m_Src.compare(SRC_AUDIO_CD))
	{
		m_pMgr->RequestQueueList(m_TimeStamp);
	}
	else if (!m_Src.compare(SRC_I_RADIO)
			 || !m_Src.compare(SRC_PODCAST)
			 || !m_Src.compare(SRC_TIDAL)
			 || !m_Src.compare(SRC_DEEZER)
			 || !m_Src.compare(SRC_NAPSTER)
			 || !m_Src.compare(SRC_HIGH_RES_AUDIO)
			 || !m_Src.compare(SRC_AMAZON)
			 || !m_Src.compare(SRC_AIRABLE_UPNP)
			 || !m_Src.compare(SRC_QOBUZ))
	{
		emit SigSetPlayInfo(m_PlayInfo, true);
	}
}

void PlayWindow::SlotBtnPlayPrev()
{
	m_pMgr->RequestPlayState(PlayManager::PLAY_MODE_PREV);
}

void PlayWindow::SlotBtnPlay()
{
	m_bPause = !m_bPause;
	SetPlayState();

	if (m_bPause)
	{
		m_pMgr->RequestPlayState(PlayManager::PLAY_MODE_PAUSE);
	}
	else
	{
		m_pMgr->RequestPlayState(PlayManager::PLAY_MODE_PLAY);
	}
}

void PlayWindow::SlotBtnStop()
{
	m_pMgr->RequestPlayState(PlayManager::PLAY_MODE_STOP);
}

void PlayWindow::SlotBtnPlayNext()
{
	m_pMgr->RequestPlayState(PlayManager::PLAY_MODE_NEXT);
}

void PlayWindow::SlotBtnRandom()
{
	if (m_Recordable)
	{
		if (m_Record)
		{
			SlotBtnStop();
		}
		else
		{
			m_pMgr->RequestRecord();
		}
	}
	else
	{
		m_pMgr->RequestRepeatMode();
	}
}

void PlayWindow::SlotBtnVolume()
{
	LogDebug("volume [%d][%d][%d][%d]", ui->btnVolume->geometry().x(), ui->btnVolume->geometry().y(), ui->btnVolume->geometry().width(), ui->btnVolume->geometry().height());
	QPoint point = ui->btnVolume->mapToGlobal(ui->btnVolume->rect().topLeft());
	VolumeDialog dialog;
	dialog.setGeometry(point.x() - dialog.width(), point.y() - dialog.height(), dialog.width(), dialog.height());
	dialog.SetSliderValue(m_Volume);
	connect(&dialog, SIGNAL(SigIncrease(int)), this, SLOT(SlotVolumeIncrease(int)));
	connect(&dialog, SIGNAL(SigDecrease(int)), this, SLOT(SlotVolumeDecrease(int)));
	connect(&dialog, SIGNAL(SigSliderValueChanged(int)), this, SLOT(SlotVolumeSliderValueChanged(int)));

	if (dialog.exec() == QDialog::Accepted)
	{

	}
}

void PlayWindow::SlotVolumeIncrease(int value)
{
	SlotVolumeSliderValueChanged(value);
}

void PlayWindow::SlotVolumeDecrease(int value)
{
	SlotVolumeSliderValueChanged(value);
}

void PlayWindow::SlotVolumeSliderValueChanged(int value)
{
	ui->labelVolume->setText(QString::number(value));
	m_pMgr->RequestVolume(value);
}

void PlayWindow::SlotVolumeSliderSet(int value)
{
	ui->labelVolume->setText(QString::number(value));
}

void PlayWindow::SlotPlayTimeSliderValueChanged(int value)
{
//	m_pMgr->RequestSeek(value);
}

void PlayWindow::SlotPlayTimeSliderReleased()
{
	int value = ui->horizontalSlider->value();
	m_pMgr->RequestSeek(value);
}

void PlayWindow::SlotPlayTimeSliderUpdate()
{
	m_PlayTime += 1000;
	SetPlayTimeSliderState();
}

void PlayWindow::SlotEventNowPlay(CJsonNode node)
{
	QString state = node.GetString(KEY_PLAY_STATE);
	if (!state.toLower().compare(KEY_STOP))
	{
		InitPlayInfo();
		InitPlayTimeSlider();
		EnableUI(false);

		m_bPause = true;
		SetPlayState();

		QString volume = node.GetString(KEY_VOLUME_CAP);
		if (!volume.isEmpty())
		{
			m_Volume = volume.toInt();
			SlotVolumeSliderSet(m_Volume);
		}

		emit SigRemoveQueueList();
	}
	else
	{
		DoNowPlay(node);
	}
}

void PlayWindow::SlotTrackInfo(CJsonNode node)
{
//	LogDebug("node [%s]", node.ToTabedByteArray().data());
}

void PlayWindow::SlotCoverArtUpdate(QString fileName)
{
	m_pFormCoverArt->SetCoverArt(fileName);
}

void PlayWindow::SlotQueueList(CJsonNode node)
{
	emit SigAddQueueList(node);

	emit SigSetPlayInfo(m_PlayInfo, false);
}

void PlayWindow::ConnectSigToSlot()
{
//	connect(ui->btnInfo, SIGNAL(clicked()), this, SLOT(SlotBtnInfo()));
	connect(ui->btnPrev, SIGNAL(clicked()), this, SLOT(SlotBtnPlayPrev()));
	connect(ui->btnPlay, SIGNAL(clicked()), this, SLOT(SlotBtnPlay()));
	connect(ui->btnStop, SIGNAL(clicked()), this, SLOT(SlotBtnStop()));
	connect(ui->btnNext, SIGNAL(clicked()), this, SLOT(SlotBtnPlayNext()));
	connect(ui->btnRandom, SIGNAL(clicked()), this, SLOT(SlotBtnRandom()));
	connect(ui->btnVolume, SIGNAL(clicked()), this, SLOT(SlotBtnVolume()));

	connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(SlotPlayTimeSliderValueChanged(int)));
	connect(ui->horizontalSlider, SIGNAL(sliderReleased()), this, SLOT(SlotPlayTimeSliderReleased()));

	connect(m_pMgr, SIGNAL(SigTrackInfo(CJsonNode)), this, SLOT(SlotTrackInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString)), this, SLOT(SlotCoverArtUpdate(QString)));
	connect(m_pMgr, SIGNAL(SigQueueList(CJsonNode)), this, SLOT(SlotQueueList(CJsonNode)));

	connect(m_pFormCoverArt, SIGNAL(SigCoverArt(int)), this, SLOT(SlotClickCoverArt(int)));
	connect(m_pFormTitle, SIGNAL(SigTitle()), this, SLOT(SlotBtnInfo()));
	connect(m_pFormTitle, SIGNAL(SigSubtitle()), this, SLOT(SlotBtnInfo()));
}

void PlayWindow::Initialize()
{
	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);
	ui->gridLayoutFormTitle->addWidget(m_pFormTitle);

	m_pFormTitle->SetTitleFont(FONT_SIZE_ICON_TITLE, FONT_COLOR_WHITE);
	m_pFormTitle->SetSubtitleFont(FONT_SIZE_ICON_SUBTITLE, FONT_COLOR_WHITE);

//	SetPlayState();
	SetRepeatMode(RM_NORMAL);

	InitPlayInfo();
	InitPlayTimeSlider();
	EnableUI(false);

	ui->btnInfo->hide();
}

void PlayWindow::InitVariable()
{
	m_Info = false;
	m_List = false;
	m_Mute = false;
	m_Next = false;
	m_PlayPause = false;
	m_Prev = false;
	m_Program = false;
	m_Record = false;
	m_Recordable = false;
	m_Seek = false;
	m_Swap = false;

	m_Duration = -1;
	m_ID = -1;
	m_PlayTime = -1;
	m_SampleRate = -1;
	m_Volume = -1;
	m_TimeStamp = -1;

	m_Bot = "";
	m_CoverArt = "";
	m_Format = "";
	m_Input = "";
	m_PlayState = "";
	m_Repeat = "";
	m_Src = "";
	m_Top= "";
	m_Mqa = "";
}

void PlayWindow::SetVariable(CJsonNode node)
{
	m_PlayInfo = node;

	m_Info = node.GetBool(KEY_INFO);
	m_List = node.GetBool(KEY_LIST);
	m_Mute = node.GetBool(KEY_MUTE_CAP);
	m_Next = node.GetBool(KEY_NEXT);
	m_PlayPause = node.GetBool(KEY_PLAY_PAUSE);
	m_Prev = node.GetBool(KEY_PREV);
	m_Program = node.GetBool(KEY_PROGRAM);
	m_Record = node.GetBool(KEY_RECORD);
	m_Recordable = node.GetBool(KEY_RECORDABLE);
	m_Seek = node.GetBool(KEY_SEEK);
	m_Swap = node.GetBool(KEY_SWAP);

	EnableBtnInfo(m_Info);
	EnableBtnNext(m_Next);
	EnableBtnPlay(m_PlayPause);
	EnableBtnPrev(m_Prev);
//	EnableBtnRandom(m_PlayPause);
	EnableBtnSlider(m_Seek);
//	EnableBtnStop(m_PlayPause);

	m_Duration = node.GetInt(KEY_DURATION);
	m_ID = node.GetInt(KEY_ID_UPPER);
	m_PlayTime = node.GetInt(KEY_PLAY_TIME);
	m_SampleRate = node.GetInt(KEY_INPUT_SAMPLE_RATE);
	m_Volume = node.GetInt(KEY_VOLUME_CAP);
	m_TimeStamp = node.GetInt64(KEY_TIME_STAMP);

	m_Bot = node.GetString(KEY_BOT);
	m_CoverArt = node.GetString(KEY_COVER_ART);
	m_Format = node.GetString(KEY_FORMAT);
	m_Input = node.GetString(KEY_INPUT);
	m_PlayState = node.GetString(KEY_PLAY_STATE);
	m_Src = node.GetString(KEY_SRC);
	m_Top = node.GetString(KEY_TOP);
	m_Mqa = node.GetString(KEY_MQA);

	m_Repeat = node.GetString(KEY_REPEAT);
	if (!m_Repeat.isEmpty())
	{
		SetRepeatMode(m_Repeat);
	}

	emit SigSetPlayInfo(node, false);

	DebugVariable();
}

void PlayWindow::DebugVariable()
{
//	LogDebug("m_Info [%d]", m_Info);
//	LogDebug("m_List [%d]", m_List);
//	LogDebug("m_Mute [%d]", m_Mute);
//	LogDebug("m_Next [%d]", m_Next);
//	LogDebug("m_PlayPause [%d]", m_PlayPause);
//	LogDebug("m_Prev [%d]", m_Prev);
//	LogDebug("m_Program [%d]", m_Program);
//	LogDebug("m_Record [%d]", m_Record);
//	LogDebug("m_Recordable [%d]", m_Recordable);
//	LogDebug("m_Seek [%d]", m_Seek);
//	LogDebug("m_Swap [%d]", m_Swap);
//	LogDebug("m_Duration [%d]", m_Duration);
//	LogDebug("m_ID [%d]", m_ID);
//	LogDebug("m_PlayTime [%d]", m_PlayTime);
//	LogDebug("m_SampleRate [%d]", m_SampleRate);
//	LogDebug("m_Volume [%d]", m_Volume);
//	LogDebug("m_TimeStamp [%d]", m_TimeStamp);
//	LogDebug("m_Bot [%s]", m_Bot.toUtf8().data());
//	LogDebug("m_CoverArt [%s]", m_CoverArt.toUtf8().data());
//	LogDebug("m_Format [%s]", m_Format.toUtf8().data());
//	LogDebug("m_Input [%s]", m_Input.toUtf8().data());
//	LogDebug("m_PlayState [%s]", m_PlayState.toUtf8().data());
//	LogDebug("m_Repeat [%s]", m_Repeat.toUtf8().data());
//	LogDebug("m_Src [%s]", m_Src.toUtf8().data());
//	LogDebug("m_Top [%s]", m_Top.toUtf8().data());
//	LogDebug("m_Mqa [%s]", m_Mqa.toUtf8().data());
}

void PlayWindow::InitPlayInfo()
{
	m_pFormTitle->SetTitle(" ");
	m_pFormTitle->SetSubtitle(" ");
	m_pFormCoverArt->SetCoverArt(" ");

	ui->btnPrev->show();
	ui->btnPlay->show();
	ui->btnStop->show();
	ui->btnNext->show();
	ui->btnRandom->show();
}

void PlayWindow::InitPlayTimeSlider()
{
	QString initTime = ConvertMSecToHHMMSSStr(-1);
	ui->labelCurTime->setText(initTime);
	ui->labelTotalTime->setText(initTime);

	ui->horizontalSlider->setValue(0);
}

void PlayWindow::EnableUI(bool bEnable)
{
	EnableBtnInfo(bEnable);
	EnableBtnNext(bEnable);
	EnableBtnPlay(bEnable);
	EnableBtnPrev(bEnable);
	EnableBtnRandom(bEnable);
	EnableBtnSlider(bEnable);
	EnableBtnStop(bEnable);
}

void PlayWindow::EnableBtnInfo(bool bEnable)
{
	ui->btnInfo->setEnabled(bEnable);
}

void PlayWindow::EnableBtnPrev(bool bEnable)
{
	ui->btnPrev->setEnabled(bEnable);
}

void PlayWindow::EnableBtnRandom(bool bEnable)
{
	ui->btnRandom->setEnabled(bEnable);
}

void PlayWindow::EnableBtnNext(bool bEnable)
{
	ui->btnNext->setEnabled(bEnable);
}

void PlayWindow::EnableBtnPlay(bool bEnable)
{
	ui->btnPlay->setEnabled(bEnable);
}

void PlayWindow::EnableBtnSlider(bool bEnable)
{
	ui->horizontalSlider->setEnabled(bEnable);
}

void PlayWindow::EnableBtnStop(bool bEnable)
{
	ui->btnStop->setEnabled(bEnable);
}

void PlayWindow::SetTimer(bool bStart)
{
	if (m_Timer)
	{
		m_Timer->stop();
		delete m_Timer;
		m_Timer = nullptr;
	}

	if (bStart)
	{
		m_Timer = new QTimer(this);
		connect(m_Timer, SIGNAL(timeout()), this, SLOT(SlotPlayTimeSliderUpdate()));
		m_Timer->start(1000);
	}
}

void PlayWindow::SetPlayState()
{
	QString style;
	if (m_bPause)
	{
		// pause state
		style = QString("QPushButton	\
						{	\
						  border-image: url(\":/resource/btm-btn40-play-n.png\");	\
						}\
						QPushButton:hover	\
						{	\
						  border-image: url(\":/resource/btm-btn40-play-h.png\");	\
						}");

	}
	else
	{
		// play state
		style = QString("QPushButton	\
						{	\
						  border-image: url(\":/resource/btm-btn40-pause-n.png\");	\
						}\
						QPushButton:hover	\
						{	\
						  border-image: url(\":/resource/btm-btn40-pause-h.png\");	\
						}");

	}

	SetTimer(!m_bPause);
	ui->btnPlay->setStyleSheet(style);

}

void PlayWindow::SetRepeatMode(QString mode)
{
	QString style;
	if (!mode.compare(RM_NORMAL))
	{
		m_nRepeatMode = PLAY_REPEAT_NORMAL;
		style = QString("QPushButton	\
						{	\
						  border-image: url(\":/resource/btm-btn26-normal-n.png\");	\
						}\
						QPushButton:hover	\
						{	\
						  border-image: url(\":/resource/btm-btn26-normal-h.png\");	\
						}");
	}
	else if (!mode.compare(RM_RANDOM))
	{
		m_nRepeatMode = PLAY_REPEAT_RANDOM;
		style = QString("QPushButton	\
						{	\
						  border-image: url(\":/resource/btm-btn26-random-n.png\");	\
						}\
						QPushButton:hover	\
						{	\
						  border-image: url(\":/resource/btm-btn26-random-h.png\");	\
						}");
	}
	else if (!mode.compare(RM_ONCE))
	{
		m_nRepeatMode = PLAY_REPEAT_ONCE;
		style = QString("QPushButton	\
						{	\
						  border-image: url(\":/resource/btm-btn26-once-n.png\");	\
						}\
						QPushButton:hover	\
						{	\
						  border-image: url(\":/resource/btm-btn26-once-h.png\");	\
						}");
	}
	else if (!mode.compare(RM_R_ONE))
	{
		m_nRepeatMode = PLAY_REPEAT_ONE;
		style = QString("QPushButton	\
						{	\
						  border-image: url(\":/resource/btm-btn26-repeatone-n.png\");	\
						}\
						QPushButton:hover	\
						{	\
						  border-image: url(\":/resource/btm-btn26-repeatone-h.png\");	\
						}");
	}
	else if (!mode.compare(RM_R_ALL))
	{
		m_nRepeatMode = PLAY_REPEAT_ALL;
		style = QString("QPushButton	\
						{	\
						  border-image: url(\":/resource/btm-btn26-normalall-n.png\");	\
						}\
						QPushButton:hover	\
						{	\
						  border-image: url(\":/resource/btm-btn26-normalall-h.png\");	\
						}");
	}
	else if (!mode.compare(RM_SHUFFLE))
	{
		m_nRepeatMode = PLAY_REPEAT_SHFFLE;
		style = QString("QPushButton	\
						{	\
						  border-image: url(\":/resource/btm-btn26-shuffle-n.png\");	\
						}\
						QPushButton:hover	\
						{	\
						  border-image: url(\":/resource/btm-btn26-shuffle-h.png\");	\
						}");
	}

	ui->btnRandom->setStyleSheet(style);

}

void PlayWindow::SetRecordable()
{
	QString style;

	if (m_Record)
	{
		style = QString("QPushButton	\
						{	\
						  border-image: url(\":/resource/internets-btn28-record-u.png\");	\
						}");

		ui->btnStop->hide();
	}
	else
	{
		style = QString("QPushButton	\
						{	\
						  border-image: url(\":/resource/internets-btn28-record-n.png\");	\
						}\
						QPushButton:hover	\
						{	\
						  border-image: url(\":/resource/internets-btn28-record-h.png\");	\
						}");
	}

	ui->btnRandom->setStyleSheet(style);
}

void PlayWindow::SetCoverArt(QString filepath)
{
	m_pMgr->RequestCoverArt(filepath);
}

void PlayWindow::SetPlayTimeSliderState()
{
	ui->horizontalSlider->setMinimum(0);
	ui->horizontalSlider->setMaximum(m_Duration);
	ui->horizontalSlider->setValue(m_PlayTime);

	QString strTime = ConvertMSecToHHMMSSStr(m_Duration);
	ui->labelTotalTime->setText(strTime);

	strTime = ConvertMSecToHHMMSSStr(m_PlayTime);
	ui->labelCurTime->setText(strTime);

	ui->horizontalSlider->setHidden(false);
	ui->labelTotalTime->setHidden(false);
	ui->labelCurTime->setHidden(false);
}

void PlayWindow::DoNowPlay(CJsonNode node)
{
	EnableUI(true);
	SetVariable(node);

	SlotVolumeSliderSet(m_Volume);

	if (m_PlayState.compare(KEY_PLAY))
	{
		m_bPause = true;
	}
	else
	{
		m_bPause = false;
	}

	SetPlayTimeSliderState();
	SetPlayState();
	if (m_Recordable)
	{
		SetRecordable();
	}
	else
	{
		SetRepeatMode(m_Repeat);
	}


	if (!m_Src.compare(SRC_MUSIC_DB)
			|| !m_Src.compare(SRC_BROWSER)
			|| !m_Src.compare(SRC_AUDIO_CD) )
	{
		ui->btnPrev->show();
		ui->btnPlay->show();
		ui->btnStop->show();
		ui->btnNext->show();
		ui->btnRandom->show();

		m_pFormTitle->SetTitle(m_Top);
		m_pFormTitle->SetSubtitle(m_Bot);

		SetCoverArt(m_CoverArt);
	}
	else if (!m_Src.compare(SRC_AES_EBU)
			 || !m_Src.compare(SRC_COAXIAL)
			 || !m_Src.compare(SRC_TOSLINK)
			 || !m_Src.compare(SRC_ANALOG_IN)
			 || !m_Src.compare(SRC_AUX_IN)
			 || !m_Src.compare(SRC_PHONO_IN) )
	{
		ui->btnPrev->hide();
		ui->btnPlay->hide();
		ui->btnStop->show();
		ui->btnNext->hide();
		ui->btnRandom->show();

		m_pFormTitle->SetTitle(m_Top);
		m_pFormTitle->SetSubtitle(m_Format);

		m_CoverArt = UtilNovatron::GetCoverArtIcon(SIDEMENU_INPUT, m_Src);
		SlotCoverArtUpdate(m_CoverArt);
	}
	else if (!m_Src.compare(SRC_FM_RADIO)
			 || !m_Src.compare(SRC_DAB_RADIO))
	{
		ui->btnPrev->show();
		ui->btnPlay->hide();
		ui->btnStop->show();
		ui->btnNext->show();
		ui->btnRandom->show();

		m_pFormTitle->SetTitle(m_Top);
		m_pFormTitle->SetSubtitle(m_Src);

		m_CoverArt = UtilNovatron::GetCoverArtIcon(SIDEMENU_FM_RADIO);
		SlotCoverArtUpdate(m_CoverArt);
	}
	else if (!m_Src.compare(SRC_I_RADIO)
			 || !m_Src.compare(SRC_PODCAST)
			 || !m_Src.compare(SRC_TIDAL)
			 || !m_Src.compare(SRC_DEEZER)
			 || !m_Src.compare(SRC_NAPSTER)
			 || !m_Src.compare(SRC_HIGH_RES_AUDIO)
			 || !m_Src.compare(SRC_AMAZON)
			 || !m_Src.compare(SRC_AIRABLE_UPNP)
			 || !m_Src.compare(SRC_QOBUZ))
	{
		ui->btnPrev->hide();
		if (m_PlayPause)
		{
			ui->btnPlay->show();
		}
		else
		{
			ui->btnPlay->hide();
		}
		ui->btnStop->show();
		ui->btnNext->hide();
		if (m_Recordable)
		{
			ui->btnRandom->show();
		}
		else
		{
			ui->btnRandom->hide();
		}

		m_pFormTitle->SetTitle(m_Top);
		m_pFormTitle->SetSubtitle(m_Bot);

		SetCoverArt(m_CoverArt);
	}
	else if (!m_Src.compare(SRC_UPNP))
	{
		m_pFormTitle->SetTitle(m_Top);
		m_pFormTitle->SetSubtitle(m_Src);

		SetCoverArt(m_CoverArt);
	}
	else
	{
		LogWarning("====================================");
		LogWarning("This source is not supported [%s]", m_Src.toUtf8().data());
		LogWarning("====================================");
	}

}
