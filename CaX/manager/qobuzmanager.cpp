#include "qobuzmanager.h"


//#include "util/qobuz.h"

QobuzManager::QobuzManager(QObject *parent)
{
	Q_UNUSED(parent)

	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespInfo(QString, int)));

}

QobuzManager::~QobuzManager()
{

}

void QobuzManager::RequestLogin(QString userID, QString password)
{
	CJsonNode node(JSON_OBJECT);
	if (!userID.isEmpty())
		node.Add(KEY_USER, userID);
	if (!password.isEmpty())
		node.Add(KEY_PASS, password);

	node.Add(KEY_CMD0,		VAL_QOBUZ);
	node.Add(KEY_CMD1,		VAL_LOGIN);

	RequestCommand(node, QOBUZ_LOGIN);
}

void QobuzManager::RequestSearch(int nType, QString keyword, int nStart, int nCount)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0,		VAL_QOBUZ);
	node.Add(KEY_CMD1,		VAL_SEARCH);
	node.AddInt(KEY_TYPE,		nType);
	node.Add(KEY_KEYWORD,		keyword);
	node.AddInt(KEY_START,		nStart);
	node.AddInt(KEY_COUNT,		nCount);

	RequestCommand(node, QOBUZ_SEARCH);
}

void QobuzManager::RequestGenre(QString strID)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0,		VAL_QOBUZ);
	node.Add(KEY_CMD1,		VAL_GENRE);
	int nCmdID = QOBUZ_GENRE;
	if (!strID.isEmpty())
	{
		node.Add(KEY_ID_UPPER, strID);
		nCmdID = QOBUZ_GENRE_SUB;
	}

	RequestCommand(node, nCmdID);
}

void QobuzManager::RequestRecommend(int nType, QString strID, int nStart, int nCount, QString strGenreID)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0,		VAL_QOBUZ);
	node.Add(KEY_CMD1,		VAL_RECOMMEND);
	node.AddInt(KEY_TYPE,		nType);
	node.Add(KEY_ID_UPPER,		strID);
	node.AddInt(KEY_START,		nStart);
	node.AddInt(KEY_COUNT,		nCount);
	if (!strGenreID.isEmpty())
	{
		node.Add(KEY_GENRE_ID, strGenreID);
	}

	RequestCommand(node, QOBUZ_RECOMMEND);
}

void QobuzManager::RequestFavorite(int nType, int nStart, int nCount)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0,		VAL_QOBUZ);
	node.Add(KEY_CMD1,		VAL_FAVORITE);
	node.AddInt(KEY_TYPE,		nType);
	node.AddInt(KEY_START,		nStart);
	node.AddInt(KEY_COUNT,		nCount);

	RequestCommand(node, QOBUZ_FAVORITE);
}

void QobuzManager::RequestPlaylist(int nStart, int nCount)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0,		VAL_QOBUZ);
	node.Add(KEY_CMD1,		VAL_PLAYLIST);
	node.AddInt(KEY_START,		nStart);
	node.AddInt(KEY_COUNT,		nCount);

	RequestCommand(node, QOBUZ_PLAYLIST);
}

void QobuzManager::RequestCategory(int nType, QString strID, int nStart, int nCount)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0,		VAL_QOBUZ);
	node.Add(KEY_CMD1,		VAL_CATEGORY);
	node.AddInt(KEY_TYPE,		nType);
	node.Add(KEY_ID_UPPER,		strID);
	node.AddInt(KEY_START,		nStart);
	node.AddInt(KEY_COUNT,		nCount);

	RequestCommand(node, QOBUZ_CATEGORY);
}

void QobuzManager::RequestPlay(QMap<int, CJsonNode> nodeMap, int nWhere)
{
	CJsonNode nodeArr(JSON_ARRAY);
	QMap<int, CJsonNode>::iterator i;
	for (i = nodeMap.begin(); i!= nodeMap.end(); i++)
	{
		LogDebug("key [%d] value [%d]", i.key(), i.value().ToCompactByteArray().data());
		nodeArr.AppendArray(i.value());
	}

	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_QOBUZ);
	node.Add	(KEY_CMD1,		VAL_PLAY);
	node.AddInt	(KEY_WHERE,		nWhere);

	node.Add	(KEY_TRACKS,	nodeArr);

	RequestCommand(node, QOBUZ_PLAY);
}

void QobuzManager::RequestAddToFavorite(int nType, QMap<int, CJsonNode> nodeMap)
{
	CJsonNode nodeArr(JSON_ARRAY);
	QMap<int, CJsonNode>::iterator i;
	for (i = nodeMap.begin(); i!= nodeMap.end(); i++)
	{
		LogDebug("key [%d] value [%d]", i.key(), i.value().ToCompactByteArray().data());
		nodeArr.AppendArray(i.value().GetString(KEY_ID_UPPER));
	}

	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_QOBUZ);
	node.Add	(KEY_CMD1,		VAL_ADD);
	node.Add	(KEY_CMD2,		VAL_FAVORITE);
	node.AddInt(KEY_TYPE,		nType);

	node.Add	(KEY_IDS,	nodeArr);

	RequestCommand(node, QOBUZ_ADD_TO_FAVORITE);
}

