#include "qobuzmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"

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

void QobuzManager::RequestCategory()
{
	CJsonNode node(JSON_OBJECT);


	RequestCommand(node, QOBUZ_CATEGORY);
}

void QobuzManager::RequestSearch()
{
	CJsonNode node(JSON_OBJECT);


	RequestCommand(node, QOBUZ_SEARCH);
}

void QobuzManager::RequestGenre()
{
	CJsonNode node(JSON_OBJECT);


	RequestCommand(node, QOBUZ_GENRE);
}

void QobuzManager::RequestRecommend()
{
	CJsonNode node(JSON_OBJECT);


	RequestCommand(node, QOBUZ_RECOMMEND);
}

void QobuzManager::RequestFavorite()
{
	CJsonNode node(JSON_OBJECT);


	RequestCommand(node, QOBUZ_FAVORITE);
}

void QobuzManager::RequestPlaylist()
{
	CJsonNode node(JSON_OBJECT);


	RequestCommand(node, QOBUZ_PLAYLIST);
}

void QobuzManager::RequestPlay()
{
	CJsonNode node(JSON_OBJECT);


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
		emit SigRespError("invalid json");
		return;
	}

	LogDebug("node [%s]", node.ToTabedByteArray().data());

	QString strMsg;
	bool	bSuccess = false;
	if (!node.GetBool(VAL_SUCCESS, bSuccess) || !bSuccess)
	{
		if (!node.GetString(VAL_MSG, strMsg) || strMsg.isEmpty())
		{
			emit SigRespError("unknown error");
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
			LogWarning("Invalid command ID");
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
			break;
		case QOBUZ_SEARCH:
			break;
		case QOBUZ_GENRE:
			break;
		case QOBUZ_RECOMMEND:
			break;
		case QOBUZ_FAVORITE:
			break;
		case QOBUZ_PLAYLIST:
			break;
		case QOBUZ_PLAY:
			break;
		case QOBUZ_ADD:
			break;
		case QOBUZ_DELETE:
			break;
		case QOBUZ_MAX:
			LogWarning("Invalid command ID");
			break;
		}
	}

}
