#ifndef UTILNOVATRON_H
#define UTILNOVATRON_H

#include <QObject>

class UtilNovatron
{
public:
	UtilNovatron();

	static QIcon GetMenuIcon(QString value);
	static QString GetCoverArtIcon(const int service, const QString value = "");
	static QString GetCoverArtIcon(const int service, const int value);

	static QString CalcSecondToHMS(int seconds);
	static QString ConvertURLToFilename(QString fullpath);

	static void DebugTypeForBrowser(QString title, int type);
	static void DebugTypeForIService(QString title, int type);
	static void DebugTypeForQobuz(QString title, int type);
	static void DebugTypeForAirable(QString title, int type);
};

#endif // UTILNOVATRON_H
