#include "iqobuzmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"

IQobuzManager::IQobuzManager(QObject *parent)
{
	Q_UNUSED(parent)

	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespInfo(QString, int)));

}

IQobuzManager::~IQobuzManager()
{

}

void IQobuzManager::RequestQobuzLogin(QString userID, QString password)
{
	CJsonNode node(JSON_OBJECT);
	if (!userID.isEmpty())
		node.Add(KEY_USER, userID);
	if (!password.isEmpty())
		node.Add(KEY_PASS, password);

	node.Add(KEY_CMD0,		VAL_QOBUZ);
	node.Add(KEY_CMD1,		VAL_LOGIN);

	RequestCommand(node, IQOBUZ_LOGIN);
}

void IQobuzManager::SlotRespInfo(QString json, int nCmdID)
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
		case IQOBUZ_LOGIN:
			emit SigRespQobuzLoginFail(node);
			break;
		case IQOBUZ_MAX:
			LogWarning("Invalid command ID");
			break;
		}

	}
	else
	{
		switch (nCmdID)
		{
		case IQOBUZ_LOGIN:
			emit SigRespQobuzLoginSuccess();
			break;
		case IQOBUZ_MAX:
			LogWarning("Invalid command ID");
			break;
		}
	}

}
