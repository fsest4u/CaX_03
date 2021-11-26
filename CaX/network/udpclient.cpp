#include "udpclient.h"

#include <QtNetwork>

#include "util/caxconstants.h"
#include "util/log.h"

UDPClient::UDPClient(QObject *parent)
	: QObject(parent)
	, m_pSocketSSDP(new QUdpSocket(this))
	, m_pSocketMSearch(new QUdpSocket(this))
	, m_pSocketWol(new QUdpSocket(this))
{
	ConnectSigToSlot();
}

UDPClient::~UDPClient()
{
	CloseSocketSSDP();
	CloseSocketMSearch();
	CloseSocketWol();
}

void UDPClient::CloseSocketSSDP()
{
	if( m_pSocketSSDP )
	{
		LogDebug("########## CloseSocketSSDP ");
		m_pSocketSSDP->leaveMulticastGroup(m_HostAddress);
		m_pSocketSSDP->close();
		delete m_pSocketSSDP;
		m_pSocketSSDP = nullptr;
	}
}

void UDPClient::CloseSocketMSearch()
{
	if( m_pSocketMSearch )
	{
		LogDebug("########## CloseSocketMSearch ");
		m_pSocketMSearch->close();
		delete m_pSocketMSearch;
		m_pSocketMSearch = nullptr;
	}
}

void UDPClient::CloseSocketWol()
{
	if (m_pSocketWol)
	{
		LogDebug("########## CloseSocketWol ");
		m_pSocketWol->close();
		delete m_pSocketWol;
		m_pSocketWol = nullptr;
	}
}

bool UDPClient::BindSocketSSDP()
{
	m_HostAddress = QHostAddress(QStringLiteral(SSDP_ADDR));

	if( !m_pSocketSSDP->bind(QHostAddress::AnyIPv4, SSDP_PORT, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint) )
	{
		LogCritical("%s", m_pSocketSSDP->errorString().toUtf8().data());
		CloseSocketSSDP();
		return false;
	}

#if 1
	if( !m_pSocketSSDP->joinMulticastGroup(m_HostAddress) )
#else
	QNetworkInterface interface = CheckIP();
	if( !m_pSocketSSDP->joinMulticastGroup(m_HostAddress, interface) )
#endif
	{
		LogCritical("%s", m_pSocketSSDP->errorString().toUtf8().data());
		CloseSocketSSDP();
		return false;
	}

	return true;
}

bool UDPClient::SendSocketMSearch(QString strSearch)
{
	m_HostAddress = QHostAddress(QStringLiteral(SSDP_ADDR));

	if( !m_pSocketMSearch->bind(m_HostAddress, SSDP_PORT, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint) )
	{
		LogCritical("%s", m_pSocketMSearch->errorString().toUtf8().data());
		CloseSocketMSearch();
		return false;
	}

	m_pSocketMSearch->writeDatagram(strSearch.toUtf8(), m_HostAddress, SSDP_PORT);
	if( !m_pSocketMSearch->waitForDisconnected(3000) )
	{
		LogCritical("%s", m_pSocketMSearch->errorString().toUtf8().data());
		CloseSocketMSearch();
		return false;
	}

	return true;
}

bool UDPClient::SendSocketWol(QString strWolAddr, QString strMac)
{
	m_HostAddress = QHostAddress(strWolAddr);

	//if (!m_pSocketWol->bind(m_HostAddress, WOL_PORT, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint))
	//{
	//	CloseSocketWol();
	//	return false;
	//}

	// QString to hex
	unsigned char macAddress[6];
	sscanf(strMac.toLatin1().data(),
		"%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
		&macAddress[0],
		&macAddress[1],
		&macAddress[2],
		&macAddress[3],
		&macAddress[4],
		&macAddress[5]);

	// Build the packet
	std::string packet;
	packet.reserve(102);
	for (int i = 0; i < 6; ++i) {
		packet.push_back(0xff);
	}
	for (int i = 0; i < 16; ++i) {
		for (unsigned int j = 0; j < 6; ++j) {
			packet.push_back(macAddress[j]);
		}
	}

	m_pSocketWol->writeDatagram(packet.c_str(), 102, m_HostAddress, WOL_PORT);
	if (!m_pSocketWol->waitForDisconnected(3000))
	{
		LogCritical("%s", m_pSocketWol->errorString().toUtf8().data());
		CloseSocketWol();
		return false;
	}

	return true;
}

void UDPClient::SlotSSDPReadData()
{
	QByteArray ssdpData;
	QHostAddress sender;
	quint16 senderPort;

//	LogDebug("########## SlotSSDPReadData");
	while( m_pSocketSSDP->hasPendingDatagrams() )
	{
		ssdpData.resize(m_pSocketSSDP->pendingDatagramSize());

		m_pSocketSSDP->readDatagram(ssdpData.data(), ssdpData.size(), &sender, &senderPort);

//		LogDebug("1 Message from: %s ", sender.toString().toUtf8().data());
//		LogDebug("1 Message from: %d ", senderPort);
//		LogDebug("1 Message from: %s ", ssdpData.data());
	}

	emit SigRespDeviceItem(QString(ssdpData));
}


