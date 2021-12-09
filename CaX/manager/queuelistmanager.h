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

	void RequestCategoryInfo(int id);
	void RequestTrackPlay(int index);

	SQLManager *GetSqlMgr() const;
	void SetSqlMgr(SQLManager *pSql);

signals:

	void SigRespError(QString errMsg);
	void SigRespCategoryInfo(CJsonNode node);

	void SigCoverArtUpdate(QString fileName, int nIndex, int mode);

private slots:

	void SlotRespInfo(QString json, int nCmdID);
	void SlotRespCoverArt(QString fileName, int nIndex, int mode);

private:

	enum {
		QUEUELIST_CATEGORY_INFO = 0,
		QUEUELIST_TRACK_PLAY,
		QUEUELIST_MAX
	};

	void ParseCategoryInfo(CJsonNode result);

	SQLManager	*m_pSql;

};

#endif // QUEUELISTMANAGER_H
