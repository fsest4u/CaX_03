//#include <QMouseEvent>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "manager/ssdpmanager.h"
#include "manager/devicemanager.h"
#include "manager/observermanager.h"
#include "manager/appmanager.h"

#include "network/udpclient.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"
#include "util/settingio.h"

#include "widget/sidemenu.h"
#include "widget/sidemenudelegate.h"
#include "widget/mymusicwindow.h"
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

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, m_pLoading(new Loading(this))
	, m_pSideMenu(new SideMenu(this))
	, m_pSsdpMgr(new SSDPManager)
	, m_pDeviceMgr(new DeviceManager)
	, m_pObsMgr(new ObserverManager)
	, m_pAppMgr(new AppManager)
//	, m_pMusicWin(new MyMusicWindow)
//	, m_pCDWin(new AudioCDWindow)
//	, m_pPlayListWin(new PlaylistWindow)
//	, m_pBrowserWin(new BrowserWindow)
//	, m_pIServiceWin(new IServiceWindow)
//	, m_pInputWin(new InputWindow)
//	, m_pFMWin(new FMRadioWindow)
//	, m_pDABWin(new DABRadioWindow)
//	, m_pGroupWin(new GroupPlayWindow)
//	, m_pSetupWin(new SetupWindow)
	, m_strCurrentMac("")
	, m_strAddr("")
	, m_bConnect(false)
	, m_bAudioCD(false)
	, m_bInput(false)
	, m_bFMRadio(false)
	, m_bGroupPlay(false)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	LogDebug("Application Start.. [%s]", "Good!!");
	ConnectSigToSlot();
	InitMain();
}

