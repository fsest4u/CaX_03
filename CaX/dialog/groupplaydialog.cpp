#include "groupplaydialog.h"
#include "ui_groupplaydialog.h"

GroupPlayDialog::GroupPlayDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::GroupPlayDialog)
{
	ui->setupUi(this);
}

GroupPlayDialog::~GroupPlayDialog()
{
	delete ui;
}
