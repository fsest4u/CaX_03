#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "util/CJsonNode.h"

class DeviceManager;
class DeviceListWindow;

class ObserverManager;
class AppManager;

class ProgressDialog;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

public slots:

	void SlotMenu();
	void SlotMenuAction(int menuID);
	void SlotBtnHome();
	void SlotBtnPrev();
	void SlotBtnNext();
	void SlotBtnSearch();

	void SlotAddWidget(QWidget *widget, QString title);
	void SlotRemoveWidget(QWidget *widget);

	void SlotDeviceItem(int state);
	void SlotRespError(QString errMsg);
	void SlotRespTaskList(CJsonNode node);
	void SlotRespDeviceInfo(CJsonNode node);
	void SlotRespObserverInfo(CJsonNode node);

	void SlotEventProgress(CJsonNode node);
	void SlotClickBack(int taskID);
	void SlotClickStop(int taskID);
	void SlotClickSkip(int taskID);

	void SlotSelectDevice(QString mac);
	void SlotSelectCancel(QString mac);

	void SlotWolDevice(QString mac);
	void SlotWolCancel(QString mac);

	void SlotDevice();
	void SlotDeviceAction(QString menuID);

	void SlotDisconnectObserver();
	void SlotRespAirableLogout();

private:

	void ReadSettings();
	void WriteSettings();

	void ObserverConnect();
	void ObserverDisconnect();

	void Initialize();

	void ConnectSigToSlot();
	void ConnectForUI();
	void ConnectForApp();

	// side menu
	void DoDeviceListHome();
	void DoMusicDBHome();
	void DoAudioCDHome();
	void DoPlaylistHome();
	void DoBrowserHome();
	void DoIServiceHome();
	void DoInputHome();
	void DoFmRadioHome();
	void DoDabRadioHome();
	void DoGroupPlayHome();
	void DoSetupHome();
	void DoPowerOff();
	void DoPowerOn();
	void DoAbout();

	void RemoveAllWidget();
	void UpdateStackState();

	DeviceManager		*m_pDeviceMgr;
	DeviceListWindow	*m_pDeviceWin;

	ObserverManager	*m_pObsMgr;
	AppManager		*m_pAppMgr;

	ProgressDialog	*m_ProgressDialog;


	///////////////////////////////////////////////////////

	QString			m_strCurrentMac;
	QString			m_strAddr;

	QString			m_strVersion;
	QString			m_strWolAddr;
	QString			m_strUuid;

	bool			m_bConnect;

	bool            m_bAudioCD;
	bool			m_bInput;
	bool			m_bFMRadio;
	bool			m_bGroupPlay;
//	bool            m_bSigma;
	bool            m_bScanDB;
	bool            m_bIsDel;

	int				m_EventID;

	QList<CJsonNode> m_IServiceList;
	QList<CJsonNode> m_InputList;
	QList<CJsonNode> m_SetupList;
	QList<CJsonNode> m_TaskList;


	QMap<int, QString> m_SideMenuMap;
	QMap<QString, QString> m_DeviceMap;

	Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
