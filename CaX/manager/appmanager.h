#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <QObject>

#include "basemanager.h"

#include "util/CJsonNode.h"

class AppManager : public BaseManager
{
	Q_OBJECT
public:
	explicit AppManager(QObject *parent = nullptr);
	~AppManager();

	void RequestDeviceInfo();

signals:

	void SigRespError(QString errMsg);
	void SigRespDeviceInfo(CJsonNode node);
	void SigRespCoverInfo();

private slots:

	void SlotRespDeviceInfo(QString json, int nCmdID);
	void SlotRespCoverInfo();

	void SlotPowerOffDevice(bool bWol);

};

#endif // APPMANAGER_H
