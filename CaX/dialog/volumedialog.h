#ifndef VOLUMEDIALOG_H
#define VOLUMEDIALOG_H

#include <QDialog>

namespace Ui {
class VolumeDialog;
}

class VolumeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit VolumeDialog(QWidget *parent = nullptr);
	~VolumeDialog();

	void SetSliderValue(int value);

signals:

	void SigIncrease(int value);
	void SigDecrease(int value);
	void SigSliderValueChanged(int value);

protected:

	bool eventFilter(QObject *object, QEvent *event) override;

private slots:

	void SlotBtnClose();
	void SlotBtnIncrease();
	void SlotBtnDecrease();
	void SlotSliderValueChanged(int value);

private:

	void ReadSettings();

	void ConnectSigToSlot();
	void Initialize();

	int m_MaxVolume;

	Ui::VolumeDialog *ui;
};

#endif // VOLUMEDIALOG_H
