#include <QMouseEvent>
#include <QTimer>
#include <QWidgetAction>

#include "playwindow.h"
#include "ui_playwindow.h"

#include "dialog/commondialog.h"

#include "manager/playmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/StringLib.h"

#include "widget/form/formtitle.h"
#include "widget/form/formcoverart.h"

PlayWindow::PlayWindow(QWidget *parent)	:
	QWidget(parent),
	m_pMgr(new PlayManager),
	m_pFormCoverArt(new FormCoverArt(this)),
	m_pFormTitle(new FormTitle(this)),
	m_Menu(new QMenu(this)),
	m_VolumeMenu(new QMenu(this)),
	m_Slider(new QSlider(this)),
	m_Timer(nullptr),
	m_TotTime(0),
	m_CurTime(0),
	m_bPause(false),
	m_CoverArt(""),
	m_DeviceName(""),
	ui(new Ui::PlayWindow)
{
	ui->setupUi(this);

	ConnectSigToSlot();

	Initialize();
	EnableUI(false);
}

PlayWindow::~PlayWindow()
{

	disconnect(m_Menu, SIGNAL(triggered(QAction*)));

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

	if (m_Menu)
	{
		delete m_Menu;
		m_Menu = nullptr;
	}

	if (m_VolumeMenu)
	{
		delete m_VolumeMenu;
		m_VolumeMenu = nullptr;
	}

	if (m_Slider)
	{
		delete m_Slider;
		m_Slider = nullptr;
	}

	delete ui;

}

void PlayWindow::SetAddr(const QString &addr)
{
	m_pMgr->SetAddr(addr);
}


QString PlayWindow::GetDeviceName() const
{
	return m_DeviceName;
}

void PlayWindow::SetDeviceName(const QString &DeviceName)
{
	m_DeviceName = DeviceName;
	ui->btnDevice->setText(m_DeviceName);
}

void PlayWindow::ClearMenu()
{
	m_Menu->clear();
}

void PlayWindow::SetMenu(QMap<QString, QString> map)
{
	QMap<QString, QString>::iterator i;
	for (i = map.begin(); i != map.end(); i++)
	{
		QAction *action = new QAction(i.value(), this);
		action->setData(i.key());
		m_Menu->addAction(action);
	}
}

void PlayWindow::SlotClickCoverArt()
{
	LogDebug("click label cover art");

}

void PlayWindow::SlotBtnInfo()
{
	m_pMgr->RequestSongInfo(m_ID);
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
	InitPlayTimeSlider();

	m_bPause = true;
	SetPlayState();

	m_pMgr->RequestPlayState(PlayManager::PLAY_MODE_STOP);
}

void PlayWindow::SlotBtnPlayNext()
{
	m_pMgr->RequestPlayState(PlayManager::PLAY_MODE_NEXT);
}

void PlayWindow::SlotBtnRandom()
{
	m_pMgr->RequestRepeatMode();
}

void PlayWindow::SlotMenu()
{
	emit SigMenu();
}

void PlayWindow::SlotMenuAction(QAction *action)
{
	emit SigMenuAction(action->data().toString());
}

void PlayWindow::SlotVolumeSliderValueChanged(int value)
{
	ui->labelVolume->setText(QString("%1").arg(value));
	m_pMgr->RequestVolume(value);
}

void PlayWindow::SlotVolumeSliderReleased()
{
	int value = m_Slider->value();
	ui->labelVolume->setText(QString("%1").arg(value));
	m_pMgr->RequestVolume(value);
}

void PlayWindow::SlotSetVolumeSlider(int volume)
{
	ui->labelVolume->setText(QString("%1").arg(volume));
	m_Slider->setValue(volume);
}

void PlayWindow::SlotPlayTimeSliderValueChanged(int value)
{
	m_pMgr->RequestSeek(value);
}

void PlayWindow::SlotPlayTimeSliderReleased()
{
	int value = ui->horizontalSlider->value();
	m_pMgr->RequestSeek(value);
}

void PlayWindow::SlotPlayTimeSliderUpdate()
{
	m_CurTime += 1000;
	SetPlayTimeSliderState();
}

void PlayWindow::SlotEventNowPlay(CJsonNode node)
{
	QString state = node.GetString(KEY_PLAY_STATE);
	if (!state.toLower().compare(KEY_STOP))
	{
//		EnableUI(false);
//		SetTimer(false);
	}
	else
	{
		DoNowPlay(node);
	}
}

void PlayWindow::SlotTrackInfo(CJsonNode node)
{
	LogDebug("node [%s]", node.ToTabedByteArray().data());
}

