#include "devicemanager.h"

#include "util/log.h"
#include "util/caxconstants.h"


DeviceManager::DeviceManager(QObject *parent)
	: QObject(parent)
	, m_NodeDevice(CJsonNode(JSON_OBJECT))
	, m_NodeWol(CJsonNode(JSON_OBJECT))
{

}

DeviceManager::~DeviceManager()
{
	m_NodeDevice.Clear();
	m_NodeWol.Clear();
}

QString DeviceManager::FindDeviceInfo(QString strDeviceInfo, QString strPrefix)
{
	QString strDeviceValue;
	int index = strDeviceInfo.lastIndexOf(strPrefix);
	if (index >= 0)
	{
		index = strDeviceInfo.length() - index - QString(strPrefix).length() - 1;
		strDeviceValue = strDeviceInfo.right(index);
	}
	else
	{
		strDeviceValue.clear();
	}

	return strDeviceValue;
}

bool DeviceManager::AddDevice(QString strMac, QString strAddr, QString strCaName, QString strCaDev)
{
	bool bAdd = false;

	CJsonNode nodeCheck;
	if (m_NodeDevice.GetObject(strMac, nodeCheck))
	{
		bAdd = false;
//		LogDebug("nodeCheck [%s]", nodeCheck.ToCompactByteArray().data());
	}
	else
	{
		CJsonNode nodeDevice(JSON_OBJECT);
		nodeDevice.Add(DEVICE_MAC, strMac);
		nodeDevice.Add(DEVICE_ADDR, strAddr);
		nodeDevice.Add(DEVICE_VAL, strCaName);
		nodeDevice.Add(DEVICE_DEV, strCaDev);
		m_NodeDevice.Add(strMac, nodeDevice);

		bAdd = true;
	}

	return bAdd;
}

bool DeviceManager::DelDevice(QString strMac)
{
	bool bChange = false;

	if (!m_NodeDevice.IsNull() && !m_NodeDevice.GetObject(strMac).IsNull())
	{
		m_NodeDevice.Del(strMac);
		bChange = true;
	}
	else
		bChange = false;

	return bChange;
}


void DeviceManager::InitDeviceList(int nState)
{
	bool bSelect = (nState == 1) ? true : false;

	QStringList listStrMac;
	QList<CJsonNode> listNodeDev;

	QString strAddr;
	QString strMac;
	QString strVal;
	QString strDev;

	int nCount = m_NodeDevice.GetObjectList(listStrMac, listNodeDev);
	for (int i = 0; i < nCount; i++)
	{
		listNodeDev[i].GetString(DEVICE_ADDR, strAddr);
		listNodeDev[i].GetString(DEVICE_MAC, strMac);
		listNodeDev[i].GetString(DEVICE_VAL, strVal);
		listNodeDev[i].GetString(DEVICE_DEV, strDev);

		LogDebug("[%s][%s][%s][%s]"
					 , strAddr.toUtf8().data()
					 , strMac.toUtf8().data()
					 , strVal.toUtf8().data()
					 , strDev.toUtf8().data());
	}

	emit SigInitDeviceList(bSelect);
}


QString DeviceManager::GetDevValue(QString strMac, QString strKey)
{
	QString strValue;
	CJsonNode nodeCheck;
	if (m_NodeDevice.GetObject(strMac, nodeCheck))
	{
		nodeCheck.GetString(strKey, strValue);
		LogDebug("GetDevValue [%s][%s]", strKey.toUtf8().data(), strValue.toUtf8().data());
	}

	return strValue;

}

QString DeviceManager::GetWolValue(QString strMac, QString strKey)
{
	QString strValue;
	CJsonNode nodeDevice(JSON_OBJECT);;

	if (m_NodeWol.GetObject(strMac, nodeDevice))
	{
		nodeDevice.GetString(strKey, strValue);
		LogDebug("GetWolValue [%s][%s]", strKey.toUtf8().data(), strValue.toUtf8().data());
	}

	return strValue;

}

