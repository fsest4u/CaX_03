#ifndef TIMEMANUALDIALOG_H
#define TIMEMANUALDIALOG_H

#include <QDialog>

#include "util/CJsonNode.h"

namespace Ui {
class TimeManualDialog;
}

class TimeManualDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TimeManualDialog(QWidget *parent = nullptr);
	~TimeManualDialog();

	void SetNodeForm(CJsonNode node);

	QString GetYear() const;
	QString GetMonth() const;
	QString GetDay() const;

	QString GetAMPM() const;
	QString GetHour() const;
	QString GetMinute() const;

private slots:

	void accept();
	void reject();

private:

	void ConnectSigToSlot();
	void Initialize();

	void SetOK(QString title, int action);
	void SetCancel(QString title, int action);
	void SetInputs(CJsonNode node);

	int m_ActionOK;
	int m_ActionCancel;

	Ui::TimeManualDialog *ui;
};

#endif // TIMEMANUALDIALOG_H