void UDPClient::SlotMSearchReadData()
{
	LogDebug("########## SlotMSearchReadData");
	while( m_pSocketMSearch->hasPendingDatagrams() )
	{
		QByteArray ssdpData;
		QHostAddress sender;
		quint16 senderPort;

		ssdpData.resize(m_pSocketMSearch->pendingDatagramSize());

		m_pSocketMSearch->readDatagram(ssdpData.data(), ssdpData.size(), &sender, &senderPort);

		LogDebug("2 Message from: %s ", sender.toString().toUtf8().data());
		LogDebug("2 Message from: %d ", senderPort);
		LogDebug("2 Message from: %s ", ssdpData.data());

	}
}

void UDPClient::SlotWolReadData()
{
	LogDebug("########## SlotWolReadData");
	while (m_pSocketWol->hasPendingDatagrams())
	{
		QByteArray ssdpData;
		QHostAddress sender;
		quint16 senderPort;

		ssdpData.resize(m_pSocketWol->pendingDatagramSize());

		m_pSocketWol->readDatagram(ssdpData.data(), ssdpData.size(), &sender, &senderPort);

		LogDebug("3 Message from: %s ", sender.toString().toUtf8().data());
		LogDebug("3 Message from: %d ", senderPort);
		LogDebug("3 Message from: %s ", ssdpData.data());

	}
}




void UDPClient::SlotSSDPConnected()
{
	LogDebug("########## SlotSSDPConnected ");

}
void UDPClient::SlotSSDPDisconnected()
{
	LogDebug("########## SlotSSDPDisconnected ");

}
void UDPClient::SlotSSDPBytesWritten()
{
	LogDebug("########## SlotSSDPBytesWritten ");

}


void UDPClient::SlotMSearchConnected()
{
	LogDebug("########## SlotMSearchConnected ");

}
void UDPClient::SlotMSearchDisconnected()
{
	LogDebug("########## SlotMSearchDisconnected ");

}
void UDPClient::SlotMSearchBytesWritten()
{
	LogDebug("########## SlotMSearchBytesWritten ");

}

void UDPClient::SlotWolConnected()
{
	LogDebug("########## SlotWolConnected ");

}
void UDPClient::SlotWolDisconnected()
{
	LogDebug("########## SlotWolDisconnected ");

}
void UDPClient::SlotWolBytesWritten()
{
	LogDebug("########## SlotWolBytesWritten ");

}


void UDPClient::ConnectSigToSlot()
{
	connect(m_pSocketSSDP, SIGNAL(connected()),				this, SLOT(SlotSSDPConnected()));
	connect(m_pSocketSSDP, SIGNAL(disconnected()),			this, SLOT(SlotSSDPDisconnected()));
	connect(m_pSocketSSDP, SIGNAL(bytesWritten(qint64)),	this, SLOT(SlotSSDPBytesWritten()));
	connect(m_pSocketSSDP, SIGNAL(readyRead()),				this, SLOT(SlotSSDPReadData()));

	connect(m_pSocketMSearch, SIGNAL(connected()),			this, SLOT(SlotMSearchConnected()));
	connect(m_pSocketMSearch, SIGNAL(disconnected()),		this, SLOT(SlotMSearchDisconnected()));
	connect(m_pSocketMSearch, SIGNAL(bytesWritten(qint64)), this, SLOT(SlotMSearchBytesWritten()));
	connect(m_pSocketMSearch, SIGNAL(readyRead()),			this, SLOT(SlotMSearchReadData()));

	connect(m_pSocketWol, SIGNAL(readyRead()), this, SLOT(SlotWolReadData()));

}

QNetworkInterface UDPClient::CheckIP()
{
	QString macAddress;
	QString ipAddress;
	QString Adddress;
	QNetworkInterface interface;
	QList<QNetworkInterface> macList = interface.allInterfaces();
	QList<QHostAddress> ipList = interface.allAddresses();
	for (int i = 0; i < macList.size(); i++)
	{
//		LogDebug("macList hardwareAddress : ", macList.at(i).hardwareAddress().data());
		QString str = macList.at(i).hardwareAddress();       // MAC
		if(str != "" )  // at windows os
//		if(str != "00:00:00:00:00:00") // at linux os
		{
			macAddress = str;
			break;
		}
	}

	QNetworkInterface myInterface;
	for (int i = 0; i < ipList.size(); i++)
	{
//		LogDebug("ipList i : ", i);
//		LogDebug("ipList at(i) : ", ipList.at(i).toString().data());
//		LogDebug("ipList toIPv4Address : ", ipList.at(i).toIPv4Address());
		myInterface = interface.interfaceFromIndex(i);
		if (ipList.at(i) != QHostAddress::LocalHost && ipList.at(i).toIPv4Address())
		{
			ipAddress = ipList.at(i).toString();
			break;
		}
	}

	return myInterface;

}
