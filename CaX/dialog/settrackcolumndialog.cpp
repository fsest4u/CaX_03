#include "settrackcolumndialog.h"
#include "ui_settrackcolumndialog.h"

SetTrackColumnDialog::SetTrackColumnDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SetTrackColumnDialog)
{
	ui->setupUi(this);

	ui->cbRating->hide();
}

SetTrackColumnDialog::~SetTrackColumnDialog()
{
	delete ui;
}

bool SetTrackColumnDialog::GetCBFavorite()
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

void SetTrackColumnDialog::SetCBFavorite(bool bCheck)
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

bool SetTrackColumnDialog::GetCBTime()
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

void SetTrackColumnDialog::SetCBTime(bool bCheck)
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

bool SetTrackColumnDialog::GetCBArtist()
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

void SetTrackColumnDialog::SetCBArtist(bool bCheck)
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

bool SetTrackColumnDialog::GetCBAlbum()
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

void SetTrackColumnDialog::SetCBAlbum(bool bCheck)
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

bool SetTrackColumnDialog::GetCBGenre()
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

void SetTrackColumnDialog::SetCBGenre(bool bCheck)
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

bool SetTrackColumnDialog::GetCBAlbumArtist()
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

void SetTrackColumnDialog::SetCBAlbumArtist(bool bCheck)
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

bool SetTrackColumnDialog::GetCBComposer()
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

void SetTrackColumnDialog::SetCBComposer(bool bCheck)
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

bool SetTrackColumnDialog::GetCBYear()
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

void SetTrackColumnDialog::SetCBYear(bool bCheck)
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

bool SetTrackColumnDialog::GetCBMood()
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

void SetTrackColumnDialog::SetCBMood(bool bCheck)
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

bool SetTrackColumnDialog::GetCBTempo()
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

void SetTrackColumnDialog::SetCBTempo(bool bCheck)
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

bool SetTrackColumnDialog::GetCBFormat()
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

void SetTrackColumnDialog::SetCBFormat(bool bCheck)
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

bool SetTrackColumnDialog::GetCBSampleRate()
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

void SetTrackColumnDialog::SetCBSampleRate(bool bCheck)
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

bool SetTrackColumnDialog::GetCBBitrate()
{
	if (ui->cbBitrate->checkState() == Qt::Checked)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SetTrackColumnDialog::SetCBBitrate(bool bCheck)
{
	if (bCheck)
	{
		ui->cbBitrate->setCheckState(Qt::Checked);
	}
	else
	{
		ui->cbBitrate->setCheckState(Qt::Unchecked);
	}
}

bool SetTrackColumnDialog::GetCBRating()
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

void SetTrackColumnDialog::SetCBRating(bool bCheck)
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

