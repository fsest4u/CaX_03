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

#if 1
	if( !m_pSocketSSDP->bind(QHostAddress::AnyIPv4, SSDP_PORT, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint) )
#else	// for test
	QHostAddress addr = QHostAddress("192.168.0.100");
	if( !m_pSocketSSDP->bind(addr, SSDP_PORT, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint) )
#endif
	{
		LogCritical("%s", m_pSocketSSDP->errorString().toUtf8().data());
		CloseSocketSSDP();
		return false;
	}

#if 0
	if( !m_pSocketSSDP->joinMulticastGroup(m_HostAddress) )
#else	// for test
	QNetworkInterface interface = CheckInterface();
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

	m_pSocketSSDP->setSocketOption(QAbstractSocket::LowDelayOption, 1);

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
		QString hardwareAddress = macList.at(i).hardwareAddress();       // MAC
		LogDebug("hardwareAddress : [%s]", hardwareAddress.toUtf8().data());
		if(hardwareAddress != "" )  // at windows os
//		if(hardwareAddress != "00:00:00:00:00:00") // at linux os
		{
			macAddress = hardwareAddress;
			LogDebug("macAddress :[%s] ", macAddress.toUtf8().data());
			break;
		}
	}

	QNetworkInterface myInterface;
	for (int i = 0; i < ipList.size(); i++)
	{
		LogDebug("i [%d] ipList.at(i) [%s] toIPv4Address [%u]", i, ipList.at(i).toString().toUtf8().data(), ipList.at(i).toIPv4Address());
		myInterface = interface.interfaceFromIndex(i);
		if (ipList.at(i) != QHostAddress::LocalHost && ipList.at(i).toIPv4Address())
		{
			ipAddress = ipList.at(i).toString();
			LogDebug("ipAddress :[%s] ", ipAddress.toUtf8().data());
			break;
		}
	}

	return myInterface;

}

///////////////////////////////////////////////////////////////////////////////////////////
// https://stackoverflow.com/questions/19218994/qt-joinmulticastgroup-for-all-interface
///////////////////////////////////////////////////////////////////////////////////////////
QNetworkInterface UDPClient::CheckInterface()
{
//	// Windows is weird
//	QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
//	// Try the multicast UDP socket for SA first
//	saMC = new QUdpSocket(this);
//	saMC->setSocketOption(QAbstractSocket::LowDelayOption, 1);
//	//saMC->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
//	connect(saMC, &QUdpSocket::readyRead, this, &Commander::handleDatagram, Qt::UniqueConnection);
//	QHostAddress toBind("192.168.0.51");
//	if(saMC->bind(toBind, 6969, QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint))
//	{
//		foreach(QNetworkInterface interface, interfaces)
//		{
//			//qDebug()<<"interface:"<<interface.isValid()<<interface.flags();
//			QNetworkInterface::InterfaceFlags iflags = interface.flags();
//			if(interface.isValid() && !iflags.testFlag(QNetworkInterface::IsLoopBack) && iflags.testFlag(QNetworkInterface::CanMulticast) && iflags.testFlag(QNetworkInterface::IsRunning))
//			{

//				QList<QNetworkAddressEntry> addressEntries = interface.addressEntries();
//				for (int i = 0; i < addressEntries.length(); i++) {
//					QNetworkAddressEntry ae = addressEntries.at(i);
//					if(ae.ip() == toBind)
//					{
//						bool ok = false;
//						if (ae.ip().protocol() == QAbstractSocket::IPv4Protocol)
//						{
//							ok = saMC->joinMulticastGroup(QHostAddress("239.2.3.1"), interface);
//						}
//						if(ok)
//						{
//							qDebug()<<"SA bound...join mc group:"<<ae.ip();
//						}
//						else
//						{
//							qDebug()<<"SA bound...interface unsuitable for Multicast:"<<ae.ip();
//						}
//					}
//				}
//			}
//		}
//	}
//	else
//	{
//		qDebug()<<"SA multicast socket unable to bind...."<<saMC->errorString();
//	}

	// Windows is weird
	QNetworkInterface myInterface;
	QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

	foreach(QNetworkInterface interface, interfaces)
	{
		//qDebug()<<"interface:"<<interface.isValid()<<interface.flags();
		QNetworkInterface::InterfaceFlags iflags = interface.flags();
		if(interface.isValid()
				&& !iflags.testFlag(QNetworkInterface::IsLoopBack)
				&& iflags.testFlag(QNetworkInterface::CanMulticast)
				&& iflags.testFlag(QNetworkInterface::IsRunning))
		{
			QList<QNetworkAddressEntry> addressEntries = interface.addressEntries();
			for (int i = 0; i < addressEntries.length(); i++)
			{
				QNetworkAddressEntry ae = addressEntries.at(i);
				if (ae.ip().protocol() == QAbstractSocket::IPv4Protocol
						&& ae.ip() != QHostAddress::LocalHost
						&& ae.ip().toIPv4Address())
				{
					LogDebug("ipAddress :[%s] ", ae.ip().toString().toUtf8().data());
					myInterface = interface;
					break;
				}
			}
		}
	}

	return myInterface;
}
