#include "queuelistmanager.h"

QueuelistManager::QueuelistManager() :
	m_pSql(new SQLManager)
{
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespInfo(QString, int)));
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespCoverArt(QString, int, int)), this, SLOT(SlotRespCoverArt(QString, int, int)));

}

QueuelistManager::~QueuelistManager()
{
	if (m_pSql)
	{
		delete m_pSql;
		m_pSql = nullptr;
	}
}

void QueuelistManager::RequestCategoryInfo(int id)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, m_pSql->GetQueryQueueCategoryInfo(id));

	RequestCommand(node, QUEUELIST_CATEGORY_INFO);
}

void QueuelistManager::RequestTrackPlay(int index)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_PLAY);
	node.Add(KEY_CMD1, VAL_INDEX);
	node.AddInt(KEY_INDEX, index);

	RequestCommand(node, QUEUELIST_TRACK_PLAY);
}

SQLManager *QueuelistManager::GetSqlMgr() const
{
	return m_pSql;
}

void QueuelistManager::SetSqlMgr(SQLManager *pSql)
{
	m_pSql = pSql;
}

void QueuelistManager::SlotRespInfo(QString json, int nCmdID)
{
	CJsonNode node;
	if (!node.SetContent(json))
	{
		emit SigRespError(STR_INVALID_JSON);
		return;
	}

	LogDebug("cmdID [%d] node [%s]", nCmdID, node.ToTabedByteArray().data());

	QString message = node.GetString(VAL_MSG);
	bool success = node.GetBool(VAL_SUCCESS);
	if (!success)
	{
		if (message.isEmpty())
		{
			emit SigRespError(STR_UNKNOWN_ERROR);
			return;
		}

		emit SigRespError(message.left((MSG_LIMIT_COUNT)));
		return;
	}

	CJsonNode result = node.GetArray(VAL_RESULT);
	if (result.ArraySize() <= 0)
	{
//		emit SigRespError(message.left(MSG_LIMIT_COUNT));
		return;
	}

	switch (nCmdID)
	{
	case QUEUELIST_CATEGORY_INFO:
		ParseCategoryInfo(result);
		break;
	case QUEUELIST_TRACK_PLAY:
		break;
	case QUEUELIST_MAX:
		emit SigRespError(STR_INVALID_ID);
		break;
	}
}

void QueuelistManager::SlotRespCoverArt(QString fileName, int nIndex, int mode)
{
	emit SigCoverArtUpdate(fileName, nIndex, mode);
}

void QueuelistManager::ParseCategoryInfo(CJsonNode result)
{
	CJsonNode node = result.GetArrayAt(0);
	emit SigRespCategoryInfo(node);
}
