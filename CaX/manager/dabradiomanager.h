#ifndef DABRADIOMANAGER_H
#define DABRADIOMANAGER_H

#include "basemanager.h"

class DabRadioManager : public BaseManager
{
	Q_OBJECT
public:
	DabRadioManager();
	~DabRadioManager();

	void RequestList();
	void RequestPlay(int index);
	void RequestSeek(bool bDel);
	void RequestSeekStop();
//	void RequestAdd();
	void RequestDelete(QMap<int, bool> idMap);
	void RequestSet(int index, QString name);
	void RequestRecordList();
	void RequestRecordSet();

signals:

	void SigRespError(QString errMsg);
//	void SigRespList(QList<CJsonNode> list);
	void SigRespList(CJsonNode node);
	void SigRespRecordList(QList<CJsonNode> list);

private slots:

	void SlotRespInfo(QString json, int nCmdID);

private:

	enum {
		DAB_LIST = 0,
		DAB_PLAY,
		DAB_SEEK,
		DAB_SEEK_STOP,
//		DAB_ADD,
		DAB_DELETE,
		DAB_SET,
		DAB_RECORD_LIST,
		DAB_RECORD_SET,
		DAB_MAX
	};

	void ParseList(CJsonNode node);
	void ParseRecordList(CJsonNode node);

};

#endif // DABRADIOMANAGER_H
