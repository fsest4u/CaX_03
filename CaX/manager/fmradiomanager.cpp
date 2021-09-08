#include "fmradiomanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"

FmRadioManager::FmRadioManager()
{
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespInfo(QString, int)));
//	connect((QObject*)GetTcpClient(), SIGNAL(SigRespCoverArt(QString, int, int)), this, SLOT(SlotRespCoverArt(QString, int, int)));

}

FmRadioManager::~FmRadioManager()
{

}

void FmRadioManager::RequestList()
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_FM_RADIO);
	node.Add	(KEY_CMD1,		VAL_LIST);
	node.Add	(KEY_APP,		true);

	RequestCommand(node, FM_LIST);
}

void FmRadioManager::RequestPlay(int index)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_PLAY);
	node.Add	(KEY_CMD1,		VAL_FM_RADIO);
	node.AddInt	(KEY_INDEX,		index);

	RequestCommand(node, FM_PLAY);
}

void FmRadioManager::RequestSeek()
{

}

void FmRadioManager::RequestSeekStop()
{

}

void FmRadioManager::RequestAdd()
{

}

void FmRadioManager::RequestDelete()
{

}

void FmRadioManager::RequestSet()
{

}

void FmRadioManager::RequestRecordList()
{

}

void FmRadioManager::RequestRecordSet()
{

}

void FmRadioManager::SlotRespInfo(QString json, int nCmdID)
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
	case FM_LIST:
		ParseList(node);
		break;
	case FM_PLAY:
		break;
	case FM_MAX:
		LogWarning("Invalid command ID");
		break;
	}

}

void FmRadioManager::ParseList(CJsonNode node)
{
	CJsonNode result;
	if (!node.GetArray(VAL_RESULT, result))
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
