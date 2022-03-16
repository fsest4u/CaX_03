#ifndef QUEUELISTMANAGER_H
#define QUEUELISTMANAGER_H

#include <QObject>

#include "basemanager.h"
#include "sqlmanager.h"

class SQLManager;

class QueuelistManager : public BaseManager
{
	Q_OBJECT
public:
	QueuelistManager();
	~QueuelistManager();

	void RequestTrackInfo(int id);
	void RequestTrackPlay(int index);
	void RequestUpdateTrackFavorite(int id, int favorite);

	void RequestRadioPlay(int index, int eventID);
	void RequestRadioSet(QString name, int64_t freq, int index);
	void RequestRadioDelete(QMap<int, bool> idMap);
	void RequestRadioRecordSet(CJsonNode node);


	void RequestAddToPlaylist(int id, QMap<int, bool> idMap);
	void RequestQueueList(uint timestamp);
	void RequestDeletePlayQueue(int id, int eventID);


	SQLManager *GetSqlMgr() const;
	void SetSqlMgr(SQLManager *pSql);

signals:

	void SigRespError(QString errMsg);
	void SigRespTrackInfo(CJsonNode node);
	void SigRespQueueList(CJsonNode node);
	void SigRespDeleteQueue(CJsonNode node);
	void SigRespRecordSet(CJsonNode node);

	void SigCoverArtUpdate(QString fileName, int nIndex, int mode);

private slots:

	void SlotRespInfo(QString json, int nCmdID);
	void SlotRespCoverArt(QString fileName, int nIndex, int mode);

private:

	enum {
		QUEUELIST_TRACK_INFO = 0,
		QUEUELIST_TRACK_PLAY,
		QUEUELIST_UPDATE_TRACK_FAVORITE,
		QUEUELIST_RADIO_PLAY,
		QUEUELIST_ADD_TO_PLAYLIST,
		QUEUELIST_QUEUE_LIST,
		QUEUELIST_DELETE_PLAY_QUEUE,
		QUEUELIST_FM_SET,
		QUEUELIST_FM_DELETE,
		QUEUELIST_FM_RECORD_SET,
		QUEUELIST_MAX
	};

	void ParseTrackInfo(CJsonNode node);
	void ParseQueueList(CJsonNode node);
	void ParseDeleteQueue(CJsonNode node);
	void ParseRadioRecordSet(CJsonNode node);

	SQLManager	*m_pSql;

};

#endif // QUEUELISTMANAGER_H
