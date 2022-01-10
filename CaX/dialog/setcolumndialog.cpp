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

bool SetColumnDialog::GetCBFavorite()
{
	if (ui->cbFavorite->checkState() == Qt::Checked)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SetColumnDialog::SetCBFavorite(bool bCheck)
{
	if (bCheck)
	{
		ui->cbFavorite->setCheckState(Qt::Checked);
	}
	else
	{
		ui->cbFavorite->setCheckState(Qt::Unchecked);
	}
}

bool SetColumnDialog::GetCBTime()
{
	if (ui->cbTime->checkState() == Qt::Checked)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SetColumnDialog::SetCBTime(bool bCheck)
{
	if (bCheck)
	{
		ui->cbTime->setCheckState(Qt::Checked);
	}
	else
	{
		ui->cbTime->setCheckState(Qt::Unchecked);
	}
}

bool SetColumnDialog::GetCBArtist()
{
	if (ui->cbArtist->checkState() == Qt::Checked)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SetColumnDialog::SetCBArtist(bool bCheck)
{
	if (bCheck)
	{
		ui->cbArtist->setCheckState(Qt::Checked);
	}
	else
	{
		ui->cbArtist->setCheckState(Qt::Unchecked);
	}
}

bool SetColumnDialog::GetCBAlbum()
{
	if (ui->cbAlbum->checkState() == Qt::Checked)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SetColumnDialog::SetCBAlbum(bool bCheck)
{
	if (bCheck)
	{
		ui->cbAlbum->setCheckState(Qt::Checked);
	}
	else
	{
		ui->cbAlbum->setCheckState(Qt::Unchecked);
	}
}

bool SetColumnDialog::GetCBGenre()
{
	if (ui->cbGenre->checkState() == Qt::Checked)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SetColumnDialog::SetCBGenre(bool bCheck)
{
	if (bCheck)
	{
		ui->cbGenre->setCheckState(Qt::Checked);
	}
	else
	{
		ui->cbGenre->setCheckState(Qt::Unchecked);
	}
}

bool SetColumnDialog::GetCBAlbumArtist()
{
	if (ui->cbAlbumArtist->checkState() == Qt::Checked)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SetColumnDialog::SetCBAlbumArtist(bool bCheck)
{
	if (bCheck)
	{
		ui->cbAlbumArtist->setCheckState(Qt::Checked);
	}
	else
	{
		ui->cbAlbumArtist->setCheckState(Qt::Unchecked);
	}
}

bool SetColumnDialog::GetCBComposer()
{
	if (ui->cbComposer->checkState() == Qt::Checked)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SetColumnDialog::SetCBComposer(bool bCheck)
{
	if (bCheck)
	{
		ui->cbComposer->setCheckState(Qt::Checked);
	}
	else
	{
		ui->cbComposer->setCheckState(Qt::Unchecked);
	}
}

bool SetColumnDialog::GetCBYear()
{
	if (ui->cbYear->checkState() == Qt::Checked)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SetColumnDialog::SetCBYear(bool bCheck)
{
	if (bCheck)
	{
		ui->cbYear->setCheckState(Qt::Checked);
	}
	else
	{
		ui->cbYear->setCheckState(Qt::Unchecked);
	}
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

