#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LoginDialog)
{
	ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
	delete ui;
}

QString LoginDialog::GetUserID()
{
	return ui->lineEditID->text();
}

void LoginDialog::SetUserID(QString userID)
{
	ui->lineEditID->setText(userID);
}

QString LoginDialog::GetPassword()
{
	return ui->lineEditPW->text();
}

void LoginDialog::SetPassword(QString password)
{
	ui->lineEditPW->setText(password);
}
