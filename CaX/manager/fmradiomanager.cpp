#include "fmradiomanager.h"


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

void FmRadioManager::RequestSeek(bool bDel)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_FM_RADIO);
	node.Add	(KEY_CMD1,		VAL_SEEK);
	node.Add	(KEY_CLEAR,		bDel);

	RequestCommand(node, FM_SEEK);
}

void FmRadioManager::RequestSeekStop()
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_FM_RADIO);
	node.Add	(KEY_CMD1,		VAL_SEEK_STOP);

	RequestCommand(node, FM_SEEK_STOP);
}

void FmRadioManager::RequestAdd(int64_t freq, QString name)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_FM_RADIO);
	node.Add	(KEY_CMD1,		VAL_ADD);
	node.Add	(KEY_FREQ,		freq);
	node.Add	(KEY_NAME_CAP,		name);

	RequestCommand(node, FM_ADD);
}

void FmRadioManager::RequestDelete(QMap<int, bool> idMap)
{
	CJsonNode idArr(JSON_ARRAY);
	QMap<int, bool>::iterator i;
	for (i = idMap.begin(); i!= idMap.end(); i++)
	{
//		LogDebug("key [%d] value [%d]", i.key(), i.value());
		idArr.AppendArray((int64_t)i.key());
	}

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_INDEXES, idArr);
	node.Add	(KEY_CMD0,		VAL_FM_RADIO);
	node.Add	(KEY_CMD1,		VAL_DEL);

	RequestCommand(node, FM_DELETE);
}

void FmRadioManager::RequestSet(int index, QString name)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_FM_RADIO);
	node.Add	(KEY_CMD1,		VAL_SET);
	node.AddInt	(KEY_INDEX,		index);
	node.Add	(KEY_NAME_CAP,		name);

	RequestCommand(node, FM_SET);
}

void FmRadioManager::RequestRecordList()
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_FM_RADIO);
	node.Add	(KEY_CMD1,		VAL_RECORD_LIST);

	RequestCommand(node, FM_RECORD_LIST);
}

void FmRadioManager::RequestRecordSet()
{
	CJsonNode node(JSON_OBJECT);

	RequestCommand(node, FM_RECORD_SET);
}

void FmRadioManager::SlotRespInfo(QString json, int nCmdID)
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

	switch (nCmdID)
	{
	case FM_LIST:
		ParseList(node);
		break;
	case FM_RECORD_LIST:
		ParseRecordList(node);
		break;
	case FM_PLAY:
	case FM_SEEK:
	case FM_ADD:
	case FM_DELETE:
		break;
	case FM_MAX:
		emit SigRespError(STR_INVALID_ID);
		break;
	}

}

void FmRadioManager::ParseList(CJsonNode node)
{
//	CJsonNode result;
//	if (!node.GetArray(VAL_RESULT, result) || result.ArraySize() <= 0)
//	{
//		emit SigRespError(STR_NO_RESULT);
//		return;
//	}

//	QList<CJsonNode> nodeList;
//	for (int i = 0; i < result.ArraySize(); i++)
//	{
//		nodeList.append(result.GetArrayAt(i));
//	}

//	emit SigRespList(nodeList);

	emit SigRespList(node);
}

void FmRadioManager::ParseRecordList(CJsonNode node)
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

	emit SigRespRecordList(nodeList);
}
