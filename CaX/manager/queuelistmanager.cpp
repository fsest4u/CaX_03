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

void QueuelistManager::RequestTrackInfo(int id)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, m_pSql->GetQueryQueueTrackInfo(id));

	RequestCommand(node, QUEUELIST_TRACK_INFO);
}

void QueuelistManager::RequestTrackPlay(int index)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_PLAY);
	node.Add(KEY_CMD1, VAL_INDEX);
	node.AddInt(KEY_INDEX, index);

	RequestCommand(node, QUEUELIST_TRACK_PLAY);
}

void QueuelistManager::RequestUpdateTrackFavorite(int id, int favorite)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_QUERY);
	node.Add	(KEY_CMD1,		VAL_SONG);
	node.Add	(KEY_SQL,		m_pSql->GetQueryUpdateTrackFavorite(id, favorite));
	RequestCommand(node, QUEUELIST_UPDATE_TRACK_FAVORITE);
}

void QueuelistManager::RequestRadioPlay(int index, int eventID)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_PLAY);
	node.Add(KEY_CMD1, VAL_FM_RADIO);
	node.AddInt(KEY_EVENT_ID, eventID);
	node.AddInt(KEY_INDEX, index);

	RequestCommand(node, QUEUELIST_RADIO_PLAY);
}

void QueuelistManager::RequestRadioSet(QString name, int64_t freq, int index)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_FM_RADIO);
	node.Add	(KEY_CMD1,		VAL_SET);
	node.Add	(KEY_NAME_CAP,	name);
	node.Add	(KEY_FREQ,		freq);
	node.AddInt	(KEY_INDEX,		index);

	RequestCommand(node, QUEUELIST_FM_SET);
}

void QueuelistManager::RequestRadioDelete(QMap<int, bool> idMap)
{
	CJsonNode idArr(JSON_ARRAY);
	QMap<int, bool>::iterator i;
	for (i = idMap.begin(); i!= idMap.end(); i++)
	{
//		LogDebug("key [%d] value [%d]", i.key(), i.value());
		idArr.AppendArray((int64_t)i.key());
	}

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_INDEXES, idArr);
	node.Add	(KEY_CMD0,		VAL_FM_RADIO);
	node.Add	(KEY_CMD1,		VAL_DEL);

	RequestCommand(node, QUEUELIST_FM_DELETE);
}

void QueuelistManager::RequestRadioRecordSet(CJsonNode node)
{
	node.Add(KEY_CMD0,		VAL_FM_RADIO);
	node.Add(KEY_CMD1,		VAL_RECORD_SET);

	RequestCommand(node, QUEUELIST_FM_RECORD_SET);
}

void QueuelistManager::RequestAddToPlaylist(int id, QMap<int, bool> idMap)
{
	CJsonNode idArr(JSON_ARRAY);
	QMap<int, bool>::iterator i;
	for (i = idMap.begin(); i!= idMap.end(); i++)
	{
		idArr.AppendArray((int64_t)i.key());
	}

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_PLAYLIST);
	node.Add(KEY_CMD1, VAL_ADD);
	node.AddInt(KEY_PLS_ID, id);
	node.AddInt(KEY_SONG_ORDER, 0);
	node.Add(KEY_CMD2, VAL_SONG);
	node.Add(KEY_IDS, idArr);

	RequestCommand(node, QUEUELIST_ADD_TO_PLAYLIST);
}

void QueuelistManager::RequestQueueList(uint timestamp, int start)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_PLAY);
	node.Add	(KEY_CMD1,		VAL_LIST);
	node.AddInt	(KEY_TIME_STAMP,timestamp);
	node.AddInt	(KEY_INDEX,		start);
	node.AddInt	(KEY_PAGE_CNT,	QUEUE_COUNT);

	RequestCommand(node, QUEUELIST_QUEUE_LIST);
}

void QueuelistManager::RequestDeletePlayQueue(int id, int eventID)
{
	QList<int> list;
	list.append(id);

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_PLAY);
	node.Add(KEY_CMD1, VAL_DEL);
	node.AddInt(KEY_EVENT_ID, eventID);
	node.Add(KEY_INDEXES, list);

	RequestCommand(node, QUEUELIST_DELETE_PLAY_QUEUE);
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
	if (json.isEmpty())
	{
		return;
	}

	CJsonNode node;
	if (!node.SetContent(json))
	{
		emit SigRespError(STR_INVALID_JSON);
		return;
	}

//	LogDebug("cmdID [%d] node [%s]", nCmdID, node.ToTabedByteArray().data());

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

	switch (nCmdID)
	{
	case QUEUELIST_TRACK_INFO:
		ParseTrackInfo(node);
		break;
	case QUEUELIST_QUEUE_LIST:
		ParseQueueList(node);
		break;
	case QUEUELIST_DELETE_PLAY_QUEUE:
		ParseDeleteQueue(node);
		break;
	case QUEUELIST_FM_RECORD_SET:
		ParseRadioRecordSet(node);
		break;
	case QUEUELIST_TRACK_PLAY:
	case QUEUELIST_RADIO_PLAY:
	case QUEUELIST_ADD_TO_PLAYLIST:
	case QUEUELIST_UPDATE_TRACK_FAVORITE:
	case QUEUELIST_FM_SET:
	case QUEUELIST_FM_DELETE:
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

void QueuelistManager::ParseTrackInfo(CJsonNode node)
{

	CJsonNode result = node.GetArray(VAL_RESULT);
	if (result.ArraySize() <= 0)
	{
//		emit SigRespError(message.left(MSG_LIMIT_COUNT));
		return;
	}

	CJsonNode tempNode = result.GetArrayAt(0);
	emit SigRespTrackInfo(tempNode);
}

void QueuelistManager::ParseQueueList(CJsonNode node)
{
	emit SigRespQueueList(node);
}

void QueuelistManager::ParseDeleteQueue(CJsonNode node)
{
	emit SigRespDeleteQueue(node);
}

void QueuelistManager::ParseRadioRecordSet(CJsonNode node)
{
	emit SigRespRecordSet(node);
}
