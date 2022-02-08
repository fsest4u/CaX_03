#include "setcategorycolumndialog.h"
#include "ui_setcategorycolumndialog.h"

SetCategoryColumnDialog::SetCategoryColumnDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SetCategoryColumnDialog)
{
	ui->setupUi(this);
}

SetCategoryColumnDialog::~SetCategoryColumnDialog()
{
	delete ui;
}

bool SetCategoryColumnDialog::GetCBFavorite()
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

void SetCategoryColumnDialog::SetCBFavorite(bool bCheck)
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

bool SetCategoryColumnDialog::GetCBRating()
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

void SetCategoryColumnDialog::SetCBRating(bool bCheck)
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

bool SetCategoryColumnDialog::GetCBArtist()
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

void SetCategoryColumnDialog::SetCBArtist(bool bCheck)
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

bool SetCategoryColumnDialog::GetCBAlbum()
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

void SetCategoryColumnDialog::SetCBAlbum(bool bCheck)
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

bool SetCategoryColumnDialog::GetCBGenre()
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

void SetCategoryColumnDialog::SetCBGenre(bool bCheck)
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

bool SetCategoryColumnDialog::GetCBAlbumArtist()
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

void SetCategoryColumnDialog::SetCBAlbumArtist(bool bCheck)
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

bool SetCategoryColumnDialog::GetCBTrackCount()
{
	if (ui->cbTrackCount->checkState() == Qt::Checked)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SetCategoryColumnDialog::SetCBTrackCount(bool bCheck)
{
	if (bCheck)
	{
		ui->cbTrackCount->setCheckState(Qt::Checked);
	}
	else
	{
		ui->cbTrackCount->setCheckState(Qt::Unchecked);
	}
}
