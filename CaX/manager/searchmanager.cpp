#include "searchmanager.h"
#include "sqlmanager.h"

SearchManager::SearchManager() :
	m_pSql(new SQLManager)
{
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespInfo(QString, int)));
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespCoverArt(QString, int, int)), this, SLOT(SlotRespCoverArt(QString, int, int)));

}

SearchManager::~SearchManager()
{
	if (m_pSql)
	{
		delete m_pSql;
		m_pSql = nullptr;
	}
}

void SearchManager::RequestSearchAlbumList(QString keyword)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, m_pSql->GetQuerySearchList(SQLManager::CATEGORY_ALBUM, keyword));

	RequestCommand(node, SEARCH_ALBUM);
}

void SearchManager::RequestSearchArtistList(QString keyword)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, m_pSql->GetQuerySearchList(SQLManager::CATEGORY_ARTIST, keyword));

	RequestCommand(node, SEARCH_ARTIST);
}

void SearchManager::RequestSearchTrackList(QString keyword)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, m_pSql->GetQuerySearchList(SQLManager::CATEGORY_TRACK, keyword));

	RequestCommand(node, SEARCH_TRACK);
}

void SearchManager::RequestPlayTrack(QMap<int, bool> idMap, int nWhere)
{
	CJsonNode idArr(JSON_ARRAY);
	QMap<int, bool>::iterator i;
	for (i = idMap.begin(); i!= idMap.end(); i++)
	{
		LogDebug("key [%d] value [%d]", i.key(), i.value());
		idArr.AppendArray((int64_t)i.key());
	}

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_IDS, idArr);
	node.Add(KEY_CMD0, VAL_MUSIC_DB);
	node.Add(KEY_CMD1, VAL_PLAY);
	node.Add(KEY_CMD2, VAL_SONG);
	if (nWhere != PLAY_NONE)
	{
		node.AddInt	(KEY_WHERE,		nWhere);
	}
	RequestCommand(node, SEARCH_PLAY_TRACK);
}

SQLManager *SearchManager::GetSqlMgr() const
{
	return m_pSql;
}

void SearchManager::SlotRespInfo(QString json, int cmdID)
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

	LogDebug("cmdID [%d] node [%s]", cmdID, node.ToTabedByteArray().data());

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
		emit SigRespError(message.left(MSG_LIMIT_COUNT));
		return;
	}

	switch (cmdID)
	{
	case SEARCH_ALBUM:
		ParseAlbumList(result);
		break;
	case SEARCH_ARTIST:
		ParseArtistList(result);
		break;
	case SEARCH_TRACK:
		ParseTrackList(result);
		break;
	}
}

void SearchManager::SlotRespCoverArt(QString fileName, int nIndex, int mode)
{
	emit SigCoverArtUpdate(fileName, nIndex, mode);
}

void SearchManager::ParseAlbumList(CJsonNode result)
{
	QList<CJsonNode> nodeList;
	for (int i = 0; i < result.ArraySize(); i++)
	{
		nodeList.append(result.GetArrayAt(i));
	}

	emit SigRespAlbumList(nodeList);
}

void SearchManager::ParseArtistList(CJsonNode result)
{
	QList<CJsonNode> nodeList;
	for (int i = 0; i < result.ArraySize(); i++)
	{
		nodeList.append(result.GetArrayAt(i));
	}

	emit SigRespArtistList(nodeList);
}

void SearchManager::ParseTrackList(CJsonNode result)
{
	QList<CJsonNode> nodeList;
	for (int i = 0; i < result.ArraySize(); i++)
	{
		nodeList.append(result.GetArrayAt(i));
	}

	emit SigRespTrackList(nodeList);
}
