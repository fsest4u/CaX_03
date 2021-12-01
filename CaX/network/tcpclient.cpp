#include <QFile>
#include <QImage>

#include "tcpclient.h"

#include "util/log.h"
#include "util/utilnovatron.h"

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
//			LogDebug("success to create temp dir...");
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

void TCPClient::RequestCommand(QByteArray jsonData, int nCmdID, int nIndex)
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
		if (nIndex < 0)
		{
			emit SigRespInfo(jsonValue, nCmdID);
		}
		else
		{
			emit SigRespInfo(jsonValue, nCmdID, nIndex);
		}
	}
	else
	{
		QString err = m_pReply->errorString();
		LogCritical("Network Error : [%s]", err.toUtf8().data());

	}
	m_pReply->deleteLater();

}

void TCPClient::RequestCoverArt(QString fullpath, int nIndex, int nMode)
{
#if 1
	QString filename = UtilNovatron::ConvertURLToFilename(fullpath);
	filename = m_DirTemp + "/" + filename;

	if (filename.isEmpty())
	{
		LogDebug("fileName is empty");
		return;
	}

	QFileInfo fileInfo(filename);
	QString suffix = fileInfo.suffix();
	if (suffix.toLower().compare("png")
			|| suffix.toLower().compare("jpg")
			|| suffix.toLower().compare("jpeg"))
	{
		suffix = "jpg";
	}

//	if (QFileInfo::exists(filename))
//	{
//		LogDebug("file exists [%s]", filename.toUtf8().data());
//		emit SigRespCoverArt(filename, nIndex, nMode);
//		return;
//	}

	const QUrl url = QUrl::fromUserInput(fullpath);
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
			if (image.save(filename, suffix.toLatin1().data()))
			{
				LogDebug("file saves [%s]", filename.toUtf8().data());
				emit SigRespCoverArt(filename, nIndex, nMode);
			}
		}
	}
	else
	{
		QString err = m_ListReply[count]->errorString();
		LogCritical("Network Error : [%s] fullPath [%s] fileName [%s]"
					, err.toUtf8().data()
					, fullpath.toUtf8().data()
					, filename.toUtf8().data());
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
//			LogCritical("Network Error : [%s] fullPath [%s] fileName [%s]"
//					, err.toUtf8().data()
//					, fullPath.toUtf8().data()
//					, fileName.toUtf8().data());
//		}
//		m_ListReply[count]->deleteLater();
//	});
#endif
}

void TCPClient::RequestCoverArt(QString fullpath)
{
#if 1
	QString filename = UtilNovatron::ConvertURLToFilename(fullpath);
	filename = m_DirTemp + "/" + filename;

	if (filename.isEmpty())
	{
		LogDebug("fileName is empty");
		return;
	}

	QFileInfo fileInfo(filename);
	QString suffix = fileInfo.suffix();
	if (suffix.toLower().compare("png")
			|| suffix.toLower().compare("jpg")
			|| suffix.toLower().compare("jpeg"))
	{
		suffix = "jpg";
	}

//	if (QFileInfo::exists(filename))
//	{
//		LogDebug("file exists [%s]", filename.toUtf8().data());
//		emit SigRespCoverArt(filename);
//		return;
//	}

	const QUrl url = QUrl::fromUserInput(fullpath);
	QNetworkRequest request(url);

	int count = m_ListReply.size();

	QNetworkReply *reply = m_pManager->get(request);
	m_ListReply.insert(count, reply);

	QEventLoop loop;
	connect(m_ListReply[count], SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	if (m_ListReply[count]->error() == QNetworkReply::NoError)
	{
#if 0
//		QByteArray jpegData = reply->readAll();
//		QPixmap pixmap;
//		pixmap.loadFromData(jpegData);
//		label->setPixmap(pixmap);
		QByteArray data = m_ListReply[count]->readAll();
		emit SigRespCoverArt(data);
#else
		QImage image;
		if (image.loadFromData(m_ListReply[count]->readAll()))
		{
			if (image.save(filename, suffix.toLatin1().data()))
			{
				LogDebug("file saves [%s]", filename.toUtf8().data());
				emit SigRespCoverArt(filename);
			}
		}
#endif
	}
	else
	{
		QString err = m_ListReply[count]->errorString();
		LogCritical("Network Error : [%s] fullPath [%s] fileName [%s]"
					, err.toUtf8().data()
					, fullpath.toUtf8().data()
					, filename.toUtf8().data());
	}
	m_ListReply[count]->deleteLater();
#endif
}

// https://forum.qt.io/topic/6853/solved-display-image-through-http-url/6
void TCPClient::RequestSearchCoverArt(QString strUrl, int index)
{
	QString filename = QString("Search%1").arg(index);
	filename = m_DirTemp + "/" + filename + ".jpg";

	if (strUrl.isEmpty())
	{
		LogDebug("strUrl is empty");
		return;
	}

	QFileInfo fileInfo(filename);
	QString suffix = fileInfo.suffix();
	if (suffix.toLower().compare("png")
			|| suffix.toLower().compare("jpg")
			|| suffix.toLower().compare("jpeg"))
	{
		suffix = "jpg";
	}

	const QUrl url = QUrl::fromUserInput(strUrl);
	QNetworkRequest request(url);

	int count = m_ListReply.size();

	QNetworkReply *reply = m_pManager->get(request);
	m_ListReply.insert(count, reply);

	QEventLoop loop;
	connect(m_ListReply[count], SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	if (m_ListReply[count]->error() == QNetworkReply::NoError)
	{
#if 0
		QByteArray data = m_ListReply[count]->readAll();
		emit SigRespSearchCoverArt(data, index);
#else
		QImage image;
		if (image.loadFromData(m_ListReply[count]->readAll()))
		{
			if (image.save(filename, suffix.toLatin1().data()))
			{
				LogDebug("file saves [%s]", filename.toUtf8().data());
				emit SigRespSearchCoverArt(filename, index);
			}
		}
#endif
	}
	else
	{
		QString err = m_ListReply[count]->errorString();
		LogCritical("Network Error : [%s] fullPath [%s] fileName [%s]"
					, err.toUtf8().data()
					, strUrl.toUtf8().data()
					, strUrl.toUtf8().data());
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

