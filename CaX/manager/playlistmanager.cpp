#include "playlistmanager.h"
#include "sqlmanager.h"


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
	node.Add(KEY_SQL, m_pSql->GetQuerySongsOfPlaylist(id));

	RequestCommand(node, PLAYLIST_TRACK_LIST);
}

void PlaylistManager::RequestTrackPlay(int id)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_MUSIC_DB);
	node.Add(KEY_CMD1, VAL_PLAY);
	node.Add(KEY_CMD2, VAL_SONG);
	node.AddInt(KEY_WHERE, 0);
	node.AddInt(KEY_ID_UPPER, id);

	RequestCommand(node, PLAYLIST_PLAY_TRACK);
}

void PlaylistManager::RequestTracksPlay(QList<int> ids)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_MUSIC_DB);
	node.Add(KEY_CMD1, VAL_PLAY);
	node.Add(KEY_CMD2, VAL_SONG);
	node.AddInt(KEY_WHERE, 0);

	CJsonNode nodeIDS(JSON_ARRAY);
	foreach (int id, ids)
	{
		nodeIDS.AppendArray((int64_t)id);
	}

	node.Add(KEY_IDS, nodeIDS);

//	LogDebug("node [%s]", node.ToCompactByteArray().data());

	RequestCommand(node, PLAYLIST_PLAY_TRACKS);
}

void PlaylistManager::RequestPlaylistPlay(int id)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_PLAYLIST);
	node.Add(KEY_CMD1, VAL_PLAY);
	node.AddInt(KEY_WHERE, 0);
	node.AddInt(KEY_ID_LOWER, id);

	RequestCommand(node, PLAYLIST_PLAY_PLAYLIST);
}

void PlaylistManager::RequestPlaylistsPlay(QList<int> ids)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_PLAYLIST);
	node.Add(KEY_CMD1, VAL_PLAY);
	node.AddInt(KEY_WHERE, 3);

	CJsonNode nodeIDS(JSON_ARRAY);
	foreach (int id, ids)
	{
		nodeIDS.AppendArray((int64_t)id);
	}

	node.Add(KEY_IDS, nodeIDS);

//	LogDebug("node [%s]", node.ToCompactByteArray().data());

	RequestCommand(node, PLAYLIST_PLAY_PLAYLISTS);
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

	if (cmdID == PLAYLIST_PLAY_TRACK
			|| cmdID == PLAYLIST_PLAY_TRACKS
			|| cmdID == PLAYLIST_PLAY_PLAYLIST
			|| cmdID == PLAYLIST_PLAY_PLAYLISTS
			|| cmdID == PLAYLIST_RANDOM
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
	//		LogDebug("node : [%s]", m_Node.ToCompactByteArray().data());
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
	case PLAYLIST_PLAY_TRACKS:
		break;
	case PLAYLIST_NEW:
		break;
	case PLAYLIST_RENAME:
		break;
	case PLAYLIST_DELETE:
		break;
	case PLAYLIST_ADD_TRACK:
		break;
	case PLAYLIST_DEL_TRACK:
		break;
	case PLAYLIST_SWAP:
		break;
	case PLAYLIST_EXPORT:
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