bool DeviceManager::AddWolDevice(QString strMac, QString strVersion, QString strWolAddr, QString strUUID)
{
	QString devAddr = GetDevValue(strMac, DEVICE_ADDR);
	QString devName = GetDevValue(strMac, DEVICE_DEV);

	if ( devAddr.isEmpty() || devName.isEmpty() )	return false;

	bool bAdd = false;
	CJsonNode nodeCheck;
	if (m_NodeWol.GetObject(strMac, nodeCheck))
	{
		bAdd = false;
//		LogDebug("nodeCheck [%s]", nodeCheck.ToCompactByteArray().data());
	}
	else
	{
		CJsonNode nodeDevice(JSON_OBJECT);
		nodeDevice.Add(DEVICE_ADDR, devAddr);
		nodeDevice.Add(DEVICE_MAC, strMac);
		nodeDevice.Add(DEVICE_VAL, devName);
		nodeDevice.Add(DEVICE_APP, true);
		nodeDevice.Add(DEVICE_MAC, strMac);
		nodeDevice.Add(DEVICE_VERSION, strVersion);
		nodeDevice.Add(DEVICE_WOL_ADDR, strWolAddr);
		nodeDevice.Add(DEVICE_UUID, strUUID);
		m_NodeWol.Add(strMac, nodeDevice);

		bAdd = true;
	}

	return bAdd;
}

bool DeviceManager::DelWolDevice(QString strMac)
{
	bool bChange = false;

	if (m_NodeWol.Del(strMac))	bChange = true;
	else						bChange = false;

	return bChange;
}

///////////////////////////////////////////////////////////
// Slots
///////////////////////////////////////////////////////////
void DeviceManager::SlotRespDeviceInfo(QString deviceData)
{
	QStringList arrDeviceInfo = QString(deviceData).split(SSDP_CRLF);

	QString strAddr = "";
	QString strMac = "";
	QString strCaName = "";
	QString strCaDev = "";

	bool	bWebServer = false;
	int		nState = 0;

	foreach (QString strDeviceInfo, arrDeviceInfo)
	{
		if (strDeviceInfo.startsWith(SSDP_NT))
		{
			if (strDeviceInfo.lastIndexOf(SSDP_ST_DATA) >= 0)
			{
				bWebServer = true;
			}
		}
		else if (strDeviceInfo.startsWith(SSDP_ST))
		{
			if (strDeviceInfo.lastIndexOf(SSDP_ST_DATA) >= 0)
			{
				bWebServer = true;
				nState = 1;
			}
		}
		else if (strDeviceInfo.startsWith(SSDP_NTS))
		{
			if (strDeviceInfo.lastIndexOf(SSDP_NTS_DATA) >= 0)		nState = 1;
			else													nState = 2;
		}
		else if (strDeviceInfo.startsWith(SSDP_LOCATION))			strAddr = FindDeviceInfo(strDeviceInfo, SSDP_LOCATION);
		else if (strDeviceInfo.startsWith(SSDP_MAC_ADDR))			strMac = FindDeviceInfo(strDeviceInfo, SSDP_MAC_ADDR);
		else if (strDeviceInfo.startsWith(SSDP_CA_NAME))			strCaName = FindDeviceInfo(strDeviceInfo, SSDP_CA_NAME);
		else if (strDeviceInfo.startsWith(SSDP_CA_DEVICE))			strCaDev = FindDeviceInfo(strDeviceInfo, SSDP_CA_DEVICE);

	}

	if (!bWebServer
		|| nState == 0
		|| strAddr.isEmpty()
		|| strMac.isEmpty()
		|| strCaName.isEmpty()
		|| strCaDev.isEmpty())
	{
		LogDebug("is not CaX device...");
		return;
	}

	// add or delete device
	if (nState == 1 && !AddDevice(strMac, strAddr, strCaName, strCaDev))
	{
//		LogDebug("already add or fail to add");
		return;
	}
	else if (nState == 2 && !DelDevice(strMac))
	{
//		LogDebug("already delete or fail to delete");
		return;
	}

	// change device list
	InitDeviceList(nState);
}

int DeviceManager::GetNodeDeviceCount()
{

	QStringList listStrMac;
	QList<CJsonNode> listNodeDev;

	int nCount = m_NodeDevice.GetObjectList(listStrMac, listNodeDev);
	return nCount;
}
