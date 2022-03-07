#include "changemetainfodialog.h"
#include "ui_changemetainfodialog.h"

ChangeMetaInfoDialog::ChangeMetaInfoDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ChangeMetaInfoDialog)
{
	ui->setupUi(this);
}

ChangeMetaInfoDialog::~ChangeMetaInfoDialog()
{
	delete ui;
}

int ChangeMetaInfoDialog::GetIndex()
{
	return ui->comboBox->currentIndex();
}

void ChangeMetaInfoDialog::SetIndex(QStringList list)
{
	ui->comboBox->addItems(list);
}
