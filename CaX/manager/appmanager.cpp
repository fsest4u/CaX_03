#include "appmanager.h"

AppManager::AppManager(QObject *parent)
{
	Q_UNUSED(parent)

	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespDeviceInfo(QString, int)));
//	connect((QObject*)GetTcpClient(), SIGNAL(SigReadCoverImage(QString, int, int)), this, SLOT(SlotReadCoverImage(QString, int, int)));
//	connect((QObject*)GetTcpClient(), SIGNAL(SigReadCoverImage(QString)), this, SLOT(SlotReadCoverImage(QString)));
}

AppManager::~AppManager()
{
	disconnect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString)));
}

void AppManager::RequestDeviceInfo()
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_ETC);
	node.Add(KEY_CMD1, VAL_APP_INFO);

	RequestCommand(node);
}

void AppManager::RequestDevicePowerOff(bool bWol)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, KEY_SETUP);
	node.Add(KEY_CMD1, VAL_POWER_OFF);
	node.Add(KEY_WOL, bWol);

	RequestCommand(node);

}

void AppManager::SlotRespDeviceInfo(QString json, int nCmdID)
{
	Q_UNUSED(nCmdID)

	CJsonNode node;
	if (!node.SetContent(json)) {
		emit SigRespError(STR_INVALID_JSON);
		return;
	}

//	LogDebug("node [%s]", node.ToTabedByteArray().data());

	emit SigRespDeviceInfo(node);
}

void AppManager::SlotRespCoverInfo()
{
	emit SigRespCoverInfo();
}


