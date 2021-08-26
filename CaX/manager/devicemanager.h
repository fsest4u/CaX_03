#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>

#include <QMap>
#include <QVariant>

#include "util/CJsonNode.h"

class DeviceManager :
	public QObject
{
	Q_OBJECT

public:

	explicit DeviceManager(QObject *parent = 0);
	~DeviceManager();

	CJsonNode		GetNodeDevice()					{ return m_NodeDevice; }
	int				GetNodeDeviceCount();

	CJsonNode		GetNodeWol()					{ return m_NodeWol; }
	void			SetNodeWol(CJsonNode nodeWol)	{ m_NodeWol = nodeWol; }

	QString			GetDevValue(QString strMac, QString strKey);
	QString			GetWolValue(QString strMac, QString strKey);

	bool			DelDevice(QString strMac);

	bool			AddWolDevice(QString strMac, QString strVersion, QString strWolAddr, QString strUUID);
	bool			DelWolDevice(QString strMac);

signals:

	void			SigInitDeviceList(bool bSelect);

public slots:

	void			SlotRespDeviceInfo(QString deviceData);

private:

	QString			FindDeviceInfo(QString strDeviceInfo, QString strPrefix);
	bool			AddDevice(QString strMac, QString strAddr, QString strCaName, QString strCaDev);
	void			InitDeviceList(int nState);

	CJsonNode		m_NodeDevice;
	CJsonNode		m_NodeWol;

};

#endif // DEVICEMANAGER_H
