#include "selectformatdialog.h"
#include "ui_selectformatdialog.h"

#include "util/caxconstants.h"

SelectFormatDialog::SelectFormatDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SelectFormatDialog)
{
	ui->setupUi(this);

	connect(ui->radioButtonCaf, SIGNAL(clicked()), this, SLOT(SlotClickFormat()));
	connect(ui->radioButtonWav, SIGNAL(clicked()), this, SLOT(SlotClickFormat()));
	connect(ui->radioButtonAiff, SIGNAL(clicked()), this, SLOT(SlotClickFormat()));
	connect(ui->radioButtonFlac, SIGNAL(clicked()), this, SLOT(SlotClickFormat()));
	connect(ui->radioButtonM4a, SIGNAL(clicked()), this, SLOT(SlotClickFormat()));
	connect(ui->radioButtonMp3, SIGNAL(clicked()), this, SLOT(SlotClickFormat()));
	connect(ui->radioButtonOgg, SIGNAL(clicked()), this, SLOT(SlotClickFormat()));
	connect(ui->radioButtonWma, SIGNAL(clicked()), this, SLOT(SlotClickFormat()));

	SetFormat(FORMAT_CAF);
}

SelectFormatDialog::~SelectFormatDialog()
{
	delete ui;
}

QString SelectFormatDialog::GetFormat() const
{
	return m_Format;
}

void SelectFormatDialog::SetFormat(const QString &Format)
{
	m_Format = Format;
}

void SelectFormatDialog::SlotClickFormat()
{
	if (ui->radioButtonCaf->isChecked())
	{
		SetFormat(FORMAT_CAF);
	}
	else if (ui->radioButtonWav->isChecked())
	{
		SetFormat(FORMAT_WAV);
	}
	else if (ui->radioButtonAiff->isChecked())
	{
		SetFormat(FORMAT_AIFF);
	}
	else if (ui->radioButtonFlac->isChecked())
	{
		SetFormat(FORMAT_FLAC);
	}
	else if (ui->radioButtonM4a->isChecked())
	{
		SetFormat(FORMAT_M4A);
	}
	else if (ui->radioButtonMp3->isChecked())
	{
		SetFormat(FORMAT_MP3);
	}
	else if (ui->radioButtonOgg->isChecked())
	{
		SetFormat(FORMAT_OGG);
	}
	else if (ui->radioButtonWma->isChecked())
	{
		SetFormat(FORMAT_WMA);
	}

}
