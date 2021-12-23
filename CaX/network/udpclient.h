#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

class UDPClient : public QObject
{
	Q_OBJECT

public:

	explicit UDPClient(QObject *parent = nullptr);
	~UDPClient();

	void CloseSocketSSDP();
	void CloseSocketMSearch();
	void CloseSocketWol();

	bool BindSocketSSDP();
	bool SendSocketMSearch(QString strSearch);
	bool SendSocketWol(QString strWolAddr, QString strMac);

signals:

	void SigRespDeviceItem(QString deviceInfo);

public slots:

	// for ssdp
	void SlotSSDPConnected();
	void SlotSSDPDisconnected();
	void SlotSSDPBytesWritten();
	void SlotSSDPReadData();

	// for msearch
	void SlotMSearchConnected();
	void SlotMSearchDisconnected();
	void SlotMSearchBytesWritten();
	void SlotMSearchReadData();

	// for wol
	void SlotWolConnected();
	void SlotWolDisconnected();
	void SlotWolBytesWritten();
	void SlotWolReadData();

private:

	void ConnectSigToSlot();
	QNetworkInterface CheckIP();
	QNetworkInterface CheckInterface();

	QHostAddress	m_HostAddress;
	QUdpSocket		*m_pSocketSSDP;
	QUdpSocket		*m_pSocketMSearch;
	QUdpSocket		*m_pSocketWol;

};

#endif // UDPCLIENT_H
