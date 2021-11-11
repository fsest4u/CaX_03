#ifndef AUDIOCDMANAGER_H
#define AUDIOCDMANAGER_H

#include "basemanager.h"
#include "sqlmanager.h"

class AudioCDManager : public BaseManager
{
	Q_OBJECT
public:
	AudioCDManager();
	~AudioCDManager();

	void RequestTrackList(int source = -1);
	void RequestTrackInfo(int id = 1);
	void RequestTrackPlay(int id = 1);
	void RequestEject();
	void RequestCDRipInfo(int index, QMap<int, bool> idMap);
	void RequestCDRip(CJsonNode node);
	void RequestCategoryList(int nCategory);
	void RequestRandom();



signals:

	void SigRespError(QString msg);
	void SigRespTrackList(QList<CJsonNode> list);
	void SigRespTrackInfo(CJsonNode node);
	void SigRespCDRipInfo(CJsonNode node);
	void SigRespCategoryList(QList<CJsonNode> list);

private slots:

	void SlotRespInfo(QString json, int cmdID);

private:

	enum {
		AUDIO_CD_TRACK_LIST = 0,
		AUDIO_CD_TRACK_INFO,
		AUDIO_CD_TRACK_PLAY,
		AUDIO_CD_EJECT,
		AUDIO_CD_RIP_INFO,
		AUDIO_CD_RIP,
		AUDIO_CD_CATEGORY_LIST,
		AUDIO_CD_RANDOM,
		AUDIO_CD_MAX
	};

	void ParseTrackList(CJsonNode result);
	void ParseCategoryList(CJsonNode result);

	SQLManager	*m_pSql;

	QList<CJsonNode>	m_NodeList;
	CJsonNode			m_Node;

};

#endif // AUDIOCDMANAGER_H
