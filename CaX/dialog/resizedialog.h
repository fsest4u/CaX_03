#ifndef RESIZEDIALOG_H
#define RESIZEDIALOG_H

#include <QDialog>

namespace Ui {
class ResizeDialog;
}

class ResizeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ResizeDialog(QWidget *parent = nullptr);
	~ResizeDialog();

	void SetSliderValue(int min, int max, int value);

signals:

	void SigSliderValueChanged(int value);

protected:

	bool eventFilter(QObject *object, QEvent *event) override;

private slots:

	void SlotBtnClose();
	void SlotSliderValueChanged(int value);

private:

	void ConnectSigToSlot();
	void Initialize();

	Ui::ResizeDialog *ui;
};

#endif // RESIZEDIALOG_H
