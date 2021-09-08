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
	void RequestSeek();
	void RequestSeekStop();
	void RequestAdd();
	void RequestDelete();
	void RequestSet();
	void RequestRecordList();
	void RequestRecordSet();

signals:

	void SigRespError(QString errMsg);
	void SigRespList(QList<CJsonNode> list);

private slots:

	void SlotRespInfo(QString json, int nCmdID);

private:

	enum reqFmRadioIndex {
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


};

#endif // FMRADIOMANAGER_H