void PlayWindow::SlotCoverArtUpdate(QString fileName)
{
	m_pFormCoverArt->SetCoverArt(fileName);
}

void PlayWindow::SlotQueueList(CJsonNode node)
{
//	LogDebug("node [%s]", node.ToTabedByteArray().data());

//	CommonDialog dialog(this, STR_WARNING, STR_COMING_SOON);
//	dialog.exec();

}


void PlayWindow::ConnectSigToSlot()
{
	connect(ui->btnInfo, SIGNAL(clicked()), this, SLOT(SlotBtnInfo()));
	connect(ui->btnPrev, SIGNAL(clicked()), this, SLOT(SlotBtnPlayPrev()));
	connect(ui->btnPlay, SIGNAL(clicked()), this, SLOT(SlotBtnPlay()));
	connect(ui->btnStop, SIGNAL(clicked()), this, SLOT(SlotBtnStop()));
	connect(ui->btnNext, SIGNAL(clicked()), this, SLOT(SlotBtnPlayNext()));
	connect(ui->btnRandom, SIGNAL(clicked()), this, SLOT(SlotBtnRandom()));
	connect(ui->btnDevice, SIGNAL(pressed()), this, SLOT(SlotMenu()));

	connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(SlotPlayTimeSliderValueChanged(int)));
	connect(ui->horizontalSlider, SIGNAL(sliderReleased()), this, SLOT(SlotPlayTimeSliderReleased()));

	connect(this, SIGNAL(SigSetVolumeSlider(int)), this, SLOT(SlotSetVolumeSlider(int)));
	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuAction(QAction*)));

	connect(m_pMgr, SIGNAL(SigTrackInfo(CJsonNode)), this, SLOT(SlotTrackInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString)), this, SLOT(SlotCoverArtUpdate(QString)));
	connect(m_pMgr, SIGNAL(SigQueueList(CJsonNode)), this, SLOT(SlotQueueList(CJsonNode)));

}

void PlayWindow::Initialize()
{
	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);
	ui->gridLayoutFormTitle->addWidget(m_pFormTitle);

	m_pFormTitle->SetTitleFont(14, "FFFFFF");
	m_pFormTitle->SetTitle("-");
	m_pFormTitle->SetSubtitleFont(14, "FFFFFF");
	m_pFormTitle->SetSubtitle("-");
	m_pFormCoverArt->SetCoverArt("");

	InitPlayTimeSlider();

//	SetPlayState();
	SetRepeatMode("");
	SetDeviceMenu();
	SetVolumeMenu();

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
	ui->btnInfo->setEnabled(bEnable);
	ui->btnPrev->setEnabled(bEnable);
	ui->btnPlay->setEnabled(bEnable);
	ui->btnStop->setEnabled(bEnable);
	ui->btnNext->setEnabled(bEnable);
	ui->btnRandom->setEnabled(bEnable);
	ui->btnVolume->setEnabled(bEnable);

	ui->horizontalSlider->setEnabled(bEnable);
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
						  border-image: url(\":/resource/btm-btn40-play-n@3x.png\");	\
						}\
						QPushButton:hover	\
						{	\
						  border-image: url(\":/resource/btm-btn40-play-h@3x.png\");	\
						}");

	}
	else
	{
		// play state
		style = QString("QPushButton	\
						{	\
						  border-image: url(\":/resource/btm-btn40-pause-n@3x.png\");	\
						}\
						QPushButton:hover	\
						{	\
						  border-image: url(\":/resource/btm-btn40-pause-h@3x.png\");	\
						}");

	}

	SetTimer(!m_bPause);
	ui->btnPlay->setStyleSheet(style);

}

void PlayWindow::SetRepeatMode(QString mode)
{
	if (!mode.compare(RM_SHUFFLE))
	{
		m_nRepeatMode = PLAY_REPEAT_SHFFLE;
	}
	else if (!mode.compare(RM_RANDOM))
	{
		m_nRepeatMode = PLAY_REPEAT_RANDOM;
	}
	else if (!mode.compare(RM_ONCE))
	{
		m_nRepeatMode = PLAY_REPEAT_ONCE;
	}
	else if (!mode.compare(RM_R_ONE))
	{
		m_nRepeatMode = PLAY_REPEAT_ONE;
	}
	else if (!mode.compare(RM_R_ALL))
	{
		m_nRepeatMode = PLAY_REPEAT_ALL;
	}
	else //if (!mode.compare(RM_NORMAL))
	{
		m_nRepeatMode = PLAY_REPEAT_NORMAL;
	}
}

void PlayWindow::SetCoverArt(QString filepath)
{
	m_pMgr->RequestCoverArt(filepath);
}

