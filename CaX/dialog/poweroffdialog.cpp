#include "poweroffdialog.h"
#include "ui_poweroffdialog.h"

#include "util/caxtranslate.h"
#include "util/log.h"

PowerOffDialog::PowerOffDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PowerOffDialog)
{
	ui->setupUi(this);

	ui->labelPowerOff->setText(STR_ASK_POWER_OFF);
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
