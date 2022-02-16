#include <QTime>

#include "alarmdialog.h"
#include "ui_alarmdialog.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/utilnovatron.h"

#include "widget/setup.h"

AlarmDialog::AlarmDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AlarmDialog)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();
}

AlarmDialog::~AlarmDialog()
{
	delete ui;
}

CJsonNode AlarmDialog::GetNodeForm()
{
	QString active = ui->checkBoxActive->isChecked() ? "On" : "Off";
	int hour;
	if (ui->cbAMPM->currentIndex() == 0)
	{
		hour = ui->cbHour->currentText().toInt();
	}
	else
	{
		hour = ui->cbHour->currentText().toInt() + 12;
	}

	if (hour >= 24)
	{
		hour = 0;
	}

	CJsonNode nodeRepeat(JSON_OBJECT);
	nodeRepeat.Add(KEY_KEY, ui->cbRepeat->currentData().toString());
	nodeRepeat.Add(KEY_VALUE, ui->cbRepeat->currentText());
	nodeRepeat.AddInt(KEY_INDEX, ui->cbRepeat->currentIndex());

	CJsonNode nodeType(JSON_OBJECT);
	nodeType.Add(KEY_KEY, ui->cbType->currentData().toString());
	nodeType.Add(KEY_VALUE, ui->cbType->currentText());
	nodeType.AddInt(KEY_INDEX, ui->cbType->currentIndex());

	CJsonNode nodeSound(JSON_OBJECT);
	nodeSound.Add(KEY_KEY, ui->cbSound->currentData().toString());
	nodeSound.Add(KEY_VALUE, ui->cbSound->currentText());
	nodeSound.AddInt(KEY_INDEX, ui->cbSound->currentIndex());

	CJsonNode node(JSON_OBJECT);
	node.Add(m_HiddenKey, m_HiddenValue);
	node.Add(KEY_ACTIVE, active);
	node.Add(KEY_TIME_CAP, QString("%1:%2").arg(QString::number(hour).rightJustified(2, '0')).arg(ui->cbMinute->currentText().rightJustified(2, '0')));
	if (!ui->frameVolume->isHidden())
	{
		node.AddInt(KEY_VOLUME_CAP, ui->horizontalSlider->value());
	}
	if (!ui->frameRepeat->isHidden())
	{
		node.Add(KEY_REPEAT, nodeRepeat);
	}
	if (!ui->frameType->isHidden())
	{
		node.Add(KEY_TYPE, nodeType);
	}
	if (!ui->frameSound->isHidden())
	{
		node.Add(KEY_SOUND, nodeSound);
	}

	return node;
}

void AlarmDialog::SetNodeForm(CJsonNode node)
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

void AlarmDialog::SlotActive(bool active)
{
	ui->checkBoxActive->setChecked(active);
}

void AlarmDialog::SlotSliderValueChanged(int value)
{
	ui->labelVolumeValue->setText(QString::number(value));
}

void AlarmDialog::accept()
{
	done(QDialog::Accepted);
}

void AlarmDialog::reject()
{
	done(QDialog::Rejected);
}

void AlarmDialog::ConnectSigToSlot()
{
	connect(ui->checkBoxActive, SIGNAL(clicked(bool)), this, SLOT(SlotActive(bool)));
	connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(SlotSliderValueChanged(int)));

	connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void AlarmDialog::Initialize()
{
	ui->frameActive->hide();
	ui->frameVolume->hide();
	ui->frameRepeat->hide();
	ui->frameType->hide();
	ui->frameSound->hide();
	ui->frameTime->hide();

	ui->btnOK->hide();
	ui->btnCancel->hide();

	ui->cbAMPM->addItems({"AM", "PM"});
	for (int i = 0; i <= 12; i++)
	{
		ui->cbHour->addItem(QString::number(i));
	}
	for (int i = 0; i < 60; i++)
	{
		ui->cbMinute->addItem(QString::number(i));
	}

	QString ampm = QTime::currentTime().toString("AP");
	QString hour = QTime::currentTime().toString("hh");
	QString minute = QTime::currentTime().toString("mm");
	ui->cbAMPM->setCurrentText(ampm);
	ui->cbHour->setCurrentText(hour);
	ui->cbMinute->setCurrentText(minute);
}

