#include "analoginvolumedialog.h"
#include "ui_analoginvolumedialog.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/utilnovatron.h"

#include "widget/setup.h"

AnalogInVolumeDialog::AnalogInVolumeDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AnalogInVolumeDialog)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();
}

AnalogInVolumeDialog::~AnalogInVolumeDialog()
{
	delete ui;
}

void AnalogInVolumeDialog::SetNodeForm(CJsonNode node)
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

int AnalogInVolumeDialog::GetSliderValue0() const
{
	return ui->horizontalSlider0->value();
}

int AnalogInVolumeDialog::GetSliderValue1() const
{
	return ui->horizontalSlider1->value();
}

int AnalogInVolumeDialog::GetSliderValue2() const
{
	return ui->horizontalSlider2->value();
}

void AnalogInVolumeDialog::SlotSliderValueChanged0(int value)
{
	ui->labelVol0->setText(QString::number(value));
}

void AnalogInVolumeDialog::SlotSliderValueChanged1(int value)
{
	ui->labelVol1->setText(QString::number(value));
}

void AnalogInVolumeDialog::SlotSliderValueChanged2(int value)
{
	ui->labelVol2->setText(QString::number(value));
}

void AnalogInVolumeDialog::accept()
{
	done(QDialog::Accepted);
}

void AnalogInVolumeDialog::reject()
{
	done(QDialog::Rejected);
}

void AnalogInVolumeDialog::ConnectSigToSlot()
{
	connect(ui->horizontalSlider0, SIGNAL(valueChanged(int)), this, SLOT(SlotSliderValueChanged0(int)));
	connect(ui->horizontalSlider1, SIGNAL(valueChanged(int)), this, SLOT(SlotSliderValueChanged1(int)));
	connect(ui->horizontalSlider2, SIGNAL(valueChanged(int)), this, SLOT(SlotSliderValueChanged2(int)));

	connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void AnalogInVolumeDialog::Initialize()
{
	ui->frame0->hide();
	ui->frame1->hide();
	ui->frame2->hide();

	ui->btnOK->hide();
	ui->btnCancel->hide();
}

void AnalogInVolumeDialog::SetOK(QString title, int action)
{
	UtilNovatron::DebugTypeForSetupBtn("SetOK", action);
	if (!title.isEmpty())
	{
		ui->btnOK->setText(title);
		ui->btnOK->show();

		m_ActionOK = action;
	}
}

void AnalogInVolumeDialog::SetCancel(QString title, int action)
{
	UtilNovatron::DebugTypeForSetupBtn("SetCancel", action);
	if (!title.isEmpty())
	{
		ui->btnCancel->setText(title);
		ui->btnCancel->show();

		m_ActionCancel = action;
	}
}

void AnalogInVolumeDialog::SetInputs(CJsonNode node)
{
	CJsonNode input;
	for (int i = 0; i < node.ArraySize(); i++)
	{
		input = node.GetArrayAt(i);
		LogDebug("node : [%s]", input.ToCompactByteArray().data());

		int typeInput = input.GetInt(KEY_TYPE);
		UtilNovatron::DebugTypeForSetupInput("SetInputs", typeInput);

		if (iSetupInput_Slider == typeInput)
		{
			QString labelKey = input.GetString(KEY_KEY);
			int value = input.GetInt(KEY_VALUE);
			int min = input.GetInt(KEY_MIN);
			int max = input.GetInt(KEY_MAX);
			bool show = input.GetBool(KEY_SHOW_LABEL);

			if (show)
			{
				if (i == 0)
				{
					ui->frame0->show();
					ui->label0->setText(labelKey);
				}
				else if (i == 1)
				{
					ui->frame1->show();
					ui->label1->setText(labelKey);
				}
				else if (i == 2)
				{
					ui->frame2->show();
					ui->label2->setText(labelKey);
				}
			}

			if (min <= value && value <= max)
			{
				if (i == 0)
				{
					ui->frame0->show();
					ui->horizontalSlider0->setMinimum(min);
					ui->horizontalSlider0->setMaximum(max);
					ui->horizontalSlider0->setValue(value);
				}
				else if (i == 1)
				{
					ui->frame1->show();
					ui->horizontalSlider1->setMinimum(min);
					ui->horizontalSlider1->setMaximum(max);
					ui->horizontalSlider1->setValue(value);
				}
				else if (i == 2)
				{
					ui->frame2->show();
					ui->horizontalSlider2->setMinimum(min);
					ui->horizontalSlider2->setMaximum(max);
					ui->horizontalSlider2->setValue(value);
				}
			}
		}

	}

}
