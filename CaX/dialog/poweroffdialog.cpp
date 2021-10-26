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
	ui->labelWol->setText(STR_WOL);

}

PowerOffDialog::~PowerOffDialog()
{
	delete ui;
}

QPushButton *PowerOffDialog::GetBtnWol()
{
	return ui->btnWol;
}
