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

	void SigRespDeviceInfo(CJsonNode node);
	void SigRespCoverInfo();

private slots:

	void SlotRespDeviceInfo(QString json, int nCmdID);
	void SlotRespCoverInfo();

};

#endif // APPMANAGER_H