void PlayWindow::SetQueueList(uint timestamp)
{
	m_pMgr->RequestQueueList(timestamp);
}

void PlayWindow::SetPlayTimeSliderState()
{
	ui->horizontalSlider->setMinimum(0);
	ui->horizontalSlider->setMaximum(m_TotTime);
	ui->horizontalSlider->setValue(m_CurTime);

	QString strTime = ConvertMSecToHHMMSSStr(m_TotTime);
	ui->labelTotalTime->setText(strTime);

	strTime = ConvertMSecToHHMMSSStr(m_CurTime);
	ui->labelCurTime->setText(strTime);

	ui->horizontalSlider->setHidden(false);
	ui->labelTotalTime->setHidden(false);
	ui->labelCurTime->setHidden(false);
}

void PlayWindow::SetDeviceMenu()
{
	QString style = QString("QMenu {	\
								background-color: rgb(0, 0, 0);	\
							}	\
							QMenu::item {	\
								width: 160px;	\
								height: 40px;	\
								color: rgb(174,176,179);	\
								font-size: 16pt;	\
								padding: 0px 20px 0px 20px;	\
							}	\
							QMenu::item:selected {	\
								background: rgbargb(238,238,238,255);	\
							}");

	m_Menu->setStyleSheet(style);
	ui->btnDevice->setMenu(m_Menu);

}

void PlayWindow::SetVolumeMenu()
{
	m_Slider->setOrientation(Qt::Horizontal);
	m_Slider->setMinimum(VOLUME_MIN);
	m_Slider->setMaximum(VOLUME_MAX);
	m_Slider->setGeometry( 0, 0, 400, 40 );

	QWidgetAction *action = new QWidgetAction(this);
	action->setDefaultWidget(m_Slider);
	m_VolumeMenu->addAction(action);

	ui->btnVolume->setMenu(m_VolumeMenu);

	connect(m_Slider, SIGNAL(valueChanged(int)), this, SLOT(SlotVolumeSliderValueChanged(int)));
	connect(m_Slider, SIGNAL(sliderReleased()), this, SLOT(SlotVolumeSliderReleased()));
}

void PlayWindow::DoNowPlay(CJsonNode node)
{
	EnableUI(true);

	QString strSrc = node.GetString(KEY_SRC);

	if (!strSrc.compare(SRC_MUSIC_DB)
			|| !strSrc.compare(SRC_BROWSER)
			|| !strSrc.compare(SRC_AUDIO_CD) )
	{
		m_pFormTitle->SetTitle(node.GetString(KEY_TOP));
		m_pFormTitle->SetSubtitle(node.GetString(KEY_BOT));

		m_ID = node.GetInt(KEY_ID_UPPER);
		m_TotTime = node.GetInt(KEY_DURATION);
		m_CurTime = node.GetInt(KEY_PLAY_TIME);
		emit SigSetVolumeSlider(node.GetInt(KEY_VOLUME_CAP));
		if (node.GetString(KEY_PLAY_STATE).compare(KEY_PLAY))
		{
			m_bPause = true;
		}
		else
		{
			m_bPause = false;
		}

		SetPlayTimeSliderState();
		SetPlayState();
		SetRepeatMode(node.GetString(KEY_REPEAT));
		SetCoverArt(node.GetString(KEY_COVER_ART));
		SetQueueList(node.GetInt64(KEY_TIME_STAMP));
	}
	else if (!strSrc.compare(SRC_AES_EBU)
			 || !strSrc.compare(SRC_COAXIAL)
			 || !strSrc.compare(SRC_TOSLINK)
			 || !strSrc.compare(SRC_ANALOG_IN)
			 || !strSrc.compare(SRC_AUX_IN)
			 || !strSrc.compare(SRC_PHONO_IN) )
	{
		LogDebug("todo src is [%s]", strSrc.toUtf8().data());
	}
	else if (!strSrc.compare(SRC_FM_RADIO))
	{
		LogDebug("todo src is [%s]", strSrc.toUtf8().data());
	}
	else if (!strSrc.compare(SRC_I_RADIO)
			 || !strSrc.compare(SRC_PODCAST)
			 || !strSrc.compare(SRC_TIDAL)
			 || !strSrc.compare(SRC_DEEZER)
			 || !strSrc.compare(SRC_NAPSTER)
			 || !strSrc.compare(SRC_HIGH_RES_AUDIO)
			 || !strSrc.compare(SRC_AMAZON)
			 || !strSrc.compare(SRC_AIRABLE_UPNP)
			 || !strSrc.compare(SRC_QOBUZ) )
	{
		LogDebug("todo src is [%s]", strSrc.toUtf8().data());
	}

}
