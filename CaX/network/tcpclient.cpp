#include <QFile>

#include "tcpclient.h"

#include "util/log.h"

#define CONTENT_TYPE			"application/x-www-form-urlencoded"

TCPClient::TCPClient(QObject *parent)
	: QObject(parent)
	, m_pManager(new QNetworkAccessManager)
{
	QString dirTemp = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
	QString organName = QCoreApplication::organizationName();
	m_DirTemp = dirTemp + "/"+ organName;

	QDir dir(m_DirTemp);
	if (!dir.exists(m_DirTemp))
	{
		if (dir.mkdir(m_DirTemp))
		{
			LogDebug("success to create temp dir...");
		}
	}
}

TCPClient::~TCPClient()
{

	if (m_pManager) {
		delete m_pManager;
		m_pManager = nullptr;
	}
}

void TCPClient::RequestCommand(QByteArray jsonData, int nCmdID)
{
	LogDebug("RequestCmd : [%s]", jsonData.data());

	if (m_Addr.isEmpty())
	{
		LogCritical("address is required");
		return;
	}

	const QUrl url = QUrl::fromUserInput(m_Addr);
	if (url.isEmpty() || !url.isValid())
	{
		LogCritical("url is invalid");
		return;
	}
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, CONTENT_TYPE);

	m_pReply = m_pManager->post(request, jsonData);

	QEventLoop loop;
	connect(m_pReply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	if (m_pReply->error() == QNetworkReply::NoError)
	{

		QString jsonValue = QString::fromUtf8(m_pReply->readAll());
		//LoggingDebug("Network Success : [%s]", jsonValue.toUtf8().data());
		emit SigRespInfo(jsonValue, nCmdID);

	}
	else
	{
		QString err = m_pReply->errorString();
		LogCritical("Network Error : [%s]", err.toUtf8().data());

	}
	m_pReply->deleteLater();

}

void TCPClient::RequestCoverArt(QString fullPath, int nIndex, int nMode)
{
	QString fileName = fullPath;
	int colon = fileName.length() - fileName.indexOf(":") - 1;
	fileName = fileName.right(colon);
	colon = fileName.length() - fileName.indexOf("/") - 1;
	fileName = fileName.right(colon);
	fileName.replace("/", "_");
	fileName = m_DirTemp + "/" + fileName + ".jpg";

	if (fileName.isEmpty() || QFile::exists(fileName))
	{
		LogDebug("fileName is exist [%s]", fileName.toUtf8().data());
		emit SigRespCoverArt(fileName, nIndex, nMode);
		return;
	}

	const QUrl url = QUrl::fromUserInput(fullPath);
	QNetworkRequest request(url);

	int count = m_ListReply.size();
	LogDebug("########### count [%d]", count);

	QNetworkReply *reply = m_pManager->get(request);
	m_ListReply.insert(count, reply);

	QEventLoop loop;
	connect(m_ListReply[count], SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	if (m_ListReply[count]->error() == QNetworkReply::NoError)
	{
		QImage image;
		if (image.loadFromData(m_ListReply[count]->readAll()))
		{
			if (image.save(fileName, "JPG"))
			{
				emit SigRespCoverArt(fileName, nIndex, nMode);
			}
		}
	}
	else
	{
		QString err = m_ListReply[count]->errorString();
		LogCritical("Network Error : [%s] fullPath [%s] fileName [%s]", err.toUtf8().data(), fullPath.toUtf8().data(), fileName.toUtf8().data());
	}
	m_ListReply[count]->deleteLater();

//	connect(m_ListReply[count], &QNetworkReply::finished, [=]() {

//		if (m_ListReply[count]->error() == QNetworkReply::NoError)
//		{
//			QImage image;
//			image.loadFromData(m_ListReply[count]->readAll());
//			if (!image.isNull())
//			{
//				TCPClient::mapImage.insert(fileName, image);
//				emit SigRespCoverArt(fileName, nIndex, nMode);
//			}
//		}
//		else
//		{
//			QString err = m_ListReply[count]->errorString();
//			LogCritical("Network Error : [%s] fullPath [%s] fileName [%s]", err.toUtf8().data(), fullPath.toUtf8().data(), fileName.toUtf8().data());
//		}
//		m_ListReply[count]->deleteLater();
//	});

}

void TCPClient::RequestCoverArt(QString fullPath)
{
	QString newID = fullPath;
	newID.replace(":", "_");
	newID.replace("/", "_");
	QString fileName = m_DirTemp + "/" + newID + ".jpg";

	if (fileName.isEmpty() || QFile::exists(fileName))
	{
		LogDebug("fileName is exist [%s]", fileName.toUtf8().data());
		emit SigRespCoverArt(fileName);
		return;
	}

	const QUrl url = QUrl::fromUserInput(fullPath);
	QNetworkRequest request(url);

	int count = m_ListReply.size();

	QNetworkReply *reply = m_pManager->get(request);
	m_ListReply.insert(count, reply);

	QEventLoop loop;
	connect(m_ListReply[count], SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	if (m_ListReply[count]->error() == QNetworkReply::NoError)
	{
		QImage image;
		if (image.loadFromData(m_ListReply[count]->readAll()))
		{
			if (image.save(fileName, "JPG"))
			{
				emit SigRespCoverArt(fileName);
			}
		}
	}
	else
	{
		QString err = m_ListReply[count]->errorString();
		LogCritical("Network Error : [%s]", err.toUtf8().data());
	}
	m_ListReply[count]->deleteLater();
}

QString TCPClient::GetAddr() const
{
	return m_Addr;
}

void TCPClient::SetAddr(const QString &Addr)
{
	m_Addr = Addr;
}
