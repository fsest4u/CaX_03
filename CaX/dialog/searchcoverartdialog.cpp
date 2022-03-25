#include "searchcoverartdialog.h"
#include "ui_searchcoverartdialog.h"

#include "dialog/commondialog.h"

#include "util/caxconstants.h"
#include "util/caxtranslate.h"
#include "util/log.h"

SearchCoverArtDialog::SearchCoverArtDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SearchCoverArtDialog)
{
	ui->setupUi(this);

	connect(ui->radioButtonBrowser, SIGNAL(clicked()), this, SLOT(SlotClickSearchEngine()));
	connect(ui->radioButtonGoogle, SIGNAL(clicked()), this, SLOT(SlotClickSearchEngine()));
	connect(ui->radioButtonBing, SIGNAL(clicked()), this, SLOT(SlotClickSearchEngine()));
	connect(ui->radioButtonAmazon, SIGNAL(clicked()), this, SLOT(SlotClickSearchEngine()));
	connect(ui->radioButtonGraceNote, SIGNAL(clicked()), this, SLOT(SlotClickSearchEngine()));

	SetSite(SEARCH_BROWSER);

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
	if (m_Site.contains(SEARCH_BROWSER))
	{
		ui->lineEditKeyword->setEnabled(false);
		ui->lineEditArtist->setEnabled(false);
	}
	else
	{
		ui->lineEditKeyword->setEnabled(true);
		ui->lineEditArtist->setEnabled(true);
	}
}

QString SearchCoverArtDialog::GetKeyword() const
{
	return ui->lineEditKeyword->text();
}

void SearchCoverArtDialog::SetKeyWord(QString value)
{
	ui->lineEditKeyword->setText(value);
}

QString SearchCoverArtDialog::GetArtist() const
{
	return ui->lineEditArtist->text();
}

void SearchCoverArtDialog::SetArtist(QString value)
{
	ui->lineEditArtist->setText(value);
}

void SearchCoverArtDialog::SlotClickSearchEngine()
{
	if (ui->radioButtonBrowser->isChecked())
	{
		SetSite(SEARCH_BROWSER);
	}
	else if (ui->radioButtonGoogle->isChecked())
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

void SearchCoverArtDialog::accept()
{
	if (!m_Site.contains(SEARCH_BROWSER)
			&& GetKeyword().length() < SEARCH_WORD_LIMIT_COUNT
			&& GetArtist().length() < SEARCH_WORD_LIMIT_COUNT)
	{
		CommonDialog dialog(this, STR_WARNING, STR_ENTER_SEARCH_WORD);
		dialog.exec();
		return;
	}

	done(QDialog::Accepted);
}

void SearchCoverArtDialog::reject()
{
	done(QDialog::Rejected);
}
