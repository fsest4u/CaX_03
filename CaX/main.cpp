#include <QApplication>
#include <QSharedMemory>
#include <QTranslator>
#include <QDir>
//#include <QStyleFactory>

#include "widget/mainwindow.h"

#include "util/caxtranslate.h"
#include "util/settingio.h"

const QString SETTINGS_GROUP = "MainWindow";

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QCoreApplication::setOrganizationName(STR_ORGANIZATION);

	QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);

	QApplication a(argc, argv);

	SettingIO settings;
	settings.beginGroup(SETTINGS_GROUP);
	QString language = "CaX_" + settings.value("cax_language").toString();
	settings.endGroup();

	QTranslator translator;
	QString transPath = QCoreApplication::applicationDirPath() + "/translations/";
	if (QDir(transPath).exists())
	{
		if (translator.load(language, transPath))
		{
			a.installTranslator(&translator);
		}
	}

	MainWindow w;
	w.show();
	return a.exec();
}
