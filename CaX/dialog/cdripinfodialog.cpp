#include "cdripinfodialog.h"
#include "ui_cdripinfodialog.h"

CDRipInfoDialog::CDRipInfoDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CDRipInfoDialog)
{
	ui->setupUi(this);
}

CDRipInfoDialog::~CDRipInfoDialog()
{
	delete ui;
}

void CDRipInfoDialog::SetTitle(const QString &value)
{
	ui->labelTitle->setText(value);
}

void CDRipInfoDialog::SetArtist(const QString &value)
{
	ui->labelArtist->setText(value);
}

void CDRipInfoDialog::SetGenre(const QString &value)
{
	ui->labelGenre->setText(value);
}

void CDRipInfoDialog::SetComposer(const QString &value)
{
	ui->labelComposer->setText(value);
}

void CDRipInfoDialog::SetMood(const QString &value)
{
	ui->labelMood->setText(value);
}

void CDRipInfoDialog::SetTempo(const QString &value)
{
	ui->labelTempo->setText(value);
}

void CDRipInfoDialog::SetYear(const QString &value)
{
	ui->labelYear->setText(value);
}

