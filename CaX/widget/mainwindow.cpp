//#include <QMouseEvent>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialog/poweroffdialog.h"

#include "manager/devicemanager.h"
#include "manager/observermanager.h"
#include "manager/appmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"
#include "util/settingio.h"

#include "widget/sidemenu.h"
#include "widget/sidemenudelegate.h"
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
	m_pLoading(new Loading(this)),
	m_pSideMenu(new SideMenu(this)),
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

	LogDebug("Application Start.. [%s]", "Good!!");
	ConnectSigToSlot();
	ReadSettings();
	InitMain();
}

MainWindow::~MainWindow()
{
	delete ui;

	WriteSettings();

	if (m_pSideMenu)
	{
		delete m_pSideMenu;
		m_pSideMenu = nullptr;
	}

	if (m_pLoading)
	{
		delete m_pLoading;
		m_pLoading = nullptr;
	}


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

}

void MainWindow::SlotBtnMenu()
{
	if (m_pSideMenu->isHidden())
	{
//		LogDebug("show side menu");
		m_pSideMenu->SetEnableInput(m_bInput);
		m_pSideMenu->SetEnableFMRadio(m_bFMRadio);
		m_pSideMenu->SetEnableGroupPlay(m_bGroupPlay);

		m_pSideMenu->resize(ui->centralwidget->frameGeometry().width(), ui->centralwidget->frameGeometry().height());
		m_pSideMenu->move(mapToGlobal(ui->centralwidget->frameGeometry().topLeft()).x(), mapToGlobal(ui->centralwidget->frameGeometry().topLeft()).y());
		m_pSideMenu->ShowMenu();
	}
	else
	{
//		LogDebug("hide side menu");
		m_pSideMenu->HideMenu();
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

	m_pObsMgr->RequestObserverInfo(strAddr);
}

void MainWindow::ObserverDisconnect()
{
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
//	CloseNPMenu();

	QString strMsg;
	bool bSuccess;

	if (!node.GetString(VAL_MSG, strMsg) || strMsg.isEmpty()) { return; }
	if (!node.GetBool(VAL_SUCCESS, bSuccess)) { return; }

	if (!bSuccess)
	{
		LogCritical("Error [%s]", strMsg.toUtf8().data());
		ObserverDisconnect();
		return;
	}
	else
	{
		if (!node.GetString(DEVICE_MAC, m_strCurrentMac) || m_strCurrentMac.isEmpty()) { return; }
		if (!node.GetString(DEVICE_VERSION, m_strVersion) || m_strVersion.isEmpty()) { return; }
		if (!node.GetString(DEVICE_WOL_ADDR, m_strWolAddr) || m_strWolAddr.isEmpty()) { return; }
		if (!node.GetString(DEVICE_UUID, m_strUuid) || m_strUuid.isEmpty()) { return; }
		if (!node.GetBool(KEY_FM_RADIO, m_bFMRadio)) { return; }
		if (!node.GetBool(KEY_GROUP_PLAY, m_bGroupPlay)) { return; }
		if (!node.GetBool(KEY_INPUT, m_bInput)) { return; }

		m_bConnect = true;

		ObserverConnect();

		DoMusicDBHome();
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

	if (!node.GetBool(KEY_AUDIO_CD, bAudioCD)) { bAudioCD = false; }
	// todo-dylee
//	if (!node.GetBool(KEY_SIGMA, bSigma)) { bSigma = false; }
	if (!node.GetBool(KEY_SCAN_DB, bScanDB)) { bScanDB = false; }
	if (!node.GetBool(KEY_IS_DEL_DB, bIsDelDB)) { bIsDelDB = false; }
	if (!node.GetInt(KEY_EVENT_ID, nEventID)) { nEventID = -1; }
	if (!node.GetArray(KEY_SETUP, nodeSetup)) { nodeSetup.Clear(); }
	if (!node.GetArray(KEY_ISERVICE, nodeIService)) { nodeIService.Clear(); }
	if (!node.GetArray(KEY_INPUT, nodeInput)) { nodeInput.Clear(); }

//	m_bSigma = bSigma;
	m_bScanDB = bScanDB;
	m_bIsDel = bIsDelDB;

	m_nEventID = nEventID;
	m_bAudioCD = bAudioCD;
	m_pSideMenu->SetEnableAudioCD(m_bAudioCD);

	if (!nodeSetup.IsNull()) {
		m_SetUpList.clear();
		for(int i = 0; i < nodeSetup.ArraySize(); i++) {
			m_SetUpList.append(nodeSetup.GetArrayAt(i));
		}
	}
	if (!nodeIService.IsNull()) {
		m_IServiceList.clear();
		for(int i = 0; i < nodeIService.ArraySize(); i++) {
			m_IServiceList.append(nodeIService.GetArrayAt(i));
		}
	}
	if (!nodeInput.IsNull()) {
		m_InputList.clear();
		for(int i = 0; i < nodeInput.ArraySize(); i++) {
			m_InputList.append(nodeInput.GetArrayAt(i));
		}
	}

	// 최초 Response�"EventID"�받으� "Task" -> "List" Request르야 (EventID == 1)
	// todo-dylee
	//	emit SigTaskList();
}

void MainWindow::SlotSelectDevice(QString mac)
{
	if (mac.isEmpty())
		return;

	QString strAddr = m_pDeviceMgr->GetDeviceValue(mac, DEVICE_ADDR);
	if (strAddr.isEmpty())
		return;

	m_strAddr = strAddr;
	WriteSettings();

	ui->widgetPlay->SetAddr(m_strAddr);
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
	LogDebug("good wol device");
	if (mac.isEmpty())
		return;

	QString strAddr = m_pDeviceMgr->GetDeviceValueWol(mac, DEVICE_WOL_ADDR);
	if (strAddr.isEmpty())
		return;

	m_pDeviceMgr->RequestDevicePowerOn(strAddr, mac);
}

void MainWindow::SlotWolCancel(QString mac)
{
	LogDebug("good wol cancel");
}


void MainWindow::SlotDisconnectObserver()
{
	// exit application
	if (!m_bConnect)
	{
		return;
	}

	if (m_pDeviceMgr)
	{
		int index = m_pDeviceMgr->CheckDevice(m_strCurrentMac);
		if (index >= 0)
		{
			m_pDeviceMgr->DelDevice(index);
		}
	}

//	DoDeviceListHome();

}

void MainWindow::SlotSelectSideMenu(int menuIndex)
{
	m_pSideMenu->HideMenu();

	switch (menuIndex)
	{
	case SideMenuDelegate::SIDEMENU_SERVICE_MUSIC_DB:
		DoMusicDBHome();
		break;
	case SideMenuDelegate::SIDEMENU_SERVICE_AUDIO_CD:
		DoAudioCDHome();
		break;
	case SideMenuDelegate::SIDEMENU_SERVICE_PLAYLIST:
		DoPlaylistHome();
		break;
	case SideMenuDelegate::SIDEMENU_SERVICE_BROWSER:
		DoBrowserHome();
		break;
	case SideMenuDelegate::SIDEMENU_SERVICE_ISERVICE:
		DoIServiceHome();
		break;
	case SideMenuDelegate::SIDEMENU_SERVICE_INPUT:
		DoInputHome();
		break;
	case SideMenuDelegate::SIDEMENU_SERVICE_FM_RADIO:
		DoFmRadioHome();
		break;
	case SideMenuDelegate::SIDEMENU_SERVICE_DAB_RADIO:
		DoDabRadioHome();
		break;
	case SideMenuDelegate::SIDEMENU_SERVICE_GROUP_PLAY:
		DoGroupPlayHome();
		break;
	case SideMenuDelegate::SIDEMENU_SERVICE_SETUP:
		DoSetupHome();
		break;
	case SideMenuDelegate::SIDEMENU_SERVICE_POWER_OFF:
		DoPowerOff();
		break;
	case SideMenuDelegate::SIDEMENU_SERVICE_POWER_ON:
		DoPowerOn();
		break;
	default:
		break;
	}

}

void MainWindow::SlotRespAirableLogout()
{
//	RemoveAllWidget();
//	DoMusicDBHome();

	DoDeviceListHome();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{

//		if (obj == ui->widgetTop->GetBtnMenu())
//		{
//			SlotBtnMenu();
//		}
//		else if (obj == ui->widgetTop->GetBtnPrev())
//		{
//			SlotBtnPrev();
//		}
//		else if (obj == ui->widgetTop->GetBtnNext())
//		{
//			SlotBtnNext();
//		}
//		else if (obj == ui->widgetTop->GetBtnSearch())
//		{
//			SlotBtnSearch();
//		}

	}

	return QObject::eventFilter(obj, event);
}

void MainWindow::InitMain()
{
//	instead of MainWindow::ConnectForUI()
//	InitMenu(true);

	DoDeviceListHome();

	m_pDeviceMgr->RequestDeviceInfo();
	m_pSideMenu->HideMenu();

	UpdateStackState();

	m_IServiceList.clear();
	m_InputList.clear();
	m_SetUpList.clear();

}

void MainWindow::InitMenu(bool bEnable)
{

	if (bEnable)
	{
//		ui->widgetTop->GetBtnMenu()->installEventFilter(this);
//		ui->widgetTop->GetBtnPrev()->installEventFilter(this);
//		ui->widgetTop->GetBtnNext()->installEventFilter(this);
//		ui->widgetTop->GetBtnSearch()->installEventFilter(this);
	}
	else
	{
//		ui->widgetTop->GetBtnMenu()->removeEventFilter(this);
//		ui->widgetTop->GetBtnPrev()->removeEventFilter(this);
//		ui->widgetTop->GetBtnNext()->removeEventFilter(this);
//		ui->widgetTop->GetBtnSearch()->removeEventFilter(this);

	}
}

void MainWindow::ConnectSigToSlot()
{
	ConnectForUI();
	ConnectForApp();
}

void MainWindow::ConnectForUI()
{
	// top menu
	connect(ui->widgetTop->GetBtnMenu(), SIGNAL(clicked()), this, SLOT(SlotBtnMenu()));
	connect(ui->widgetTop->GetBtnPrev(), SIGNAL(clicked()), this, SLOT(SlotBtnPrev()));
	connect(ui->widgetTop->GetBtnNext(), SIGNAL(clicked()), this, SLOT(SlotBtnNext()));
	connect(ui->widgetTop->GetBtnSearch(), SIGNAL(clicked()), this, SLOT(SlotBtnSearch()));

	// side menu
	connect(m_pSideMenu, SIGNAL(SigSelectSideMenu(int)), this, SLOT(SlotSelectSideMenu(int)));

	// play menu
	connect((QObject*)ui->widgetPlay->GetManager(), SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));


}

void MainWindow::ConnectForApp()
{
	connect(m_pDeviceMgr, SIGNAL(SigDeviceItem(int)), this, SLOT(SlotDeviceItem(int)));
//	connect(m_pDeviceMenu, SIGNAL(SigDeviceSelect(QListWidgetItem*)), this,	SLOT(SlotDeviceSelect(QListWidgetItem*)));
	connect(m_pAppMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pAppMgr, SIGNAL(SigRespDeviceInfo(CJsonNode)), this, SLOT(SlotRespDeviceInfo(CJsonNode)));

	connect(m_pObsMgr, SIGNAL(SigDisconnectObserver()), this, SLOT(SlotDisconnectObserver()));
	connect(m_pObsMgr, SIGNAL(SigRespObserverInfo(CJsonNode)), this, SLOT(SlotRespObserverInfo(CJsonNode)));
	connect(m_pObsMgr, SIGNAL(SigRespNowPlay(CJsonNode)), ui->widgetPlay, SLOT(SlotRespNowPlay(CJsonNode)));

	connect(m_pDeviceWin, SIGNAL(SigSelectDevice(QString)), this, SLOT(SlotSelectDevice(QString)));
	connect(m_pDeviceWin, SIGNAL(SigSelectCancel(QString)), this, SLOT(SlotSelectCancel(QString)));

	connect(this, SIGNAL(SigPowerOffDevice(bool)), m_pAppMgr, SLOT(SlotPowerOffDevice(bool)));
}

void MainWindow::DoDeviceListHome()
{
	RemoveAllWidget();

	ui->widgetTop->SetMainTitle(tr("Select device"));
	m_pDeviceWin->SetTitle(tr("Select device"));
	SlotAddWidget(m_pDeviceWin);
}

void MainWindow::DoMusicDBHome()
{
	ui->widgetTop->SetMainTitle(tr("Music DB"));
	MusicDBWindow *widget = new MusicDBWindow(this, m_strAddr);
	SlotAddWidget(widget);
	widget->RequestMusicHome();
}

void MainWindow::DoAudioCDHome()
{
	ui->widgetTop->SetMainTitle(tr("Audio CD"));
	AudioCDWindow *widget = new AudioCDWindow(this, m_strAddr);
	SlotAddWidget(widget);
	widget->TrackList();
}

void MainWindow::DoPlaylistHome()
{
	ui->widgetTop->SetMainTitle(tr("Playlist"));
	PlaylistWindow *widget = new PlaylistWindow(this, m_strAddr);
	SlotAddWidget(widget);
	widget->PlayList();
}

void MainWindow::DoBrowserHome()
{
	ui->widgetTop->SetMainTitle(tr("Browser"));
	BrowserWindow *widget = new BrowserWindow(this, m_strAddr);
	SlotAddWidget(widget);
	widget->RequestRoot();


}

void MainWindow::DoIServiceHome()
{
	ui->widgetTop->SetMainTitle(tr("Internet service"));
	IServiceWindow *widget = new IServiceWindow(this, m_strAddr);
	SlotAddWidget(widget);
	widget->IServiceHome(m_IServiceList);
}

void MainWindow::DoInputHome()
{
	ui->widgetTop->SetMainTitle(tr("Input"));
	InputWindow *widget = new InputWindow(this, m_strAddr);
	SlotAddWidget(widget);
	widget->InputHome(m_InputList);
}

void MainWindow::DoFmRadioHome()
{
	ui->widgetTop->SetMainTitle(tr("FM radio"));
	FMRadioWindow *widget = new FMRadioWindow(this, m_strAddr);
	SlotAddWidget(widget);
	widget->RequestList();
}

void MainWindow::DoDabRadioHome()
{
	ui->widgetTop->SetMainTitle(tr("DAB radio"));
	DABRadioWindow *widget = new DABRadioWindow(this, m_strAddr);
	SlotAddWidget(widget);
	widget->RequestList();
}

void MainWindow::DoGroupPlayHome()
{
	ui->widgetTop->SetMainTitle(tr("Group play"));
	GroupPlayWindow *widget = new GroupPlayWindow;
	SlotAddWidget(widget);


}

void MainWindow::DoSetupHome()
{
	ui->widgetTop->SetMainTitle(tr("Setup"));
	SetupWindow *widget = new SetupWindow;
	SlotAddWidget(widget);
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

		emit SigPowerOffDevice(bWol);

		ObserverDisconnect();
	}

}

void MainWindow::DoPowerOn()
{
	ui->widgetTop->SetMainTitle(tr("Power on"));
	DeviceListWindow *widget = new DeviceListWindow;
	SlotAddWidget(widget);

	connect(widget, SIGNAL(SigSelectDevice(QString)), this, SLOT(SlotWolDevice(QString)));
	connect(widget, SIGNAL(SigSelectCancel(QString)), this, SLOT(SlotWolCancel(QString)));

	widget->SetTitle(tr("Power on"));
	widget->SetDeviceList(m_pDeviceMgr->GetDeviceListWol());
}

void MainWindow::SlotAddWidget(QWidget *widget)
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

	UpdateStackState();
}

void MainWindow::SlotRemoveWidget(QWidget *widget)
{
	ui->stackMain->removeWidget(widget);
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
