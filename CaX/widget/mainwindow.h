#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "util/CJsonNode.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Loading;
class SideMenu;

class SSDPManager;
class DeviceManager;
class ObserverManager;

class AppManager;

//class MyMusicWindow;
//class AudioCDWindow;
//class PlaylistWindow;
//class BrowserWindow;
//class IServiceWindow;
//class InputWindow;
//class FMRadioWindow;
//class DABRadioWindow;
//class GroupPlayWindow;
//class SetupWindow;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

public slots:

	void SlotBtnMenu();
	void SlotBtnPrev();
	void SlotBtnNext();
	void SlotBtnSearch();

	void SlotClickCoverArt();
	void SlotBtnInfo();
	void SlotBtnPlaylist();
	void SlotBtnPlayPrev();
	void SlotBtnPlay();
	void SlotBtnPlayNext();
	void SlotBtnRandom();
	void SlotBtnRepeat();
	void SlotBtnDevice();
	void SlotBtnVolume();

	void SlotInitDeviceList(bool bSelect);
	void SlotRespError(QString errMsg);
	void SlotRespDeviceInfo(CJsonNode node);
	void SlotRespObserverInfo(CJsonNode rootNode);
	void SlotDisconnectObserver();

	void SlotSelectSideMenu(int menuIndex);

	void SlotCategoryInfo(int nID, int nCategory);



protected:

	bool eventFilter(QObject *obj, QEvent *event);

private:

	void ReadSettings();
	void WriteSettings();

	void ObserverConnect();
	void ObserverDisconnect();

	void SetTitle(bool bEnable);


	void InitMain();
	void InitMenu(bool bEnable);

	void ConnectSigToSlot();
	void ConnectForUI();
	void ConnectForApp();

	// side menu
	void DoMyMusicHome();
	void DoAudioCDHome();
	void DoPlaylistHome();
	void DoBrowserHome();
	void DoIServiceHome();
	void DoInputHome();
	void DoFmRadioHome();
	void DoDabRadioHome();
	void DoGroupPlayHome();
	void DoSetupHome();

	void AddWidget(QWidget *widget);

	void UpdateStackState();

	Loading			*m_pLoading;
	SideMenu		*m_pSideMenu;

	SSDPManager		*m_pSsdpMgr;
	DeviceManager	*m_pDeviceMgr;
	ObserverManager	*m_pObsMgr;

	AppManager		*m_pAppMgr;

//	MyMusicWindow	*m_pMusicWin;
//	AudioCDWindow	*m_pCDWin;
//	PlaylistWindow	*m_pPlayListWin;
//	BrowserWindow	*m_pBrowserWin;
//	IServiceWindow	*m_pIServiceWin;
//	InputWindow		*m_pInputWin;
//	FMRadioWindow	*m_pFMWin;
//	DABRadioWindow	*m_pDABWin;
//	GroupPlayWindow	*m_pGroupWin;
//	SetupWindow		*m_pSetupWin;

	///////////////////////////////////////////////////////

	QString			m_strCurrentMac;
	QString			m_strAddr;

	bool			m_bConnect;

	bool            m_bAudioCD;
	bool			m_bInput;
	bool			m_bFMRadio;
	bool			m_bGroupPlay;
	bool            m_bScanDB;

	int				m_nEventID;

	QList<CJsonNode> m_IServiceList;
	QList<CJsonNode> m_InputList;
	QList<CJsonNode> m_SetUpList;

	Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
