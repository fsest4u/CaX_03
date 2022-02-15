#include "customeqdialog.h"
#include "ui_customeqdialog.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/utilnovatron.h"

#include "widget/setup.h"

CustomEQDialog::CustomEQDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CustomEQDialog)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();
}

CustomEQDialog::~CustomEQDialog()
{
	delete ui;
}

void CustomEQDialog::SetNodeForm(CJsonNode node)
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

int CustomEQDialog::GetSliderValue00() const
{
	return ui->verticalSlider00->value();
}

int CustomEQDialog::GetSliderValue01() const
{
	return ui->verticalSlider01->value();
}

int CustomEQDialog::GetSliderValue02() const
{
	return ui->verticalSlider02->value();
}

int CustomEQDialog::GetSliderValue03() const
{
	return ui->verticalSlider03->value();
}

int CustomEQDialog::GetSliderValue04() const
{
	return ui->verticalSlider04->value();
}

int CustomEQDialog::GetSliderValue05() const
{
	return ui->verticalSlider05->value();
}

int CustomEQDialog::GetSliderValue06() const
{
	return ui->verticalSlider06->value();
}

int CustomEQDialog::GetSliderValue07() const
{
	return ui->verticalSlider07->value();
}

int CustomEQDialog::GetSliderValue08() const
{
	return ui->verticalSlider08->value();
}

int CustomEQDialog::GetSliderValue09() const
{
	return ui->verticalSlider09->value();
}

void CustomEQDialog::SlotSliderValueChanged00(int value)
{
	ui->labelEQ00->setText(QString::number(value));
}

void CustomEQDialog::SlotSliderValueChanged01(int value)
{
	ui->labelEQ01->setText(QString::number(value));
}

void CustomEQDialog::SlotSliderValueChanged02(int value)
{
	ui->labelEQ02->setText(QString::number(value));
}

void CustomEQDialog::SlotSliderValueChanged03(int value)
{
	ui->labelEQ03->setText(QString::number(value));
}

void CustomEQDialog::SlotSliderValueChanged04(int value)
{
	ui->labelEQ04->setText(QString::number(value));
}

void CustomEQDialog::SlotSliderValueChanged05(int value)
{
	ui->labelEQ05->setText(QString::number(value));
}

void CustomEQDialog::SlotSliderValueChanged06(int value)
{
	ui->labelEQ06->setText(QString::number(value));
}

void CustomEQDialog::SlotSliderValueChanged07(int value)
{
	ui->labelEQ07->setText(QString::number(value));
}

void CustomEQDialog::SlotSliderValueChanged08(int value)
{
	ui->labelEQ08->setText(QString::number(value));
}

void CustomEQDialog::SlotSliderValueChanged09(int value)
{
	ui->labelEQ09->setText(QString::number(value));
}

void CustomEQDialog::accept()
{
	done(QDialog::Accepted);
}

void CustomEQDialog::reject()
{
	done(QDialog::Rejected);
}

