//#include <QMouseEvent>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialog/aboutdialog.h"
#include "dialog/poweroffdialog.h"
#include "dialog/progressdialog.h"

#include "manager/devicemanager.h"
#include "manager/observermanager.h"
#include "manager/appmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"
#include "util/settingio.h"

#include "widget/devicelistwindow.h"
#include "widget/musicdbwindow.h"
#include "widget/audiocdwindow.h"
#include "widget/playlistwindow.h"
#include "widget/browserwindow.h"
#include "widget/iservicewindow.h"
#include "widget/inputwindow.h"
#include "widget/fmradiowindow.h"
#include "widget/dabradiowindow.h"
#include "widget/groupplaywindow.h"
#include "widget/setupwindow.h"

const QString SETTINGS_GROUP = "MainWindow";

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	m_pDeviceMgr(new DeviceManager),
	m_pDeviceWin(new DeviceListWindow),
	m_pObsMgr(new ObserverManager),
	m_pAppMgr(new AppManager),
	m_pLoading(new Loading(this)),
	m_ProgressDialog(new ProgressDialog),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ReadSettings();
	ConnectSigToSlot();
	Initialize();

	SlotMenu();
	m_pDeviceMgr->RequestDeviceList();
	DoDeviceListHome();
}

MainWindow::~MainWindow()
{
	WriteSettings();

	if (m_pObsMgr)
	{
		delete m_pObsMgr;
		m_pObsMgr = nullptr;
	}

	if (m_pAppMgr)
	{
		delete m_pAppMgr;
		m_pAppMgr = nullptr;
	}

	if (m_pDeviceMgr)
	{
		delete m_pDeviceMgr;
		m_pDeviceMgr = nullptr;
	}

	if (m_pDeviceWin)
	{
		delete m_pDeviceWin;
		m_pDeviceWin = nullptr;
	}

	if (m_pLoading)
	{
		delete m_pLoading;
		m_pLoading = nullptr;
	}

	if (m_ProgressDialog)
	{
		delete m_ProgressDialog;
		m_ProgressDialog = nullptr;
	}

	delete ui;
}

void MainWindow::SlotMenu()
{
	m_SideMenuMap.clear();
	ui->widgetTop->ClearMenu();

	if (m_bConnect)
	{

		m_SideMenuMap.insert(SIDEMENU_MUSIC_DB, STR_MUSIC_DB);
		m_SideMenuMap.insert(SIDEMENU_PLAYLIST, STR_PLAYLIST);
		m_SideMenuMap.insert(SIDEMENU_BROWSER, STR_BROWSER);
		m_SideMenuMap.insert(SIDEMENU_ISERVICE, STR_ISERVICE);
		m_SideMenuMap.insert(SIDEMENU_SETUP, STR_SETUP);
		m_SideMenuMap.insert(SIDEMENU_SELECT_DEVICE, STR_SELECT_DEVICE);
		m_SideMenuMap.insert(SIDEMENU_POWER_ON, STR_POWER_ON);
		m_SideMenuMap.insert(SIDEMENU_POWER_OFF, STR_POWER_OFF);
		m_SideMenuMap.insert(SIDEMENU_ABOUT, STR_ABOUT);

		if (m_bAudioCD)
		{
			m_SideMenuMap.insert(SIDEMENU_AUDIO_CD, QString("%1 - %2").arg(STR_AUDIO_CD).arg(STR_AVAILABLE));
		}
		else
		{
			m_SideMenuMap.insert(SIDEMENU_AUDIO_CD, QString("%1 - %2").arg(STR_AUDIO_CD).arg(STR_NOT_AVAILABLE));
		}

		if (m_bInput)
		{
			m_SideMenuMap.insert(SIDEMENU_INPUT, STR_INPUT);
		}

		if (m_bFMRadio)
		{
			m_SideMenuMap.insert(SIDEMENU_FM_RADIO, STR_FM_RADIO);
			m_SideMenuMap.insert(SIDEMENU_DAB_RADIO, STR_DAB_RADIO);
		}

		if (m_bGroupPlay)
		{
			m_SideMenuMap.insert(SIDEMENU_GROUP_PLAY, STR_GROUP_PLAY);
		}
	}
	else
	{
		m_SideMenuMap.insert(SIDEMENU_SELECT_DEVICE, STR_SELECT_DEVICE);
		m_SideMenuMap.insert(SIDEMENU_POWER_ON, STR_POWER_ON);
		m_SideMenuMap.insert(SIDEMENU_ABOUT, STR_ABOUT);
	}

	ui->widgetTop->SetMenu(m_SideMenuMap);
}

