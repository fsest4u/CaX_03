#include "playlistmanager.h"
#include "sqlmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"


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

void PlaylistManager::SlotRespInfo(QString json, int cmdID)
{
	CJsonNode node;
	if (!node.SetContent(json))
	{
		emit SigRespError("invalid json");
		return;
	}

	LogDebug("cmdID [%d] node [%s]", cmdID, node.ToTabedByteArray().data());

	QString message = node.GetString(VAL_MSG);
	bool success = node.GetBool(VAL_SUCCESS);
	if (!success)
	{
		if (message.isEmpty())
		{
			emit SigRespError("unknown error - message is empty");
			return;
		}

		emit SigRespError(message.left((MSG_LIMIT_COUNT)));
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
	case PLAYLIST_TRACK_LIST:
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
	case PLAYLIST_PLAY:
		break;
	case PLAYLIST_PLAY_TRACKS:
		break;
	case PLAYLIST_EXPORT:
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
