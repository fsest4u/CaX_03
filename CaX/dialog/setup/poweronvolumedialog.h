#ifndef POWERONVOLUMEDIALOG_H
#define POWERONVOLUMEDIALOG_H

#include <QDialog>

#include "util/CJsonNode.h"

namespace Ui {
class PowerOnVolumeDialog;
}

class PowerOnVolumeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PowerOnVolumeDialog(QWidget *parent = nullptr);
	~PowerOnVolumeDialog();

	void SetNodeForm(CJsonNode node);

	int GetSliderValue0() const;

	QString GetHiddenKey() const;
	void SetHiddenKey(const QString &HiddenKey);

	QString GetHiddenValue() const;
	void SetHiddenValue(const QString &HiddenValue);

private slots:

	void SlotSliderValueChanged0(int value);

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

	Ui::PowerOnVolumeDialog *ui;
};

#endif // POWERONVOLUMEDIALOG_H