void MainWindow::SlotMenuAction(int menuID)
{
	LogDebug("menu action [%d]", menuID);
	switch (menuID)
	{
	case SIDEMENU_MUSIC_DB:
		DoMusicDBHome();
		break;
	case SIDEMENU_AUDIO_CD:
	{
		if (m_bAudioCD)
		{
			DoAudioCDHome();
		}
		else
		{
			QMessageBox::warning(this, STR_WARNING, STR_INSERT_CD);
		}
		break;
	}
	case SIDEMENU_PLAYLIST:
		DoPlaylistHome();
		break;
	case SIDEMENU_BROWSER:
		DoBrowserHome();
		break;
	case SIDEMENU_ISERVICE:
		DoIServiceHome();
		break;
	case SIDEMENU_INPUT:
		DoInputHome();
		break;
	case SIDEMENU_FM_RADIO:
		DoFmRadioHome();
		break;
	case SIDEMENU_DAB_RADIO:
		DoDabRadioHome();
		break;
	case SIDEMENU_GROUP_PLAY:
		DoGroupPlayHome();
		break;
	case SIDEMENU_SETUP:
		DoSetupHome();
		break;
	case SIDEMENU_SELECT_DEVICE:
		DoDeviceListHome();
		break;
	case SIDEMENU_POWER_ON:
		DoPowerOn();
		break;
	case SIDEMENU_POWER_OFF:
		DoPowerOff();
		break;
	case SIDEMENU_ABOUT:
		DoAbout();
		break;
	default:
		break;
	}
}

void MainWindow::SlotBtnHome()
{
	RemoveAllWidget();

	if (m_bConnect)
	{
		DoMusicDBHome();
	}
	else
	{
		DoDeviceListHome();
	}
}

void MainWindow::SlotBtnPrev()
{
	auto idx = ui->stackMain->currentIndex();
	ui->stackMain->setCurrentIndex(--idx);

	UpdateStackState();
}

void MainWindow::SlotBtnNext()
{
	auto idx = ui->stackMain->currentIndex();
	ui->stackMain->setCurrentIndex(++idx);

	UpdateStackState();
}

void MainWindow::SlotBtnSearch()
{
	LogDebug("click btn search");
	QMessageBox::warning(this, STR_WARNING, STR_COMING_SOON);
}

void MainWindow::ReadSettings()
{
	SettingIO settings;
	settings.beginGroup(SETTINGS_GROUP);

	QByteArray geometry = settings.value("geometry").toByteArray();
	if (!geometry.isNull()) {
		restoreGeometry(geometry);
	}

	m_strCurrentMac = settings.value("recent_device").toString();
	m_strAddr = settings.value("recent_addr").toString();

	ui->widgetPlay->SetAddr(m_strAddr);
	m_pAppMgr->SetAddr(m_strAddr);

	// load wol list
	QString strNodeWol = settings.value("node_wol").toString();
	if (!strNodeWol.isEmpty())
	{
		CJsonNode nodeList;
		if (nodeList.SetContent(strNodeWol))
		{
			LogDebug("ReadSettings [%s]", nodeList.ToCompactByteArray().data());
			if (!nodeList.IsNull() && nodeList.ArraySize() > 0)
			{
//				for (int i = 0; i < nodeList.ArraySize(); i++)
//				{
//					CJsonNode node = nodeList.GetArrayAt(i);
//					LogDebug("wol [%s]", node.ToCompactByteArray().data());
//				}
				m_pDeviceMgr->SetDeviceListWol(nodeList);
			}
		}
		else
		{
			LogCritical("invalid json - wake on lan setting");
			return;
		}
	}

	settings.endGroup();
}

