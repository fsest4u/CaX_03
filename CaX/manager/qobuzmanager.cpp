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

void QobuzManager::RequestPlay(CJsonNode srcNode)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_QOBUZ);
	node.Add	(KEY_CMD1,		VAL_PLAY);
	node.AddInt	(KEY_WHERE,		3);	// play new

	CJsonNode trackNode(JSON_ARRAY);
	trackNode.AppendArray(srcNode);
	node.Add	(KEY_TRACKS,	trackNode);

	RequestCommand(node, QOBUZ_PLAY);
}

void QobuzManager::RequestAdd()
{
	CJsonNode node(JSON_OBJECT);


	RequestCommand(node, QOBUZ_ADD);
}

void QobuzManager::RequestDelete()
{
	CJsonNode node(JSON_OBJECT);


	RequestCommand(node, QOBUZ_DELETE);
}

void QobuzManager::SlotRespInfo(QString json, int nCmdID)
{
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
			ParseList(node);
			break;
		case QOBUZ_GENRE_SUB:
			ParseGenreSubList(node);
			break;
		case QOBUZ_PLAY:
			break;
		case QOBUZ_ADD:
			break;
		case QOBUZ_DELETE:
			break;
		case QOBUZ_MAX:
			emit SigRespError(STR_INVALID_ID);
			break;
		}
	}

}

void QobuzManager::ParseList(CJsonNode node)
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

	emit SigRespList(nodeList);
}

void QobuzManager::ParseGenreSubList(CJsonNode node)
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

	emit SigRespGenreSubList(nodeList);
}

