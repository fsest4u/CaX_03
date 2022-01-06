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

void GroupPlayManager::RequestGroupPlay(bool enable, int eventID)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_GROUP_PLAY);
	node.Add(KEY_CMD1, VAL_ENABLE);
	node.Add(KEY_ENABLE_CAP, enable);
	node.AddInt(KEY_EVENT_ID, eventID);

	RequestCommand(node, GROUP_PLAY_ENABLE);
}

void GroupPlayManager::RequestAutoJoin(bool enable, int eventID)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_GROUP_PLAY);
	node.Add(KEY_CMD1, VAL_JOIN);
	node.Add(KEY_AUTO_JOIN, enable);
	node.AddInt(KEY_EVENT_ID, eventID);

	RequestCommand(node, GROUP_PLAY_AUTO_JOIN);
}

void GroupPlayManager::RequestMute(bool enable, int eventID)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_PLAY);
	node.Add(KEY_CMD1, VAL_SET_MUTE);
	node.Add(KEY_MUTE_CAP, enable);
	node.AddInt(KEY_EVENT_ID, eventID);

	RequestCommand(node, GROUP_PLAY_MUTE);
}

void GroupPlayManager::RequestPlayStop(int eventID)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_PLAY);
	node.Add(KEY_CMD1, VAL_PLAY);
	node.Add(KEY_PLAY, VAL_STOP);
	node.AddInt(KEY_EVENT_ID, eventID);

	RequestCommand(node, GROUP_PLAY_MUTE);
}

void GroupPlayManager::RequestPowerOff(bool wol)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_SETUP);
	node.Add(KEY_CMD1, VAL_POWER_OFF);
	node.Add(KEY_WOL, wol);

	RequestCommand(node, GROUP_PLAY_POWER_OFF);
}

void GroupPlayManager::RequestVolume(int value, int eventID)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_PLAY);
	node.Add(KEY_CMD1, VAL_SET_VOLUME);
	node.AddInt(KEY_VOLUME_CAP, value);
	node.AddInt(KEY_EVENT_ID, eventID);

	RequestCommand(node, GROUP_PLAY_VOLUME);
}

void GroupPlayManager::RequestChannel(int value, int eventID)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_GROUP_PLAY);
	node.Add(KEY_CMD1, VAL_CHANNEL);
	node.AddInt(KEY_CHANNELS, value);
	node.AddInt(KEY_EVENT_ID, eventID);

	RequestCommand(node, GROUP_PLAY_CHANNEL);
}

void GroupPlayManager::SlotRespInfo(QString json, int cmdID)
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

	switch (cmdID)
	{
	case GROUP_PLAY_LIST:
		ParseGroupPlayList(node);
		break;
	case GROUP_PLAY_ENABLE:
	case GROUP_PLAY_AUTO_JOIN:
	case GROUP_PLAY_MUTE:
	case GROUP_PLAY_POWER_OFF:
	case GROUP_PLAY_VOLUME:
	case GROUP_PLAY_CHANNEL:
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

//	foreach (CJsonNode node, m_NodeList)
//	{
//		LogDebug("node [%s]", node.ToCompactByteArray().data());
//	}

	emit SigRespGroupPlayList(m_NodeList);
}
