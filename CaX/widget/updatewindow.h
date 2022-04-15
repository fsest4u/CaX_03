#ifndef UPDATEWINDOW_H
#define UPDATEWINDOW_H

#include <QWidget>
#include "manager/updatemanager.h"
#include "util/waitingspinnerwidget.h"

namespace Ui {
class UpdateWindow;
}

class WaitingSpinnerWidget;

class UpdateWindow : public QWidget
{
    Q_OBJECT

public:
    explicit UpdateWindow(QWidget *parent = nullptr);
    ~UpdateWindow();

private:
    Ui::UpdateWindow *ui;
    UpdateManager *m_upmgr;
    bool           m_isUpdate;
    WaitingSpinnerWidget *m_spinner;

    tUpdateInfo m_updateInfo;
    QString     m_localStorage_fullFileName;

private slots:
    void slot_updateInfo_response(const tUpdateInfo &);
    void slot_updateInfo_errorMsg(const QString &);

    void slot_pbt_update_start();
    void slot_download_progress(qint64 bytesRead, qint64 totalBytes);
    void slot_download_finished(const QString &localStorage_fullFileName);
    void slot_execute_new_version_to_update();

};

#endif // UPDATEWINDOW_H
