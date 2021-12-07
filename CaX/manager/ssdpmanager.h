#ifndef SSDPMANAGER_H
#define SSDPMANAGER_H

#include <QObject>

class UDPClient;

class SSDPManager : public QObject
{
	Q_OBJECT

public:

	explicit SSDPManager(QObject *parent = nullptr);
	~SSDPManager();

	void RequestDeviceList();
	void RequestDevicePowerOn(QString strWolAddr, QString strMac);

	UDPClient *GetUdpClient() const;
	void SetUdpClient(UDPClient *pUdpClient);

	void ResetUdpClient();

private:

	void ConnectToSlot();
	QString MakeStringMSearch();

	UDPClient *m_pUdpClient;

};

#endif // SSDPMANAGER_H
