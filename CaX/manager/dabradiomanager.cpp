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

void DabRadioManager::RequestSeek()
{

}

void DabRadioManager::RequestSeekStop()
{

}

void DabRadioManager::RequestDelete()
{

}

void DabRadioManager::RequestSet()
{

}

void DabRadioManager::RequestRecordList()
{

}

void DabRadioManager::RequestRecordSet()
{

}

void DabRadioManager::SlotRespInfo(QString json, int nCmdID)
{

}
