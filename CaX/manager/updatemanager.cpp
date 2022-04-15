#include "updatemanager.h"
#include <QtWidgets>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include "util/caxconstants.h"

UpdateManager::UpdateManager(QObject *parent)
    : QObject{parent}
{
    m_infoManager = new QNetworkAccessManager(this);

    connect(m_infoManager, &QNetworkAccessManager::finished,
            this,          &UpdateManager::slot_info_replyFinished);
//    connect(m_infoManager, &QNetworkAccessManager::sslErrors,
//            this,          &UpdateManager::slot_info_errorOccurred);

    m_downManager = new QNetworkAccessManager(this);
}

UpdateManager::~UpdateManager()
{
    m_infoReply->deleteLater();
    m_infoManager->deleteLater();
}

// [ PUBLIC ]
void UpdateManager::updateInfo_request()
{
    QUrl infoUrl(UPDATE_DOWNLOAD_URL);

    m_infoReply = m_infoManager->get(QNetworkRequest(infoUrl));

    connect(m_infoReply, &QIODevice::readyRead,
            this,        &UpdateManager::slot_info_readyRead);
    connect(m_infoReply, &QNetworkReply::finished,
            this,        &UpdateManager::slot_info_replyFinished);
}

void UpdateManager::download_request()
{
    //qDebug() << Q_FUNC_INFO;
    QString fileName = m_updateInfo.download_fileName.trimmed();

    QString downloadDirectory = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    //QString downloadDirectory = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    //qDebug() << Q_FUNC_INFO << "download directory : " << downloadDirectory;

    fileName.prepend(downloadDirectory + '/');
    //bool useDirectory = !downloadDirectory.isEmpty() && QFileInfo(downloadDirectory).isDir();


    if (QFile::exists(fileName))
    {
        QFile::remove(fileName);
    }

    m_downFile = openFileForWrite(fileName);
    if (!m_downFile) {
        return;
    }

    QUrl download_url(m_updateInfo.download_url);

    m_downReply = m_downManager->get(QNetworkRequest(download_url));
    connect(m_downReply, &QNetworkReply::readyRead,
            this,        &UpdateManager::slot_down_readyRead);
    connect(m_downReply, &QNetworkReply::finished,
            this,        &UpdateManager::slot_down_replyFinished);

    connect(m_downReply, &QNetworkReply::downloadProgress,
            this,        &UpdateManager::slot_down_progress);
}

// [ PRIVATE ]

QFile *UpdateManager::openFileForWrite(const QString &fileName)
{
    QScopedPointer<QFile> file(new QFile(fileName));
    if (!file->open(QIODevice::WriteOnly)) {

        QString errorMsg = QString("Unable to save the file %1 : %2")
                                     .arg(QDir::toNativeSeparators(fileName))
                                     .arg(QDir::toNativeSeparators(fileName));

        emit sig_updateInfo_errorMessage(errorMsg);

        return nullptr;
    }
    return file.take();
}


// [ SLOTS ]

void UpdateManager::slot_info_errorOccurred(QNetworkReply *,
                                            const QList<QSslError> &errors)
{
    QString errorString;

    foreach (const QSslError &error, errors) {
        if (!errorString.isEmpty())
            errorString += '\n';
        errorString += error.errorString();
    }

    //qDebug() << "errorString : !!! " << errorString;
    emit sig_updateInfo_errorMessage(errorString);


    m_infoReply->disconnect();
    m_infoReply->deleteLater();
}

void UpdateManager::slot_info_readyRead()
{
    m_infoData = m_infoReply->readAll();
}

void UpdateManager::slot_info_replyFinished()
{
    if(m_infoReply->error()) {
        //qDebug() << Q_FUNC_INFO << m_infoReply->errorString();
        emit sig_updateInfo_errorMessage(m_infoReply->errorString());
    }

    if(m_infoData.size() > 100)
    {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(m_infoData);
        QJsonObject jsonObj = jsonResponse.object();

        m_updateInfo.signature  = jsonObj["signature"].toString();

#ifdef Q_OS_WIN
        m_updateInfo.ver               = jsonObj["win_ver"].toDouble();
        m_updateInfo.kind              = jsonObj["win_kind"].toInt();
        m_updateInfo.download_url      = jsonObj["win_file_url"].toString().trimmed();
        m_updateInfo.download_fileName = jsonObj["win_file_name"].toString().trimmed();
        m_updateInfo.file_size         = jsonObj["win_file_size"].toInt();
#else
        m_updateInfo.ver               = jsonObj["mac_ver"].toDouble();
        m_updateInfo.kind              = jsonObj["mac_kind"].toInt();
        m_updateInfo.download_url      = jsonObj["mac_file_url"].toString();
        m_updateInfo.download_fileName = jsonObj["mac_file_name"].toString();
        m_updateInfo.file_size         = jsonObj["mac_file_size"].toInt();
#endif

#if 0
        qDebug("=====================================================");
        qDebug("signature          : %s ",    updateInfo.signature.toLocal8Bit().data());
        qDebug("-----------------------------------------------------");
        qDebug("version            : %.1f ",  m_updateInfo.ver);
        qDebug("kind               : %d ",    m_updateInfo.kind);
        qDebug("download url       : %s ",    m_updateInfo.download_url.toLocal8Bit().data());
        qDebug("download file name : %s ",    m_updateInfo.download_fileName.toLocal8Bit().data());
        qDebug("file size          : %d ",    m_updateInfo.file_size);
#endif

        emit sig_updateInfo_response(m_updateInfo);
    }

    m_infoReply->disconnect();
    m_infoReply->deleteLater();
}

void UpdateManager::slot_down_readyRead()
{
    if(m_downFile)
        m_downFile->write(m_downReply->readAll());
}

void UpdateManager::slot_down_replyFinished()
{
    QFileInfo fi;
    if (m_downFile) {
        fi.setFile(m_downFile->fileName());
        m_downFile->close();
        delete m_downFile;
        m_downFile = nullptr;
    }

    if (m_downReply->error()) {
        QFile::remove(fi.absoluteFilePath());
        m_downReply->deleteLater();
        m_downReply = nullptr;
        return;
    }

    const QVariant redirectionTarget = m_downReply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    m_downReply->deleteLater();
    m_downReply = nullptr;

    emit sig_download_finished( fi.absoluteFilePath() );
}

void UpdateManager::slot_down_progress(qint64 bytesRead,
                                       qint64 totalBytes)
{
    emit sig_download_progress(bytesRead, totalBytes);
}

























































