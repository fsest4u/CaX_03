#include "maxvolumedialog.h"
#include "ui_maxvolumedialog.h"

MaxVolumeDialog::MaxVolumeDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::MaxVolumeDialog)
{
	ui->setupUi(this);

	ConnectSigToSlot();
}

MaxVolumeDialog::~MaxVolumeDialog()
{
	delete ui;
}

int MaxVolumeDialog::GetMaxVolume() const
{
	return ui->horizontalSliderMaxVol->value();
}

void MaxVolumeDialog::SetMaxVolume(const int volume)
{
	ui->horizontalSliderMaxVol->setValue(volume);
	ui->labelMaxVol->setText(QString::number(volume));
}

int MaxVolumeDialog::GetCurVolume() const
{
	return ui->horizontalSliderCurVol->value();
}

void MaxVolumeDialog::SetCurVolume(const int volume)
{
	ui->horizontalSliderCurVol->setValue(volume);
	ui->labelCurVol->setText(QString::number(volume));
}

void MaxVolumeDialog::SlotMaxVolValueChanged(int value)
{
	ui->labelMaxVol->setText(QString::number(value));
}

void MaxVolumeDialog::SlotCurVolValueChanged(int value)
{
	int maxVol = ui->horizontalSliderMaxVol->value();
	if (maxVol < value)
	{
		value = maxVol;
	}
	ui->horizontalSliderCurVol->setValue(value);
	ui->labelCurVol->setText(QString::number(value));

}

void MaxVolumeDialog::ConnectSigToSlot()
{
	connect(ui->horizontalSliderMaxVol, SIGNAL(valueChanged(int)), this, SLOT(SlotMaxVolValueChanged(int)));
	connect(ui->horizontalSliderCurVol, SIGNAL(valueChanged(int)), this, SLOT(SlotCurVolValueChanged(int)));
}

