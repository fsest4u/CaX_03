#include "progressfmdialog.h"
#include "ui_progressfmdialog.h"

ProgressFmDialog::ProgressFmDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ProgressFmDialog)
{
	ui->setupUi(this);
}

ProgressFmDialog::~ProgressFmDialog()
{
	delete ui;
}

void ProgressFmDialog::SetTitle(QString title)
{
	ui->labelTitle->setText(title);
}
