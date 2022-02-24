#include "wiredlansetupdialog.h"
#include "ui_wiredlansetupdialog.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/utilnovatron.h"

#include "widget/setup.h"

WiredLanSetupDialog::WiredLanSetupDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::WiredLanSetupDialog)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();
}

WiredLanSetupDialog::~WiredLanSetupDialog()
{
	delete ui;
}

CJsonNode WiredLanSetupDialog::GetNodeForm()
{
	QString key0 = ui->label0->text();
	QString key1 = ui->label1->text();
	QString key2 = ui->label2->text();
	QString key3 = ui->label3->text();

	QString value0 = ui->lineEdit0->text();
	QString value1 = ui->lineEdit1->text();
	QString value2 = ui->lineEdit2->text();
	QString value3 = ui->lineEdit3->text();

	CJsonNode node(JSON_OBJECT);
	if (!key0.isEmpty())
	{
		node.Add(key0,	value0);
	}
	if (!key1.isEmpty())
	{
		node.Add(key1,	value1);
	}
	if (!key2.isEmpty())
	{
		node.Add(key2,	value2);
	}
	if (!key3.isEmpty())
	{
		node.Add(key3,	value3);
	}
	node.Add(m_HiddenKey,		m_HiddenValue);
	return node;
}

void WiredLanSetupDialog::SetNodeForm(CJsonNode node)
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

void WiredLanSetupDialog::accept()
{
	done(QDialog::Accepted);
}

void WiredLanSetupDialog::reject()
{
	done(QDialog::Rejected);
}

void WiredLanSetupDialog::ConnectSigToSlot()
{
	connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void WiredLanSetupDialog::Initialize()
{
	ui->frame0->hide();
	ui->frame1->hide();
	ui->frame2->hide();
	ui->frame3->hide();

	ui->lineEdit0->setInputMask("000.000.000.000");
	ui->lineEdit1->setInputMask("000.000.000.000");
	ui->lineEdit2->setInputMask("000.000.000.000");
	ui->lineEdit3->setInputMask("000.000.000.000");

	ui->btnOK->hide();
	ui->btnCancel->hide();
}

void WiredLanSetupDialog::SetOK(QString title, int action)
{
	UtilNovatron::DebugTypeForSetupBtn("SetOK", action);
	if (!title.isEmpty())
	{
		ui->btnOK->setText(title);
		ui->btnOK->show();

		m_ActionOK = action;
	}
}

void WiredLanSetupDialog::SetCancel(QString title, int action)
{
	UtilNovatron::DebugTypeForSetupBtn("SetCancel", action);
	if (!title.isEmpty())
	{
		ui->btnCancel->setText(title);
		ui->btnCancel->show();

		m_ActionCancel = action;
	}
}

void WiredLanSetupDialog::SetInputs(CJsonNode node)
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

		if (iSetupInput_Hidden == typeInput)
		{
			m_HiddenKey = input.GetString(KEY_KEY);
			m_HiddenValue = input.GetString(KEY_VALUE);
		}

		if (i == 0 || i == 1)
		{
			if (iSetupInput_Disp == typeInput)
			{
			}
			else if (iSetupInput_IP == typeInput)
			{
				ui->frame0->show();
				ui->label0->setText(labelKey);
				ui->lineEdit0->setText(labelValue);
			}
		}
		if (i == 2 || i == 3)
		{
			if (iSetupInput_Disp == typeInput)
			{

			}
			else if (iSetupInput_IP == typeInput)
			{
				ui->frame1->show();
				ui->label1->setText(labelKey);
				ui->lineEdit1->setText(labelValue);
			}
		}
		if (i == 4 || i == 5)
		{
			if (iSetupInput_Disp == typeInput)
			{

			}
			else if (iSetupInput_IP == typeInput)
			{
				ui->frame2->show();
				ui->label2->setText(labelKey);
				ui->lineEdit2->setText(labelValue);
			}
		}
		if (i == 6 || i == 7)
		{
			if (iSetupInput_Disp == typeInput)
			{
			}
			else if (iSetupInput_IP == typeInput)
			{
				ui->frame3->show();
				ui->label3->setText(labelKey);
				ui->lineEdit3->setText(labelValue);
			}
		}
	}
}
