#include "limitcountdialog.h"
#include "ui_limitcountdialog.h"

LimitCountDialog::LimitCountDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LimitCountDialog)
{
	ui->setupUi(this);

	ui->comboBox->addItem("100", 100);
	ui->comboBox->addItem("300", 300);
	ui->comboBox->addItem("500", 500);
	ui->comboBox->addItem("1000", 1000);
	ui->comboBox->addItem("All", 0);

}

LimitCountDialog::~LimitCountDialog()
{
	delete ui;
}

int LimitCountDialog::GetLimitCount()
{
	return ui->comboBox->currentText().toInt();
}

void LimitCountDialog::SetLimitCount(int count)
{
	QString strCnt = QString("%1").arg(count);
	ui->comboBox->setCurrentText(strCnt);
}
