#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialog/aboutdialog.h"
#include "dialog/commondialog.h"
#include "dialog/poweroffdialog.h"
#include "dialog/progressdialog.h"

#include "manager/devicemanager.h"
#include "manager/observermanager.h"
#include "manager/appmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/settingio.h"
#include "util/utilnovatron.h"

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
#include "widget/searchwindow.h"
#include "widget/setupwindow.h"
#include "widget/queuelistwindow.h"


const QString SETTINGS_GROUP = "MainWindow";

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	m_pDeviceMgr(new DeviceManager),
	m_pDeviceWin(new DeviceListWindow),
	m_pObsMgr(new ObserverManager),
	m_pAppMgr(new AppManager),
	m_ProgressDialog(new ProgressDialog),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QString appPath = QCoreApplication::applicationDirPath();
	QFile file(appPath + "/log.txt");
	if (file.exists())
	{
		file.remove();
	}

	ConnectSigToSlot();
	ReadSettings();
	Initialize();
	SlotMenu();

	DoDeviceListHome();
}

MainWindow::~MainWindow()
{
//	WriteSettings();

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

	if (m_pQueueWin)
	{
		delete m_pQueueWin;
		m_pQueueWin = nullptr;
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
	ui->widgetTop->ClearSideMenu();

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

//		if (m_bGroupPlay)
//		{
//			m_SideMenuMap.insert(SIDEMENU_GROUP_PLAY, STR_GROUP_PLAY);
//		}
	}
	else
	{
		m_SideMenuMap.insert(SIDEMENU_SELECT_DEVICE, STR_SELECT_DEVICE);
		m_SideMenuMap.insert(SIDEMENU_POWER_ON, STR_POWER_ON);
		m_SideMenuMap.insert(SIDEMENU_ABOUT, STR_ABOUT);
	}

	ui->widgetTop->SetSideMenu(m_SideMenuMap);
}

