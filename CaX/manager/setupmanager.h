#ifndef SETUPMANAGER_H
#define SETUPMANAGER_H

#include "basemanager.h"

class SetupManager : public BaseManager
{
	Q_OBJECT
public:
	SetupManager();
	~SetupManager();

	void RequestSetupGroup(int eventID, QString id, int index);
	void RequestSetupSet(int eventID, QString id, QString value = "");
	void RequestSetupSet(int eventID, QString id, bool ok);

signals:

	void SigRespError(QString message);
	void SigRespGroup(QList<CJsonNode> list, int index);
	void SigRespSet(CJsonNode node);

private slots:

	void SlotRespInfo(QString json, int cmdID);

private:

	enum {
		SETUP_GROUP = 0,
		SETUP_SET,
		SETUP_MAX
	};

	void ParseGroup(CJsonNode node);
	void ParseSet(CJsonNode node);

	int m_Index;

};

#endif // SETUPMANAGER_H
