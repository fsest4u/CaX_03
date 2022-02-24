#include <QDate>
#include <QTime>

#include "timemanualdialog.h"
#include "ui_timemanualdialog.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/utilnovatron.h"

#include "widget/setup.h"

TimeManualDialog::TimeManualDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TimeManualDialog)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();
}

TimeManualDialog::~TimeManualDialog()
{
	delete ui;
}

CJsonNode TimeManualDialog::GetNodeForm()
{
	QString date = QString("%1-%2-%3").arg(ui->cbYear->currentText()).arg(ui->cbMonth->currentText()).arg(ui->cbDay->currentText());
	int hour = 0;
	if (!ui->cbAMPM->currentText().compare("PM"))
	{
		hour = ui->cbHour->currentText().toInt() + 12;
	}
	else
	{
		hour = ui->cbHour->currentText().toInt();
	}
	QString time = QString("%1:%2").arg(hour).arg(ui->cbMinute->currentText());


	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_DATE,		date);
	node.Add(KEY_TIME_CAP,	time);

	return node;
}

void TimeManualDialog::SetNodeForm(CJsonNode node)
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

void TimeManualDialog::accept()
{
	done(QDialog::Accepted);
}

void TimeManualDialog::reject()
{
	done(QDialog::Rejected);
}

void TimeManualDialog::ConnectSigToSlot()
{
	connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void TimeManualDialog::Initialize()
{
	ui->btnOK->hide();
	ui->btnCancel->hide();

	for (int i = 2000; i <= 2100; i++)
	{
		ui->cbYear->addItem(QString::number(i));
	}
	for (int i = 1; i <= 12; i++)
	{
		ui->cbMonth->addItem(QString::number(i));
	}
	for (int i = 1; i <= 31; i++)
	{
		ui->cbDay->addItem(QString::number(i));
	}

	ui->cbAMPM->addItems({"AM", "PM"});
	for (int i = 0; i < 12; i++)
	{
		ui->cbHour->addItem(QString::number(i));
	}
	for (int i = 0; i < 60; i++)
	{
		ui->cbMinute->addItem(QString::number(i));
	}

	int year = QDate::currentDate().year();
	int month = QDate::currentDate().month();
	int day = QDate::currentDate().day();
	ui->cbYear->setCurrentText(QString::number(year));
	ui->cbMonth->setCurrentText(QString::number(month));
	ui->cbDay->setCurrentText(QString::number(day));

	QString ampm = QTime::currentTime().toString("AP");
	QString hour = QTime::currentTime().toString("hh");
	QString minute = QTime::currentTime().toString("mm");
	ui->cbAMPM->setCurrentText(ampm);
	ui->cbHour->setCurrentText(hour);
	ui->cbMinute->setCurrentText(minute);
}

void TimeManualDialog::SetOK(QString title, int action)
{
	UtilNovatron::DebugTypeForSetupBtn("SetOK", action);
	if (!title.isEmpty())
	{
		ui->btnOK->setText(title);
		ui->btnOK->show();

		m_ActionOK = action;
	}
}

void TimeManualDialog::SetCancel(QString title, int action)
{
	UtilNovatron::DebugTypeForSetupBtn("SetCancel", action);
	if (!title.isEmpty())
	{
		ui->btnCancel->setText(title);
		ui->btnCancel->show();

		m_ActionCancel = action;
	}
}

void TimeManualDialog::SetInputs(CJsonNode node)
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

		if (iSetupInput_CurrDate == typeInput)
		{
			ui->labelDate->setText(labelKey);
		}
		else if (iSetupInput_CurrTime == typeInput)
		{
			ui->labelTime->setText(labelKey);
		}
	}
}
