#include "appmanager.h"

AppManager::AppManager(QObject *parent)
{
	Q_UNUSED(parent)

	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespInfo(QString, int)));
//	connect((QObject*)GetTcpClient(), SIGNAL(SigReadCoverImage(QString, int, int)), this, SLOT(SlotReadCoverImage(QString, int, int)));
//	connect((QObject*)GetTcpClient(), SIGNAL(SigReadCoverImage(QString)), this, SLOT(SlotReadCoverImage(QString)));
}

AppManager::~AppManager()
{
	disconnect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString)));
}

void AppManager::RequestTaskList()
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_TASK);
	node.Add(KEY_CMD1, VAL_LIST);

	RequestCommand(node, APP_MGR_TASK_LIST);
}

void AppManager::RequestDeviceInfo()
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_ETC);
	node.Add(KEY_CMD1, VAL_APP_INFO);

	RequestCommand(node, APP_MGR_DEVICE_INFO);
}

void AppManager::RequestDevicePowerOff(bool bWol)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, KEY_SETUP);
	node.Add(KEY_CMD1, VAL_POWER_OFF);
	node.Add(KEY_WOL, bWol);

	RequestCommand(node);

}

void AppManager::SlotRespInfo(QString json, int cmdID)
{

	CJsonNode node;
	if (!node.SetContent(json)) {
		emit SigRespError(STR_INVALID_JSON);
		return;
	}

	LogDebug("node [%s]", node.ToTabedByteArray().data());

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
	case APP_MGR_TASK_LIST:
		emit SigRespTaskList(node);
		break;
	case APP_MGR_DEVICE_INFO:
		emit SigRespDeviceInfo(node);
		break;
	case APP_MGR_MAX:
		emit SigRespError(STR_INVALID_ID);
		break;
	}
}




