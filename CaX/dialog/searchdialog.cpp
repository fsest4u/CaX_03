#include "searchdialog.h"
#include "ui_searchdialog.h"

#include "dialog/commondialog.h"

#include "util/caxconstants.h"
#include "util/caxtranslate.h"

SearchDialog::SearchDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SearchDialog)
{
	ui->setupUi(this);
}

SearchDialog::~SearchDialog()
{
	delete ui;
}

QString SearchDialog::GetKeyword()
{
	return ui->lineEdit->text();
}

void SearchDialog::SetKeyword(QString keyword)
{
	ui->lineEdit->setText(keyword);
}

void SearchDialog::accept()
{
	if (GetKeyword().length() < SEARCH_WORD_LIMIT_COUNT)
	{
		CommonDialog dialog(this, STR_WARNING, STR_ENTER_SEARCH_WORD);
		dialog.exec();
		return;
	}

	done(QDialog::Accepted);
}

void SearchDialog::reject()
{
	done(QDialog::Rejected);
}
