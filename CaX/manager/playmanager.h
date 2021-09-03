#ifndef PLAYMANAGER_H
#define PLAYMANAGER_H

#include <QObject>

#include "basemanager.h"

#include "util/CJsonNode.h"

#define	PLAY_NONE	1000


class PlayManager : public BaseManager
{
	Q_OBJECT
public:
	explicit PlayManager(QObject *parent = nullptr);
	~PlayManager();

	enum reqPlayIndex {
		PLAY_STATE = Qt::UserRole + PLAY_NONE,
		PLAY_VOLUME,
		PLAY_SEEK,
		PLAY_SHUFFLE,
		PLAY_MUTE,
		PLAY_SONG_INFO,
		PLAY_QUEUE_LIST,
		PLAY_QUEUE_PLAY,
		PLAY_MAX
	};

	enum playModeIndex {
		PLAY_MODE_UNKNOWN		= -1,
		PLAY_MODE_PLAY			= 0,
		PLAY_MODE_PAUSE			= 1,
		PLAY_MODE_NEXT			= 2,
		PLAY_MODE_PREV			= 3,
		PLAY_MODE_STOP			= 4,
		PLAY_MODE_RECORD		= 5,
		PLAY_MODE_MAX			= 10
	};

	void RequestPlayState(int mode);	// play, pause, next, prev, stop, record
	void RequestSeek(int msec);
	void RequestVolume(int value);
	void RequestRepeatMode();
	void RequestMute();
	void RequestRate();
	void RequestSongInfo(int nID);

	void RequestQueueList(uint timestamp);
	void RequestQueuePlay(int nID);



signals:

	void SigRespError(QString errMsg);
	void SigTrackInfo(CJsonNode node);
	void SigCoverArtUpdate(QString fileName);
	void SigQueueList(CJsonNode node);

private slots:

	void SlotRespInfo(QString json, int nCmdID);
	void SlotRespCoverArt(QString fileName);


private:

	void ParseTrackInfo(CJsonNode node);
	void ParseQueueList(CJsonNode node);

	QString GetPlayMode(int mode);

	int					m_nRateMode;

	QList<CJsonNode>	m_ListQueue;


};

#endif // PLAYMANAGER_H
