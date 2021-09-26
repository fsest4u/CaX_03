#include "trackinfodialog.h"
#include "ui_trackinfodialog.h"

TrackInfoDialog::TrackInfoDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TrackInfoDialog)
{
	ui->setupUi(this);
}

TrackInfoDialog::~TrackInfoDialog()
{
	delete ui;
}

void TrackInfoDialog::SetTitle(const QString &value)
{
	ui->labelTitle->setText(value);
}

void TrackInfoDialog::SetAlbum(const QString &value)
{
	ui->labelAlbum->setText(value);
}

void TrackInfoDialog::SetArtist(const QString &value)
{
	ui->labelArtist->setText(value);
}

void TrackInfoDialog::SetGenre(const QString &value)
{
	ui->labelGenre->setText(value);
}

void TrackInfoDialog::SetAlbumArtist(const QString &value)
{
	ui->labelAlbumArtist->setText(value);
}

void TrackInfoDialog::SetComposer(const QString &value)
{
	ui->labelComposer->setText(value);
}

void TrackInfoDialog::SetMood(const QString &value)
{
	ui->labelMood->setText(value);
}

void TrackInfoDialog::SetTempo(const QString &value)
{
	ui->labelTempo->setText(value);
}

void TrackInfoDialog::SetTrack(const QString &value)
{
	ui->labelTrack->setText(value);
}

void TrackInfoDialog::SetYear(const QString &value)
{
	ui->labelYear->setText(value);
}

void TrackInfoDialog::SetCDYear(const QString &value)
{
	ui->labelCDYear->setText(value);
}

void TrackInfoDialog::SetCDNumber(const QString &value)
{
	ui->labelCDNumber->setText(value);
}


