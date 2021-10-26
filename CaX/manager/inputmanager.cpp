#include "inputmanager.h"



InputManager::InputManager()
{
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespInfo(QString, int)));

}

InputManager::~InputManager()
{

}

void InputManager::RequestInput(int index)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0,		VAL_PLAY);
	node.Add(KEY_CMD1,		VAL_INPUT);
	node.AddInt(KEY_INDEX,		index);

	RequestCommand(node, INPUT_NORMAL);
}

void InputManager::SlotRespInfo(QString json, int nCmdID)
{
	CJsonNode node;
	if (!node.SetContent(json))
	{
		emit SigRespError(STR_INVALID_JSON);
		return;
	}

	LogDebug("node [%d] [%s]", nCmdID, node.ToTabedByteArray().data());

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
}
