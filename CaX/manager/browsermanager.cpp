#include "browsermanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"

BrowserManager::BrowserManager(QObject *parent)
{
	Q_UNUSED(parent)

}

BrowserManager::~BrowserManager()
{

}

void BrowserManager::RequestRoot()
{
	CJsonNode node(JSON_OBJECT);

	RequestCommand(node, BROWSER_ROOT);

}

void BrowserManager::RequestFolder()
{
	CJsonNode node(JSON_OBJECT);

	RequestCommand(node, BROWSER_FOLDER);
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

void BrowserManager::RequestPlay()
{
	CJsonNode node(JSON_OBJECT);

	RequestCommand(node, BROWSER_PLAY);
}

void BrowserManager::SlotRespInfo(QString json, int nCmdID)
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
		ParseRoot(node);
		break;
	case BROWSER_FOLDER:
	case BROWSER_COPY:
	case BROWSER_MOVE:
	case BROWSER_DELETE:
	case BROWSER_CREATE:
	case BROWSER_RENAME:
	case BROWSER_PLAY:
		break;
//	case BROWSER_PLAYLIST:
//	case BROWSER_INFO:
//	case BROWSER_INFO_BOT:
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

void BrowserManager::ParseRoot(CJsonNode node)
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
