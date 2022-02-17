#ifndef CUSTOMEQDIALOG_H
#define CUSTOMEQDIALOG_H

#include <QDialog>

#include "util/CJsonNode.h"

namespace Ui {
class CustomEQDialog;
}

class CustomEQDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CustomEQDialog(QWidget *parent = nullptr);
	~CustomEQDialog();

	CJsonNode GetNodeForm();
	void SetNodeForm(CJsonNode node);

private slots:

	void SlotSliderValueChanged00(int value);
	void SlotSliderValueChanged01(int value);
	void SlotSliderValueChanged02(int value);
	void SlotSliderValueChanged03(int value);
	void SlotSliderValueChanged04(int value);
	void SlotSliderValueChanged05(int value);
	void SlotSliderValueChanged06(int value);
	void SlotSliderValueChanged07(int value);
	void SlotSliderValueChanged08(int value);
	void SlotSliderValueChanged09(int value);

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

	Ui::CustomEQDialog *ui;
};

#endif // CUSTOMEQDIALOG_H
