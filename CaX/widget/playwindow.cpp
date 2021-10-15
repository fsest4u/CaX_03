#include <QMouseEvent>
#include <QTimer>

#include "playwindow.h"
#include "ui_playwindow.h"

#include "manager/playmanager.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/StringLib.h"

#define RM_NORMAL	"RM_NORMAL"
#define RM_SHUFFLE	"RM_SHUFFLE"
#define RM_RANDOM	"RM_RANDOM"
#define RM_ONCE		"RM_ONCE"
#define RM_R_ONE	"RM_R_ONE"
#define RM_R_ALL	"RM_R_ALL"

#define SRC_MUSIC_DB		"Music DB"
#define SRC_BROWSER			"File"
#define SRC_AUDIO_CD		"Audio CD"

#define SRC_AES_EBU			"AES"
#define SRC_COAXIAL			"COAXIAL"
#define SRC_TOSLINK			"TOSLINK"
#define SRC_ANALOG_IN		"ANALOG IN"
#define SRC_AUX_IN			"AUX IN"
#define SRC_PHONO_IN		"PHONO IN"

#define SRC_FM_RADIO		"FM Radio"
#define SRC_I_RADIO			"i-Radio"
#define SRC_PODCAST			"Podcasts"
#define SRC_TIDAL			"TIDAL"
#define SRC_DEEZER			"Deezer"
#define SRC_NAPSTER			"Napster"
#define SRC_HIGH_RES_AUDIO	"HighResAudio"
#define SRC_AMAZON			"Amazon"
#define SRC_AIRABLE_UPNP	"Airable UPNP"
#define SRC_QOBUZ			"Qobuz"


PlayWindow::PlayWindow(QWidget *parent)	:
	QWidget(parent),
	m_pMgr(new PlayManager),
	m_Timer(nullptr),
	m_TotTime(0),
	m_CurTime(0),
	m_bPlay(false),
	m_CoverArt(""),
	m_DeviceName(""),
	ui(new Ui::PlayWindow)
{
	ui->setupUi(this);

	ConnectSigToSlot();

	InitUI();
	EnableUI(false);
}

PlayWindow::~PlayWindow()
{
	ui->labelCoverArt->removeEventFilter(this);

	delete ui;

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


bool PlayWindow::eventFilter(QObject *object, QEvent *event)
{
	if(event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->labelCoverArt)
		{
			SlotClickCoverArt();
		}

	}

	return QObject::eventFilter(object, event);
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
	if (m_bPlay)
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
	m_pMgr->RequestRepeatMode();
}

void PlayWindow::SlotBtnDevice()
{
	// todo-dylee
	m_pMgr->RequestMute();
}

void PlayWindow::SlotBtnVolume()
{
	LogDebug("click btn volume");
	m_pMgr->RequestVolume(30);
}

void PlayWindow::SlotSliderReleased()
{
	int mSec = ui->horizontalSlider->value();
	m_pMgr->RequestSeek(mSec);
}

void PlayWindow::SlotSliderUpdate()
{
	m_CurTime += 1000;
	SetSliderState();
}

void PlayWindow::SlotRespNowPlay(CJsonNode node)
{
	QString state = node.GetString(KEY_PLAY_STATE);
	if (!state.toLower().compare("stop"))
	{
		InitUI();
		EnableUI(false);
		SetTimer(false);
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
	m_CoverArt = fileName;

	if (m_CoverArt.isEmpty())
	{
		m_CoverArt = ":/resource/Icon-playbar-volume-160.png";
	}

	QImage image;
	if (image.load(m_CoverArt))
	{
//		painter->drawImage(coverRect, image);
		QPixmap pixmap = QPixmap::fromImage(image);
		ui->labelCoverArt->setPixmap(pixmap.scaled(ui->labelCoverArt->width()
												   , ui->labelCoverArt->height()
												   , Qt::KeepAspectRatio));
	}
}

void PlayWindow::SlotQueueList(CJsonNode node)
{
	LogDebug("node [%s]", node.ToTabedByteArray().data());

}


void PlayWindow::ConnectSigToSlot()
{
	ui->labelCoverArt->installEventFilter(this);

	connect(ui->btnInfo, SIGNAL(clicked()), this, SLOT(SlotBtnInfo()));
	connect(ui->btnPrev, SIGNAL(clicked()), this, SLOT(SlotBtnPlayPrev()));
	connect(ui->btnPlay, SIGNAL(clicked()), this, SLOT(SlotBtnPlay()));
	connect(ui->btnStop, SIGNAL(clicked()), this, SLOT(SlotBtnStop()));
	connect(ui->btnNext, SIGNAL(clicked()), this, SLOT(SlotBtnPlayNext()));
	connect(ui->btnRandom, SIGNAL(clicked()), this, SLOT(SlotBtnRandom()));
	connect(ui->btnDevice, SIGNAL(clicked()), this, SLOT(SlotBtnDevice()));
	connect(ui->btnVolume, SIGNAL(clicked()), this, SLOT(SlotBtnVolume()));
	connect(ui->horizontalSlider, SIGNAL(sliderReleased()), this, SLOT(SlotSliderReleased()));

	connect(m_pMgr, SIGNAL(SigTrackInfo(CJsonNode)), this, SLOT(SlotTrackInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString)), this, SLOT(SlotCoverArtUpdate(QString)));
	connect(m_pMgr, SIGNAL(SigQueueList(CJsonNode)), this, SLOT(SlotQueueList(CJsonNode)));



}

void PlayWindow::InitUI()
{
	ui->labelTitle->setText("-");
	ui->labelArtist->setText("-");
	ui->labelCoverArt->clear();

	QString initTime = ConvertMSecToHHMMSSStr(-1);
	ui->labelCurTime->setText(initTime);
	ui->labelTotalTime->setText(initTime);

	ui->horizontalSlider->setValue(0);

	m_TotTime = 0;
	m_CurTime = 0;

	SetPlayState("");
	SetRepeatMode("");
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
		connect(m_Timer, SIGNAL(timeout()), this, SLOT(SlotSliderUpdate()));
		m_Timer->start(1000);
	}
}

void PlayWindow::SetPlayState(QString state)
{
	if (!state.compare("Play"))
	{
		m_bPlay = true;
		SetTimer(true);
	}
	else
	{
		m_bPlay = false;
		SetTimer(false);
	}
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

void PlayWindow::SetSliderState()
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

void PlayWindow::DoNowPlay(CJsonNode node)
{
	EnableUI(true);

	QString strSrc = node.GetString(KEY_SRC);

	if (!strSrc.compare(SRC_MUSIC_DB)
			|| !strSrc.compare(SRC_BROWSER)
			|| !strSrc.compare(SRC_AUDIO_CD) )
	{
		ui->labelTitle->setText(node.GetString(KEY_TOP));
		ui->labelArtist->setText(node.GetString(KEY_BOT));

		m_ID = node.GetInt(KEY_ID_UPPER);
		m_TotTime = node.GetInt(KEY_DURATION);
		m_CurTime = node.GetInt(KEY_PLAY_TIME);

		SetSliderState();
		SetPlayState(node.GetString(KEY_PLAY_STATE));
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
