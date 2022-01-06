#include "playlistmanager.h"
#include "sqlmanager.h"

#include "util/utilnovatron.h"

PlaylistManager::PlaylistManager() :
	m_pSql(new SQLManager)
{
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespInfo(QString, int)));
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespCoverArt(QString, int, int)), this, SLOT(SlotRespCoverArt(QString, int, int)));

}

PlaylistManager::~PlaylistManager()
{
	if (m_pSql)
	{
		delete m_pSql;
		m_pSql = nullptr;
	}
}

void PlaylistManager::RequestPlaylist()
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_SQL, m_pSql->GetQueryPlaylist());

	RequestCommand(node, PLAYLIST_LIST);
}

void PlaylistManager::RequestPlaylistInfo(int id)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_SQL, m_pSql->GetQueryPlaylistInfo(id));

	RequestCommand(node, PLAYLIST_INFO);
}

void PlaylistManager::RequestTrackList(int id)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_SQL, m_pSql->GetQueryPlaylistTrackList(id));

	RequestCommand(node, PLAYLIST_TRACK_LIST);
}

void PlaylistManager::RequestPlayTrack(QMap<int, bool> idMap, int nWhere)
{
	CJsonNode idArr(JSON_ARRAY);
	QMap<int, bool>::iterator i;
	for (i = idMap.begin(); i!= idMap.end(); i++)
	{
//		LogDebug("key [%d] value [%d]", i.key(), i.value());
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
	RequestCommand(node, PLAYLIST_PLAY_TRACK);
}

void PlaylistManager::RequestPlayPlaylist(QMap<int, bool> idMap, int nWhere)
{
	CJsonNode idArr(JSON_ARRAY);
	QMap<int, bool>::iterator i;
	for (i = idMap.begin(); i!= idMap.end(); i++)
	{
//		LogDebug("key [%d] value [%d]", i.key(), i.value());
		idArr.AppendArray((int64_t)i.key());
	}

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_IDS, idArr);
	node.Add(KEY_CMD0, VAL_PLAYLIST);
	node.Add(KEY_CMD1, VAL_PLAY);
	if (nWhere != PLAY_NONE)
	{
		node.AddInt	(KEY_WHERE,		nWhere);
	}
	RequestCommand(node, PLAYLIST_PLAY_PLAYLIST);
}

void PlaylistManager::RequestAddPlaylist(QString name)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0,		VAL_PLAYLIST);
	node.Add(KEY_CMD1,		VAL_NEW);
	node.Add(KEY_NAME_CAP,	name);

	RequestCommand(node, PLAYLIST_ADD_PLAYLIST);

}

void PlaylistManager::RequestRenamePlaylist(int id, QString name)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0,		VAL_PLAYLIST);
	node.Add(KEY_CMD1,		VAL_RENAME);
	node.AddInt(KEY_PLS_ID,	id);
	node.Add(KEY_NAME_CAP,	name);

	RequestCommand(node, PLAYLIST_RENAME_PLAYLIST);
}

void PlaylistManager::RequestDeletePlaylist(QMap<int, bool> idMap)
{
	CJsonNode idArr(JSON_ARRAY);
	QMap<int, bool>::iterator i;
	for (i = idMap.begin(); i!= idMap.end(); i++)
	{
//		LogDebug("key [%d] value [%d]", i.key(), i.value());
		idArr.AppendArray((int64_t)i.key());
	}

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_IDS, idArr);
	node.Add	(KEY_CMD0,					VAL_PLAYLIST);
	node.Add	(KEY_CMD1,					VAL_DEL);
	node.Add	(KEY_CMD2,					VAL_PLS);

	RequestCommand(node, PLAYLIST_DELETE_PLAYLIST);

}

void PlaylistManager::RequestAddCategoryFromPlaylist(int id, QMap<int, bool> idMap, int category)
{

	QString strCat = UtilNovatron::GetCategoryName(category);

	CJsonNode idArr(JSON_ARRAY);
	QMap<int, bool>::iterator i;
	for (i = idMap.begin(); i!= idMap.end(); i++)
	{
		idArr.AppendArray((int64_t)i.key());
	}

	CJsonNode orderInfo(JSON_OBJECT);
	orderInfo.Add(KEY_CATEGORY, strCat);
	orderInfo.AddInt(KEY_ORDER, 0);

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_PLAYLIST);
	node.Add(KEY_CMD1, VAL_ADD);
	node.Add(KEY_CMD2, VAL_CATEGORY);
	node.AddInt(KEY_PLS_ID, id);
	node.AddInt(KEY_SONG_ORDER, 0);
	node.Add(KEY_CAT_ORDER, orderInfo);
	node.Add(KEY_CATEGORY, strCat);
	node.Add(KEY_IDS, idArr);

	RequestCommand(node, PLAYLIST_ADD_CATEGORY_FROM_PLAYLIST);
}