void MainWindow::WriteSettings()
{
	SettingIO settings;
	settings.beginGroup(SETTINGS_GROUP);

	settings.setValue("geometry", saveGeometry());

	settings.setValue("recent_device", m_strCurrentMac);
	settings.setValue("recent_addr", m_strAddr);

	// save wol list
	CJsonNode nodeList = m_pDeviceMgr->GetDeviceListWol();
	if (!nodeList.IsNull() && nodeList.ArraySize() > 0)
	{
//		for (int i = 0; i < nodeList.ArraySize(); i++)
//		{
//			CJsonNode node = nodeList.GetArrayAt(i);
//			LogDebug("wol [%s]", node.ToCompactByteArray().data());
//		}
		LogDebug("wol list [%s]", nodeList.ToCompactByteArray().data());
		settings.setValue("node_wol", nodeList.ToCompactString());
	}

	settings.endGroup();
}

void MainWindow::ObserverConnect()
{
	if (m_strCurrentMac.isEmpty())
		return;

	QString strAddr = m_pDeviceMgr->GetDeviceValue(m_strCurrentMac, DEVICE_ADDR);
	if (strAddr.isEmpty())
		return;

	m_bConnect = true;
	ui->widgetTop->setDisabled(false);
	ui->widgetPlay->setDisabled(false);

	m_pObsMgr->RequestObserverInfo(strAddr);

	SlotMenu();

}

void MainWindow::ObserverDisconnect()
{
	ui->widgetTop->setDisabled(true);
	ui->widgetPlay->setDisabled(true);

	m_pObsMgr->RequestDisconnectObserver();
}

void MainWindow::SlotDeviceItem(int state)
{
	Q_UNUSED(state)

	m_pDeviceWin->SetDeviceList(m_pDeviceMgr->GetDeviceList());
}

void MainWindow::SlotRespError(QString errMsg)
{
	QMessageBox::critical(this, "critical", errMsg);
}


void MainWindow::SlotRespTaskList(CJsonNode node)
{
	CJsonNode tasks = node.GetArray(VAL_TASKS);
	if (tasks.ArraySize() <= 0)
	{
		LogDebug("### no task");
		return;
	}

	m_TaskList.clear();
	for (int i = 0; i < tasks.ArraySize(); i++)
	{
		m_TaskList.append(tasks.GetArrayAt(i));
		LogDebug("node : [%s]", m_TaskList[i].ToCompactByteArray().data());
	}
}

void MainWindow::SlotRespDeviceInfo(CJsonNode node)
{
	if (!node.GetString(DEVICE_MAC, m_strCurrentMac) || m_strCurrentMac.isEmpty())
	{
		return;
	}
	if (!node.GetString(DEVICE_VERSION, m_strVersion) || m_strVersion.isEmpty())
	{
		return;
	}
	if (!node.GetString(DEVICE_WOL_ADDR, m_strWolAddr) || m_strWolAddr.isEmpty())
	{
		return;
	}
	if (!node.GetString(DEVICE_UUID, m_strUuid) || m_strUuid.isEmpty())
	{
		return;
	}
	if (!node.GetBool(KEY_FM_RADIO, m_bFMRadio))
	{
//			return;
	}
	if (!node.GetBool(KEY_GROUP_PLAY, m_bGroupPlay))
	{
//			return;
	}
	if (!node.GetBool(KEY_INPUT, m_bInput))
	{
//			return;
	}

	int index = m_strVersion.lastIndexOf(".r") + QString(".r").length();
//	LogDebug("length [%d]", m_strVersion.length());
//	LogDebug("index [%d]", index);
//	LogDebug("right [%s]", m_strVersion.right(m_strVersion.length() - index).toUtf8().data());

	int version = m_strVersion.right(m_strVersion.length() - index).toInt();
	if (version < FIRMWARE_MIN_VERSION)
	{
		LogDebug("version [%d]", version);
		QMessageBox::warning(this, STR_WARNING, STR_UPDATE_FIRMWARE.arg(m_strVersion));
		return;
	}

	ObserverConnect();
}

