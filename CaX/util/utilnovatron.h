#ifndef UTILNOVATRON_H
#define UTILNOVATRON_H

#include <QObject>

#include "util/CJsonNode.h"

class UtilNovatron
{
public:
	UtilNovatron();

	static QIcon GetMenuIcon(QString value);
	static QString GetCoverArtIcon(const int service, const QString value = "");
	static QString GetCoverArtIcon(const int service, const int value);

	static QString CalcSecondToHMS(int seconds);

	static void CreateTempDirectory();
	static QString GetTempDirectory();
	static void	RemoveTempDirectory();
	static QString ConvertURLToFilename(QString fullpath);
	static QString GetCategoryName(int category);
	static QString GetCategoryTitleName(int category);
	static int GetCategoryCount(int category, CJsonNode node);

	static void DebugTypeForBrowser(QString title, int type);
	static void DebugTypeForIService(QString title, int type);
	static void DebugTypeForQobuz(QString title, int type);
	static void DebugTypeForQobuzMenu(QString title, int type);
	static void DebugTypeForAirable(QString title, int type);
	static void DebugTypeForSetup(QString title, int type);
	static void DebugTypeForSetupBtn(QString title, int type);
	static void DebugTypeForSetupInput(QString title, int type);

	static void DebugTypeForUDP(QString title, int flag);

};

#endif // UTILNOVATRON_H
