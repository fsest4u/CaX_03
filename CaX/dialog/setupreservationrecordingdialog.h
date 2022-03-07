#ifndef SETUPRESERVATIONRECORDINGDIALOG_H
#define SETUPRESERVATIONRECORDINGDIALOG_H

#include <QDialog>

#include "util/CJsonNode.h"

namespace Ui {
class SetupReservationRecordingDialog;
}

class SetupReservationRecordingDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SetupReservationRecordingDialog(QWidget *parent = nullptr);
	~SetupReservationRecordingDialog();

	CJsonNode GetNodeData();
	void SetNodeData(CJsonNode node);

private slots:

	void accept();
	void reject();

private:

	void ConnectSigToSlot();
	void Initialize();

	int		m_ID;

	Ui::SetupReservationRecordingDialog *ui;
};

#endif // SETUPRESERVATIONRECORDINGDIALOG_H