void CustomEQDialog::ConnectSigToSlot()
{
	connect(ui->verticalSlider00, SIGNAL(valueChanged(int)), this, SLOT(SlotSliderValueChanged00(int)));
	connect(ui->verticalSlider01, SIGNAL(valueChanged(int)), this, SLOT(SlotSliderValueChanged01(int)));
	connect(ui->verticalSlider02, SIGNAL(valueChanged(int)), this, SLOT(SlotSliderValueChanged02(int)));
	connect(ui->verticalSlider03, SIGNAL(valueChanged(int)), this, SLOT(SlotSliderValueChanged03(int)));
	connect(ui->verticalSlider04, SIGNAL(valueChanged(int)), this, SLOT(SlotSliderValueChanged04(int)));
	connect(ui->verticalSlider05, SIGNAL(valueChanged(int)), this, SLOT(SlotSliderValueChanged05(int)));
	connect(ui->verticalSlider06, SIGNAL(valueChanged(int)), this, SLOT(SlotSliderValueChanged06(int)));
	connect(ui->verticalSlider07, SIGNAL(valueChanged(int)), this, SLOT(SlotSliderValueChanged07(int)));
	connect(ui->verticalSlider08, SIGNAL(valueChanged(int)), this, SLOT(SlotSliderValueChanged08(int)));
	connect(ui->verticalSlider09, SIGNAL(valueChanged(int)), this, SLOT(SlotSliderValueChanged09(int)));

	connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void CustomEQDialog::Initialize()
{
	ui->frame00->hide();
	ui->frame01->hide();
	ui->frame02->hide();
	ui->frame03->hide();
	ui->frame04->hide();
	ui->frame05->hide();
	ui->frame06->hide();
	ui->frame07->hide();
	ui->frame08->hide();
	ui->frame09->hide();

	ui->btnOK->hide();
	ui->btnCancel->hide();
}

void CustomEQDialog::SetOK(QString title, int action)
{
	UtilNovatron::DebugTypeForSetupBtn("SetOK", action);
	if (!title.isEmpty())
	{
		ui->btnOK->setText(title);
		ui->btnOK->show();

		m_ActionOK = action;
	}
}

void CustomEQDialog::SetCancel(QString title, int action)
{
	UtilNovatron::DebugTypeForSetupBtn("SetCancel", action);
	if (!title.isEmpty())
	{
		ui->btnCancel->setText(title);
		ui->btnCancel->show();

		m_ActionCancel = action;
	}
}

void CustomEQDialog::SetInputs(CJsonNode node)
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
				if (i == 1)
				{
					ui->frame00->show();
					ui->label00->setText(labelKey);
				}
				else if (i == 2)
				{
					ui->frame01->show();
					ui->label01->setText(labelKey);
				}
				else if (i == 3)
				{
					ui->frame02->show();
					ui->label02->setText(labelKey);
				}
				else if (i == 4)
				{
					ui->frame03->show();
					ui->label03->setText(labelKey);
				}
				else if (i == 5)
				{
					ui->frame04->show();
					ui->label04->setText(labelKey);
				}
				else if (i == 6)
				{
					ui->frame05->show();
					ui->label05->setText(labelKey);
				}
				else if (i == 7)
				{
					ui->frame06->show();
					ui->label06->setText(labelKey);
				}
				else if (i == 8)
				{
					ui->frame07->show();
					ui->label07->setText(labelKey);
				}
				else if (i == 9)
				{
					ui->frame08->show();
					ui->label08->setText(labelKey);
				}
				else if (i == 10)
				{
					ui->frame09->show();
					ui->label09->setText(labelKey);
				}
			}

			if (min <= value && value <= max)
			{
				if (i == 1)
				{
					ui->frame00->show();
					ui->verticalSlider00->setMinimum(min);
					ui->verticalSlider00->setMaximum(max);
					ui->verticalSlider00->setValue(value);
				}
				else if (i == 2)
				{
					ui->frame01->show();
					ui->verticalSlider01->setMinimum(min);
					ui->verticalSlider01->setMaximum(max);
					ui->verticalSlider01->setValue(value);
				}
				else if (i == 3)
				{
					ui->frame02->show();
					ui->verticalSlider02->setMinimum(min);
					ui->verticalSlider02->setMaximum(max);
					ui->verticalSlider02->setValue(value);
				}
				else if (i == 4)
				{
					ui->frame03->show();
					ui->verticalSlider03->setMinimum(min);
					ui->verticalSlider03->setMaximum(max);
					ui->verticalSlider03->setValue(value);
				}
				else if (i == 5)
				{
					ui->frame04->show();
					ui->verticalSlider04->setMinimum(min);
					ui->verticalSlider04->setMaximum(max);
					ui->verticalSlider04->setValue(value);
				}
				else if (i == 6)
				{
					ui->frame05->show();
					ui->verticalSlider05->setMinimum(min);
					ui->verticalSlider05->setMaximum(max);
					ui->verticalSlider05->setValue(value);
				}
				else if (i == 7)
				{
					ui->frame06->show();
					ui->verticalSlider06->setMinimum(min);
					ui->verticalSlider06->setMaximum(max);
					ui->verticalSlider06->setValue(value);
				}
				else if (i == 8)
				{
					ui->frame07->show();
					ui->verticalSlider07->setMinimum(min);
					ui->verticalSlider07->setMaximum(max);
					ui->verticalSlider07->setValue(value);
				}
				else if (i == 9)
				{
					ui->frame08->show();
					ui->verticalSlider08->setMinimum(min);
					ui->verticalSlider08->setMaximum(max);
					ui->verticalSlider08->setValue(value);
				}
				else if (i == 10)
				{
					ui->frame09->show();
					ui->verticalSlider09->setMinimum(min);
					ui->verticalSlider09->setMaximum(max);
					ui->verticalSlider09->setValue(value);
				}
			}
		}

	}

}