void MainWindow::SlotRespObserverInfo(CJsonNode node)
{
	CJsonNode nodeSetup;
	CJsonNode nodeIService;
	CJsonNode nodeInput;

	bool    bAudioCD = false;
//	bool    bSigma = false;
	bool    bScanDB = false;
	bool    bIsDelDB = false;
	int     eventID = false;

	if (!node.GetBool(KEY_AUDIO_CD, bAudioCD))
	{
		bAudioCD = false;
	}

//	if (!node.GetBool(KEY_SIGMA, bSigma))
//	{
//		bSigma = false;
//	}
	if (!node.GetBool(KEY_SCAN_DB, bScanDB))
	{
		bScanDB = false;
	}
	if (!node.GetBool(KEY_IS_DEL_DB, bIsDelDB))
	{
		bIsDelDB = false;
	}
	if (!node.GetInt(KEY_EVENT_ID, eventID))
	{
		eventID = -1;
	}
	if (!node.GetArray(KEY_SETUP, nodeSetup))
	{
		nodeSetup.Clear();
	}
	if (!node.GetArray(KEY_ISERVICE, nodeIService))
	{
		nodeIService.Clear();
	}
	if (!node.GetArray(KEY_INPUT, nodeInput))
	{
		nodeInput.Clear();
	}

//	m_bSigma = bSigma;
	m_bScanDB = bScanDB;
	m_bIsDel = bIsDelDB;

	m_EventID = eventID;
	m_bAudioCD = bAudioCD;

	if (m_bAudioCD)
	{
		DoAudioCDHome();
	}
	else
	{
		DoMusicDBHome();
	}

	SlotMenu();

	if (!nodeSetup.IsNull())
	{
		m_SetupList.clear();
		for(int i = 0; i < nodeSetup.ArraySize(); i++)
		{
			m_SetupList.append(nodeSetup.GetArrayAt(i));
		}
	}
	if (!nodeIService.IsNull())
	{
		m_IServiceList.clear();
		for(int i = 0; i < nodeIService.ArraySize(); i++)
		{
			m_IServiceList.append(nodeIService.GetArrayAt(i));
		}
	}
	if (!nodeInput.IsNull()) {
		m_InputList.clear();
		for(int i = 0; i < nodeInput.ArraySize(); i++)
		{
			m_InputList.append(nodeInput.GetArrayAt(i));
		}
	}

	m_pAppMgr->RequestTaskList();

}

void MainWindow::SlotEventProgress(CJsonNode node)
{
	QString cmd1 = node.GetString(KEY_CMD1);

	if (cmd1.toLower().contains("info"))
	{
		m_ProgressDialog->SetWindowTitle(node.GetString(VAL_TITLE_CAP));
		m_ProgressDialog->SetContent(node.GetString(VAL_DISP));
		m_ProgressDialog->SetCurTime(node.GetInt(VAL_CUR_SEC));
		m_ProgressDialog->SetTotTime(node.GetInt(VAL_TOTAL_SEC));
		m_ProgressDialog->SetProgress(node.GetInt(VAL_COUNT), node.GetInt(VAL_TOTAL));
		m_ProgressDialog->SetBtnBack(node.GetBool(VAL_BACK));
		m_ProgressDialog->SetBtnStop(node.GetBool(VAL_STOP));
		m_ProgressDialog->SetBtnSkip(node.GetBool(VAL_SKIP));
		m_ProgressDialog->SetBtnClose(false);
		m_ProgressDialog->SetTaskID(node.GetInt(VAL_TASK_ID));
		m_ProgressDialog->SetEventID(m_EventID);

		m_ProgressDialog->show();

	}
	else if (cmd1.toLower().contains("end"))
	{
//		m_ProgressDialog->hide();
		m_ProgressDialog->SetWindowTitle(node.GetString(VAL_TITLE_CAP));
		m_ProgressDialog->SetContent(node.GetString(VAL_MSG));

		m_ProgressDialog->SetProgress(100, 100);
		m_ProgressDialog->SetBtnBack(false);
		m_ProgressDialog->SetBtnStop(false);
		m_ProgressDialog->SetBtnSkip(false);
		m_ProgressDialog->SetBtnClose(true);

	}
}

