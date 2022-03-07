#include <QDate>
#include <QTime>

#include "setupreservationrecordingdialog.h"
#include "ui_setupreservationrecordingdialog.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/utilnovatron.h"

SetupReservationRecordingDialog::SetupReservationRecordingDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SetupReservationRecordingDialog)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();
}

SetupReservationRecordingDialog::~SetupReservationRecordingDialog()
{
	delete ui;
}

CJsonNode SetupReservationRecordingDialog::GetNodeData()
{
	int hour = 0;
	if (!ui->cbAMPM->currentText().compare("PM"))
	{
		hour = ui->cbHour->currentText().toInt() + 12;
	}
	else
	{
		hour = ui->cbHour->currentText().toInt();
	}
	QString time = QString("%1-%2-%3 %4:%5")
			.arg(ui->cbYear->currentText())
			.arg(ui->cbMonth->currentText(), 2, '0')
			.arg(ui->cbDay->currentText(), 2, '0')
			.arg(QString::number(hour), 2, '0')
			.arg(ui->cbMinute->currentText(), 2, '0');

	hour = ui->cbRecordHour->currentText().toInt() * 60;
	int minute = ui->cbRecordMinute->currentText().toInt();

	CJsonNode node(JSON_OBJECT);
	if (m_ID >= 0)
	{
		node.Add(KEY_ID_UPPER, QString::number(m_ID));
	}
	node.Add(KEY_NAME_CAP, ui->labelTitle->text());
	node.Add(KEY_ACTIVE, ui->cbActive->isChecked());
	node.Add(KEY_TIME_CAP,	time);
	node.AddInt(KEY_DURATION,	hour + minute);

	return node;
}

void SetupReservationRecordingDialog::SetNodeData(CJsonNode node)
{
	m_ID = node.GetInt(KEY_ID_UPPER);
	ui->labelTitle->setText(node.GetString(KEY_TOP));
	ui->cbActive->setChecked(node.GetBool(KEY_ACTIVE));
	int duration = node.GetInt(KEY_DURATION);

	ui->cbRecordHour->setCurrentText(QString::number(duration / 60));
	ui->cbRecordMinute->setCurrentText(QString::number(duration % 60));

	QString nodeDate = node.GetString(KEY_DATE);

	if (nodeDate.isEmpty())
	{
		int year = QDate::currentDate().year();
		int month = QDate::currentDate().month();
		int day = QDate::currentDate().day();
		ui->cbYear->setCurrentText(QString::number(year));
		ui->cbMonth->setCurrentText(QString::number(month));
		ui->cbDay->setCurrentText(QString::number(day));

		int hour = QTime::currentTime().addSecs(600).toString("h").toInt();
		QString minute = QTime::currentTime().addSecs(600).toString("m");
		if (hour < 12)
		{
			ui->cbAMPM->setCurrentIndex(0);
		}
		else
		{
			ui->cbAMPM->setCurrentIndex(1);
		}
		ui->cbHour->setCurrentText(QString::number(hour % 12));
		ui->cbMinute->setCurrentText(minute);

	}
	else
	{
		QString date = nodeDate.split(" ").at(0);
		QString time = nodeDate.split(" ").at(1);

		QStringList listDate = date.split("-");
		QStringList listTime = time.split(":");

		ui->cbYear->setCurrentText(listDate[0]);
		ui->cbMonth->setCurrentText(listDate[1]);
		ui->cbDay->setCurrentText(listDate[2]);

		int hour = listTime[0].toInt();

		if (hour < 12)
		{
			ui->cbAMPM->setCurrentIndex(0);
		}
		else
		{
			ui->cbAMPM->setCurrentIndex(1);
		}
		ui->cbHour->setCurrentText(QString::number(hour % 12));
		ui->cbMinute->setCurrentText(listTime[1]);
	}


}

void SetupReservationRecordingDialog::accept()
{

	done(QDialog::Accepted);
}

void SetupReservationRecordingDialog::reject()
{
	done(QDialog::Rejected);
}

void SetupReservationRecordingDialog::ConnectSigToSlot()
{

}

void SetupReservationRecordingDialog::Initialize()
{
	m_ID = -1;

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
	for (int i = 1; i <= 12; i++)
	{
		ui->cbHour->addItem(QString::number(i));
	}
	for (int i = 0; i < 60; i++)
	{
		ui->cbMinute->addItem(QString::number(i));
	}

	for (int i = 0; i < 6; i++)
	{
		ui->cbRecordHour->addItem(QString::number(i));
	}
	for (int i = 0; i < 60; i++)
	{
		ui->cbRecordMinute->addItem(QString::number(i));
	}

	int year = QDate::currentDate().year();
	int month = QDate::currentDate().month();
	int day = QDate::currentDate().day();
	ui->cbYear->setCurrentText(QString::number(year));
	ui->cbMonth->setCurrentText(QString::number(month));
	ui->cbDay->setCurrentText(QString::number(day));

	int hour = QTime::currentTime().toString("hh").toInt();
	QString minute = QTime::currentTime().toString("mm");
	if (hour < 12)
	{
		ui->cbAMPM->setCurrentIndex(0);
	}
	else
	{
		ui->cbAMPM->setCurrentIndex(1);
	}
	ui->cbHour->setCurrentText(QString::number(hour % 12));
	ui->cbMinute->setCurrentText(minute);
}
