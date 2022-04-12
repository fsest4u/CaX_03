#include <QDesktopServices>

#include "formdialog4.h"
#include "ui_formdialog4.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/utilnovatron.h"

#include "widget/setup.h"

FormDialog4::FormDialog4(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::FormDialog4)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();
}

FormDialog4::~FormDialog4()
{
	delete ui;
}

CJsonNode FormDialog4::GetNodeForm()
{
	CJsonNode node(JSON_OBJECT);
	node.Add(m_HiddenKey, m_HiddenValue);

	return node;
}

void FormDialog4::SetNodeForm(CJsonNode node)
{
//	LogDebug("node [%s]", node.ToCompactByteArray().data());
	setWindowTitle(node.GetString(KEY_TITLE_CAP));

	CJsonNode nodeOK = node.GetObject(KEY_OK);
	CJsonNode nodeCancel = node.GetObject(KEY_CANCEL);
	CJsonNode arrNodeInput = node.GetArray(KEY_INPUTS);

	m_Url = node.GetString(KEY_URL);

	SetOK(nodeOK.GetString(KEY_NAME_CAP), nodeOK.GetInt(KEY_ACTION));
	SetCancel(nodeCancel.GetString(KEY_NAME_CAP), nodeCancel.GetInt(KEY_ACTION));
	SetInputs(arrNodeInput);
}

void FormDialog4::accept()
{
	if (iSetupBtnAction_Request == m_ActionOK
			|| iSetupBtnAction_Event == m_ActionOK)
	{
		done(QDialog::Accepted);
	}
	else if (iSetupBtnAction_OpenWeb == m_ActionOK)
	{
		done(QDialog::Rejected);

		// web browser
		if (!m_Url.isEmpty())
		{
			QDesktopServices::openUrl(QUrl(m_Url));
		}
	}
}

void FormDialog4::reject()
{
	if (iSetupBtnAction_Cancel == m_ActionCancel)
	{
		done(QDialog::Rejected);
	}
}

void FormDialog4::ConnectSigToSlot()
{
	connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void FormDialog4::Initialize()
{
	ui->frame0->hide();
	ui->frame1->hide();
	ui->frame2->hide();
	ui->frame3->hide();

	ui->btnOK->hide();
	ui->btnCancel->hide();
}

void FormDialog4::SetOK(QString title, int action)
{
	UtilNovatron::DebugTypeForSetupBtn("SetOK", action);
	if (!title.isEmpty())
	{
		ui->btnOK->setText(title);
		ui->btnOK->show();

		m_ActionOK = action;
	}
}

void FormDialog4::SetCancel(QString title, int action)
{
	UtilNovatron::DebugTypeForSetupBtn("SetCancel", action);
	if (!title.isEmpty())
	{
		ui->btnCancel->setText(title);
		ui->btnCancel->show();

		m_ActionCancel = action;
	}
}

void FormDialog4::SetInputs(CJsonNode node)
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

		if (iSetupInput_Disp == typeInput)
		{
			if (!labelValue.isEmpty())
			{
				QString style = "";
				bool readOnly = true;
				if (iSetupInput_Text & typeInput)
				{
					readOnly = false;
				}
				else
				{
					readOnly = true;
					style = QString("QLineEdit	\
									{	\
										border: none;	\
										background: transparent;	\
									}");
				}

				if (i == 0)
				{
					ui->frame0->show();
					ui->labelValue0->setText(labelValue);
					ui->labelValue0->setStyleSheet(style);
				}
				else if (i == 1)
				{
					ui->frame1->show();
					ui->lineEdit1->setText(labelValue);
					ui->lineEdit1->setReadOnly(readOnly);
					ui->lineEdit1->setStyleSheet(style);
				}
				else if (i == 2)
				{
					ui->frame2->show();
					ui->lineEdit2->setText(labelValue);
					ui->lineEdit2->setReadOnly(readOnly);
					ui->lineEdit2->setStyleSheet(style);
				}
				else if (i == 3)
				{
					ui->frame3->show();
					ui->labelValue3->setText(labelValue);
					ui->labelValue3->setStyleSheet(style);
				}
			}
		}
		else if (iSetupInput_Label == typeInput
				 || iSetupInput_Text == typeInput)
		{
			if (!labelKey.isEmpty())
			{
				if (i == 0)
				{
					ui->frame0->show();
					ui->labelKey0->setMinimumWidth(100);
					ui->labelKey0->setText(labelKey);
				}
				else if (i == 1)
				{
					ui->frame1->show();
					ui->labelKey1->setMinimumWidth(100);
					ui->labelKey1->setText(labelKey);
				}
				else if (i == 2)
				{
					ui->frame2->show();
					ui->labelKey2->setMinimumWidth(100);
					ui->labelKey2->setText(labelKey);
				}
				else if (i == 3)
				{
					ui->frame3->show();
					ui->labelKey3->setMinimumWidth(100);
					ui->labelKey3->setText(labelKey);
				}
			}

			if (!labelValue.isEmpty())
			{
				QString style = "";
				bool readOnly = true;
				if (iSetupInput_Text & typeInput)
				{
					readOnly = false;
				}
				else
				{
					readOnly = true;
					style = QString("QLineEdit	\
									{	\
										border: none;	\
										background: transparent;	\
									}");
				}

				if (i == 0)
				{
					ui->frame0->show();
					ui->labelValue0->setText(labelValue);
					ui->labelValue0->setStyleSheet(style);
				}
				else if (i == 1)
				{
					ui->frame1->show();
					ui->lineEdit1->setText(labelValue);
					ui->lineEdit1->setReadOnly(readOnly);
					ui->lineEdit1->setStyleSheet(style);
				}
				else if (i == 2)
				{
					ui->frame2->show();
					ui->lineEdit2->setText(labelValue);
					ui->lineEdit2->setReadOnly(readOnly);
					ui->lineEdit2->setStyleSheet(style);
				}
				else if (i == 3)
				{
					ui->frame3->show();
					ui->labelValue3->setText(labelValue);
					ui->labelValue3->setStyleSheet(style);
				}
			}
		}
		else if (iSetupInput_Hidden == typeInput)
		{
			m_HiddenKey = labelKey;
			m_HiddenValue = labelValue;
		}
	}
}
