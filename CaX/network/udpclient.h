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
	void SlotSSDPReadData();

	// for msearch
	void SlotMSearchReadData();

	// for wol
	void SlotWolReadData();

private:

	void ConnectSigToSlot();
	QNetworkInterface CheckIP();
	QMap<QString, QNetworkInterface> GetInterface();

	QHostAddress	m_HostAddress;

//	QList<QUdpSocket*>	m_SocketSSDPList;

	QUdpSocket		*m_pSocketSSDP;
	QUdpSocket		*m_pSocketMSearch;
	QUdpSocket		*m_pSocketWol;

};

#endif // UDPCLIENT_H
