#ifndef SETUPMANAGER_H
#define SETUPMANAGER_H

#include "basemanager.h"

class SetupManager : public BaseManager
{
	Q_OBJECT
public:
	SetupManager();
	~SetupManager();

	void RequestSetupGroup(int eventID, QString id);

signals:

	void SigRespError(QString message);
	void SigRespList(QList<CJsonNode> list);

private slots:

	void SlotRespInfo(QString json, int cmdID);

private:

	enum {
		SETUP_GROUP = 0,
		SETUP_SET,
		SETUP_MAX
	};

	void ParseGroup(CJsonNode node);

};

#endif // SETUPMANAGER_H
