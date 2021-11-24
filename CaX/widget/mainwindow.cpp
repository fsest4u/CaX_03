//#include <QMouseEvent>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialog/poweroffdialog.h"
#include "dialog/aboutdialog.h"

#include "manager/devicemanager.h"
#include "manager/observermanager.h"
#include "manager/appmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
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
	m_strCurrentMac(""),
	m_strAddr(""),
	m_strVersion(""),
	m_strWolAddr(""),
	m_strUuid(""),
	m_bConnect(false),
	m_bAudioCD(false),
	m_bInput(false),
	m_bFMRadio(false),
	m_bGroupPlay(false),
//	m_bSigma(false),
	m_bScanDB(false),
	m_bIsDel(false),
	m_nEventID(-1),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ReadSettings();
	ConnectSigToSlot();
	Initialize();
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
		DoAudioCDHome();
		break;
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

}

void MainWindow::ObserverDisconnect()
{
	m_bConnect = false;
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

void MainWindow::SlotRespDeviceInfo(CJsonNode node)
{

	QString strMsg;
	bool bSuccess;

	if (!node.GetString(VAL_MSG, strMsg) || strMsg.isEmpty())
	{
		return;
	}
	if (!node.GetBool(VAL_SUCCESS, bSuccess))
	{
		return;
	}

	if (!bSuccess)
	{
		LogCritical("Error [%s]", strMsg.toUtf8().data());
		ObserverDisconnect();
		return;
	}
	else
	{
		LogDebug("node [%s]", node.ToCompactByteArray().data());

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
		LogDebug("length [%d]", m_strVersion.length());
		LogDebug("index [%d]", index);
		LogDebug("right [%s]", m_strVersion.right(m_strVersion.length() - index).toUtf8().data());

		int version = m_strVersion.right(m_strVersion.length() - index).toInt();
		if (version < 1720)
		{
			LogDebug("version [%d]", version);
			QMessageBox::warning(this, STR_WARNING, STR_UPDATE_FIRMWARE.arg(m_strVersion));
			return;
		}

		ObserverConnect();

	}
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
	int     nEventID = false;

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
	if (!node.GetInt(KEY_EVENT_ID, nEventID))
	{
		nEventID = -1;
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

	m_nEventID = nEventID;
	m_bAudioCD = bAudioCD;

	if (m_bAudioCD)
	{
		m_SideMenuMap.insert(SIDEMENU_AUDIO_CD, QString("%1 - %2").arg(STR_AUDIO_CD).arg(STR_AVAILABLE));
	}
	else
	{
		m_SideMenuMap.insert(SIDEMENU_AUDIO_CD, QString("%1 - %2").arg(STR_AUDIO_CD).arg(STR_NOT_AVAILABLE));
	}

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

//	emit SigTaskList();

	DoMusicDBHome();

}

void MainWindow::SlotSelectDevice(QString mac)
{
	if (mac.isEmpty())
	{
		return;
	}

//	if (m_bConnect)
//	{
//		ObserverDisconnect();
//	}

	QString strAddr = m_pDeviceMgr->GetDeviceValue(mac, DEVICE_ADDR);
	QString strDev = m_pDeviceMgr->GetDeviceValue(mac, DEVICE_DEV);

	if (strAddr.isEmpty())
		return;

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

	ObserverDisconnect();
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
	RemoveAllWidget();
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
	DoMusicDBHome();
}


void MainWindow::Initialize()
{
	RemoveAllWidget();
	DoDeviceListHome();

	m_pDeviceMgr->RequestDeviceInfo();

	UpdateStackState();

	m_IServiceList.clear();
	m_InputList.clear();
	m_SetupList.clear();

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
	connect(m_pAppMgr, SIGNAL(SigRespDeviceInfo(CJsonNode)), this, SLOT(SlotRespDeviceInfo(CJsonNode)));

	connect(m_pObsMgr, SIGNAL(SigDisconnectObserver()), this, SLOT(SlotDisconnectObserver()));
	connect(m_pObsMgr, SIGNAL(SigRespObserverInfo(CJsonNode)), this, SLOT(SlotRespObserverInfo(CJsonNode)));
	connect(m_pObsMgr, SIGNAL(SigRespNowPlay(CJsonNode)), ui->widgetPlay, SLOT(SlotRespNowPlay(CJsonNode)));

	connect(m_pDeviceWin, SIGNAL(SigSelectDevice(QString)), this, SLOT(SlotSelectDevice(QString)));
	connect(m_pDeviceWin, SIGNAL(SigSelectCancel(QString)), this, SLOT(SlotSelectCancel(QString)));

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
	MusicDBWindow *widget = new MusicDBWindow(this, m_strAddr, m_nEventID);
	widget->AddWidgetItem();
	SlotAddWidget(widget, STR_MUSIC_DB);
	widget->RequestCategoryList();
}

void MainWindow::DoAudioCDHome()
{
	AudioCDWindow *widget = new AudioCDWindow(this, m_strAddr, m_nEventID);
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
	BrowserWindow *widget = new BrowserWindow(this, m_strAddr, m_nEventID);
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
}

void MainWindow::DoDabRadioHome()
{
	DABRadioWindow *widget = new DABRadioWindow(this, m_strAddr);
	widget->AddWidgetDABRadioHome();
	SlotAddWidget(widget, STR_DAB_RADIO);
	widget->RequestList();
}

void MainWindow::DoGroupPlayHome()
{
	GroupPlayWindow *widget = new GroupPlayWindow(this, m_strAddr);
	SlotAddWidget(widget, STR_GROUP_PLAY);
	widget->GroupPlayList(m_nEventID);

	// event
	connect(m_pObsMgr, SIGNAL(SigRespGroupPlayUpdate()), widget, SLOT(SlotRespGroupPlayUpdate()));

}

void MainWindow::DoSetupHome()
{
//	SetupWindow *widget = new SetupWindow(this, m_strAddr);
//	SlotAddWidget(widget, STR_SETUP);
//	widget->SetupHome(m_SetupList, m_nEventID);

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
	auto idx = ui->stackMain->currentIndex();
	auto cnt = ui->stackMain->count();

	ui->widgetTop->SetTitle(idx);

	// Update buttons depending on the page count.
	auto hasPage = cnt > 0;
	if (!hasPage)
	{
		ui->widgetTop->GetBtnNext()->setEnabled(false);
		ui->widgetTop->GetBtnPrev()->setEnabled(false);
		return;
	}

	// Update buttons depending on the current page index.
	auto isLastPage = idx == cnt - 1;
	auto isFirstPage = idx == 0;
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


