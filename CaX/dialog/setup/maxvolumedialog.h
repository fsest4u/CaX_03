#ifndef MAXVOLUMEDIALOG_H
#define MAXVOLUMEDIALOG_H

#include <QDialog>

namespace Ui {
class MaxVolumeDialog;
}

class MaxVolumeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit MaxVolumeDialog(QWidget *parent = nullptr);
	~MaxVolumeDialog();

	int GetMaxVolume() const;
	void SetMaxVolume(const int volume);

	int GetCurVolume() const;
	void SetCurVolume(const int volume);

private slots:

	void SlotMaxVolValueChanged(int value);
	void SlotCurVolValueChanged(int value);

private:

	void ConnectSigToSlot();

	Ui::MaxVolumeDialog *ui;
};

#endif // SETUPMAXVOLUMEDIALOG_H