void PlaylistManager::RequestAddTrackFromPlaylist(int id, QMap<int, bool> idMap)
{
	CJsonNode idArr(JSON_ARRAY);
	QMap<int, bool>::iterator i;
	for (i = idMap.begin(); i!= idMap.end(); i++)
	{
//		LogDebug("key [%d] value [%d]", i.key(), i.value());
		idArr.AppendArray((int64_t)i.key());
	}

	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,					VAL_PLAYLIST);
	node.Add	(KEY_CMD1,					VAL_ADD);
	node.Add	(KEY_CMD2,					VAL_SONG);
	node.AddInt(KEY_PLS_ID, id);
	node.AddInt(KEY_SONG_ORDER, 0);
	node.Add(KEY_IDS, idArr);

	RequestCommand(node, PLAYLIST_ADD_TRACK_FROM_PLAYLIST);
}


void PlaylistManager::RequestDelTrack(int id, QMap<int, bool> idMap)
{
	CJsonNode idArr(JSON_ARRAY);
	QMap<int, bool>::iterator i;
	for (i = idMap.begin(); i!= idMap.end(); i++)
	{
//		LogDebug("key [%d] value [%d]", i.key(), i.value());
		idArr.AppendArray((int64_t)i.key());
	}

	CJsonNode node(JSON_OBJECT);
	node.AddInt	(KEY_PLS_ID, id);
	node.Add(KEY_IDS, idArr);
	node.Add	(KEY_CMD0, VAL_PLAYLIST);
	node.Add	(KEY_CMD1, VAL_DEL);
	node.Add	(KEY_CMD2, VAL_SONG);

	RequestCommand(node, PLAYLIST_DEL_TRACK);

}

void PlaylistManager::RequestRandom()
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_REMOTE);
	node.Add(KEY_KEY, VAL_SHUFFLE);

	RequestCommand(node, PLAYLIST_RANDOM);
}

void PlaylistManager::SlotRespInfo(QString json, int cmdID)
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

//	LogDebug("cmdID [%d] node [%s]", cmdID, node.ToTabedByteArray().data());

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

	if (cmdID == PLAYLIST_PLAY_TRACK
			|| cmdID == PLAYLIST_PLAY_PLAYLIST
			|| cmdID == PLAYLIST_RANDOM
			|| cmdID == PLAYLIST_ADD_PLAYLIST
			|| cmdID == PLAYLIST_RENAME_PLAYLIST
			|| cmdID == PLAYLIST_DELETE_PLAYLIST
			|| cmdID == PLAYLIST_ADD_CATEGORY_FROM_PLAYLIST
			|| cmdID == PLAYLIST_ADD_TRACK_FROM_PLAYLIST
			)
	{
		return;
	}

	switch (cmdID)
	{
	case PLAYLIST_LIST:
	{
		CJsonNode result = node.GetArray(VAL_RESULT);
		if (result.ArraySize() <= 0)
		{
			emit SigRespError(message.left(MSG_LIMIT_COUNT));
			return;
		}

		ParsePlaylist(result);
	}
		break;
	case PLAYLIST_INFO:
	{
		CJsonNode result = node.GetArray(VAL_RESULT);
		if (result.ArraySize() <= 0)
		{
			emit SigRespError(message.left(MSG_LIMIT_COUNT));
			return;
		}

		m_Node.Clear();
		for (int i = 0; i < result.ArraySize(); i++)
		{
			m_Node = result.GetArrayAt(i);
//			LogDebug("node : [%s]", m_Node.ToCompactByteArray().data());
		}

		emit SigRespPlaylistInfo(m_Node);
	}
		break;
	case PLAYLIST_TRACK_LIST:
	{
		CJsonNode result = node.GetArray(VAL_RESULT);
		if (result.ArraySize() <= 0)
		{
			emit SigRespError(message.left(MSG_LIMIT_COUNT));
			return;
		}

		ParseTrackList(result);
	}
		break;
	case PLAYLIST_PLAY_TRACK:
		break;
	case PLAYLIST_DEL_TRACK:
		break;
	case PLAYLIST_RANDOM:
		break;
	}
}

void PlaylistManager::SlotRespCoverArt(QString fileName, int nIndex, int mode)
{
	emit SigCoverArtUpdate(fileName, nIndex, mode);
}

void PlaylistManager::ParsePlaylist(CJsonNode result)
{
	m_NodeList.clear();
	for (int i = 0; i < result.ArraySize(); i++)
	{
		m_NodeList.append(result.GetArrayAt(i));
//		LogDebug("node : [%s]", m_NodeList[i].ToCompactByteArray().data());
	}

	emit SigRespPlaylist(m_NodeList);
}

void PlaylistManager::ParseTrackList(CJsonNode result)
{
	m_NodeList.clear();
	for (int i = 0; i < result.ArraySize(); i++)
	{
		m_NodeList.append(result.GetArrayAt(i));
//		LogDebug("node : [%s]", m_NodeList[i].ToCompactByteArray().data());
	}

	emit SigRespTrackList(m_NodeList);
}
