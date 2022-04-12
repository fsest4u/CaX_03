#include <QDesktopServices>

#include "formdialog2.h"
#include "ui_formdialog2.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/utilnovatron.h"

#include "widget/setup.h"

FormDialog2::FormDialog2(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::FormDialog2)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();
}

FormDialog2::~FormDialog2()
{
	delete ui;
}

CJsonNode FormDialog2::GetNodeForm()
{
	CJsonNode node(JSON_OBJECT);
	node.Add(m_HiddenKey, m_HiddenValue);

	return node;
}

void FormDialog2::SetNodeForm(CJsonNode node)
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

void FormDialog2::accept()
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

void FormDialog2::reject()
{
	if (iSetupBtnAction_Cancel == m_ActionCancel)
	{
		done(QDialog::Rejected);
	}
}

void FormDialog2::ConnectSigToSlot()
{
	connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void FormDialog2::Initialize()
{
	ui->frame0->hide();
	ui->frame1->hide();

	ui->btnOK->hide();
	ui->btnCancel->hide();
}

void FormDialog2::SetOK(QString title, int action)
{
	UtilNovatron::DebugTypeForSetupBtn("SetOK", action);
	if (!title.isEmpty())
	{
		ui->btnOK->setText(title);
		ui->btnOK->show();

		m_ActionOK = action;
	}
}

void FormDialog2::SetCancel(QString title, int action)
{
	UtilNovatron::DebugTypeForSetupBtn("SetCancel", action);
	if (!title.isEmpty())
	{
		ui->btnCancel->setText(title);
		ui->btnCancel->show();

		m_ActionCancel = action;
	}
}

void FormDialog2::SetInputs(CJsonNode node)
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
					ui->textEdit1->setText(labelValue);
					ui->textEdit1->setStyleSheet(style);
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
									}	\
									QTextEdit	\
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
					ui->textEdit1->setText(labelValue);
					ui->textEdit1->setStyleSheet(style);
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
