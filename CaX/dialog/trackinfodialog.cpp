#include "trackinfodialog.h"
#include "ui_trackinfodialog.h"

TrackInfoDialog::TrackInfoDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TrackInfoDialog)
{
	ui->setupUi(this);

	ui->frameTitle->hide();
	ui->frameAlbum->hide();
	ui->frameArtist->hide();
	ui->frameGenre->hide();
	ui->frameAlbumArtist->hide();
	ui->frameComposer->hide();
	ui->frameMood->hide();
	ui->frameTempo->hide();
	ui->frameTrack->hide();
	ui->framePath->hide();
	ui->frameBitrate->hide();
	ui->frameDuration->hide();
	ui->frameSampleRate->hide();
	ui->frameChannel->hide();
	ui->frameFormat->hide();
	ui->frameYear->hide();
	ui->frameCDYear->hide();
	ui->frameCDNumber->hide();
	ui->frameCDTotal->hide();

}

TrackInfoDialog::~TrackInfoDialog()
{
	delete ui;
}

void TrackInfoDialog::SetTitle(const QString &value)
{
	ui->frameTitle->show();
	ui->labelValueTitle->setText(value);
}

void TrackInfoDialog::SetAlbum(const QString &value)
{
	ui->frameAlbum->show();
	ui->labelValueAlbum->setText(value);
}

void TrackInfoDialog::SetArtist(const QString &value)
{
	ui->frameArtist->show();
	ui->labelValueArtist->setText(value);
}

void TrackInfoDialog::SetGenre(const QString &value)
{
	ui->frameGenre->show();
	ui->labelValueGenre->setText(value);
}

void TrackInfoDialog::SetAlbumArtist(const QString &value)
{
	ui->frameAlbumArtist->show();
	ui->labelValueAlbumArtist->setText(value);
}

void TrackInfoDialog::SetComposer(const QString &value)
{
	ui->frameComposer->show();
	ui->labelValueComposer->setText(value);
}

void TrackInfoDialog::SetMood(const QString &value)
{
	ui->frameMood->show();
	ui->labelValueMood->setText(value);
}

void TrackInfoDialog::SetTempo(const QString &value)
{
	ui->frameTempo->show();
	ui->labelValueTempo->setText(value);
}

void TrackInfoDialog::SetTrack(const QString &value)
{
	ui->frameTrack->show();
	ui->labelValueTrack->setText(value);
}

void TrackInfoDialog::SetPath(const QString &value)
{
	ui->framePath->show();
	ui->labelValuePath->setText(value);
}

void TrackInfoDialog::SetBitrate(const QString &value)
{
	ui->frameBitrate->show();
	ui->labelValueBitrate->setText(value);
}

void TrackInfoDialog::SetDuration(const QString &value)
{
	ui->frameDuration->show();
	ui->labelValueDuration->setText(value);
}

void TrackInfoDialog::SetSampleRate(const QString &value)
{
	ui->frameSampleRate->show();
	ui->labelValueSampleRate->setText(value);
}

void TrackInfoDialog::SetChannel(const QString &value)
{
	ui->frameChannel->show();
	ui->labelValueChannel->setText(value);
}

void TrackInfoDialog::SetFormat(const QString &value)
{
	ui->frameFormat->show();
	ui->labelValueFormat->setText(value);
}

void TrackInfoDialog::SetYear(const QString &value)
{
	ui->frameAlbum->show();
	ui->labelValueYear->setText(value);
}

void TrackInfoDialog::SetCDYear(const QString &value)
{
	ui->frameCDYear->show();
	ui->labelValueCDYear->setText(value);
}

void TrackInfoDialog::SetCDNumber(const QString &value)
{
	ui->frameCDNumber->show();
	ui->labelValueCDNumber->setText(value);
}

void TrackInfoDialog::SetCDTotal(const QString &value)
{
	ui->frameCDTotal->show();
	ui->labelValueCDTotal->setText(value);
}


