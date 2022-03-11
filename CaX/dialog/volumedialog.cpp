#include <QMouseEvent>

#include "volumedialog.h"
#include "ui_volumedialog.h"

#include "util/log.h"
#include "util/settingio.h"

const QString SETTINGS_GROUP = "Setup";

VolumeDialog::VolumeDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::VolumeDialog)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	ReadSettings();
	Initialize();
}

VolumeDialog::~VolumeDialog()
{
	delete ui;
}

void VolumeDialog::SetSliderValue(int value)
{
	if (value <= 0)
	{
		value = 0;
	}
	if (m_MaxVolume <= value)
	{
		value = m_MaxVolume;
	}
	ui->horizontalSlider->setValue(value);
}

bool VolumeDialog::eventFilter(QObject *object, QEvent *event)
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

void VolumeDialog::SlotBtnClose()
{
	close();
}

void VolumeDialog::SlotBtnIncrease()
{
	int value = ui->horizontalSlider->value();
	if (m_MaxVolume > value)
	{
		value++;
		ui->horizontalSlider->setValue(value);
		emit SigIncrease(value);
	}
}

void VolumeDialog::SlotBtnDecrease()
{
	int value = ui->horizontalSlider->value();
	if (0 < value)
	{
		value--;
		ui->horizontalSlider->setValue(value);
		emit SigDecrease(value);
	}
}

void VolumeDialog::SlotSliderValueChanged(int value)
{
	if (value < 0)
	{
		value = 0;
	}
	if (m_MaxVolume < value)
	{
		value = m_MaxVolume;
	}
	ui->horizontalSlider->setValue(value);

	emit SigSliderValueChanged(value);
}

void VolumeDialog::ReadSettings()
{
	SettingIO settings;
	settings.beginGroup(SETTINGS_GROUP);

	m_MaxVolume = settings.value("setup_max_volume").toInt();

	settings.endGroup();

	if (m_MaxVolume <= 0)
	{
		m_MaxVolume = 30;
	}
}

void VolumeDialog::ConnectSigToSlot()
{
	connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(SlotBtnClose()));
	connect(ui->btnIncrease, SIGNAL(clicked()), this, SLOT(SlotBtnIncrease()));
	connect(ui->btnDecrease, SIGNAL(clicked()), this, SLOT(SlotBtnDecrease()));
	connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(SlotSliderValueChanged(int)));

}

void VolumeDialog::Initialize()
{
	setWindowFlags(Qt::FramelessWindowHint);
	installEventFilter(this);
}
