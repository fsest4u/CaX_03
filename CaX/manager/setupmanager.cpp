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

void SetupManager::RequestSetupSet(int eventID, QString id, QString value)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0,		VAL_SETUP);
	node.Add(KEY_CMD1,		VAL_SET);
	node.AddInt(KEY_EVENT_ID,		eventID);
	node.Add(KEY_ID_UPPER,		id);
	if (!value.isEmpty())
	{
		node.Add(KEY_VALUE,		value);
	}

	RequestCommand(node, SETUP_SET);
}

void SetupManager::RequestSetupSet(int eventID, QString id, bool ok)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0,		VAL_SETUP);
	node.Add(KEY_CMD1,		VAL_SET);
	node.AddInt(KEY_EVENT_ID,		eventID);
	node.Add(KEY_ID_UPPER,	id);
	node.Add(KEY_OK,		ok);

	RequestCommand(node, SETUP_SET);
}

void SetupManager::RequestSetupSet(int eventID, QString id, bool ok, int analog, int aux, int phono)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0,		VAL_SETUP);
	node.Add(KEY_CMD1,		VAL_SET);
	node.AddInt(KEY_EVENT_ID,	eventID);
	node.Add(KEY_ID_UPPER,	id);
	node.Add(KEY_OK,		ok);
	node.AddInt(KEY_ANALOG_IN,	analog);
	node.AddInt(KEY_AUX_IN,		aux);
	node.AddInt(KEY_PHONO_IN,	phono);

	RequestCommand(node, SETUP_SET);
}

void SetupManager::RequestSetupSet(int eventID, QString id, bool ok, int volume, QString key, QString value)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0,		VAL_SETUP);
	node.Add(KEY_CMD1,		VAL_SET);
	node.AddInt(KEY_EVENT_ID,	eventID);
	node.Add(KEY_ID_UPPER,	id);
	node.Add(KEY_OK,		ok);
	node.Add(KEY_VOLUME_CAP,	QString::number(volume));
	node.Add(key,		value);

	RequestCommand(node, SETUP_SET);
}

void SetupManager::RequestSetupSet(int eventID, QString id, bool ok,
								   QString key0, QString value0,
								   QString key1, QString value1,
								   QString key2, QString value2,
								   QString key3, QString value3,
								   QString hiddenKey, QString hiddenValue)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0,		VAL_SETUP);
	node.Add(KEY_CMD1,		VAL_SET);
	node.AddInt(KEY_EVENT_ID,	eventID);
	node.Add(KEY_ID_UPPER,	id);
	node.Add(KEY_OK,		ok);
	if (!key0.isEmpty())
	{
		node.Add(key0,	value0);
	}
	if (!key1.isEmpty())
	{
		node.Add(key1,	value1);
	}
	if (!key2.isEmpty())
	{
		node.Add(key2,	value2);
	}
	if (!key3.isEmpty())
	{
		node.Add(key3,	value3);
	}
	node.Add(hiddenKey,		hiddenValue);

	RequestCommand(node, SETUP_SET);
}

void SetupManager::RequestSetupSet(int eventID, QString id, bool ok,
								   QString usernameKey, QString username,
								   QString passwordKey, QString password)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0,		VAL_SETUP);
	node.Add(KEY_CMD1,		VAL_SET);
	node.AddInt(KEY_EVENT_ID,	eventID);
	node.Add(KEY_ID_UPPER,	id);
	node.Add(KEY_OK,		ok);
	if (!usernameKey.isEmpty())
	{
		node.Add(usernameKey,		username);
	}
	if (!passwordKey.isEmpty())
	{
		node.Add(passwordKey,		password);
	}
	RequestCommand(node, SETUP_SET);
}

void SetupManager::RequestSetupSet(int eventID, QString id, bool ok, QString value,
								   int value00,
								   int value01,
								   int value02,
								   int value03,
								   int value04,
								   int value05,
								   int value06,
								   int value07,
								   int value08,
								   int value09)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0,		VAL_SETUP);
	node.Add(KEY_CMD1,		VAL_SET);
	node.AddInt(KEY_EVENT_ID,	eventID);
	node.Add(KEY_ID_UPPER,	id);
	node.Add(KEY_OK,		ok);
	node.Add(KEY_VALUE,		value);
	node.Add(KEY_063HZ, QString::number(value00));
	node.Add(KEY_150HZ, QString::number(value01));
	node.Add(KEY_300HZ, QString::number(value02));
	node.Add(KEY_600HZ, QString::number(value03));
	node.Add(KEY_01KHZ, QString::number(value04));
	node.Add(KEY_03KHZ, QString::number(value05));
	node.Add(KEY_05KHZ, QString::number(value06));
	node.Add(KEY_10KHZ, QString::number(value07));
	node.Add(KEY_16KHZ, QString::number(value08));
	node.Add(KEY_20KHZ, QString::number(value09));

	RequestCommand(node, SETUP_SET);
}

void SetupManager::RequestSetupSet(int eventID, QString id, bool ok, QString date, QString time)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0,		VAL_SETUP);
	node.Add(KEY_CMD1,		VAL_SET);
	node.AddInt(KEY_EVENT_ID,	eventID);
	node.Add(KEY_ID_UPPER,	id);
	node.Add(KEY_OK,		ok);
	node.Add(KEY_DATE,		date);
	node.Add(KEY_TIME_CAP,	time);

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
