#include "setuplogindialog.h"
#include "ui_setuplogindialog.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/utilnovatron.h"

#include "widget/setup.h"

SetupLoginDialog::SetupLoginDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SetupLoginDialog)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();
}

SetupLoginDialog::~SetupLoginDialog()
{
	delete ui;
}

CJsonNode SetupLoginDialog::GetNodeForm()
{
	QString usernameKey = ui->label0->text();
	QString passwordKey = ui->label1->text();
	QString username = ui->lineEdit0->text();
	QString password = ui->lineEdit1->text();

	CJsonNode node(JSON_OBJECT);
	if (!usernameKey.isEmpty())
	{
		node.Add(usernameKey,		username);
	}
	if (!passwordKey.isEmpty())
	{
		node.Add(passwordKey,		password);
	}

	return node;
}

void SetupLoginDialog::SetNodeForm(CJsonNode node)
{
//	LogDebug("node [%s]", node.ToCompactByteArray().data());
	setWindowTitle(node.GetString(KEY_TITLE_CAP));

	CJsonNode nodeOK = node.GetObject(KEY_OK);
	CJsonNode nodeCancel = node.GetObject(KEY_CANCEL);
	CJsonNode arrNodeInput = node.GetArray(KEY_INPUTS);

	SetOK(nodeOK.GetString(KEY_NAME_CAP), nodeOK.GetInt(KEY_ACTION));
	SetCancel(nodeCancel.GetString(KEY_NAME_CAP), nodeCancel.GetInt(KEY_ACTION));
	SetInputs(arrNodeInput);
}

void SetupLoginDialog::accept()
{
	done(QDialog::Accepted);
}

void SetupLoginDialog::reject()
{
	done(QDialog::Rejected);
}

void SetupLoginDialog::ConnectSigToSlot()
{
	connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void SetupLoginDialog::Initialize()
{
	ui->frame0->hide();
	ui->frame1->hide();

	ui->btnOK->hide();
	ui->btnCancel->hide();
}

void SetupLoginDialog::SetOK(QString title, int action)
{
	UtilNovatron::DebugTypeForSetupBtn("SetOK", action);
	if (!title.isEmpty())
	{
		ui->btnOK->setText(title);
		ui->btnOK->show();

		m_ActionOK = action;
	}
}

void SetupLoginDialog::SetCancel(QString title, int action)
{
	UtilNovatron::DebugTypeForSetupBtn("SetCancel", action);
	if (!title.isEmpty())
	{
		ui->btnCancel->setText(title);
		ui->btnCancel->show();

		m_ActionCancel = action;
	}
}

void SetupLoginDialog::SetInputs(CJsonNode node)
{
	CJsonNode input;
	for (int i = 0; i < node.ArraySize(); i++)
	{
		input = node.GetArrayAt(i);
//		LogDebug("node : [%s]", input.ToCompactByteArray().data());

		int typeInput = input.GetInt(KEY_TYPE);
		UtilNovatron::DebugTypeForSetupInput("SetInputs", typeInput);

		QString labelKey = input.GetString(KEY_KEY);
		QString labelValue = input.GetString(KEY_VALUE);

		if (iSetupInput_Text == typeInput)
		{
			ui->frame0->show();
			ui->label0->setText(labelKey);
			ui->lineEdit0->setText(labelValue);
		}
		else if (iSetupInput_Password == typeInput)
		{
			ui->frame1->show();
			ui->label1->setText(labelKey);
			ui->lineEdit1->setText(labelValue);
		}
	}
}
