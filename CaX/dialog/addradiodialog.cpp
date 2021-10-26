#include "addradiodialog.h"
#include "ui_addradiodialog.h"

AddRadioDialog::AddRadioDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddRadioDialog)
{
	ui->setupUi(this);
}

AddRadioDialog::~AddRadioDialog()
{
	delete ui;
}

QString AddRadioDialog::GetName()
{
	return ui->lineEdit->text();
}

void AddRadioDialog::SetName(QString name)
{
	ui->lineEdit->setText(name);
}

double AddRadioDialog::GetFrequency()
{
	return ui->doubleSpinBox->value() * 1000;
}

void AddRadioDialog::SetFrequency(double freq)
{
	ui->doubleSpinBox->setValue(freq / 1000);
}

void AddRadioDialog::SetRange(double min, double max, double step)
{
	ui->labelMin->setText(QString("%1").arg(min));
	ui->labelMax->setText(QString("%1").arg(max));
	ui->doubleSpinBox->setSingleStep(step);
	ui->doubleSpinBox->setRange(min, max);
}
