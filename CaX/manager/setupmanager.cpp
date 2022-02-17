#include "setupmanager.h"


SetupManager::SetupManager()
{
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespInfo(QString, int)));

}

SetupManager::~SetupManager()
{

}

void SetupManager::RequestSetupGroup(int eventID, QString id)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0,		VAL_SETUP);
	node.Add(KEY_CMD1,		VAL_GROUP);
	node.AddInt(KEY_EVENT_ID,	eventID);
	node.Add(KEY_ID_UPPER,		id);

	RequestCommand(node, SETUP_GROUP);
}

void SetupManager::RequestSetupSet(CJsonNode node)
{
	node.Add(KEY_CMD0,		VAL_SETUP);
	node.Add(KEY_CMD1,		VAL_SET);

	RequestCommand(node, SETUP_SET);
}

void SetupManager::RequestVolume(int eventID, int value)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_PLAY);
	node.Add	(KEY_CMD1,		VAL_SET_VOLUME);
	node.AddInt(KEY_EVENT_ID,	eventID);
	node.AddInt(KEY_VOLUME_CAP,	value);
	RequestCommand(node, SETUP_VOLUME);
}

void SetupManager::SlotRespInfo(QString json, int cmdID)
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

//	LogDebug("node [%d] [%s]", cmdID, node.ToTabedByteArray().data());

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

	switch (cmdID)
	{
	case SETUP_GROUP:
		ParseGroup(node);
		break;
	case SETUP_SET:
		ParseSet(node);
		break;
	}

}

void SetupManager::ParseGroup(CJsonNode node)
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

	emit SigRespGroup(nodeList);
}

void SetupManager::ParseSet(CJsonNode node)
{
	emit SigRespSet(node);
}
