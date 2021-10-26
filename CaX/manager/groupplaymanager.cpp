#include "groupplaymanager.h"



GroupPlayManager::GroupPlayManager()
{
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespInfo(QString, int)));
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespCoverArt(QString, int, int)), this, SLOT(SlotRespCoverArt(QString, int, int)));

}

void GroupPlayManager::RequestGroupPlayList(int eventID)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_GROUP_PLAY);
	node.Add(KEY_CMD1, VAL_DEV_LIST);
	node.AddInt(KEY_EVENT_ID, eventID);

	RequestCommand(node, GROUP_PLAY_LIST);
}

void GroupPlayManager::RequestGroupPlayEnable(bool enable)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_GROUP_PLAY);
	node.Add(KEY_CMD1, VAL_ENABLE);
	node.Add(KEY_ENABLE_CAP, enable);

	RequestCommand(node, GROUP_PLAY_ENABLE);
}

void GroupPlayManager::SlotRespInfo(QString json, int cmdID)
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

	switch (cmdID)
	{
	case GROUP_PLAY_LIST:
		ParseGroupPlayList(node);
		break;
	case GROUP_PLAY_ENABLE:
		break;
	}

}

void GroupPlayManager::SlotRespCoverArt(QString fileName, int nIndex, int mode)
{
	emit SigCoverArtUpdate(fileName, nIndex, mode);
}

void GroupPlayManager::ParseGroupPlayList(CJsonNode node)
{
	m_NodeList.clear();

	CJsonNode nodeSelf;
	if (!node.GetObject(KEY_SELF, nodeSelf))
	{
		emit SigRespError(STR_NO_RESULT);
		return;
	}
	m_NodeList.append(nodeSelf);

	CJsonNode nodeDevList;
	if (!node.GetArray(KEY_DEV_LIST, nodeDevList))
	{
//		emit SigRespError(STR_NO_RESULT);
//		return;
	}
	for(int i = 0; i < nodeDevList.ArraySize(); i++)
	{
		m_NodeList.append(nodeDevList.GetArrayAt(i));
	}

	CJsonNode nodeClientList;
	if (!node.GetArray(KEY_CLIENT_LIST,	nodeClientList))
	{
//		emit SigRespError(STR_NO_RESULT);
//		return;
	}
	for(int i = 0; i < nodeClientList.ArraySize(); i++)
	{
		m_NodeList.append(nodeClientList.GetArrayAt(i));
	}

	emit SigRespGroupPlayList(m_NodeList);
}
