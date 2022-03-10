#include <QMouseEvent>

#include "resizedialog.h"
#include "ui_resizedialog.h"

#include "util/log.h"

ResizeDialog::ResizeDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ResizeDialog)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();
}

ResizeDialog::~ResizeDialog()
{
	delete ui;
}

void ResizeDialog::SetSliderValue(int min, int max, int value)
{
	ui->horizontalSlider->setMinimum(min);
	ui->horizontalSlider->setMaximum(max);

	if (value <= ui->horizontalSlider->minimum())
	{
		value = ui->horizontalSlider->minimum();
	}
	if (ui->horizontalSlider->maximum() <= value)
	{
		value = ui->horizontalSlider->maximum();
	}
	ui->horizontalSlider->setValue(value);
}

bool ResizeDialog::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (((QMouseEvent*)event)->button() == Qt::LeftButton)
		{
			if (object == this)
			{
				close();
				return true;
			}
		}
	}

	return QObject::eventFilter(object, event);
}

void ResizeDialog::SlotSliderValueChanged(int value)
{
	if (value <= ui->horizontalSlider->minimum())
	{
		value = ui->horizontalSlider->minimum();
	}
	if (ui->horizontalSlider->maximum() <= value)
	{
		value = ui->horizontalSlider->maximum();
	}
	ui->horizontalSlider->setValue(value);

	emit SigSliderValueChanged(value);
}

void ResizeDialog::ConnectSigToSlot()
{
	connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(SlotSliderValueChanged(int)));
}

void ResizeDialog::Initialize()
{
	setWindowFlags(Qt::FramelessWindowHint);
	installEventFilter(this);
}

