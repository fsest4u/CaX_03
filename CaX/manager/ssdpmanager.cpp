#include "ssdpmanager.h"

#include "network/udpclient.h"

#include "util/caxconstants.h"
#include "util/log.h"


SSDPManager::SSDPManager(QObject *parent)
	: QObject(parent)
	, m_pUdpClient(new UDPClient())
{
	ConnectToSlot();
}

SSDPManager::~SSDPManager()
{
	if (m_pUdpClient)
	{
		delete m_pUdpClient;
		m_pUdpClient = nullptr;
	}
}

void SSDPManager::ConnectToSlot()
{

}

void SSDPManager::RequestDeviceInfo()
{
	QString strSearch = MakeStringMSearch();
	m_pUdpClient->BindSocketSSDP();
	m_pUdpClient->SendSocketMSearch(strSearch);
}

QString SSDPManager::MakeStringMSearch()
{
	//QString SendString = QString("M-SEARCH * HTTP/1.1\r\nHost: %1:%2\r\nMAN: \"ssdp:discover\"\r\nST: %3\r\nMX: 3\r\n\r\n").arg(SSDP_ADDR).arg(SSDP_PORT).arg(SSDP_ST_DATA);
	QString strSearch =
		QString("M-SEARCH * HTTP/1.1")							+ SSDP_CRLF
		+ QString("Host: %1:%2").arg(SSDP_ADDR).arg(SSDP_PORT)	+ SSDP_CRLF
		+ QString("MAN: \"ssdp:discover\"")						+ SSDP_CRLF
		+ QString("ST: %1").arg(SSDP_ST_DATA)					+ SSDP_CRLF
		+ QString("MX: 3")										+ SSDP_CRLF + SSDP_CRLF;

	return strSearch;
}

UDPClient *SSDPManager::GetUdpClient() const
{
	return m_pUdpClient;
}

void SSDPManager::SetUdpClient(UDPClient *pUdpClient)
{
	m_pUdpClient = pUdpClient;
}

void SSDPManager::SlotPowerOnDevice(QString strWolAddr, QString strMac)
{
	m_pUdpClient->SendSocketWol(strWolAddr, strMac);
}

