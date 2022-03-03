#include "poweroffdialog.h"
#include "ui_poweroffdialog.h"

#include "util/caxtranslate.h"
#include "util/log.h"

PowerOffDialog::PowerOffDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PowerOffDialog)
{
	ui->setupUi(this);

	ui->labelPowerOff->setText(STR_DO_YOU_WANT_TO_POWER_OFF);
	ui->labelDevices->hide();
	ui->cbWol->setText(STR_WOL);
	ui->cbWol->setCheckState(Qt::Checked);

}

PowerOffDialog::~PowerOffDialog()
{
	delete ui;
}

bool PowerOffDialog::GetIsWol() const
{
	if (ui->cbWol->checkState() == Qt::Checked)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void PowerOffDialog::SetDevices(QString devices)
{
	ui->labelDevices->show();
	ui->labelDevices->setText(devices);
}