void MainWindow::SlotClickBack(int taskID)
{
	m_pAppMgr->RequestProgressBack(m_EventID, taskID);
}

void MainWindow::SlotClickStop(int taskID)
{
	m_pAppMgr->RequestProgressStop(m_EventID, taskID);
}

void MainWindow::SlotClickSkip(int taskID)
{
	m_pAppMgr->RequestProgressSkip(m_EventID, taskID);
}

void MainWindow::SlotSelectDevice(QString mac)
{
	if (mac.isEmpty())
	{
		return;
	}

	QString strAddr = m_pDeviceMgr->GetDeviceValue(mac, DEVICE_ADDR);
	QString strDev = m_pDeviceMgr->GetDeviceValue(mac, DEVICE_DEV);

	if (strAddr.isEmpty())
	{
		return;
	}

	m_strAddr = strAddr;
	WriteSettings();

	ui->widgetPlay->SetAddr(m_strAddr);
	ui->widgetPlay->SetDeviceName(strDev);

	m_pAppMgr->SetAddr(m_strAddr);
	m_pAppMgr->RequestDeviceInfo();

}

void MainWindow::SlotSelectCancel(QString mac)
{
	if (mac.isEmpty() || mac.compare(m_strCurrentMac))
		return;

	if (m_bConnect)
	{
		ObserverDisconnect();
	}

	Initialize();

	SlotMenu();
}

void MainWindow::SlotWolDevice(QString mac)
{
	if (mac.isEmpty())
		return;

	RemoveAllWidget();

	QString strAddr = m_pDeviceMgr->GetDeviceValueWol(mac, DEVICE_WOL_ADDR);
	if (strAddr.isEmpty())
		return;

	m_pDeviceMgr->RequestDevicePowerOn(strAddr, mac);
}

void MainWindow::SlotWolCancel(QString mac)
{
	Q_UNUSED(mac)

	LogDebug("SlotWolCancel");
}

void MainWindow::SlotDevice()
{
	m_DeviceMap.clear();
	CJsonNode nodeList = m_pDeviceMgr->GetDeviceList();
	for (int i = 0; i < nodeList.ArraySize(); i++)
	{
		CJsonNode node = nodeList.GetArrayAt(i);
		LogDebug("node [%s]", node.ToCompactByteArray().data());
		m_DeviceMap.insert(node.GetString(KEY_MAC), node.GetString(KEY_DEV));
	}

	ui->widgetPlay->ClearMenu();
	if (m_DeviceMap.count() > 0)
	{
		ui->widgetPlay->SetMenu(m_DeviceMap);
	}
}

void MainWindow::SlotDeviceAction(QString menuID)
{
	if (m_bConnect)
	{
		ObserverDisconnect();
	}

	Initialize();

	SlotMenu();
	SlotSelectDevice(menuID);
}


void MainWindow::SlotDisconnectObserver()
{
	RemoveAllWidget();
	DoDeviceListHome();

	if (m_pDeviceMgr)
	{
		int index = m_pDeviceMgr->CheckDevice(m_strCurrentMac);
		if (index >= 0)
		{
			m_pDeviceMgr->DelDevice(index);
		}
	}
}

void MainWindow::SlotRespAirableLogout()
{
	DoIServiceHome();
}


void MainWindow::Initialize()
{
	RemoveAllWidget();
	UpdateStackState();

	m_IServiceList.clear();
	m_InputList.clear();
	m_SetupList.clear();

	m_strCurrentMac = "",
	m_strAddr = "",
	m_strVersion = "",
	m_strWolAddr = "",
	m_strUuid = "",
	m_bConnect = false;
	m_bAudioCD = false;
	m_bInput = false;
	m_bFMRadio = false;
	m_bGroupPlay = false;
//	m_bSigma = false;
	m_bScanDB = false;
	m_bIsDel = false;
	m_EventID = -1;

}

