#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

#include "util/CJsonNode.h"

class PlayManager;

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

protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotClickCoverArt();
	void SlotBtnInfo();
	void SlotBtnPlayPrev();
	void SlotBtnPlay();
	void SlotBtnPlayNext();
	void SlotBtnRandom();
	void SlotBtnDevice();
	void SlotBtnVolume();
	void SlotSliderReleased();
	void SlotSliderUpdate();

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
	void InitUI();
	void EnableUI(bool bEnable);

	void SetTimer(bool bStart);
	void SetPlayState(QString state);
	void SetRepeatMode(QString mode);
	void SetCoverArt(QString filepath);
	void SetQueueList(uint timestamp);
	void SetSliderState();

	void DoNowPlay(CJsonNode node);

	PlayManager		*m_pMgr;

	QTimer				*m_Timer;
	int					m_TotTime;
	int					m_CurTime;

	int					m_ID;
	bool				m_bPlay;
	int					m_nRepeatMode;

	QString				m_CoverArt;

	Ui::PlayWindow *ui;
};

#endif // PLAYWINDOW_H
