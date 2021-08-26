#include "observerclient.h"

#include "util/log.h"
//#include "util/QHttpHeader.h"

#define CRLF_DOUBLE				"\r\n\r\n"

ObserverClient::ObserverClient(QObject *parent) : QObject(parent)
{
	m_nContentSize = -1;

	m_Socket = new QTcpSocket();

	ConnectSigToSlot();

}

ObserverClient::~ObserverClient()
{
	Disconnect();
	if (m_Socket)
	{
		delete m_Socket;
		m_Socket = nullptr;
	}

}

void ObserverClient::ConnectSigToSlot()
{
	connect(m_Socket, SIGNAL(connected()),				this, SLOT(SlotConnect())				);
	connect(m_Socket, SIGNAL(disconnected()),			this, SLOT(SlotDisconnect())			);
	connect(m_Socket, SIGNAL(bytesWritten(qint64)),		this, SLOT(SlotWrite(qint64))	);
	connect(m_Socket, SIGNAL(readyRead()),				this, SLOT(SlotRead())	);
}

void ObserverClient::Connect(QString strAddr, QByteArray jsonData)
{
	m_nContentSize = jsonData.size();

	QStringList lsAddr = strAddr.split(":");
	m_StrAddr = lsAddr[0];
	m_nPort = lsAddr[1].toInt();

	m_Socket->connectToHost(m_StrAddr, m_nPort);
	m_Socket->setSocketOption(QAbstractSocket::KeepAliveOption, true);
	if (!m_Socket->waitForConnected(3000))
	{
		LogCritical("%s", m_Socket->errorString().toUtf8().data());
	}
	if (!m_Socket->waitForBytesWritten(3000))
	{
		LogCritical("%s", m_Socket->errorString().toUtf8().data());
	}

	m_Socket->write(jsonData);
	if (!m_Socket->waitForBytesWritten(3000))
	{
		LogCritical("%s", m_Socket->errorString().toUtf8().data());
	}
	LogDebug("Socket Data : [%s]", jsonData.data());
}

void ObserverClient::Disconnect()
{
	if (m_Socket)
	{
		m_Socket->close();
	}
}

void ObserverClient::SlotConnect()
{
//	QHttpRequestHeader qHeader;
//	qHeader.setRequest("POST", "/");
//	qHeader.setContentLength(m_nContentSize);

//	m_Socket->write(qHeader.toString().toUtf8());

//	LoggingDebug("~~~~~~~~~~~~~~~~~~~~~~ [%s]", qHeader.toString().toUtf8().data());
	//"~~~~~~~~~~~~~~~~~~~~~~ [POST / HTTP/1.1\r\ncontent-length: 18\r\n\r\n]"
	m_Socket->write(QString("POST / HTTP/1.1\r\ncontent-length: %1\r\n\r\n").arg(m_nContentSize).toUtf8());
}

void ObserverClient::SlotDisconnect()
{
	LogDebug("DISCONNECTED");
	emit SigDisconnect();
}

void ObserverClient::SlotWrite(qint64 bytes)
{
	LogDebug("Bytes Written [size :%d]", bytes);
}

void ObserverClient::SlotRead()
{
	if (true) //(m_Socket->error() < QAbstractSocket::SocketError::ConnectionRefusedError)
	{
		QString contents = QString::fromUtf8(m_Socket->readAll());
		LogDebug("Network Success : [%s]", contents.toUtf8().data());
		QStringList resList = contents.split("HTTP/");
		QStringList jsonValueList;

		foreach (QString resData, resList)
		{
//			QHttpResponseHeader qHeader(resData);
//			int len = qHeader.contentLength();
//			QString type = qHeader.contentType();

			int index = resData.indexOf(CRLF_DOUBLE);
			if (index >= 0)
			{
				index = resData.length() - index - sizeof(CRLF_DOUBLE) + 1;
				jsonValueList.append(resData.right(index));
			}
			else
			{
				jsonValueList.clear();
			}
		}

		if (jsonValueList.isEmpty())
		{
			LogCritical("## Parsing Error");
		}
		else
		{
			emit SigConnect(jsonValueList);
		}

	}
	else
	{
		LogCritical("%s", m_Socket->errorString().toUtf8().data());
	}

	// observer is not disconnect
	//	m_Socket->close();

}
