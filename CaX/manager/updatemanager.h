#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QSslError>
#include <QAuthenticator>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>

typedef struct _tUpdateInfo
{
    QString signature;
    double  ver;
    quint32 kind;
    QString download_url;
    QString download_fileName;
    qint64  file_size;

} tUpdateInfo;

class UpdateManager : public QObject
{
    Q_OBJECT
public:
    explicit UpdateManager(QObject *parent = nullptr);
    ~UpdateManager();

public:
    void updateInfo_request();
    void download_request();

private:
    tUpdateInfo            m_updateInfo;

    QNetworkAccessManager *m_infoManager;
    QNetworkReply         *m_infoReply;
    QByteArray             m_infoData;

    QNetworkAccessManager *m_downManager;
    QNetworkReply         *m_downReply;
    QFile                 *m_downFile;

    QFile *openFileForWrite(const QString &fileName);

private slots:
    void slot_info_errorOccurred(QNetworkReply *,
                                 const QList<QSslError> &errors);
    void slot_info_readyRead();
    void slot_info_replyFinished();

    void slot_down_readyRead();
    void slot_down_replyFinished();
    void slot_down_progress(qint64 bytesRead, qint64 totalBytes);


signals:
    void sig_updateInfo_response(const tUpdateInfo &);
    void sig_updateInfo_errorMessage(const QString &);

    void sig_download_progress(qint64, qint64);
    void sig_download_finished(const QString &downloaded_fullFileName);

};

#endif // UPDATEMANAGER_H
