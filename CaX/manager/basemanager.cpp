#include "basemanager.h"

#include "network/tcpclient.h"
#include "util/log.h"

BaseManager::BaseManager(QObject *parent)
	: QObject(parent)
	, m_pTcpClient(new TCPClient(this))
{

}

BaseManager::~BaseManager()
{
	if (m_pTcpClient)
	{
		delete m_pTcpClient;
		m_pTcpClient = nullptr;
	}
}

QString BaseManager::GetAddr() const
{
	return m_pTcpClient->GetAddr();
}

void BaseManager::SetAddr(const QString &Addr)
{
	m_pTcpClient->SetAddr(Addr);
}

TCPClient *BaseManager::GetTcpClient() const
{
	return m_pTcpClient;
}

void BaseManager::SetTcpClient(TCPClient *pTcpClient)
{
	m_pTcpClient = pTcpClient;
}

void BaseManager::RequestCommand(CJsonNode node, int nCmdID, int nIndex)
{
	if (!m_pTcpClient)
	{
		LogCritical("todo is not ready");
		return;
	}

	QByteArray jsonData = node.ToCompactByteArray();
	m_pTcpClient->RequestCommand(jsonData, nCmdID, nIndex);

}

void BaseManager::RequestCoverArt(QString fullpath, int nIndex, int nMode)
{
	if (fullpath.isEmpty())
	{
		LogCritical("path is empty");
		return;
	}
	if (!m_pTcpClient)
	{
		LogCritical("tcp is null");
		return;
	}
	m_pTcpClient->RequestCoverArt(fullpath, nIndex, nMode);
}

void BaseManager::RequestCoverArt(QString fullpath)
{
	if (fullpath.isEmpty())
	{
		LogCritical("path is empty");
		return;
	}
	if (!m_pTcpClient)
	{
		LogCritical("tcp is null");
		return;
	}
	m_pTcpClient->RequestCoverArt(fullpath);
}

