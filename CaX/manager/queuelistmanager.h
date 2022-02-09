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

	SQLManager *GetSqlMgr() const;
	void SetSqlMgr(SQLManager *pSql);

signals:

	void SigRespError(QString errMsg);
	void SigRespTrackInfo(CJsonNode node);

	void SigCoverArtUpdate(QString fileName, int nIndex, int mode);

private slots:

	void SlotRespInfo(QString json, int nCmdID);
	void SlotRespCoverArt(QString fileName, int nIndex, int mode);

private:

	enum {
		QUEUELIST_TRACK_INFO = 0,
		QUEUELIST_TRACK_PLAY,
		QUEUELIST_UPDATE_TRACK_FAVORITE,
		QUEUELIST_MAX
	};

	void ParseTrackInfo(CJsonNode result);

	SQLManager	*m_pSql;

};

#endif // QUEUELISTMANAGER_H
