#include "formdialog.h"
#include "ui_formdialog.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/utilnovatron.h"

#include "widget/setup.h"

FormDialog::FormDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::FormDialog)
{
	ui->setupUi(this);

	ui->labelKey0->hide();
	ui->labelKey1->hide();
	ui->labelKey2->hide();

	ui->labelValue0->hide();
	ui->labelValue1->hide();
	ui->labelValue2->hide();

	ui->btnOK->hide();
	ui->btnCancel->hide();

	connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

FormDialog::~FormDialog()
{
	delete ui;
}

void FormDialog::SetNodeForm(CJsonNode node)
{
	LogDebug("node [%s]", node.ToCompactByteArray().data());
	setWindowTitle(node.GetString(KEY_TITLE_CAP));

	CJsonNode nodeOK = node.GetObject(KEY_OK);
	CJsonNode nodeCancel = node.GetObject(KEY_CANCEL);
	CJsonNode arrNodeInput = node.GetArray(KEY_INPUTS);

	SetOK(nodeOK.GetString(KEY_NAME_CAP), nodeOK.GetInt(KEY_ACTION));
	SetCancel(nodeCancel.GetString(KEY_NAME_CAP), nodeCancel.GetInt(KEY_ACTION));
	SetInputs(arrNodeInput);
}

void FormDialog::accept()
{
	done(QDialog::Accepted);
}

void FormDialog::reject()
{
	done(QDialog::Rejected);
}

void FormDialog::SetOK(QString title, int action)
{
	UtilNovatron::DebugTypeForSetupBtn("SetOK", action);
	if (!title.isEmpty())
	{
		ui->btnOK->setText(title);
		ui->btnOK->show();

		m_ActionOK = action;
	}
}

void FormDialog::SetCancel(QString title, int action)
{
	UtilNovatron::DebugTypeForSetupBtn("SetCancel", action);
	if (!title.isEmpty())
	{
		ui->btnCancel->setText(title);
		ui->btnCancel->show();

		m_ActionCancel = action;
	}
}

void FormDialog::SetInputs(CJsonNode node)
{
	CJsonNode input;
	for (int i = 0; i < node.ArraySize(); i++)
	{
		input = node.GetArrayAt(i);
		LogDebug("node : [%s]", input.ToCompactByteArray().data());

		int typeInput = input.GetInt(KEY_TYPE);
		UtilNovatron::DebugTypeForSetupInput("SetInputs", typeInput);

		QString labelKey = input.GetString(KEY_KEY);
		QString labelValue = input.GetString(KEY_VALUE);

		if (!labelKey.isEmpty())
		{
			if (i == 0)
			{
				ui->labelKey0->show();
				ui->labelKey0->setText(labelKey);
			}
			else if (i == 1)
			{
				ui->labelKey1->show();
				ui->labelKey1->setText(labelKey);
			}
			else if (i == 2)
			{
				ui->labelKey2->show();
				ui->labelKey2->setText(labelKey);
			}
		}

		if (!labelValue.isEmpty())
		{
			if (i == 0)
			{
				ui->labelValue0->show();
				ui->labelValue0->setText(labelValue);
			}
			else if (i == 1)
			{
				ui->labelValue1->show();
				ui->labelValue1->setText(labelValue);
			}
			else if (i == 2)
			{
				ui->labelValue2->show();
				ui->labelValue2->setText(labelValue);
			}
		}
	}
}
