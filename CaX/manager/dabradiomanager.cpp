#include "dabradiomanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"

DabRadioManager::DabRadioManager()
{
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespInfo(QString, int)));
//	connect((QObject*)GetTcpClient(), SIGNAL(SigRespCoverArt(QString, int, int)), this, SLOT(SlotRespCoverArt(QString, int, int)));

}

DabRadioManager::~DabRadioManager()
{

}

void DabRadioManager::RequestList()
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_DAB_RADIO);
	node.Add	(KEY_CMD1,		VAL_LIST);
	node.Add	(KEY_APP,		true);

	RequestCommand(node, DAB_LIST);
}

void DabRadioManager::RequestPlay(int index)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_PLAY);
	node.Add	(KEY_CMD1,		VAL_DAB_RADIO);
	node.AddInt	(KEY_INDEX,		index);

	RequestCommand(node, DAB_PLAY);
}

void DabRadioManager::RequestSeek(bool bDel)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_DAB_RADIO);
	node.Add	(KEY_CMD1,		VAL_SEEK);
	node.Add	(KEY_CLEAR,		bDel);

	RequestCommand(node, DAB_SEEK);
}

void DabRadioManager::RequestSeekStop()
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_DAB_RADIO);
	node.Add	(KEY_CMD1,		VAL_SEEK_STOP);

	RequestCommand(node, DAB_SEEK_STOP);
}

void DabRadioManager::RequestDelete(QMap<int, bool> idMap)
{
	CJsonNode idArr(JSON_ARRAY);
	QMap<int, bool>::iterator i;
	for (i = idMap.begin(); i!= idMap.end(); i++)
	{
		LogDebug("key [%d] value [%d]", i.key(), i.value());
		idArr.AppendArray((int64_t)i.key());
	}

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_INDEXES, idArr);
	node.Add	(KEY_CMD0,		VAL_DAB_RADIO);
	node.Add	(KEY_CMD1,		VAL_DEL);

	RequestCommand(node, DAB_DELETE);
}

void DabRadioManager::RequestSet(int index, QString name)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_DAB_RADIO);
	node.Add	(KEY_CMD1,		VAL_SET);
	node.AddInt	(KEY_INDEX,		index);
	node.Add	(KEY_NAME,		name);

	RequestCommand(node, DAB_SET);
}

void DabRadioManager::RequestRecordList()
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_DAB_RADIO);
	node.Add	(KEY_CMD1,		VAL_RECORD_LIST);

	RequestCommand(node, DAB_RECORD_LIST);
}

void DabRadioManager::RequestRecordSet()
{
	CJsonNode node(JSON_OBJECT);

	RequestCommand(node, DAB_RECORD_SET);
}

void DabRadioManager::SlotRespInfo(QString json, int nCmdID)
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
	case DAB_LIST:
		ParseList(node);
		break;
	case DAB_PLAY:
	case DAB_SEEK:
	case DAB_SEEK_STOP:
	case DAB_DELETE:
		break;
	case DAB_RECORD_LIST:
		ParseRecordList(node);
		break;
	case DAB_MAX:
		LogWarning("Invalid command ID");
		break;
	}
}

void DabRadioManager::ParseList(CJsonNode node)
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

void DabRadioManager::ParseRecordList(CJsonNode node)
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

	emit SigRespRecordList(nodeList);
}