void MainWindow::SlotMenuAction(int menuID)
{
//	LogDebug("menu action [%d]", menuID);
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
			CommonDialog dialog(this, STR_WARNING, STR_INSERT_CD);
			dialog.exec();
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
//	case SIDEMENU_GROUP_PLAY:
//		DoGroupPlayHome();
//		break;
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

void MainWindow::SlotSearchKeyword(QString keyword)
{
	// save keyword

	DoSearchHome(keyword);
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
	m_bCBSearch = !m_bCBSearch;
	ui->widgetTop->ShowCBSearch(m_bCBSearch);
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

	LogDebug("*****************************************");
	LogDebug("mac read [%s]", m_strCurrentMac.toUtf8().data());
	LogDebug("*****************************************");


	// load wol list
	QString strNodeWol = settings.value("node_wol").toString();
	if (!strNodeWol.isEmpty())
	{
		CJsonNode nodeList;
		if (nodeList.SetContent(strNodeWol))
		{
//			LogDebug("ReadSettings [%s]", nodeList.ToCompactByteArray().data());
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

	if (!m_strCurrentMac.isEmpty())
	{
		settings.setValue("recent_device", m_strCurrentMac);
		LogDebug("*****************************************");
		LogDebug("mac write [%s]", m_strCurrentMac.toUtf8().data());
		LogDebug("*****************************************");
	}
	if (!m_strAddr.isEmpty())
	{
		settings.setValue("recent_addr", m_strAddr);
	}

	// save wol list
	CJsonNode nodeList = m_pDeviceMgr->GetDeviceListWol();
	if (!nodeList.IsNull() && nodeList.ArraySize() > 0)
	{
//		for (int i = 0; i < nodeList.ArraySize(); i++)
//		{
//			CJsonNode node = nodeList.GetArrayAt(i);
//			LogDebug("wol [%s]", node.ToCompactByteArray().data());
//		}
//		LogDebug("wol list [%s]", nodeList.ToCompactByteArray().data());
		settings.setValue("node_wol", nodeList.ToCompactString());
	}

	settings.endGroup();
}

void MainWindow::ObserverConnect()
{
	if (m_strCurrentMac.isEmpty())
		return;

	if (m_strAddr.isEmpty())
	{
		m_strAddr = m_pDeviceMgr->GetDeviceValue(m_strCurrentMac, DEVICE_ADDR);
		if (m_strAddr.isEmpty())
			return;
	}

	m_bConnect = true;
	ui->widgetTop->setDisabled(false);
	ui->widgetTop->GetBtnSearch()->setDisabled(false);
	ui->widgetPlay->setDisabled(false);

	m_pObsMgr->RequestObserverInfo(m_strAddr);

	SlotMenu();

	UtilNovatron::RemoveTempDirectory();
	UtilNovatron::CreateTempDirectory();

}

void MainWindow::ObserverDisconnect()
{
	ui->widgetTop->setDisabled(true);
	ui->widgetTop->GetBtnSearch()->setDisabled(true);
	ui->widgetPlay->setDisabled(true);

	m_pObsMgr->RequestDisconnectObserver();
}

void MainWindow::SlotDeviceItem(int state)
{
	Q_UNUSED(state)

	m_pDeviceWin->SetDeviceList(m_pDeviceMgr->GetDeviceList());
}

void MainWindow::SlotAutoConnectDevice(QString mac, QString addr, QString val, QString dev)
{
	if (!m_strCurrentMac.compare(mac) && !m_bConnect)
	{
		SlotSelectDevice(mac, addr, val, dev);
	}
}

void MainWindow::SlotRespError(QString errMsg)
{
	CommonDialog dialog(this, STR_CRITICAL, errMsg);
	dialog.exec();
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

//	int index = m_strVersion.lastIndexOf(".r") + QString(".r").length();
//	LogDebug("length [%d]", m_strVersion.length());
//	LogDebug("index [%d]", index);
//	LogDebug("right [%s]", m_strVersion.right(m_strVersion.length() - index).toUtf8().data());

	int version = m_strVersion.right(4).toInt();
	if (version < FIRMWARE_MIN_VERSION)
	{
		CommonDialog dialog(this, STR_WARNING, STR_UPDATE_FIRMWARE.arg(m_strVersion));
		dialog.ShowBtnOk(false);
		dialog.ShowBtnCancel(true);
		if (dialog.exec() == QDialog::Rejected)
		{
//			DoDeviceListHome();
		}

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
	int     eventID = -1;

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
//		eventID = -1;
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

	if (eventID >= 0)
	{
		m_EventID = eventID;
	}
	m_bAudioCD = bAudioCD;

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

	if (m_bAudioCD)
	{
		DoAudioCDHome();
	}
	else
	{
		DoMusicDBHome();
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

void MainWindow::SlotClickClose()
{
	emit SigEjectCD();
}

void MainWindow::SlotSelectDevice(QString mac, QString addr, QString val, QString dev)
{
	LogDebug("*****************************************");
	LogDebug("mac select [%s]", mac.toUtf8().data());
	LogDebug("*****************************************");

	if (mac.isEmpty())
	{
		return;
	}

	if (m_bConnect)
	{
		ObserverDisconnect();
	}

	Initialize();
	SlotMenu();

//	QString strAddr = m_pDeviceMgr->GetDeviceValue(mac, DEVICE_ADDR);
//	QString strDev = m_pDeviceMgr->GetDeviceValue(mac, DEVICE_DEV);

	if (addr.isEmpty())
	{
		return;
	}

	m_strAddr = addr;
	m_strCurrentMac = mac;

	WriteSettings();

	ui->widgetPlay->SetAddr(m_strAddr);
	ui->widgetTop->SetDeviceName(val);

	m_pAppMgr->SetAddr(m_strAddr);
	m_pAppMgr->RequestDeviceInfo();

}

void MainWindow::SlotSelectCancel(QString mac, QString addr, QString val, QString dev)
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

void MainWindow::SlotWolDevice(QString mac, QString addr, QString val, QString dev)
{
	LogDebug("*****************************************");
	LogDebug("mac select [%s]", mac.toUtf8().data());
	LogDebug("*****************************************");

	if (mac.isEmpty())
	{
//		LogWarning("mac is empty");
		return;
	}

	if (m_bConnect)
	{
		ObserverDisconnect();
	}

	Initialize();
	SlotMenu();

	QString strAddr = m_pDeviceMgr->GetDeviceValueWol(mac, DEVICE_WOL_ADDR);
	if (strAddr.isEmpty())
	{
//		LogWarning("strAddr is empty");
		return;
	}

	m_strAddr = strAddr;
	m_strCurrentMac = mac;

	WriteSettings();

	ui->widgetPlay->SetAddr(m_strAddr);

	m_pAppMgr->SetAddr(m_strAddr);

	m_pDeviceMgr->RequestDevicePowerOn(m_strAddr, m_strCurrentMac);
}

void MainWindow::SlotWolCancel(QString mac, QString addr, QString val, QString dev)
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

void MainWindow::SlotPowerOff(QList<CJsonNode> list)
{
	QStringList devices;
	foreach (CJsonNode node, list)
	{
//		LogDebug("node [%s]", node.ToCompactByteArray().data());
		devices.append(node.GetString(KEY_CA_NAME));
	}

	PowerOffDialog dialog;
	dialog.SetDevices(devices.join(','));
	if (dialog.exec() == QDialog::Accepted)
	{
		foreach (CJsonNode node, list)
		{
			QString mac = node.GetString(KEY_MAC_ADDR);
			QString addr = node.GetString(KEY_LOCATION);

			int indexWol = m_pDeviceMgr->CheckDeviceWol(mac);
			int indexDev = m_pDeviceMgr->CheckDevice(mac);

			bool bWol = dialog.GetIsWol();
			if (bWol)
			{
				if (indexWol < 0)
				{
					m_pDeviceMgr->AddDeviceWol(mac, addr);
				}
				if (indexDev >= 0)
				{
					m_pDeviceMgr->DelDevice(indexDev);
				}
			}
			else
			{
				if (indexWol >= 0)
				{
					m_pDeviceMgr->DelDeviceWol(indexWol);
				}
			}
//			m_pAppMgr->SetAddr(addr);
			m_pAppMgr->RequestDevicePowerOff(m_EventID, bWol);
//			m_pAppMgr->SetAddr(m_strAddr);
		}
	}
}

void MainWindow::SlotDevice()
{
	m_DeviceMap.clear();
	CJsonNode nodeList = m_pDeviceMgr->GetDeviceList();
	for (int i = 0; i < nodeList.ArraySize(); i++)
	{
		CJsonNode node = nodeList.GetArrayAt(i);
//		LogDebug("node [%s]", node.ToCompactByteArray().data());
		m_DeviceMap.insert(node.GetString(KEY_MAC), node.GetString(KEY_VAL));
	}

	ui->widgetTop->ClearDeviceMenu();
	if (m_DeviceMap.count() > 0)
	{
		ui->widgetTop->SetDeviceMenu(m_DeviceMap);
	}
}

void MainWindow::SlotDeviceAction(QString mac)
{
	QString addr = m_pDeviceMgr->GetDeviceValue(mac, DEVICE_ADDR);
	QString val = m_pDeviceMgr->GetDeviceValue(mac, DEVICE_VAL);
	QString dev = m_pDeviceMgr->GetDeviceValue(mac, DEVICE_DEV);

	SlotSelectDevice(mac, addr, val, dev);
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

void MainWindow::SlotAddQueueList(CJsonNode node)
{
	if (m_pQueueWin)
	{
		SlotRemoveWidget(m_pQueueWin);

		delete m_pQueueWin;
		m_pQueueWin = nullptr;
	}

	m_pQueueWin = new QueuelistWindow(this, m_strAddr, m_EventID);
	SlotAddWidget(m_pQueueWin, STR_NOW_PLAY);
	m_pQueueWin->SetNodeInfo(node);

	connect(m_pQueueWin, SIGNAL(SigUpdateTimeStamp(uint)), ui->widgetPlay, SLOT(SlotUpdateTimeStamp(uint)));
}

void MainWindow::SlotSetPlayInfo(CJsonNode node, bool show)
{
	m_NodePlay = node;
	if (m_pQueueWin)
	{
		m_pQueueWin->SetPlayInfo(m_NodePlay);
	}
	else
	{
		if (show)
		{
			m_pQueueWin = new QueuelistWindow(this, m_strAddr, m_EventID);
			SlotAddWidget(m_pQueueWin, STR_NOW_PLAY);
			m_pQueueWin->SetPlayInfo(m_NodePlay);

			connect(m_pQueueWin, SIGNAL(SigUpdateTimeStamp(uint)), ui->widgetPlay, SLOT(SlotUpdateTimeStamp(uint)));
		}
	}
}

void MainWindow::SlotUpdateFavoriteParent(int id, int favorite)
{
	Q_UNUSED(id)
	Q_UNUSED(favorite)

//	LogDebug("dummy function");
}

void MainWindow::SlotUpdateRatingParent(int id, int rating)
{
	Q_UNUSED(id)
	Q_UNUSED(rating)

//	LogDebug("dummy function");
}

void MainWindow::SlotBrowserPath(QString path)
{
	Q_UNUSED(path)

//	LogDebug("dummy function");
}

void MainWindow::SlotCopyHere(bool move, QString dstPath, QString path, int type)
{
	Q_UNUSED(move)
	Q_UNUSED(dstPath)
	Q_UNUSED(path)
	Q_UNUSED(type)

//	LogDebug("dummy function");
}

void MainWindow::Initialize()
{
	RemoveAllWidget();
	UpdateStackState();

	m_IServiceList.clear();
	m_InputList.clear();
	m_SetupList.clear();

//	m_strCurrentMac = "",
//	m_strAddr = "",
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

	m_bCBSearch = false;
	m_pQueueWin = nullptr;
}

void MainWindow::ConnectSigToSlot()
{
	ConnectForUI();
	ConnectForApp();
}

void MainWindow::ConnectForUI()
{
	connect(ui->widgetTop, SIGNAL(SigSideMenu()), this, SLOT(SlotMenu()));
	connect(ui->widgetTop, SIGNAL(SigSideMenuAction(int)), this, SLOT(SlotMenuAction(int)));
	connect(ui->widgetTop, SIGNAL(SigSearchKeyword(QString)), this, SLOT(SlotSearchKeyword(QString)));
	connect(ui->widgetTop->GetBtnHome(), SIGNAL(clicked()), this, SLOT(SlotBtnHome()));
	connect(ui->widgetTop->GetBtnPrev(), SIGNAL(clicked()), this, SLOT(SlotBtnPrev()));
	connect(ui->widgetTop->GetBtnNext(), SIGNAL(clicked()), this, SLOT(SlotBtnNext()));
	connect(ui->widgetTop, SIGNAL(SigDeviceMenu()), this, SLOT(SlotDevice()));
	connect(ui->widgetTop, SIGNAL(SigDeviceMenuAction(QString)), this, SLOT(SlotDeviceAction(QString)));
	connect(ui->widgetTop->GetBtnSearch(), SIGNAL(clicked()), this, SLOT(SlotBtnSearch()));

	connect((QObject*)ui->widgetPlay->GetManager(), SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(ui->widgetPlay, SIGNAL(SigAddQueueList(CJsonNode)), this, SLOT(SlotAddQueueList(CJsonNode)));
	connect(ui->widgetPlay, SIGNAL(SigSetPlayInfo(CJsonNode, bool)), this, SLOT(SlotSetPlayInfo(CJsonNode, bool)));
	connect(ui->widgetPlay, SIGNAL(SigRemoveQueueList()), this, SLOT(SlotRemoveQueueWidget()));

}

void MainWindow::ConnectForApp()
{
	connect(m_pDeviceMgr, SIGNAL(SigDeviceItem(int)), this, SLOT(SlotDeviceItem(int)));
	connect(m_pDeviceMgr, SIGNAL(SigAutoConnectDevice(QString, QString, QString, QString)), this, SLOT(SlotAutoConnectDevice(QString, QString, QString, QString)));
	connect(m_pAppMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pAppMgr, SIGNAL(SigRespTaskList(CJsonNode)), this, SLOT(SlotRespTaskList(CJsonNode)));
	connect(m_pAppMgr, SIGNAL(SigRespDeviceInfo(CJsonNode)), this, SLOT(SlotRespDeviceInfo(CJsonNode)));

	connect(m_pObsMgr, SIGNAL(SigDisconnectObserver()), this, SLOT(SlotDisconnectObserver()));
	connect(m_pObsMgr, SIGNAL(SigRespObserverInfo(CJsonNode)), this, SLOT(SlotRespObserverInfo(CJsonNode)));
	connect(m_pObsMgr, SIGNAL(SigEventProgress(CJsonNode)), this, SLOT(SlotEventProgress(CJsonNode)));
	connect(m_pObsMgr, SIGNAL(SigEventNowPlay(CJsonNode)), ui->widgetPlay, SLOT(SlotEventNowPlay(CJsonNode)));

	connect(m_pDeviceWin, SIGNAL(SigSelectDevice(QString, QString, QString, QString)), this, SLOT(SlotSelectDevice(QString, QString, QString, QString)));
	connect(m_pDeviceWin, SIGNAL(SigSelectCancel(QString, QString, QString, QString)), this, SLOT(SlotSelectCancel(QString, QString, QString, QString)));

	connect(m_ProgressDialog, SIGNAL(SigClickBack(int)), this, SLOT(SlotClickBack(int)));
	connect(m_ProgressDialog, SIGNAL(SigClickStop(int)), this, SLOT(SlotClickStop(int)));
	connect(m_ProgressDialog, SIGNAL(SigClickSkip(int)), this, SLOT(SlotClickSkip(int)));
	connect(m_ProgressDialog, SIGNAL(SigClickClose()), this, SLOT(SlotClickClose()));
}

void MainWindow::DoDeviceListHome()
{

//	m_pDeviceMgr->ClearDeviceList();
//	m_pDeviceMgr->ClearDeviceListWol();
	m_pDeviceMgr->RequestDeviceList();

//	ui->widgetTop->setDisabled(true);
	ui->widgetTop->GetBtnSearch()->setDisabled(true);
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

	connect(this, SIGNAL(SigEjectCD()), widget, SLOT(SlotEjectCD()));
}

void MainWindow::DoPlaylistHome()
{
	UtilNovatron::RemoveFilesInTempDirectory(KEY_PLS);

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
	IServiceWindow *widget = new IServiceWindow(this, m_strAddr, m_EventID);
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
	FMRadioWindow *widget = new FMRadioWindow(this, m_strAddr, m_EventID);
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
	GroupPlayWindow *widget = new GroupPlayWindow(this, m_strAddr, m_EventID);
	SlotAddWidget(widget, STR_GROUP_PLAY);
	widget->GroupPlayList();

	// event
	connect(m_pObsMgr, SIGNAL(SigEventGroupPlayUpdate()), widget, SLOT(SlotEventGroupPlayUpdate()));
	connect(widget, SIGNAL(SigPowerOff(QList<CJsonNode>)), this, SLOT(SlotPowerOff(QList<CJsonNode>)));

}

void MainWindow::DoSetupHome()
{
//	CommonDialog dialog(this, STR_WARNING, STR_COMING_SOON);
//	dialog.exec();

	SetupWindow *widget = new SetupWindow(this, m_strAddr);
	SlotAddWidget(widget, STR_SETUP);
	widget->SetupHome(m_SetupList, m_EventID);
}

void MainWindow::DoPowerOff()
{
	PowerOffDialog dialog;
	if (dialog.exec() == QDialog::Accepted)
	{
		int indexWol = m_pDeviceMgr->CheckDeviceWol(m_strCurrentMac);
		int indexDev = m_pDeviceMgr->CheckDevice(m_strCurrentMac);

		bool bWol = dialog.GetIsWol();
		if (bWol)
		{
			if (indexWol < 0)
			{
				m_pDeviceMgr->AddDeviceWol(m_strCurrentMac, m_strWolAddr, m_strVersion, m_strUuid);
			}
			if (indexDev >= 0)
			{
				m_pDeviceMgr->DelDevice(indexDev);
			}
		}
		else
		{
			if (indexWol >= 0)
			{
				m_pDeviceMgr->DelDeviceWol(indexWol);
			}
		}

//		m_pAppMgr->SetAddr(m_strWolAddr);
		m_pAppMgr->RequestDevicePowerOff(m_EventID, bWol);
//		m_pAppMgr->SetAddr(m_strAddr);
	}

}

void MainWindow::DoPowerOn()
{
	CJsonNode list = m_pDeviceMgr->GetDeviceListWol();
	if (list.ArraySize() <= 0)
	{
		CommonDialog dialog(this, STR_WARNING, STR_NO_DEVICES);
		dialog.exec();
	}
	else
	{
		DeviceListWindow *widget = new DeviceListWindow;
		SlotAddWidget(widget, STR_POWER_ON);

		connect(widget, SIGNAL(SigSelectDevice(QString, QString, QString, QString)), this, SLOT(SlotWolDevice(QString, QString, QString, QString)));
		connect(widget, SIGNAL(SigSelectCancel(QString, QString, QString, QString)), this, SLOT(SlotWolCancel(QString, QString, QString, QString)));

		widget->SetTitle(STR_POWER_ON);
		widget->SetDeviceList(list);
	}

}

void MainWindow::DoAbout()
{
	AboutDialog dialog;
	dialog.exec();
}

void MainWindow::DoSearchHome(QString keyword)
{
//	LogDebug("search [%s]", keyword.toUtf8().data());
	SearchWindow *widget = new SearchWindow(this, m_strAddr);
	SlotAddWidget(widget, STR_SEARCH);

	widget->SetKeyword(keyword);
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
	ui->widgetTop->AddTitle(idx, title);

	UpdateStackState();
}

void MainWindow::SlotRemoveWidget(QWidget *widget)
{
	ui->stackMain->removeWidget(widget);
	ui->widgetTop->RemoveTitle();

	UpdateStackState();
}

void MainWindow::SlotRemoveQueueWidget()
{
	if (m_pQueueWin)
	{
		SlotRemoveWidget(m_pQueueWin);

		delete m_pQueueWin;
		m_pQueueWin = nullptr;
	}
}

void MainWindow::RemoveAllWidget()
{
	auto count = ui->stackMain->count();
	for(int i = count; i >= 0; i--)
	{
		auto backWidget = ui->stackMain->widget(i);
		ui->stackMain->removeWidget(backWidget);
		ui->widgetTop->RemoveTitle();
	}
//	count = ui->stackMain->count();
//	LogDebug("remove all count [%d]", count);
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