void QobuzManager::RequestDeleteToFavorite(int nType, QMap<int, CJsonNode> nodeMap)
{
	CJsonNode nodeArr(JSON_ARRAY);
	QMap<int, CJsonNode>::iterator i;
	for (i = nodeMap.begin(); i!= nodeMap.end(); i++)
	{
		LogDebug("key [%d] value [%d]", i.key(), i.value().ToCompactByteArray().data());
		nodeArr.AppendArray(i.value().GetString(KEY_ID_UPPER));
	}

	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_QOBUZ);
	node.Add	(KEY_CMD1,		VAL_DEL);
	node.Add	(KEY_CMD2,		VAL_FAVORITE);
	node.AddInt(KEY_TYPE,		nType);

	node.Add	(KEY_IDS,	nodeArr);

	RequestCommand(node, QOBUZ_DELETE_TO_FAVORITE);
}

void QobuzManager::RequestAddPlaylist(QString name, QString desc)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_QOBUZ);
	node.Add	(KEY_CMD1,		VAL_ADD);
	node.Add	(KEY_CMD2,		VAL_PLAYLIST);
	node.Add	(KEY_NAME_CAP,  name);
	node.Add	(KEY_DESC,		desc);

	RequestCommand(node, QOBUZ_ADD_PLAYLIST);
}

void QobuzManager::RequestDeletePlaylist(QString strID)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_QOBUZ);
	node.Add	(KEY_CMD1,		VAL_DEL);
	node.Add	(KEY_CMD2,		VAL_PLAYLIST);

	node.Add	(KEY_ID_UPPER,		strID);

	RequestCommand(node, QOBUZ_DELETE_PLAYLIST);
}

void QobuzManager::RequestDeleteTrack(QString playlistID, QMap<int, CJsonNode> nodeMap)
{
	CJsonNode nodeArr(JSON_ARRAY);
	QMap<int, CJsonNode>::iterator i;
	for (i = nodeMap.begin(); i!= nodeMap.end(); i++)
	{
		LogDebug("key [%d] value [%d]", i.key(), i.value().ToCompactByteArray().data());
		nodeArr.AppendArray(i.value().GetString(KEY_ID_UPPER));
	}

	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_QOBUZ);
	node.Add	(KEY_CMD1,		VAL_DEL);
	node.Add	(KEY_CMD2,		VAL_TRACK);
	node.Add	(KEY_ID_UPPER,	playlistID);

	node.Add	(KEY_IDS,	nodeArr);

	RequestCommand(node, QOBUZ_DELETE_TRACK);
}

void QobuzManager::RequestRandom()
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_REMOTE);
	node.Add(KEY_KEY, VAL_SHUFFLE);

	RequestCommand(node, QOBUZ_RANDOM);
}

void QobuzManager::SlotRespInfo(QString json, int nCmdID)
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

	LogDebug("node [%s]", node.ToTabedByteArray().data());

	QString strMsg;
	bool	bSuccess = false;
	if (!node.GetBool(VAL_SUCCESS, bSuccess) || !bSuccess)
	{
		if (!node.GetString(VAL_MSG, strMsg) || strMsg.isEmpty())
		{
			emit SigRespError(STR_UNKNOWN_ERROR);
			return;
		}

		emit SigRespError(strMsg.left(MSG_LIMIT_COUNT));
		return;
	}

	if (node.GetBool(VAL_LOGIN))
	{
		switch (nCmdID)
		{
		case QOBUZ_LOGIN:
			emit SigRespLoginFail(node);
			break;
		case QOBUZ_MAX:
			emit SigRespError(STR_INVALID_ID);
			break;
		}

	}
	else
	{
		switch (nCmdID)
		{
		case QOBUZ_LOGIN:
			emit SigRespLoginSuccess();
			break;
		case QOBUZ_CATEGORY:
		case QOBUZ_SEARCH:
		case QOBUZ_GENRE:
		case QOBUZ_RECOMMEND:
		case QOBUZ_FAVORITE:
		case QOBUZ_PLAYLIST:
			ParseList(node, false);
			break;
		case QOBUZ_GENRE_SUB:
			ParseList(node, true);
			break;
		case QOBUZ_PLAY:
		case QOBUZ_RANDOM:
			break;
		case QOBUZ_ADD_TO_FAVORITE:
			emit SigRespError(STR_INVALID_ID);
			break;
		case QOBUZ_DELETE_TO_FAVORITE:
			emit SigRespError(STR_INVALID_ID);
			break;
		case QOBUZ_ADD_PLAYLIST:
		case QOBUZ_DELETE_PLAYLIST:
		case QOBUZ_DELETE_TRACK:
			// refresh
			emit SigListUpdate();
			emit
			break;
		case QOBUZ_MAX:
			emit SigRespError(STR_INVALID_ID);
			break;
		}
	}

}

void QobuzManager::ParseList(CJsonNode node, bool genre)
{
	CJsonNode result;
	if (!node.GetArray(VAL_RESULT, result) || result.ArraySize() <= 0)
	{
		emit SigRespError(STR_NO_RESULT);
		return;
	}

	QList<CJsonNode> nodeList;
	for (int i = 0; i < result.ArraySize(); i++)
	{
		nodeList.append(result.GetArrayAt(i));
	}

	emit SigRespList(nodeList, genre);
}

