#include "formdialog.h"
#include "ui_formdialog.h"

#include "util/caxkeyvalue.h"

FormDialog::FormDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::FormDialog)
{
	ui->setupUi(this);

	ui->labelTitle1->hide();
	ui->labelTitle2->hide();
	ui->labelTitle3->hide();

	ui->btnOK->hide();
	ui->btnCancel->hide();

	connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

FormDialog::~FormDialog()
{
	delete ui;
}

void FormDialog::SetWindowTitle(QString title)
{
	setWindowTitle(title);
}

void FormDialog::SetLabelTitle(CJsonNode arrInput)
{
	CJsonNode input;
	for (int i = 0; i < arrInput.ArraySize(); i++)
	{
		input = arrInput.GetArrayAt(i);
//		LogDebug("node : [%s]", list[i].ToCompactByteArray().data());
		if (i == 0)
		{
			ui->labelTitle1->show();
			ui->labelTitle1->setText(input.GetString(KEY_VALUE));
		}
		else if (i == 1)
		{
			ui->labelTitle2->show();
			ui->labelTitle2->setText(input.GetString(KEY_VALUE));
		}
		if (i == 2)
		{
			ui->labelTitle3->show();
			ui->labelTitle3->setText(input.GetString(KEY_VALUE));
		}

	}

}

void FormDialog::SetOK(QString title, int action)
{
	if (!title.isEmpty())
	{
		ui->btnOK->setText(title);
		ui->btnOK->show();

		m_ActionOK = action;
	}
}

void FormDialog::SetCancel(QString title, int action)
{
	if (!title.isEmpty())
	{
		ui->btnCancel->setText(title);
		ui->btnCancel->show();

		m_ActionCancel = action;
	}
}

void FormDialog::accept()
{
	done(QDialog::Accepted);

}

void FormDialog::reject()
{
	done(QDialog::Rejected);

}

