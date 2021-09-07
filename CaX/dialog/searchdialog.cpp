#include "searchdialog.h"
#include "ui_searchdialog.h"

SearchDialog::SearchDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SearchDialog)
{
	ui->setupUi(this);
}

SearchDialog::~SearchDialog()
{
	delete ui;
}

QString SearchDialog::GetKeyword()
{
	return ui->lineEdit->text();
}

void SearchDialog::SetKeyword(QString keyword)
{
	ui->lineEdit->setText(keyword);
}
