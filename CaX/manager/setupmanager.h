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
	void RequestSetupSet(int eventID, QString id, QString value = "");
	void RequestSetupSet(int eventID, QString id, bool ok);
	void RequestSetupSet(int eventID, QString id, bool ok, int analog, int aux, int phono);
	void RequestSetupSet(int eventID, QString id, bool ok, int volume, QString key, QString value);
	void RequestSetupSet(int eventID, QString id, bool ok, QString usernameKey, QString username, QString passwordKey, QString password);
	void RequestVolume(int eventID, int value);

signals:

	void SigRespError(QString message);
	void SigRespGroup(QList<CJsonNode> list);
	void SigRespSet(CJsonNode node);

private slots:

	void SlotRespInfo(QString json, int cmdID);

private:

	enum {
		SETUP_GROUP = 0,
		SETUP_SET,
		SETUP_VOLUME,
		SETUP_MAX
	};

	void ParseGroup(CJsonNode node);
	void ParseSet(CJsonNode node);

};

#endif // SETUPMANAGER_H