void MainWindow::ConnectSigToSlot()
{
	ConnectForUI();
	ConnectForApp();
}

void MainWindow::ConnectForUI()
{
	// top menu
	connect(ui->widgetTop, SIGNAL(SigMenu()), this, SLOT(SlotMenu()));
	connect(ui->widgetTop, SIGNAL(SigMenuAction(int)), this, SLOT(SlotMenuAction(int)));
	connect(ui->widgetTop->GetBtnHome(), SIGNAL(clicked()), this, SLOT(SlotBtnHome()));
	connect(ui->widgetTop->GetBtnPrev(), SIGNAL(clicked()), this, SLOT(SlotBtnPrev()));
	connect(ui->widgetTop->GetBtnNext(), SIGNAL(clicked()), this, SLOT(SlotBtnNext()));
	connect(ui->widgetTop->GetBtnSearch(), SIGNAL(clicked()), this, SLOT(SlotBtnSearch()));

	// play menu
	connect(ui->widgetPlay, SIGNAL(SigMenu()), this, SLOT(SlotDevice()));
	connect(ui->widgetPlay, SIGNAL(SigMenuAction(QString)), this, SLOT(SlotDeviceAction(QString)));
	connect((QObject*)ui->widgetPlay->GetManager(), SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));



}

void MainWindow::ConnectForApp()
{
	connect(m_pDeviceMgr, SIGNAL(SigDeviceItem(int)), this, SLOT(SlotDeviceItem(int)));
	connect(m_pDeviceMgr, SIGNAL(SigAutoConnectDevice(QString)), this, SLOT(SlotSelectDevice(QString)));
	connect(m_pAppMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pAppMgr, SIGNAL(SigRespTaskList(CJsonNode)), this, SLOT(SlotRespTaskList(CJsonNode)));
	connect(m_pAppMgr, SIGNAL(SigRespDeviceInfo(CJsonNode)), this, SLOT(SlotRespDeviceInfo(CJsonNode)));

	connect(m_pObsMgr, SIGNAL(SigDisconnectObserver()), this, SLOT(SlotDisconnectObserver()));
	connect(m_pObsMgr, SIGNAL(SigRespObserverInfo(CJsonNode)), this, SLOT(SlotRespObserverInfo(CJsonNode)));
	connect(m_pObsMgr, SIGNAL(SigEventProgress(CJsonNode)), this, SLOT(SlotEventProgress(CJsonNode)));
	connect(m_pObsMgr, SIGNAL(SigEventNowPlay(CJsonNode)), ui->widgetPlay, SLOT(SlotEventNowPlay(CJsonNode)));

	connect(m_pDeviceWin, SIGNAL(SigSelectDevice(QString)), this, SLOT(SlotSelectDevice(QString)));
	connect(m_pDeviceWin, SIGNAL(SigSelectCancel(QString)), this, SLOT(SlotSelectCancel(QString)));

	connect(m_ProgressDialog, SIGNAL(SigClickBack(int)), this, SLOT(SlotClickBack(int)));
	connect(m_ProgressDialog, SIGNAL(SigClickStop(int)), this, SLOT(SlotClickStop(int)));
	connect(m_ProgressDialog, SIGNAL(SigClickSkip(int)), this, SLOT(SlotClickSkip(int)));


}

void MainWindow::DoDeviceListHome()
{
//	ui->widgetTop->setDisabled(true);
	ui->widgetPlay->setDisabled(true);

	SlotAddWidget(m_pDeviceWin, STR_SELECT_DEVICE);

	m_pDeviceWin->SetTitle(STR_SELECT_DEVICE);
	m_pDeviceWin->SetDeviceList(m_pDeviceMgr->GetDeviceList());

}

void MainWindow::DoMusicDBHome()
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_strAddr, m_EventID);
	widget->AddWidgetItem();
	SlotAddWidget(widget, STR_MUSIC_DB);
	widget->RequestCategoryList();
}

