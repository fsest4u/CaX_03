#include "devicemanager.h"

#include "manager/ssdpmanager.h"
#include "network/udpclient.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"


DeviceManager::DeviceManager(QObject *parent) :
	QObject(parent),
	m_pSsdpMgr(new SSDPManager)
{
	m_DeviceList.Clear();
	m_DeviceListWol.Clear();

	connect(m_pSsdpMgr->GetUdpClient(), SIGNAL(SigRespDeviceItem(QString)),	this, SLOT(SlotRespDeviceItem(QString)));

}

DeviceManager::~DeviceManager()
{
	m_DeviceList.Clear();
	m_DeviceListWol.Clear();

	if (m_pSsdpMgr)
	{
		delete m_pSsdpMgr;
		m_pSsdpMgr = nullptr;
	}
}

void DeviceManager::RequestDeviceInfo()
{
	m_pSsdpMgr->RequestDeviceInfo();
}

CJsonNode DeviceManager::GetDeviceList() const
{
	return m_DeviceList;
}

void DeviceManager::SetDeviceList(const CJsonNode &list)
{
	m_DeviceList = list;
}

int DeviceManager::GetDeviceCount()
{
	return m_DeviceList.ArraySize();
}

void DeviceManager::AddDevice(QString mac, QString addr, QString caName, QString caDev)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_MAC, mac);
	node.Add(KEY_ADDR, addr);
	node.Add(KEY_VAL, caName);
	node.Add(KEY_DEV, caDev);

	m_DeviceList.AppendArray(node);

}

void DeviceManager::DelDevice(int index)
{
	m_DeviceList.RemoveArray(index);
}

int DeviceManager::CheckDevice(QString mac)
{
	int index = -1;
	for (int i = 0; i < m_DeviceList.ArraySize(); i++)
	{
		if (!mac.compare(m_DeviceList.GetArrayAt(i).GetString(KEY_MAC)))
		{
			index = i;
			break;
		}
	}

	return index;
}

QString DeviceManager::GetDeviceValue(QString mac, QString key)
{
	QString value;
	int index = CheckDevice(mac);
	CJsonNode node = m_DeviceList.GetArrayAt(index);
	if (!node.IsNull())
	{
		value = node.GetString(key);
	}
	return value;
}

CJsonNode DeviceManager::GetDeviceListWol() const
{
	return m_DeviceListWol;
}

void DeviceManager::SetDeviceListWol(const CJsonNode &list)
{
	m_DeviceListWol = list;
}

void DeviceManager::AddDeviceWol(QString mac, QString version, QString addrWol, QString uuid)
{
	QString devAddr = GetDeviceValue(mac, DEVICE_ADDR);
	QString devName = GetDeviceValue(mac, DEVICE_DEV);

	CJsonNode node(JSON_OBJECT);
	node.Add(DEVICE_ADDR, devAddr);
	node.Add(DEVICE_MAC, mac);
	node.Add(DEVICE_VAL, devName);
	node.Add(DEVICE_APP, true);
	node.Add(DEVICE_VERSION, version);
	node.Add(DEVICE_WOL_ADDR, addrWol);
	node.Add(DEVICE_UUID, uuid);

	m_DeviceListWol.AppendArray(node);
}

void DeviceManager::DelDeviceWol(int index)
{
	m_DeviceListWol.RemoveArray(index);
}

int DeviceManager::CheckDeviceWol(QString mac)
{
	int index = -1;
	for (int i = 0; i < m_DeviceListWol.ArraySize(); i++)
	{
		if (!mac.compare(m_DeviceListWol.GetArrayAt(i).GetString(KEY_MAC)))
		{
			index = i;
			break;
		}
	}

	return index;
}

QString DeviceManager::GetDeviceValueWol(QString mac, QString key)
{
	QString value;
	int index = CheckDeviceWol(mac);
	CJsonNode node = m_DeviceListWol.GetArrayAt(index);
	if (!node.IsNull())
	{
		value = node.GetString(key);
	}
	return value;
}



///////////////////////////////////////////////////////////
// Slots
///////////////////////////////////////////////////////////
void DeviceManager::SlotRespDeviceItem(QString deviceData)
{
	QStringList deviceItems = QString(deviceData).split(SSDP_CRLF);

	QString strAddr = "";
	QString strMac = "";
	QString strCaName = "";
	QString strCaDev = "";

	bool	bWebServer = false;
	int 	state = DEVICE_NONE;

	foreach (QString deviceItem, deviceItems)
	{
		if (deviceItem.startsWith(SSDP_NT))
		{
			if (deviceItem.lastIndexOf(SSDP_ST_DATA) >= 0)
			{
				bWebServer = true;
			}
		}
		else if (deviceItem.startsWith(SSDP_ST))
		{
			if (deviceItem.lastIndexOf(SSDP_ST_DATA) >= 0)
			{
				bWebServer = true;
				state = DEVICE_ADD;
			}
		}
		else if (deviceItem.startsWith(SSDP_NTS))
		{
			if (deviceItem.lastIndexOf(SSDP_NTS_DATA) >= 0)
				state = DEVICE_ADD;
			else
				state = DEVICE_DEL;
		}
		else if (deviceItem.startsWith(SSDP_LOCATION))
			strAddr = GetValue(deviceItem, SSDP_LOCATION);
		else if (deviceItem.startsWith(SSDP_MAC_ADDR))
			strMac = GetValue(deviceItem, SSDP_MAC_ADDR);
		else if (deviceItem.startsWith(SSDP_CA_NAME))
			strCaName = GetValue(deviceItem, SSDP_CA_NAME);
		else if (deviceItem.startsWith(SSDP_CA_DEVICE))
			strCaDev = GetValue(deviceItem, SSDP_CA_DEVICE);

	}

	if (!bWebServer
		|| state <= DEVICE_NONE
		|| strAddr.isEmpty()
		|| strMac.isEmpty()
		|| strCaName.isEmpty()
		|| strCaDev.isEmpty())
	{
//		LogDebug("is not CaX device...");
		return;
	}

	// add or delete device
	if (state == DEVICE_ADD)
	{
		int index = CheckDevice(strMac);
		if (index < 0)
		{
			AddDevice(strMac, strAddr, strCaName, strCaDev);
			emit SigDeviceItem(state);
		}
	}
	else if (state == DEVICE_DEL)
	{
		int index = CheckDevice(strMac);
		if (index >= 0)
		{
			DelDevice(index);
			emit SigDeviceItem(state);
		}
	}

}

QString DeviceManager::GetValue(QString deviceItem, QString key)
{
	QString value;
	int index = deviceItem.lastIndexOf(key);
	if (index >= 0)
	{
		index = deviceItem.length() - index - QString(key).length() - 1;
		value = deviceItem.right(index);
	}
	else
	{
		value.clear();
	}

	return value;
}
