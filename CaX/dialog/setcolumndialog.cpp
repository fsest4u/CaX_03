#include "setcolumndialog.h"
#include "ui_setcolumndialog.h"

SetColumnDialog::SetColumnDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SetColumnDialog)
{
	ui->setupUi(this);
}

SetColumnDialog::~SetColumnDialog()
{
	delete ui;
}

bool SetColumnDialog::GetCBMood()
{
	if (ui->cbMood->checkState() == Qt::Checked)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SetColumnDialog::SetCBMood(bool bCheck)
{
	if (bCheck)
	{
		ui->cbMood->setCheckState(Qt::Checked);
	}
	else
	{
		ui->cbMood->setCheckState(Qt::Unchecked);
	}
}

bool SetColumnDialog::GetCBTempo()
{
	if (ui->cbTempo->checkState() == Qt::Checked)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SetColumnDialog::SetCBTempo(bool bCheck)
{
	if (bCheck)
	{
		ui->cbTempo->setCheckState(Qt::Checked);
	}
	else
	{
		ui->cbTempo->setCheckState(Qt::Unchecked);
	}
}

bool SetColumnDialog::GetCBFormat()
{
	if (ui->cbFormat->checkState() == Qt::Checked)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SetColumnDialog::SetCBFormat(bool bCheck)
{
	if (bCheck)
	{
		ui->cbFormat->setCheckState(Qt::Checked);
	}
	else
	{
		ui->cbFormat->setCheckState(Qt::Unchecked);
	}
}

bool SetColumnDialog::GetCBSampleRate()
{
	if (ui->cbSampleRate->checkState() == Qt::Checked)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SetColumnDialog::SetCBSampleRate(bool bCheck)
{
	if (bCheck)
	{
		ui->cbSampleRate->setCheckState(Qt::Checked);
	}
	else
	{
		ui->cbSampleRate->setCheckState(Qt::Unchecked);
	}
}

bool SetColumnDialog::GetCBBitDepth()
{
	if (ui->cbBitDepth->checkState() == Qt::Checked)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SetColumnDialog::SetCBBitDepth(bool bCheck)
{
	if (bCheck)
	{
		ui->cbBitDepth->setCheckState(Qt::Checked);
	}
	else
	{
		ui->cbBitDepth->setCheckState(Qt::Unchecked);
	}
}

bool SetColumnDialog::GetCBRating()
{
	if (ui->cbRating->checkState() == Qt::Checked)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SetColumnDialog::SetCBRating(bool bCheck)
{
	if (bCheck)
	{
		ui->cbRating->setCheckState(Qt::Checked);
	}
	else
	{
		ui->cbRating->setCheckState(Qt::Unchecked);
	}
}

