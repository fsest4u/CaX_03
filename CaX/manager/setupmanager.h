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
	void RequestSetupSet(int eventID, QString id, bool ok,
						 QString key0, QString value0,
						 QString key1, QString value1,
						 QString key2, QString value2,
						 QString key3, QString value3,
						 QString hiddenKey, QString hiddenValue);
	void RequestSetupSet(int eventID, QString id, bool ok, QString usernameKey, QString username, QString passwordKey, QString password);
	void RequestSetupSet(int eventID, QString id, bool ok,
						 QString value,
						 int value00,
						 int value01,
						 int value02,
						 int value03,
						 int value04,
						 int value05,
						 int value06,
						 int value07,
						 int value08,
						 int value09);
	void RequestSetupSet(int eventID, QString id, bool ok, QString date, QString time);
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
