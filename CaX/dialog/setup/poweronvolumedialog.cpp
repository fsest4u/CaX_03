#include "poweronvolumedialog.h"
#include "ui_poweronvolumedialog.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/utilnovatron.h"

#include "widget/setup.h"

PowerOnVolumeDialog::PowerOnVolumeDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PowerOnVolumeDialog)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();
}

PowerOnVolumeDialog::~PowerOnVolumeDialog()
{
	delete ui;
}

CJsonNode PowerOnVolumeDialog::GetNodeForm()
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_VOLUME_CAP,	QString::number(ui->horizontalSlider0->value()));
	node.Add(m_HiddenKey,		m_HiddenValue);

	return node;
}

void PowerOnVolumeDialog::SetNodeForm(CJsonNode node)
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

void PowerOnVolumeDialog::SlotSliderValueChanged0(int value)
{
	ui->labelVol0->setText(QString::number(value));
}

void PowerOnVolumeDialog::accept()
{
	done(QDialog::Accepted);
}

void PowerOnVolumeDialog::reject()
{
	done(QDialog::Rejected);
}

void PowerOnVolumeDialog::ConnectSigToSlot()
{
	connect(ui->horizontalSlider0, SIGNAL(valueChanged(int)), this, SLOT(SlotSliderValueChanged0(int)));

	connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void PowerOnVolumeDialog::Initialize()
{
	ui->frame0->hide();

	ui->btnOK->hide();
	ui->btnCancel->hide();
}

void PowerOnVolumeDialog::SetOK(QString title, int action)
{
	UtilNovatron::DebugTypeForSetupBtn("SetOK", action);
	if (!title.isEmpty())
	{
		ui->btnOK->setText(title);
		ui->btnOK->show();

		m_ActionOK = action;
	}
}

void PowerOnVolumeDialog::SetCancel(QString title, int action)
{
	UtilNovatron::DebugTypeForSetupBtn("SetCancel", action);
	if (!title.isEmpty())
	{
		ui->btnCancel->setText(title);
		ui->btnCancel->show();

		m_ActionCancel = action;
	}
}

void PowerOnVolumeDialog::SetInputs(CJsonNode node)
{
	CJsonNode input;
	for (int i = 0; i < node.ArraySize(); i++)
	{
		input = node.GetArrayAt(i);
//		LogDebug("node : [%s]", input.ToCompactByteArray().data());

		int typeInput = input.GetInt(KEY_TYPE);
		UtilNovatron::DebugTypeForSetupInput("SetInputs", typeInput);

		if (iSetupInput_Hidden == typeInput)
		{
			m_HiddenKey = input.GetString(KEY_KEY);
			m_HiddenValue = input.GetString(KEY_VALUE);
		}
		else if (iSetupInput_Slider == typeInput)
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

			}
		}

	}
}
