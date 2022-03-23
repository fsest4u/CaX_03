#ifndef PLAYMANAGER_H
#define PLAYMANAGER_H

#include <QObject>

#include "basemanager.h"

#include "util/CJsonNode.h"


class PlayManager : public BaseManager
{
	Q_OBJECT
public:
	explicit PlayManager(QObject *parent = nullptr);
	~PlayManager();

	enum {
		PLAY_STATE = 0,
		PLAY_RECORD,
		PLAY_VOLUME,
		PLAY_SEEK,
		PLAY_SHUFFLE,
		PLAY_MUTE,
		PLAY_SONG_INFO,
		PLAY_QUEUE_LIST,
		PLAY_QUEUE_PLAY,
		PLAY_MAX
	};

	enum {
		PLAY_MODE_PLAY = 0,
		PLAY_MODE_PAUSE,
		PLAY_MODE_NEXT,
		PLAY_MODE_PREV,
		PLAY_MODE_STOP,
		PLAY_MODE_RECORD,
		PLAY_MODE_MAX
	};

	void RequestPlayState(int mode);	// play, pause, next, prev, stop
	void RequestRecord();	// record
	void RequestSeek(int msec);
	void RequestVolume(int value);
	void RequestRepeatMode();
	void RequestMute();
	void RequestRate();
	void RequestSongInfo(int nID);

	void RequestQueueList(uint timestamp, int start);
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
