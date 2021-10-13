#include "browsermanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"

BrowserManager::BrowserManager(QObject *parent)
{
	Q_UNUSED(parent)
//	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespInfo(QString, int)));
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int, int)), this, SLOT(SlotRespInfo(QString, int, int)));

}

BrowserManager::~BrowserManager()
{

}

void BrowserManager::RequestRoot()
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_BROWSER);
	node.Add	(KEY_CMD1,		VAL_ROOT);
	node.Add	(KEY_DIR_ONLY,	false);
	node.Add	(KEY_IMAGE,		false);

	RequestCommand(node, BROWSER_ROOT);
}

void BrowserManager::RequestFolder(QString strPath)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_BROWSER);
	node.Add	(KEY_CMD1,		VAL_FOLDER);
	// path : folder path, cd, net, upnp ...
	node.Add	(KEY_PATH,		strPath);
	node.Add	(KEY_DIR_ONLY,	false);
	node.Add	(KEY_IMAGE,		false);

	RequestCommand(node, BROWSER_FOLDER);
}

void BrowserManager::RequestInfoBot(QString strPath, int nIndex)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_BROWSER);
	node.Add	(KEY_CMD1,		VAL_INFO);
	node.Add	(KEY_CMD2,		VAL_BOT);
	node.Add	(KEY_PATH,		strPath);

	RequestCommand(node, BROWSER_INFO_BOT, nIndex);

}



void BrowserManager::RequestCopy()
{
	CJsonNode node(JSON_OBJECT);

	RequestCommand(node, BROWSER_COPY);
}

void BrowserManager::RequestMove()
{
	CJsonNode node(JSON_OBJECT);

	RequestCommand(node, BROWSER_MOVE);
}

void BrowserManager::RequestDelete()
{
	CJsonNode node(JSON_OBJECT);

	RequestCommand(node, BROWSER_DELETE);
}

void BrowserManager::RequestCreate()
{
	CJsonNode node(JSON_OBJECT);

	RequestCommand(node, BROWSER_CREATE);
}

void BrowserManager::RequestRename()
{
	CJsonNode node(JSON_OBJECT);

	RequestCommand(node, BROWSER_RENAME);
}

void BrowserManager::RequestTrackPlay(QString root, QStringList dirs, QStringList files)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_BROWSER);
	node.Add	(KEY_CMD1,		VAL_PLAY);
	node.AddInt	(KEY_WHERE,		0);
	node.Add(KEY_SUB_DIR, true);
	node.Add(KEY_ROOT, root);
	if (dirs.count() > 0)
	{
		node.Add(KEY_DIRS, dirs);
	}
	if (files.count() > 0)
	{
		node.Add(KEY_FILES, files);
	}

	LogDebug("node [%s]", node.ToCompactByteArray().data());

	RequestCommand(node, BROWSER_TRACK_PLAY);
}

void BrowserManager::RequestPlaylistPlay(QString path, QList<int> indexes)
{

}

void BrowserManager::SlotRespInfo(QString json, int nCmdID, int nIndex)
{
	CJsonNode node;
	if (!node.SetContent(json))
	{
		emit SigRespError("invalid json");
		return;
	}

	LogDebug("node [%d] [%s]", nCmdID, node.ToTabedByteArray().data());

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

	switch (nCmdID)
	{
	case BROWSER_ROOT:
	case BROWSER_FOLDER:
		ParseFolder(node);
		break;
	case BROWSER_INFO_BOT:
		ParseInfoBot(node, nIndex);
		break;
	case BROWSER_TRACK_PLAY:
		break;
	case BROWSER_PLAYLIST_PLAY:
		break;
	case BROWSER_COPY:
	case BROWSER_MOVE:
	case BROWSER_DELETE:
	case BROWSER_CREATE:
	case BROWSER_RENAME:
//	case BROWSER_INFO:
//	case BROWSER_SET_ART:
//	case BROWSER_CONVERT:
//	case BROWSER_REPLAYGAIN_SET:
//	case BROWSER_REPLAYGAIN_CLEAR:
//	case BROWSER_IMPORT:
//	case BROWSER_UPNP_FOLDER:
//	case BROWSER_UPNP_PLAY:
	case BROWSER_MAX:
		LogWarning("Invalid command ID");
		break;
	}
}

void BrowserManager::ParseFolder(CJsonNode node)
{
	CJsonNode result;
	if (!node.GetArray(VAL_RESULT, result) || result.ArraySize() <= 0)
	{
		emit SigRespError("there is no result");
		return;
	}

	QList<CJsonNode> nodeList;
	for (int i = 0; i < result.ArraySize(); i++)
	{
		nodeList.append(result.GetArrayAt(i));
	}

	emit SigRespList(nodeList);
}

void BrowserManager::ParseInfoBot(CJsonNode node, int nIndex)
{
	emit SigRespNodeUpdate(node, nIndex);
}
