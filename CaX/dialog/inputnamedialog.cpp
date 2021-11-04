#include "inputnamedialog.h"
#include "ui_inputnamedialog.h"

InputNameDialog::InputNameDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::InputNameDialog)
{
	ui->setupUi(this);
}

InputNameDialog::~InputNameDialog()
{
	delete ui;
}

QString InputNameDialog::GetName()
{
	return ui->lineEdit->text();
}

void InputNameDialog::SetName(QString name)
{
	ui->lineEdit->setText(name);
}