void AlarmDialog::SetOK(QString title, int action)
{
	UtilNovatron::DebugTypeForSetupBtn("SetOK", action);
	if (!title.isEmpty())
	{
		ui->btnOK->setText(title);
		ui->btnOK->show();

		m_ActionOK = action;
	}
}

void AlarmDialog::SetCancel(QString title, int action)
{
	UtilNovatron::DebugTypeForSetupBtn("SetCancel", action);
	if (!title.isEmpty())
	{
		ui->btnCancel->setText(title);
		ui->btnCancel->show();

		m_ActionCancel = action;
	}
}

void AlarmDialog::SetInputs(CJsonNode node)
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

		if (iSetupInput_Switch == typeInput)
		{
			ui->frameActive->show();
			ui->labelActive->setText(labelKey);
			if (!labelValue.compare("On"))
			{
				ui->checkBoxActive->setChecked(true);
			}
			else
			{
				ui->checkBoxActive->setChecked(false);
			}

		}
		else if (iSetupInput_Slider == typeInput)
		{
			int min = input.GetInt(KEY_MIN);
			int max = input.GetInt(KEY_MAX);
			bool show = input.GetBool(KEY_SHOW_LABEL);

			if (show)
			{
				ui->frameVolume->show();
				ui->labelVolume->setText(labelKey);
				ui->horizontalSlider->setMinimum(min);
				ui->horizontalSlider->setMaximum(max);
				ui->horizontalSlider->setValue(labelValue.toInt());
			}
		}
		else if (iSetupInput_List == typeInput)
		{
			if (!labelKey.compare("Repeat"))
			{
				ui->frameRepeat->show();
				ui->labelRepeat->setText(labelKey);
				QStringList keys = input.GetStringList(KEY_KEYS);
				QStringList values = input.GetStringList(KEY_VALUES);

				for (int i = 0; i < values.count(); i++)
				{
					ui->cbRepeat->addItem(values.at(i), keys.at(i));
				}
				int index = input.GetString(KEY_VALUE).toInt();
				ui->cbRepeat->setCurrentIndex(index);
			}
			else if (!labelKey.compare("Type"))
			{
				ui->frameType->show();
				ui->labelType->setText(labelKey);
				QStringList keys = input.GetStringList(KEY_KEYS);
				QStringList values = input.GetStringList(KEY_VALUES);

				for (int i = 0; i < values.count(); i++)
				{
					ui->cbType->addItem(values.at(i), keys.at(i));
				}
				int index = input.GetString(KEY_VALUE).toInt();
				ui->cbType->setCurrentIndex(index);
			}
			else if (!labelKey.compare("Sound"))
			{
				ui->frameSound->show();
				ui->labelSound->setText(labelKey);
				QStringList keys = input.GetStringList(KEY_KEYS);
				QStringList values = input.GetStringList(KEY_VALUES);

				for (int i = 0; i < values.count(); i++)
				{
					ui->cbSound->addItem(values.at(i), keys.at(i));
				}
				int index = input.GetString(KEY_VALUE).toInt();
				ui->cbSound->setCurrentIndex(index);
			}
		}
		else if (iSetupInput_Time == typeInput)
		{
			ui->frameTime->show();
			ui->labelTime->setText(labelKey);
			QStringList listTime = labelValue.split(":");
			if (listTime.count() > 1)
			{
				int hour;
				int minute;
				if (listTime[0].toInt() < 12)
				{
					ui->cbAMPM->setCurrentIndex(0);
					hour = listTime[0].toInt();
					minute = listTime[1].toInt();
				}
				else
				{
					ui->cbAMPM->setCurrentIndex(1);
					hour = listTime[0].toInt() - 12;
					minute = listTime[1].toInt();
				}
				ui->cbHour->setCurrentText(QString::number(hour));
				ui->cbMinute->setCurrentText(QString::number(minute));
			}
		}
		else if (iSetupInput_Hidden == typeInput)
		{
			m_HiddenKey = labelKey;
			m_HiddenValue = labelValue;
		}
	}
}
