#include "commondialog.h"
#include "ui_commondialog.h"

CommonDialog::CommonDialog(QWidget *parent, QString title, QString content) :
	QDialog(parent),
	ui(new Ui::CommonDialog)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	SetTitle(title);
	SetContent(content);
}

CommonDialog::~CommonDialog()
{
	delete ui;
}

void CommonDialog::SetTitle(QString title)
{
	setWindowTitle(title);
}

void CommonDialog::SetContent(QString content)
{
	ui->labelContent->setText(content);
}

void CommonDialog::ShowBtnOk(bool show)
{
	if (!show)
	{
		QPushButton* btn = ui->buttonBox->button(QDialogButtonBox::Ok);
		ui->buttonBox->removeButton((QAbstractButton*)btn);
	}
}

void CommonDialog::ShowBtnCancel(bool show)
{
	if (!show)
	{
		QPushButton* btn = ui->buttonBox->button(QDialogButtonBox::Cancel);
		ui->buttonBox->removeButton((QAbstractButton*)btn);
	}
}

void CommonDialog::accept()
{
	done(QDialog::Accepted);
}

void CommonDialog::reject()
{
	done(QDialog::Rejected);
}

void CommonDialog::ConnectSigToSlot()
{

}