MainWindow::~MainWindow()
{
	delete ui;

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

	if (m_pSsdpMgr)
	{
		delete m_pSsdpMgr;
		m_pSsdpMgr = nullptr;
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

//	if (m_pMusicWin)
//	{
//		delete m_pMusicWin;
//		m_pMusicWin = nullptr;
//	}

//	if (m_pCDWin)
//	{
//		delete m_pCDWin;
//		m_pCDWin = nullptr;
//	}

//	if (m_pPlayListWin)
//	{
//		delete m_pPlayListWin;
//		m_pPlayListWin = nullptr;
//	}

//	if (m_pBrowserWin)
//	{
//		delete m_pBrowserWin;
//		m_pBrowserWin = nullptr;
//	}

//	if (m_pIServiceWin)
//	{
//		delete m_pIServiceWin;
//		m_pIServiceWin = nullptr;
//	}

//	if (m_pInputWin)
//	{
//		delete m_pInputWin;
//		m_pInputWin = nullptr;
//	}

//	if (m_pFMWin)
//	{
//		delete m_pFMWin;
//		m_pFMWin = nullptr;
//	}

//	if (m_pDABWin)
//	{
//		delete m_pDABWin;
//		m_pDABWin = nullptr;
//	}

//	if (m_pGroupWin)
//	{
//		delete m_pGroupWin;
//		m_pGroupWin = nullptr;
//	}

//	if (m_pSetupWin)
//	{
//		delete m_pSetupWin;
//		m_pSetupWin = nullptr;
//	}
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

void MainWindow::SlotClickCoverArt()
{
	LogDebug("click label cover art");

}

void MainWindow::SlotBtnInfo()
{
	LogDebug("click btn info");

}

void MainWindow::SlotBtnPlaylist()
{
	LogDebug("click btn playlist");

}

void MainWindow::SlotBtnPlayPrev()
{
	LogDebug("click btn prev");

}

void MainWindow::SlotBtnPlay()
{
	LogDebug("click btn play");
}

void MainWindow::SlotBtnPlayNext()
{
	LogDebug("click btn next");

}

void MainWindow::SlotBtnRandom()
{
	LogDebug("click btn random");

}

void MainWindow::SlotBtnRepeat()
{
	LogDebug("click btn repeat");

}

void MainWindow::SlotBtnDevice()
{
	LogDebug("click btn device");

}

void MainWindow::SlotBtnVolume()
{
	LogDebug("click btn volume");

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

	// load wol list
	QString strNodeWol = settings.value("node_wol").toString();
	if (!strNodeWol.isEmpty())
	{
		CJsonNode nodeWol;
		if (nodeWol.SetContent(strNodeWol))
		{
			LogDebug("[%s]", nodeWol.ToTabedByteArray().data());
			m_pDeviceMgr->SetNodeWol(nodeWol);
		}
		else
		{
			LogCritical("Invalid JSON...");
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
	CJsonNode nodeWol = m_pDeviceMgr->GetNodeWol();
	if (!nodeWol.IsNull())
	{
		LogDebug("WriteSettings wol [%s]", nodeWol.ToCompactString().toUtf8().data());
		settings.setValue("node_wol", nodeWol.ToCompactString());
	}

	settings.endGroup();
}

void MainWindow::ObserverConnect()
{
	if (m_strCurrentMac.isEmpty())	return;

	QString strAddr = m_pDeviceMgr->GetDevValue(m_strCurrentMac, DEVICE_ADDR);
	if (strAddr.isEmpty())	return;

	m_pObsMgr->RequestObserverInfo(strAddr);
}

void MainWindow::ObserverDisconnect()
{
	m_pObsMgr->RequestDisconnectObserver();
}

void MainWindow::SetTitle(bool bEnable)
{
	QString devName = bEnable ? m_pDeviceMgr->GetDevValue(m_strCurrentMac, DEVICE_DEV) : QString("Select Device");
	QString title = QString("%1 [%2]").arg(TR_APPLICATION).arg(devName);
	setWindowTitle(title);
	ui->widgetTop->SetTitle(devName);
}

void MainWindow::SlotInitDeviceList(bool bSelect)
{
	LogDebug("Load Stop.........");
	m_pLoading->Stop();

	if (bSelect)
	{
		// temp-dylee auto connect
		m_strCurrentMac = "00:17:B8:10:05:3F";
		if (m_strCurrentMac.isEmpty())	return;

		QString strAddr = m_pDeviceMgr->GetDevValue(m_strCurrentMac, DEVICE_ADDR);
		if (strAddr.isEmpty())	return;

		m_strAddr = strAddr;
		WriteSettings();

		m_pAppMgr->SetAddr(m_strAddr);
		m_pAppMgr->RequestDeviceInfo();
	}
	else
	{
//		ResetMainMenu();
//		ResetContentWidget();


		// init ui....
	}

}

void MainWindow::SlotRespDeviceInfo(CJsonNode node)
{
//	CloseNPMenu();

	QString strMsg;
	bool bSuccess;

	if (!node.GetString(KEY_MSG, strMsg) || strMsg.isEmpty()) { return; }
	if (!node.GetBool(KEY_SUCCESS, bSuccess)) { return; }

	if (!bSuccess)
	{
		LogCritical("Error [%s]", strMsg.toUtf8().data());
		ObserverDisconnect();
//		ResetMainMenu();
//		ResetContentWidget();
		return;
	}
	else
	{
		QString strMac;
		QString strVersion;
		QString strWolAddr;
		QString strUUID;
//		bool bFMRadio;
//		bool bGroupPlay;
//		bool bInput;

		if (!node.GetString(DEVICE_MAC, strMac) || strMac.isEmpty()) { return; }
		if (!node.GetString(DEVICE_VERSION, strVersion) || strVersion.isEmpty()) { return; }
		if (!node.GetString(DEVICE_WOL_ADDR, strWolAddr) || strWolAddr.isEmpty()) { return; }
		if (!node.GetString(DEVICE_UUID, strUUID) || strUUID.isEmpty()) { return; }
		if (!node.GetBool(MAIN_MENU_FM_RADIO, m_bFMRadio)) { return; }
		if (!node.GetBool(MAIN_MENU_GROUP_PLAY, m_bGroupPlay)) { return; }
		if (!node.GetBool(MAIN_MENU_INPUT, m_bInput)) { return; }

		m_bConnect = true;
		m_strCurrentMac = strMac;

//		m_bFMRadio = bFMRadio;
//		m_bGroupPlay = bGroupPlay;
//		m_bInput = bInput;

		m_pDeviceMgr->AddWolDevice(strMac, strVersion, strWolAddr, strUUID);

		ObserverConnect();
//		InitMainMenu();
//		ResetContentWidget();

	}
}

void MainWindow::SlotRespObserverInfo(CJsonNode rootNode)
{
	CJsonNode nodeSetup;
	CJsonNode nodeIService;
	CJsonNode nodeInput;

	bool    bAudioCD = false;
	bool    bSigma = false;
	bool    bScanDB = false;
	bool    bIsDelDB = false;
	int     nEventID = false;

	if (!rootNode.GetBool(MAIN_MENU_AUDIO_CD, bAudioCD)) { bAudioCD = false; }
	// todo
//	if (!rootNode.GetBool(RES_KEY_SCAN_DB, bSigma)) { bSigma = false; }
//	if (!rootNode.GetBool(RES_KEY_SCAN_DB, bScanDB)) { bScanDB = false; }
//	if (!rootNode.GetBool(RES_KEY_SCAN_DB, bIsDelDB)) { bIsDelDB = false; }
	if (!rootNode.GetInt(MAIN_EVENT_ID, nEventID)) { nEventID = -1; }
	if (!rootNode.GetArray(MAIN_MENU_SETUP, nodeSetup)) { nodeSetup.Clear(); }
	if (!rootNode.GetArray(MAIN_MENU_ISERVICE, nodeIService)) { nodeIService.Clear(); }
	if (!rootNode.GetArray(MAIN_MENU_INPUT, nodeInput)) { nodeInput.Clear(); }

	m_bScanDB = bScanDB;
	m_nEventID = nEventID;
	m_bAudioCD = bAudioCD;
//	m_MainMenu->SetVisibleAudioCD(m_bAudioCD);

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

void MainWindow::SlotDisconnectObserver()
{
	// exit application
	if (!m_bConnect)	return;

	if (!m_pDeviceMgr)	return;

	m_pDeviceMgr->DelDevice(m_strCurrentMac);

//	ResetMainMenu();
	//	ResetContentWidget();
}

void MainWindow::SlotSelectSideMenu(int menuIndex)
{
	m_pSideMenu->HideMenu();

	switch (menuIndex)
	{
	case SideMenuDelegate::SIDE_MY_MUSIC:
		DoMyMusicHome();
		break;
	case SideMenuDelegate::SIDE_AUDIO_CD:
		DoAudioCDHome();
		break;
	case SideMenuDelegate::SIDE_PLAYLIST:
		DoPlaylistHome();
		break;
	case SideMenuDelegate::SIDE_BROWSER:
		DoBrowserHome();
		break;
	case SideMenuDelegate::SIDE_ISERVICE:
		DoIServiceHome();
		break;
	case SideMenuDelegate::SIDE_INPUT:
		DoInputHome();
		break;
	case SideMenuDelegate::SIDE_FM_RADIO:
		DoFmRadioHome();
		break;
	case SideMenuDelegate::SIDE_DAB_RADIO:
		DoDabRadioHome();
		break;
	case SideMenuDelegate::SIDE_GROUP_PLAY:
		DoGroupPlayHome();
		break;
	case SideMenuDelegate::SIDE_SETUP:
		DoSetupHome();
		break;
	case SideMenuDelegate::SIDE_POWER_OFF:
		break;
	default:
		break;
	}

}

void MainWindow::SlotCategoryInfo(int nID, int nCategory)
{
	MyMusicWindow *widget = new MyMusicWindow(this, m_strAddr);
	widget->RequestCategoryInfo(nID, nCategory);
	AddWidget(widget);

}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (obj == ui->widgetPlay->GetLabelCoverArt())
		{
			SlotClickCoverArt();
		}
//		else if (obj == ui->widgetTop->GetBtnMenu())
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
	InitMenu(true);

	m_pLoading->Start();
	m_pSsdpMgr->RequestDeviceInfo();

	m_pSideMenu->HideMenu();

	UpdateStackState();

}

void MainWindow::InitMenu(bool bEnable)
{

	if (bEnable)
	{
//		ui->widgetTop->GetBtnMenu()->installEventFilter(this);
//		ui->widgetTop->GetBtnPrev()->installEventFilter(this);
//		ui->widgetTop->GetBtnNext()->installEventFilter(this);
//		ui->widgetTop->GetBtnSearch()->installEventFilter(this);
		ui->widgetPlay->GetLabelCoverArt()->installEventFilter(this);
	}
	else
	{
//		ui->widgetTop->GetBtnMenu()->removeEventFilter(this);
//		ui->widgetTop->GetBtnPrev()->removeEventFilter(this);
//		ui->widgetTop->GetBtnNext()->removeEventFilter(this);
//		ui->widgetTop->GetBtnSearch()->removeEventFilter(this);
		ui->widgetPlay->GetLabelCoverArt()->removeEventFilter(this);

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
	connect(ui->widgetPlay->GetBtnInfo(), SIGNAL(clicked()), this, SLOT(SlotBtnInfo()));
	connect(ui->widgetPlay->GetBtnPlaylist(), SIGNAL(clicked()), this, SLOT(SlotBtnPlaylist()));
	connect(ui->widgetPlay->GetBtnPrev(), SIGNAL(clicked()), this, SLOT(SlotBtnPlayPrev()));
	connect(ui->widgetPlay->GetBtnPlay(), SIGNAL(clicked()), this, SLOT(SlotBtnPlay()));
	connect(ui->widgetPlay->GetBtnNext(), SIGNAL(clicked()), this, SLOT(SlotBtnPlayNext()));
	connect(ui->widgetPlay->GetBtnRandom(), SIGNAL(clicked()), this, SLOT(SlotBtnRandom()));
	connect(ui->widgetPlay->GetBtnRepeat(), SIGNAL(clicked()), this, SLOT(SlotBtnRepeat()));
	connect(ui->widgetPlay->GetBtnDevice(), SIGNAL(clicked()), this, SLOT(SlotBtnDevice()));
	connect(ui->widgetPlay->GetBtnVolume(), SIGNAL(clicked()), this, SLOT(SlotBtnVolume()));

}

void MainWindow::ConnectForApp()
{
	connect(m_pSsdpMgr->GetUdpClient(), SIGNAL(SigRespDeviceInfo(QString)),	m_pDeviceMgr, SLOT(SlotRespDeviceInfo(QString)));
	connect(m_pDeviceMgr, SIGNAL(SigInitDeviceList(bool)),	this, SLOT(SlotInitDeviceList(bool)));
//	connect(m_pDeviceMenu, SIGNAL(SigDeviceSelect(QListWidgetItem*)), this,	SLOT(SlotDeviceSelect(QListWidgetItem*)));
	connect(m_pAppMgr, SIGNAL(SigRespDeviceInfo(CJsonNode)), this, SLOT(SlotRespDeviceInfo(CJsonNode)));

	connect(m_pObsMgr, SIGNAL(SigRespObserverInfo(CJsonNode)), this, SLOT(SlotRespObserverInfo(CJsonNode)));
	connect(m_pObsMgr, SIGNAL(SigDisconnectObserver()), this, SLOT(SlotDisconnectObserver()));

}

void MainWindow::DoMyMusicHome()
{
	MyMusicWindow *widget = new MyMusicWindow(this, m_strAddr);
	widget->RequestMusicInfo();
	AddWidget(widget);

	connect(widget, SIGNAL(SigCategoryInfo(int, int)), this, SLOT(SlotCategoryInfo(int, int)));
}

void MainWindow::DoAudioCDHome()
{
	AudioCDWindow *widget = new AudioCDWindow;
	AddWidget(widget);


}

void MainWindow::DoPlaylistHome()
{
	PlaylistWindow *widget = new PlaylistWindow;
	AddWidget(widget);


}

void MainWindow::DoBrowserHome()
{
	BrowserWindow *widget = new BrowserWindow;
	AddWidget(widget);


}

void MainWindow::DoIServiceHome()
{
	IServiceWindow *widget = new IServiceWindow;
	AddWidget(widget);


}

void MainWindow::DoInputHome()
{
	InputWindow *widget = new InputWindow;
	AddWidget(widget);


}

void MainWindow::DoFmRadioHome()
{
	FMRadioWindow *widget = new FMRadioWindow;
	AddWidget(widget);


}

void MainWindow::DoDabRadioHome()
{
	DABRadioWindow *widget = new DABRadioWindow;
	AddWidget(widget);


}

void MainWindow::DoGroupPlayHome()
{
	GroupPlayWindow *widget = new GroupPlayWindow;
	AddWidget(widget);


}

void MainWindow::DoSetupHome()
{
	SetupWindow *widget = new SetupWindow;
	AddWidget(widget);


}

void MainWindow::AddWidget(QWidget *widget)
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
