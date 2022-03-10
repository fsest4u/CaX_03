#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QMenu>

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

signals:

	void SigAddQueueList(CJsonNode node);
	void SigSetPlayInfo(CJsonNode node, bool show);
	void SigRemoveQueueList();

private slots:

	void SlotClickCoverArt(int index);
	void SlotBtnInfo();
	void SlotBtnPlayPrev();
	void SlotBtnPlay();
	void SlotBtnStop();
	void SlotBtnPlayNext();
	void SlotBtnRandom();
	void SlotBtnVolume();
	void SlotVolumeIncrease(int value);
	void SlotVolumeDecrease(int value);
	void SlotVolumeSliderValueChanged(int value);
	void SlotVolumeSliderSet(int value);
	void SlotPlayTimeSliderValueChanged(int value);
	void SlotPlayTimeSliderReleased();
	void SlotPlayTimeSliderUpdate();

	void SlotEventNowPlay(CJsonNode node);
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
	void InitVariable();
	void SetVariable(CJsonNode node);
	void DebugVariable();

	void InitPlayInfo();
	void InitPlayTimeSlider();
	void EnableUI(bool bEnable);

	void EnableBtnInfo(bool bEnable);
	void EnableBtnNext(bool bEnable);
	void EnableBtnPlay(bool bEnable);
	void EnableBtnPrev(bool bEnable);
	void EnableBtnRandom(bool bEnable);
	void EnableBtnSlider(bool bEnable);
	void EnableBtnStop(bool bEnable);

	void SetTimer(bool bStart);
	void SetPlayState();
	void SetRepeatMode(QString mode);
	void SetRecordable();
	void SetCoverArt(QString filepath);
	void SetPlayTimeSliderState();

	void DoNowPlay(CJsonNode node);

	PlayManager			*m_pMgr;
	FormCoverArt		*m_pFormCoverArt;
	FormTitle			*m_pFormTitle;

	QMenu				*m_VolumeMenu;
	QTimer				*m_Timer;

	CJsonNode			m_PlayInfo;

	bool	m_Info;
	bool	m_List;
	bool	m_Mute;
	bool	m_Next;
	bool	m_PlayPause;
	bool	m_Prev;
	bool	m_Program;
	bool	m_Record;
	bool	m_Recordable;
	bool	m_Seek;
	bool	m_Swap;

	int		m_Duration;
	int		m_ID;
	int		m_PlayTime;
	int		m_SampleRate;
	int		m_Volume;
	int64_t	m_TimeStamp;

	QString m_Bot;
	QString	m_CoverArt;
	QString m_Format;
	QString m_Input;
	QString m_PlayState;
	QString m_Repeat;
	QString m_Src;
	QString m_Top;
	QString m_Mqa;

	bool				m_bPause;
	int					m_nRepeatMode;



	Ui::PlayWindow *ui;
};

#endif // PLAYWINDOW_H
