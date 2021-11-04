#include "trackinfodialog.h"
#include "ui_trackinfodialog.h"

TrackInfoDialog::TrackInfoDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TrackInfoDialog)
{
	ui->setupUi(this);

	ui->labelTitle->hide();
	ui->labelAlbum->hide();
	ui->labelArtist->hide();
	ui->labelGenre->hide();
	ui->labelAlbumArtist->hide();
	ui->labelComposer->hide();
	ui->labelMood->hide();
	ui->labelTempo->hide();
	ui->labelTrack->hide();
	ui->labelPath->hide();
	ui->labelBitrate->hide();
	ui->labelDuration->hide();
	ui->labelSampleRate->hide();
	ui->labelChannel->hide();
	ui->labelFormat->hide();
	ui->labelYear->hide();
	ui->labelCDYear->hide();
	ui->labelCDNumber->hide();
	ui->labelCDTotal->hide();

	ui->labelValueTitle->hide();
	ui->labelValueAlbum->hide();
	ui->labelValueArtist->hide();
	ui->labelValueGenre->hide();
	ui->labelValueAlbumArtist->hide();
	ui->labelValueComposer->hide();
	ui->labelValueMood->hide();
	ui->labelValueTempo->hide();
	ui->labelValueTrack->hide();
	ui->labelValuePath->hide();
	ui->labelValueBitrate->hide();
	ui->labelValueDuration->hide();
	ui->labelValueSampleRate->hide();
	ui->labelValueChannel->hide();
	ui->labelValueFormat->hide();
	ui->labelValueYear->hide();
	ui->labelValueCDYear->hide();
	ui->labelValueCDNumber->hide();
	ui->labelValueCDTotal->hide();
}

TrackInfoDialog::~TrackInfoDialog()
{
	delete ui;
}

void TrackInfoDialog::SetTitle(const QString &value)
{
	ui->labelTitle->show();
	ui->labelValueTitle->show();
	ui->labelValueTitle->setText(value);
}

void TrackInfoDialog::SetAlbum(const QString &value)
{
	ui->labelAlbum->show();
	ui->labelValueAlbum->show();
	ui->labelValueAlbum->setText(value);
}

void TrackInfoDialog::SetArtist(const QString &value)
{
	ui->labelArtist->show();
	ui->labelValueArtist->show();
	ui->labelValueArtist->setText(value);
}

void TrackInfoDialog::SetGenre(const QString &value)
{
	ui->labelGenre->show();
	ui->labelValueGenre->show();
	ui->labelValueGenre->setText(value);
}

void TrackInfoDialog::SetAlbumArtist(const QString &value)
{
	ui->labelAlbumArtist->show();
	ui->labelValueAlbumArtist->show();
	ui->labelValueAlbumArtist->setText(value);
}

void TrackInfoDialog::SetComposer(const QString &value)
{
	ui->labelComposer->show();
	ui->labelValueComposer->show();
	ui->labelValueComposer->setText(value);
}

void TrackInfoDialog::SetMood(const QString &value)
{
	ui->labelMood->show();
	ui->labelValueMood->show();
	ui->labelValueMood->setText(value);
}

void TrackInfoDialog::SetTempo(const QString &value)
{
	ui->labelTempo->show();
	ui->labelValueTempo->show();
	ui->labelValueTempo->setText(value);
}

void TrackInfoDialog::SetTrack(const QString &value)
{
	ui->labelTrack->show();
	ui->labelValueTrack->show();
	ui->labelValueTrack->setText(value);
}

void TrackInfoDialog::SetPath(const QString &value)
{
	ui->labelPath->show();
	ui->labelValuePath->show();
	ui->labelValuePath->setText(value);
}

void TrackInfoDialog::SetBitrate(const QString &value)
{
	ui->labelBitrate->show();
	ui->labelValueBitrate->show();
	ui->labelValueBitrate->setText(value);
}

void TrackInfoDialog::SetDuration(const QString &value)
{
	ui->labelDuration->show();
	ui->labelValueDuration->show();
	ui->labelValueDuration->setText(value);
}

void TrackInfoDialog::SetSampleRate(const QString &value)
{
	ui->labelSampleRate->show();
	ui->labelValueSampleRate->show();
	ui->labelValueSampleRate->setText(value);
}

void TrackInfoDialog::SetChannel(const QString &value)
{
	ui->labelChannel->show();
	ui->labelValueChannel->show();
	ui->labelValueChannel->setText(value);
}

void TrackInfoDialog::SetFormat(const QString &value)
{
	ui->labelFormat->show();
	ui->labelValueFormat->show();
	ui->labelValueFormat->setText(value);
}

void TrackInfoDialog::SetYear(const QString &value)
{
	ui->labelAlbum->show();
	ui->labelValueAlbum->show();
	ui->labelValueYear->setText(value);
}

void TrackInfoDialog::SetCDYear(const QString &value)
{
	ui->labelCDYear->show();
	ui->labelValueCDYear->show();
	ui->labelValueCDYear->setText(value);
}

void TrackInfoDialog::SetCDNumber(const QString &value)
{
	ui->labelCDNumber->show();
	ui->labelValueCDNumber->show();
	ui->labelValueCDNumber->setText(value);
}

void TrackInfoDialog::SetCDTotal(const QString &value)
{
	ui->labelCDTotal->show();
	ui->labelValueCDTotal->show();
	ui->labelValueCDTotal->setText(value);
}


