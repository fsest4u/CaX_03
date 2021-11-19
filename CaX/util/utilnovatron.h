#ifndef UTILNOVATRON_H
#define UTILNOVATRON_H

#include <QObject>

class UtilNovatron
{
public:
	UtilNovatron();

	static QIcon GetMenuIcon(QString value);
	static QString GetCoverArtIcon(const int service, const QString value = "");
	static void DebugTypeForBrowser(int type);
};

#endif // UTILNOVATRON_H
