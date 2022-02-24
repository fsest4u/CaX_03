#include <QDesktopServices>

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

	ConnectSigToSlot();
	Initialize();
}

FormDialog::~FormDialog()
{
	delete ui;
}

CJsonNode FormDialog::GetNodeForm()
{
	CJsonNode nodeFileSystem(JSON_OBJECT);
	nodeFileSystem.Add(KEY_VALUE, ui->cbList->currentText());
	nodeFileSystem.AddInt(KEY_INDEX, ui->cbList->currentIndex());

	CJsonNode node(JSON_OBJECT);
	node.Add(m_HiddenKey, m_HiddenValue);
	if (!ui->frameList->isHidden())
	{
		node.Add(KEY_FILE_SYSTEM, nodeFileSystem);
	}

	return node;
}

void FormDialog::SetNodeForm(CJsonNode node)
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

void FormDialog::accept()
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

void FormDialog::reject()
{
	if (iSetupBtnAction_Cancel == m_ActionCancel)
	{
		done(QDialog::Rejected);
	}
}

void FormDialog::ConnectSigToSlot()
{
	connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void FormDialog::Initialize()
{
	ui->frame0->hide();
	ui->frame1->hide();
	ui->frame2->hide();
	ui->frame3->hide();
	ui->frame4->hide();
	ui->frameList->hide();

	ui->btnOK->hide();
	ui->btnCancel->hide();
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
					ui->lineEdit0->setText(labelValue);
					ui->lineEdit0->setReadOnly(readOnly);
					ui->lineEdit0->setStyleSheet(style);
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
					ui->lineEdit3->setText(labelValue);
					ui->lineEdit3->setReadOnly(readOnly);
					ui->lineEdit3->setStyleSheet(style);
				}
				else if (i == 4)
				{
					ui->frame4->show();
					ui->lineEdit4->setText(labelValue);
					ui->lineEdit4->setReadOnly(readOnly);
					ui->lineEdit4->setStyleSheet(style);
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
				else if (i == 4)
				{
					ui->frame4->show();
					ui->labelKey4->setMinimumWidth(100);
					ui->labelKey4->setText(labelKey);
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
					ui->lineEdit0->setText(labelValue);
					ui->lineEdit0->setReadOnly(readOnly);
					ui->lineEdit0->setStyleSheet(style);
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
					ui->lineEdit3->setText(labelValue);
					ui->lineEdit3->setReadOnly(readOnly);
					ui->lineEdit3->setStyleSheet(style);
				}
				else if (i == 4)
				{
					ui->frame4->show();
					ui->lineEdit4->setText(labelValue);
					ui->lineEdit4->setReadOnly(readOnly);
					ui->lineEdit4->setStyleSheet(style);
				}
			}
		}
		else if (iSetupInput_Hidden == typeInput)
		{
			m_HiddenKey = labelKey;
			m_HiddenValue = labelValue;
		}
		else if (iSetupInput_List == typeInput)
		{
			if (!labelKey.compare("File System"))
			{
				ui->frameList->show();
				ui->labelList->setText(labelKey);
				QStringList keys = input.GetStringList(KEY_KEYS);
				QStringList values = input.GetStringList(KEY_VALUES);

				QString key;
				QString value;
				for (int i = 0; i < values.count(); i++)
				{
					if (!keys.isEmpty() && keys.count() > i)
					{
						key = keys.at(i);
					}
					if (!values.isEmpty() && values.count() > i)
					{
						value = values.at(i);
					}

					if (key.isEmpty())
					{
						ui->cbList->addItem(value, QString::number(i));
					}
					else
					{
						ui->cbList->addItem(value, key);
					}
				}
				int index = input.GetString(KEY_VALUE).toInt();
				ui->cbList->setCurrentIndex(index);
			}
		}
	}
}
