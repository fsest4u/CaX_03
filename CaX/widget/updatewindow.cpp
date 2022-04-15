#include "updatewindow.h"
#include "ui_updatewindow.h"
#include "util/caxconstants.h"
#include <QtWidgets>
#include <QStandardPaths>
#include <QDebug>
#include <QStandardPaths>
#include <QTimer>

UpdateWindow::UpdateWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UpdateWindow),
    m_upmgr(new UpdateManager)
{
    ui->setupUi(this);

    ui->frame_waitloading->setVisible(false);

    m_spinner = new WaitingSpinnerWidget(parent, true, true);
    m_spinner->setRoundness(40.0);
    m_spinner->setMinimumTrailOpacity(0.0);
    m_spinner->setTrailFadePercentage(75.0);
    m_spinner->setNumberOfLines(12);
    m_spinner->setLineLength(30);
    m_spinner->setLineWidth(5);
    m_spinner->setInnerRadius(30);
    m_spinner->setRevolutionsPerSecond(1);
    m_spinner->setColor(QColor(52, 174, 214));
    m_spinner->stop();

    ui->horizontal_wait->addWidget(m_spinner);

    ui->prog_download_progress->setVisible(false);
    ui->frame_error_message->setVisible(false);

    QString verStr = APP_VERSION;
    //qDebug() << Q_FUNC_INFO << "current version : " << verStr;

    ui->lbl_current_version->setText(verStr);

    connect(m_upmgr, &UpdateManager::sig_updateInfo_response,
            this,    &UpdateWindow::slot_updateInfo_response);

    connect(m_upmgr, &UpdateManager::sig_updateInfo_errorMessage,
            this,    &UpdateWindow::slot_updateInfo_errorMsg);

    connect(m_upmgr, &UpdateManager::sig_download_progress,
            this,    &UpdateWindow::slot_download_progress);

    connect(m_upmgr, &UpdateManager::sig_download_finished,
            this,    &UpdateWindow::slot_download_finished);

    connect(ui->pbt_update_start, &QPushButton::pressed,
            this,                 &UpdateWindow::slot_pbt_update_start);

    m_upmgr->updateInfo_request();
}

UpdateWindow::~UpdateWindow()
{
    delete ui;
}

void UpdateWindow::slot_updateInfo_response(const tUpdateInfo &updateInfo)
{
    m_isUpdate = false;

    if(updateInfo.signature == "CA UPDATE")
    {
        ui->frame_update->setVisible(true);
        ui->frame_error_message->setVisible(false);

        QString _verStr = APP_VERSION;

        double curr_appVer = _verStr.toDouble();

        if(curr_appVer < updateInfo.ver)
            m_isUpdate = true;

        if(!m_isUpdate)
        {
            ui->lbl_isUpdate_Message->setText("The current version is latest version.");
            ui->pbt_update_start->setEnabled(false);
        }
        else
        {
            ui->lbl_isUpdate_Message->setText("You have the latest version to update.");
            ui->pbt_update_start->setEnabled(true);
        }

        QString verStr = QString::number(updateInfo.ver, 'f', 1);
        ui->lbl_update_version->setText(verStr);
        ui->lbl_update_filename->setText(updateInfo.download_fileName);

        qint32 fsize = updateInfo.file_size / 1000 / 1000;
        //QString fsizeStr = QString::number(fsize);
        QString fsize_unit_mb = QString("%L1").arg(fsize);
        QString fsize_unit_bytes = QString("%L1").arg(updateInfo.file_size);

        QString fsizeStr = QString("%1 MB (%2 Bytes)")
                                    .arg(fsize_unit_mb).arg(fsize_unit_bytes);

        ui->lbl_update_filesize->setText(fsizeStr);

        m_updateInfo = updateInfo;

#if 0
        qDebug("=====================================================");
        qDebug("signature          : %s ",    m_updateInfo.signature.toLocal8Bit().data());
        qDebug("-----------------------------------------------------");
        qDebug("version            : %.1f ",  m_updateInfo.ver);
        qDebug("kind               : %d ",    m_updateInfo.kind);
        qDebug("download url       : %s ",    m_updateInfo.download_url.toLocal8Bit().data());
        qDebug("download file name : %s ",    m_updateInfo.download_fileName.toLocal8Bit().data());
        qDebug("file size          : %d ",    m_updateInfo.file_size);
#endif
    }
}

void UpdateWindow::slot_updateInfo_errorMsg(const QString &errorMsg)
{
    ui->pbt_update_start->setEnabled(false);
    ui->frame_error_message->setVisible(true);

    ui->te_error_message->clear();
    ui->te_error_message->append(errorMsg);
}

void UpdateWindow::slot_pbt_update_start()
{
//    qDebug() << Q_FUNC_INFO;
    ui->te_error_message->clear();
    ui->frame_error_message->setVisible(false);


    ui->prog_download_progress->setVisible(true);
    m_upmgr->download_request();
    ui->pbt_update_start->setEnabled(false);
}

void UpdateWindow::slot_download_progress(qint64 bytesRead, qint64 totalBytes)
{
//    qDebug("Download progress [byteRead / totalBytes] : [ %lld / %lld ]",
//                                bytesRead, totalBytes);

    ui->prog_download_progress->setMaximum(totalBytes);
    ui->prog_download_progress->setValue(bytesRead);
}

void UpdateWindow::slot_download_finished(const QString &localStorage_fullFileName)
{

    QFileInfo fInfo(localStorage_fullFileName);
    qint64 fSize = fInfo.size();

    if(fSize == m_updateInfo.file_size)
    {
        ui->frame_update->setVisible(false);
        ui->frame_error_message->setVisible(false);
        ui->pbt_update_start->setEnabled(true);

        ui->frame_waitloading->setVisible(true);
        m_spinner->start();

        m_localStorage_fullFileName = localStorage_fullFileName;
        QTimer::singleShot(5000, this, SLOT(slot_execute_new_version_to_update()));
    }
    else
    {
        ui->frame_update->setVisible(true);
        ui->frame_error_message->setVisible(true);
        ui->pbt_update_start->setEnabled(true);

        ui->frame_waitloading->setVisible(false);
        m_spinner->stop();

        ui->te_error_message->append("The size of downloaded file and new version is different, please re-try.");
    }
}

void UpdateWindow::slot_execute_new_version_to_update()
{
    QApplication::quit();
    QDesktopServices::openUrl(QUrl::fromLocalFile(m_localStorage_fullFileName));
}
















