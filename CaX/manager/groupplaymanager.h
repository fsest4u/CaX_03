#ifndef GROUPPLAYMANAGER_H
#define GROUPPLAYMANAGER_H

#include "basemanager.h"

class GroupPlayManager : public BaseManager
{
	Q_OBJECT
public:
	GroupPlayManager();

	void RequestGroupPlayList(int eventID);
	void RequestGroupPlayEnable(bool enable);

signals:

	void SigRespError(QString message);
	void SigRespGroupPlayList(QList<CJsonNode> list);
	void SigCoverArtUpdate(QString fileName, int nIndex, int mode);

private slots:

	void SlotRespInfo(QString json, int cmdID);
	void SlotRespCoverArt(QString fileName, int nIndex, int mode);

private:

	enum {
		GROUP_PLAY_LIST = 0,
		GROUP_PLAY_ENABLE,
		GROUP_PLAY_MAX
	};

	void ParseGroupPlayList(CJsonNode node);

	QList<CJsonNode>	m_NodeList;
	CJsonNode			m_Node;

};

#endif // GROUPPLAYMANAGER_H
