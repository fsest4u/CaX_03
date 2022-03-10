#include "groupplaydialog.h"
#include "ui_groupplaydialog.h"

#include "util/caxkeyvalue.h"
#include "util/caxtranslate.h"

GroupPlayDialog::GroupPlayDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::GroupPlayDialog)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();
}

GroupPlayDialog::~GroupPlayDialog()
{
	delete ui;
}

void GroupPlayDialog::SetData(CJsonNode node)
{
	m_Addr = node.GetString(KEY_LOCATION);

	if (node.GetInt(KEY_GROUP_PLAY_STATUS, m_Status))
	{
		if (m_Status == GROUP_PLAY_IDLE)
		{

		}
		else if (m_Status == GROUP_PLAY_CLIENT)
		{
			m_Server = node.GetString(KEY_SERVER);
			ui->framePlayStop->show();

		}
		else if (m_Status == GROUP_PLAY_SERVER)
		{
			ui->framePlayStop->show();
		}
	}

	bool enabled;
	if (node.GetBool(KEY_ENABLED, enabled))
	{
		ui->frameGroupPlay->show();
		ui->checkBoxGroupPlay->setChecked(enabled);
	}
	if (node.GetBool(KEY_AUTO_JOIN_ENABLED, enabled))
	{
		ui->frameAutoJoin->show();
		ui->checkBoxAutoJoin->setChecked(enabled);
	}
	if (node.GetBool(KEY_MUTED, enabled))
	{
		ui->frameMute->show();
		ui->checkBoxMute->setChecked(enabled);
	}

	int volume = 0;
	if (node.GetInt(KEY_VOLUME, volume))
	{
		ui->frameVolume->show();
		ui->horizontalSlider->setValue(volume);
	}
	int channel = 0;
	if (node.GetInt(KEY_ACTIVE_CHANNEL, channel))
	{
		ui->frameChannel->show();
		ui->comboBoxChannel->setCurrentIndex(channel);
	}

	ui->checkBoxPowerOff->setChecked(false);
}

void GroupPlayDialog::SetSelf(bool enable)
{
	m_Self = enable;
	if (m_Self)
	{
		ui->labelPowerOff->setText(STR_POWER_OFF_OTHERS);
	}
	else
	{
		ui->labelPowerOff->setText(STR_POWER_OFF);
	}
}

void GroupPlayDialog::SlotGroupPlay(bool enable)
{
	emit SigGroupPlay(m_Addr, enable);
}

void GroupPlayDialog::SlotAutoJoin(bool enable)
{
	emit SigAutoJoin(m_Addr, enable);
}

void GroupPlayDialog::SlotMute(bool enable)
{
	emit SigMute(m_Addr, enable);
}

void GroupPlayDialog::SlotPlayStop(bool enable)
{
	if (!enable)
	{
		return;
	}

	emit SigPlayStop(m_Addr, enable);

	if (m_Status == GROUP_PLAY_SERVER)
	{
		ui->framePlayStop->hide();
	}
	else if (m_Status == GROUP_PLAY_CLIENT)
	{
		ui->labelPlayStop->setText(STR_ADD_TO_DEVICE);
		ui->checkBoxPlayStop->setChecked(false);
	}

}

void GroupPlayDialog::SlotPowerOff(bool enable)
{
	if (enable)
	{
		emit SigPowerOff(m_Addr, m_Self);
	}
}

void GroupPlayDialog::SlotVolumeSliderReleased()
{
	int value = ui->horizontalSlider->value();
	emit SigVolumeSliderReleased(m_Addr, value);
}

void GroupPlayDialog::SlotChangedChannel(int value)
{
	emit SigChangedChannel(m_Addr, value);
}

void GroupPlayDialog::ConnectSigToSlot()
{
	connect(ui->checkBoxGroupPlay, SIGNAL(clicked(bool)), this, SLOT(SlotGroupPlay(bool)));
	connect(ui->checkBoxAutoJoin, SIGNAL(clicked(bool)), this, SLOT(SlotAutoJoin(bool)));
	connect(ui->checkBoxMute, SIGNAL(clicked(bool)), this, SLOT(SlotMute(bool)));
	connect(ui->checkBoxPlayStop, SIGNAL(clicked(bool)), this, SLOT(SlotPlayStop(bool)));
	connect(ui->checkBoxPowerOff, SIGNAL(clicked(bool)), this, SLOT(SlotPowerOff(bool)));

	connect(ui->horizontalSlider, SIGNAL(sliderReleased()), this, SLOT(SlotVolumeSliderReleased()));
	connect(ui->comboBoxChannel, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotChangedChannel(int)));

}

void GroupPlayDialog::Initialize()
{
//	ui->frameGroupPlay->hide();
//	ui->frameAutoJoin->hide();
//	ui->frameVolume->hide();
//	ui->frameChannel->hide();
//	ui->frameMute->hide();
//	ui->framePowerOff->hide();

	ui->comboBoxChannel->addItem(STR_STEREO);
	ui->comboBoxChannel->addItem(STR_LEFT);
	ui->comboBoxChannel->addItem(STR_RIGHT);

	m_Addr = "";
	m_Status = GROUP_PLAY_IDLE;
	m_Self = false;
	m_Server = "";

	ui->framePlayStop->hide();
}
