#ifndef TOPWINDOW_H
#define TOPWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QMenu>

namespace Ui {
class TopWindow;
}

class TopWindow : public QWidget
{
	Q_OBJECT

public:
	explicit TopWindow(QWidget *parent = nullptr);
	~TopWindow();

	void AddTitle(int index, QString title);
	void RemoveTitle();
	void SetTitle(int index);

	QPushButton* GetBtnSideMenu();
	QPushButton* GetBtnHome();
	QPushButton* GetBtnPrev();
	QPushButton* GetBtnNext();
	QPushButton* GetBtnSearch();

	void ClearSideMenu();
	void SetSideMenu(QMap<int, QString> map);

	void ClearDeviceMenu();
	void SetDeviceMenu(QMap<QString, QString> map);

	QString GetDeviceName() const;
	void SetDeviceName(const QString &DeviceName);

	void ShowCBSearch(bool show);
//	void ClearCBSearch();
//	void SetCBSearch(QStringList list);
//	QStringList GetCBSearch();


signals:

	void SigSideMenu();
	void SigSideMenuAction(int menuID);
	void SigDeviceMenu();
	void SigDeviceMenuAction(QString menuID);

	void SigSearchKeyword(QString keyword);

protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotSideMenu();
	void SlotSideMenuAction(QAction *action);
	void SlotDeviceMenu();
	void SlotDeviceMenuAction(QAction *action);

	void SlotSideMenuMusicDB();
	void SlotSideMenuAudioCD();
	void SlotSideMenuPlaylist();
	void SlotSideMenuBrowser();
	void SlotSideMenuIService();
	void SlotSideMenuInput();
	void SlotSideMenuFmRadio();
	void SlotSideMenuDabRadio();
	void SlotSideMenuGroupPlay();
	void SlotSideMenuSetup();
	void SlotSideMenuSelectDevice();
	void SlotSideMenuPowerOn();
	void SlotSideMenuPowerOff();
	void SlotSideMenuLanguage();
	void SlotSideMenuAbout();

private:

	void ConnectSigToSlot();
	void Initialize();

	QList<QString>	m_TitleList;

	QMenu			*m_SideMenu;
	QMenu			*m_DeviceMenu;

	QString			m_DeviceName;


	Ui::TopWindow *ui;
};

#endif // TOPWINDOW_H
