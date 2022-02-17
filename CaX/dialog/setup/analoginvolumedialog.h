#ifndef ANALOGINVOLUMEDIALOG_H
#define ANALOGINVOLUMEDIALOG_H

#include <QDialog>

#include "util/CJsonNode.h"

namespace Ui {
class AnalogInVolumeDialog;
}

class AnalogInVolumeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AnalogInVolumeDialog(QWidget *parent = nullptr);
	~AnalogInVolumeDialog();

	CJsonNode GetNodeForm();
	void SetNodeForm(CJsonNode node);

private slots:

	void SlotSliderValueChanged0(int value);
	void SlotSliderValueChanged1(int value);
	void SlotSliderValueChanged2(int value);

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

	Ui::AnalogInVolumeDialog *ui;
};

#endif // ANALOGINVOLUMEDIALOG_H