void MainWindow::DoAudioCDHome()
{
	AudioCDWindow *widget = new AudioCDWindow(this, m_strAddr, m_EventID);
	widget->AddWidgetAudioCDHome();
	SlotAddWidget(widget, STR_AUDIO_CD);
	widget->RequestTrackList();
}

void MainWindow::DoPlaylistHome()
{
	PlaylistWindow *widget = new PlaylistWindow(this, m_strAddr);
	widget->AddWidgetItem();
	SlotAddWidget(widget, STR_PLAYLIST);
	widget->RequestPlaylist();
}

void MainWindow::DoBrowserHome()
{
	BrowserWindow *widget = new BrowserWindow(this, m_strAddr, m_EventID);
	SlotAddWidget(widget, STR_BROWSER);
	widget->RequestRoot();
}

void MainWindow::DoIServiceHome()
{
	IServiceWindow *widget = new IServiceWindow(this, m_strAddr);
	SlotAddWidget(widget, STR_ISERVICE);
	widget->IServiceHome(m_IServiceList);
}

void MainWindow::DoInputHome()
{
	InputWindow *widget = new InputWindow(this, m_strAddr);
	SlotAddWidget(widget, STR_INPUT);
	widget->InputHome(m_InputList);
}

void MainWindow::DoFmRadioHome()
{
	FMRadioWindow *widget = new FMRadioWindow(this, m_strAddr);
	widget->AddWidgetFMRadioHome();
	SlotAddWidget(widget, STR_FM_RADIO);
	widget->RequestList();

	connect(m_pObsMgr, SIGNAL(SigEventFmSeeking(CJsonNode)), widget, SLOT(SlotEventFmSeeking(CJsonNode)));
	connect(m_pObsMgr, SIGNAL(SigEventFmSeek(CJsonNode)), widget, SLOT(SlotEventFmSeek(CJsonNode)));
	connect(m_pObsMgr, SIGNAL(SigEventFmSeekStop(CJsonNode)), widget, SLOT(SlotEventFmSeekStop(CJsonNode)));
	connect(m_pObsMgr, SIGNAL(SigEventFmAdd(CJsonNode)), widget, SLOT(SlotEventFmAdd(CJsonNode)));
	connect(m_pObsMgr, SIGNAL(SigEventFmDel(CJsonNode)), widget, SLOT(SlotEventFmDel(CJsonNode)));
	connect(m_pObsMgr, SIGNAL(SigEventFmSet(CJsonNode)), widget, SLOT(SlotEventFmSet(CJsonNode)));

}

void MainWindow::DoDabRadioHome()
{
	DABRadioWindow *widget = new DABRadioWindow(this, m_strAddr);
	widget->AddWidgetDABRadioHome();
	SlotAddWidget(widget, STR_DAB_RADIO);
	widget->RequestList();

	connect(m_pObsMgr, SIGNAL(SigEventDabSeek(CJsonNode)), widget, SLOT(SlotEventDabSeek(CJsonNode)));
	connect(m_pObsMgr, SIGNAL(SigEventDabSeekStop(CJsonNode)), widget, SLOT(SlotEventDabSeekStop(CJsonNode)));
	connect(m_pObsMgr, SIGNAL(SigEventDabDel(CJsonNode)), widget, SLOT(SlotEventDabDel(CJsonNode)));
	connect(m_pObsMgr, SIGNAL(SigEventDabSet(CJsonNode)), widget, SLOT(SlotEventDabSet(CJsonNode)));

}

void MainWindow::DoGroupPlayHome()
{
	GroupPlayWindow *widget = new GroupPlayWindow(this, m_strAddr);
	SlotAddWidget(widget, STR_GROUP_PLAY);
	widget->GroupPlayList(m_EventID);

	// event
	connect(m_pObsMgr, SIGNAL(SigEventGroupPlayUpdate()), widget, SLOT(SlotEventGroupPlayUpdate()));

}

