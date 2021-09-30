#include "poweroffdialog.h"
#include "ui_poweroffdialog.h"

#include "util/log.h"

PowerOffDialog::PowerOffDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PowerOffDialog)
{
	ui->setupUi(this);

	ui->labelPowerOff->setText(tr("Do you want to power off?"));
	ui->labelWol->setText(tr("Wake on lan"));

}

PowerOffDialog::~PowerOffDialog()
{
	delete ui;
}

QPushButton *PowerOffDialog::GetBtnWol()
{
	return ui->btnWol;
}
