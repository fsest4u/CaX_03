#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>

#include "util/CJsonNode.h"

class SSDPManager;

class DeviceManager :
	public QObject
{
	Q_OBJECT

public:

	explicit DeviceManager(QObject *parent = 0);
	~DeviceManager();

	enum {
		DEVICE_NONE = -1,
		DEVICE_ADD = 0,
		DEVICE_DEL,
		DEVICE_MAX
	};

	void RequestDeviceList();
	void RequestDevicePowerOn(QString wolAddr, QString mac);

	// device list
	void			ClearDeviceList();
	CJsonNode		GetDeviceList() const;
	void			SetDeviceList(const CJsonNode &list);
	int				GetDeviceCount();

	void			AddDevice(QString mac, QString addr, QString caName, QString caDev);
	void			DelDevice(int index);
	int				CheckDevice(QString mac);

	QString			GetDeviceValue(QString mac, QString key);

	// wol list
	void			ClearDeviceListWol();
	CJsonNode		GetDeviceListWol() const;
	void			SetDeviceListWol(const CJsonNode &list);

	void			AddDeviceWol(QString mac, QString addrWol, QString version = "", QString uuid = "");
	void			DelDeviceWol(int index);
	int				CheckDeviceWol(QString mac);

	QString			GetDeviceValueWol(QString mac, QString key);

signals:

	void			SigDeviceItem(int state);
	void			SigAutoConnectDevice(QString mac, QString addr, QString val, QString dev);

public slots:

	void			SlotRespDeviceItem(QString deviceData);

private:

	SSDPManager		*m_pSsdpMgr;

	QString			GetValue(QString deviceItem, QString key);

	CJsonNode		m_DeviceList;
	CJsonNode		m_DeviceListWol;

};

#endif // DEVICEMANAGER_H
