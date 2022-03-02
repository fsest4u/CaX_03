#ifndef FMRADIOMANAGER_H
#define FMRADIOMANAGER_H

#include "basemanager.h"

class FmRadioManager : public BaseManager
{
	Q_OBJECT
public:
	FmRadioManager();
	~FmRadioManager();

	void RequestList();
	void RequestPlay(int index);
	void RequestSeek(bool bDel);
	void RequestSeekStop();
	void RequestAdd(QString name, int64_t freq);
	void RequestDelete(QMap<int, bool> idMap);
	void RequestSet(QString name, int64_t freq, int index);
	void RequestRecordList();
	void RequestRecordSet(CJsonNode node);

signals:

	void SigRespError(QString errMsg);
//	void SigRespList(QList<CJsonNode> list);
	void SigRespList(CJsonNode node);
	void SigRespRecordList(QList<CJsonNode> list);
	void SigRespRecordSet(CJsonNode node);

private slots:

	void SlotRespInfo(QString json, int nCmdID);

private:

	enum {
		FM_LIST = 0,
		FM_PLAY,
		FM_SEEK,
		FM_SEEK_STOP,
		FM_ADD,
		FM_DELETE,
		FM_SET,
		FM_RECORD_LIST,
		FM_RECORD_SET,
		FM_MAX
	};

	void ParseList(CJsonNode node);
	void ParseRecordList(CJsonNode node);
	void ParseRecordSet(CJsonNode node);


};

#endif // FMRADIOMANAGER_H
