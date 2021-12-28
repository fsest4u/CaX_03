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
	ShowBtnCancel(false);

	connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
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
	if (show)
	{
		ui->btnOK->show();
	}
	else
	{
		ui->btnOK->hide();
	}
}

void CommonDialog::ShowBtnCancel(bool show)
{
	if (show)
	{
		ui->btnCancel->show();
	}
	else
	{
		ui->btnCancel->hide();
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

