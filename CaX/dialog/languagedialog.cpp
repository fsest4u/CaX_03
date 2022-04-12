#include <QDir>

#include "languagedialog.h"
#include "ui_languagedialog.h"

#include "util/log.h"

#define TRANSLATION_FILE_PREFIX "CaX_"
#define TRANSLATION_FILE_SUFFIX ".qm"

LanguageDialog::LanguageDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LanguageDialog)
{
	ui->setupUi(this);

	Initialize();
}

LanguageDialog::~LanguageDialog()
{
	delete ui;
}

void LanguageDialog::Initialize()
{
	QString transPath = QCoreApplication::applicationDirPath() + "/translations/";
	QDir transDir(transPath);

	if (transDir.exists())
	{
		QStringList filters;
		filters << QString(TRANSLATION_FILE_PREFIX) + "*" + QString(TRANSLATION_FILE_SUFFIX);
		transDir.setNameFilters(filters);
		QStringList translation_files = transDir.entryList();
		foreach(QString file, translation_files)
		{
			QFileInfo fileInfo(file);
			QString basename = fileInfo.baseName();
			QString data = basename.right(basename.length() - QString(TRANSLATION_FILE_PREFIX).length());
//			ui_languages.append(language);

			QString language = GetLanguageName(data);
//			LogDebug("language [%s] data [%s]", language.toUtf8().data(), data.toUtf8().data());
			ui->comboBox->addItem(language, data);
		}
	}

}

QString LanguageDialog::GetLanguageName(QString data)
{
	QString name;
	if (data.contains("de_DE"))
	{
		name = "German";
	}
	else if (data.contains("en_US"))
	{
		name = "English";
	}
	else if (data.contains("fr_FR"))
	{
		name = "French";
	}
	else if (data.contains("it_IT"))
	{
		name = "Italian";
	}
	else if (data.contains("ja_JP"))
	{
		name = "Japanese";
	}
	else if (data.contains("ko_KR"))
	{
		name = "Korean";
	}
	else if (data.contains("nl_NL"))
	{
		name = "Dutch";
	}
	else if (data.contains("pl_PL"))
	{
		name = "Polish";
	}
	else if (data.contains("ru_RU"))
	{
		name = "Russian";
	}
	else if (data.contains("zh_CN"))
	{
		name = "Simplified Chinese";
	}
	else if (data.contains("zh_HK"))
	{
		name = "Hong Kong";
	}
	else if (data.contains("zh_TW"))
	{
		name = "Traditional Chinese";
	}
//	else
//	{
//		name = "English";
//	}

	return name;
}

QString LanguageDialog::GetLanguageData()
{
	return ui->comboBox->currentData().toString();
}

void LanguageDialog::SetLanguageData(QString data)
{
	int count = ui->comboBox->count();
	for (int i = 0; i < count; i++)
	{
		QString curData = ui->comboBox->itemData(i).toString();
		if (data.contains(curData))
		{
			ui->comboBox->setCurrentIndex(i);
			break;
		}
	}
}


