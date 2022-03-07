#include "searchhistorydialog.h"
#include "ui_searchhistorydialog.h"

#include "util/settingio.h"
#include "util/log.h"

const QString SETTINGS_GROUP = "SearchHistoryDialog";

SearchHistoryDialog::SearchHistoryDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SearchHistoryDialog)
{
	ui->setupUi(this);

	m_SearchList.clear();

	ReadSettings();
	Initialize();

}

SearchHistoryDialog::~SearchHistoryDialog()
{
	delete ui;
}

QString SearchHistoryDialog::GetKeyword()
{
	return ui->comboBox->currentText();
}

void SearchHistoryDialog::accept()
{
	QString keyword = ui->comboBox->currentText();
	if (!m_SearchList.contains(keyword))
	{
		m_SearchList.insert(0, keyword);
		WriteSettings();
	}

	done(QDialog::Accepted);
}

void SearchHistoryDialog::reject()
{
	done(QDialog::Rejected);
}

void SearchHistoryDialog::ReadSettings()
{
	SettingIO settings;
	settings.beginGroup(SETTINGS_GROUP);

	m_SearchList = settings.value("search_keyword_list").toStringList();

	settings.endGroup();
}

void SearchHistoryDialog::WriteSettings()
{
	SettingIO settings;
	settings.beginGroup(SETTINGS_GROUP);

	settings.setValue("search_keyword_list", m_SearchList);

	settings.endGroup();
}

void SearchHistoryDialog::Initialize()
{
	int count = m_SearchList.count();
	if (count > 0)
	{
		ui->comboBox->addItems(m_SearchList);
	}
}

