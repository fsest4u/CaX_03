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

	void RequestTaskList();
	void RequestDeviceInfo();
	void RequestDevicePowerOff(bool bWol);

	void RequestProgressBack(int eventID, int taskID);
	void RequestProgressStop(int eventID, int taskID);
	void RequestProgressSkip(int eventID, int taskID);

signals:

	void SigRespError(QString errMsg);
	void SigRespTaskList(CJsonNode node);
	void SigRespDeviceInfo(CJsonNode node);

private slots:

	void SlotRespInfo(QString json, int cmdID);

private:

	enum {
		APP_MGR_TASK_LIST = 0,
		APP_MGR_DEVICE_INFO,
		APP_MGR_POWER_OFF,
		APP_MGR_PROGRESS_BACK,
		APP_MGR_PROGRESS_STOP,
		APP_MGR_PROGRESS_SKIP,
		APP_MGR_MAX
	};

};

#endif // APPMANAGER_H
