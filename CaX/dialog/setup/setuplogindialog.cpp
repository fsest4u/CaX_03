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

void SetupLoginDialog::SetNodeForm(CJsonNode node)
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

QString SetupLoginDialog::GetUsername() const
{
	return ui->lineEdit0->text();
}

QString SetupLoginDialog::GetPassword() const
{
	return ui->lineEdit1->text();
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
		LogDebug("node : [%s]", input.ToCompactByteArray().data());

		int typeInput = input.GetInt(KEY_TYPE);
		UtilNovatron::DebugTypeForSetupInput("SetInputs", typeInput);

		QString labelKey = input.GetString(KEY_KEY);
		QString labelValue = input.GetString(KEY_VALUE);

		if (iSetupInput_Text == typeInput)
		{
			SetUsernameKey(labelKey);
			ui->frame0->show();
			ui->label0->setText(labelKey);

			ui->frame0->show();
			ui->lineEdit0->setText(labelValue);
		}
		else if (iSetupInput_Password == typeInput)
		{
			SetPasswordKey(labelKey);
			ui->frame1->show();
			ui->label1->setText(labelKey);

			ui->frame1->show();
			ui->lineEdit1->setText(labelValue);
		}
	}
}

QString SetupLoginDialog::GetPasswordKey() const
{
	return m_PasswordKey;
}

void SetupLoginDialog::SetPasswordKey(const QString &PasswordKey)
{
	m_PasswordKey = PasswordKey;
}

QString SetupLoginDialog::GetUsernameKey() const
{
	return m_UsernameKey;
}

void SetupLoginDialog::SetUsernameKey(const QString &UsernameKey)
{
	m_UsernameKey = UsernameKey;
}
