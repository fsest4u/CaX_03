#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QMenu>
#include <QSlider>

#include "util/CJsonNode.h"

class PlayManager;
class FormCoverArt;
class FormTitle;

namespace Ui {
class PlayWindow;
}

class PlayWindow : public QWidget
{
	Q_OBJECT

public:
	explicit PlayWindow(QWidget *parent = nullptr);
	~PlayWindow();

	void SetAddr(const QString &addr);

	PlayManager *GetManager() {	return m_pMgr;	}

	QString GetDeviceName() const;
	void SetDeviceName(const QString &DeviceName);

	void ClearMenu();
	void SetMenu(QMap<QString, QString> map);

signals:

	void SigMenu();
	void SigMenuAction(QString menuID);

	void SigSetVolumeSlider(int volume);

private slots:

	void SlotClickCoverArt();
	void SlotBtnInfo();
	void SlotBtnPlayPrev();
	void SlotBtnPlay();
	void SlotBtnStop();
	void SlotBtnPlayNext();
	void SlotBtnRandom();
	void SlotMenu();
	void SlotMenuAction(QAction *action);
	void SlotVolumeSliderValueChanged(int value);
	void SlotVolumeSliderReleased();
	void SlotSetVolumeSlider(int volume);
	void SlotPlayTimeSliderValueChanged(int value);
	void SlotPlayTimeSliderReleased();
	void SlotPlayTimeSliderUpdate();

	void SlotRespNowPlay(CJsonNode node);
	void SlotTrackInfo(CJsonNode node);
	void SlotCoverArtUpdate(QString fileName);
	void SlotQueueList(CJsonNode node);

private:

	enum {
		PLAY_REPEAT_NORMAL = 0,
		PLAY_REPEAT_RANDOM,
		PLAY_REPEAT_ONCE,
		PLAY_REPEAT_ONE,
		PLAY_REPEAT_ALL,
		PLAY_REPEAT_SHFFLE,
		PLAY_REPEAT_MAX
	};

	void ConnectSigToSlot();
	void Initialize();
	void InitPlayTimeSlider();
	void EnableUI(bool bEnable);

	void SetTimer(bool bStart);
	void SetPlayState();
	void SetRepeatMode(QString mode);
	void SetCoverArt(QString filepath);
	void SetQueueList(uint timestamp);
	void SetPlayTimeSliderState();
	void SetDeviceMenu();
	void SetVolumeMenu();

	void DoNowPlay(CJsonNode node);

	PlayManager			*m_pMgr;
	FormCoverArt		*m_pFormCoverArt;
	FormTitle			*m_pFormTitle;

	QMenu				*m_Menu;
	QMenu				*m_VolumeMenu;

	QSlider				*m_Slider;

	QTimer				*m_Timer;
	int					m_TotTime;
	int					m_CurTime;

	int					m_ID;
	bool				m_bPause;
	int					m_nRepeatMode;

	QString				m_CoverArt;
	QString				m_DeviceName;

	Ui::PlayWindow *ui;
};

#endif // PLAYWINDOW_H