void MainWindow::DoSetupHome()
{
//	SetupWindow *widget = new SetupWindow(this, m_strAddr);
//	SlotAddWidget(widget, STR_SETUP);
//	widget->SetupHome(m_SetupList, m_EventID);

	QMessageBox::warning(this, STR_WARNING, STR_COMING_SOON);

}

void MainWindow::DoPowerOff()
{
	PowerOffDialog dialog;
	if (dialog.exec() == QDialog::Accepted)
	{
		int index = m_pDeviceMgr->CheckDeviceWol(m_strCurrentMac);

		bool bWol = dialog.GetBtnWol()->isChecked();
		if (bWol)
		{
			if (index < 0)
			{
				m_pDeviceMgr->AddDeviceWol(m_strCurrentMac, m_strVersion, m_strWolAddr, m_strUuid);
			}
		}
		else
		{
			if (index >= 0)
			{
				m_pDeviceMgr->DelDeviceWol(index);
			}
		}

		m_pAppMgr->RequestDevicePowerOff(bWol);

	}

}

void MainWindow::DoPowerOn()
{
	DeviceListWindow *widget = new DeviceListWindow;
	SlotAddWidget(widget, STR_POWER_ON);

	connect(widget, SIGNAL(SigSelectDevice(QString)), this, SLOT(SlotWolDevice(QString)));
	connect(widget, SIGNAL(SigSelectCancel(QString)), this, SLOT(SlotWolCancel(QString)));

	widget->SetTitle(STR_POWER_ON);
	widget->SetDeviceList(m_pDeviceMgr->GetDeviceListWol());

}

void MainWindow::DoAbout()
{
	AboutDialog dialog;
	dialog.exec();
}

void MainWindow::SlotAddWidget(QWidget *widget, QString title)
{
	auto idx = ui->stackMain->currentIndex();
	auto cnt = ui->stackMain->count() - 1;

	// remove the widget after the current wiget
	if (cnt > idx)
	{
		for (int i = cnt; i > idx; i--)
		{
			auto backWidget = ui->stackMain->widget(i);
			ui->stackMain->removeWidget(backWidget);
		}
	}

	idx = ui->stackMain->addWidget(widget);
	ui->stackMain->setCurrentIndex(idx);
	ui->widgetTop->AddTitle(title);

	UpdateStackState();
}

void MainWindow::SlotRemoveWidget(QWidget *widget)
{
	ui->stackMain->removeWidget(widget);
	ui->widgetTop->RemoveTitle();

	UpdateStackState();
}

void MainWindow::RemoveAllWidget()
{
	auto count = ui->stackMain->count();
	for (int i = 0; i < count; i++)
	{
		auto backWidget = ui->stackMain->widget(i);
		ui->stackMain->removeWidget(backWidget);
	}
}

void MainWindow::UpdateStackState()
{
	auto index = ui->stackMain->currentIndex();
	auto count = ui->stackMain->count();

	if (index < 0 || count <= 0)
	{
		return;
	}

	ui->widgetTop->SetTitle(index);

	// Update buttons depending on the page count.
	auto hasPage = count > 0;
	if (!hasPage)
	{
		ui->widgetTop->GetBtnNext()->setEnabled(false);
		ui->widgetTop->GetBtnPrev()->setEnabled(false);
		return;
	}

	// Update buttons depending on the current page index.
	auto isLastPage = index == count - 1;
	auto isFirstPage = index == 0;
	if (isLastPage && isFirstPage)
	{
		ui->widgetTop->GetBtnNext()->setEnabled(false);
		ui->widgetTop->GetBtnPrev()->setEnabled(false);
		return;
	}

	if (isLastPage)
	{
		ui->widgetTop->GetBtnNext()->setEnabled(false);
		ui->widgetTop->GetBtnPrev()->setEnabled(true);
		return;
	}

	if (isFirstPage)
	{
		ui->widgetTop->GetBtnNext()->setEnabled(true);
		ui->widgetTop->GetBtnPrev()->setEnabled(false);
		return;
	}

	ui->widgetTop->GetBtnNext()->setEnabled(true);
	ui->widgetTop->GetBtnPrev()->setEnabled(true);
}


