#ifndef ALARMDIALOG_H
#define ALARMDIALOG_H

#include <QDialog>

#include "util/CJsonNode.h"

namespace Ui {
class AlarmDialog;
}

class AlarmDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AlarmDialog(QWidget *parent = nullptr);
	~AlarmDialog();

	CJsonNode GetNodeForm();
	void SetNodeForm(CJsonNode node);



private slots:

	void SlotActive(bool active);
	void SlotSliderValueChanged(int value);

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

	QString m_HiddenKey;
	QString m_HiddenValue;

	Ui::AlarmDialog *ui;
};

#endif // ALARMDIALOG_H
