#include "searchcoverartdialog.h"
#include "ui_searchcoverartdialog.h"

#include "util/caxconstants.h"
#include "util/log.h"

SearchCoverArtDialog::SearchCoverArtDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SearchCoverArtDialog)
{
	ui->setupUi(this);

	connect(ui->radioButtonGoogle, SIGNAL(clicked()), this, SLOT(SlotClickSearchEngine()));
	connect(ui->radioButtonBing, SIGNAL(clicked()), this, SLOT(SlotClickSearchEngine()));
	connect(ui->radioButtonAmazon, SIGNAL(clicked()), this, SLOT(SlotClickSearchEngine()));
	connect(ui->radioButtonGraceNote, SIGNAL(clicked()), this, SLOT(SlotClickSearchEngine()));

	SetSite(SEARCH_GOOGLE);

}

SearchCoverArtDialog::~SearchCoverArtDialog()
{
	delete ui;
}

QString SearchCoverArtDialog::GetSite() const
{
	return m_Site;
}

void SearchCoverArtDialog::SetSite(QString Site)
{
	m_Site = Site;
}

QString SearchCoverArtDialog::GetKeyword()
{
	return ui->lineEditKeyword->text();
}

QString SearchCoverArtDialog::GetArtist()
{
	return ui->lineEditArtist->text();
}

void SearchCoverArtDialog::SlotClickSearchEngine()
{
	if (ui->radioButtonGoogle->isChecked())
	{
		SetSite(SEARCH_GOOGLE);
	}
	else if (ui->radioButtonBing->isChecked())
	{
		SetSite(SEARCH_BING);
	}
	else if (ui->radioButtonAmazon->isChecked())
	{
		SetSite(SEARCH_AMAZON);
	}
	else if (ui->radioButtonGraceNote->isChecked())
	{
		SetSite(SEARCH_GRACENOTE);
	}
}
