#include <QFile>
#include <QCoreApplication>

#include "log.h"

void LogCax(int nLevel, const char *filename, const char *funcname, const int linenum, const char *fmt, ...)
{
	Q_UNUSED(filename)

	QString appPath = QCoreApplication::applicationDirPath();
	QFile file(appPath + "/log.txt");
	if(!file.open(QIODevice::Append | QIODevice::Text))
	{
		return;
	}

	QTextStream out(&file);

	va_list args;
	va_start(args, fmt);

	QString strMsg;
	strMsg.vsprintf(fmt, args);

	switch (nLevel)
	{
	case DEBUG:
		qDebug()	<< "DEB [" << funcname << "]\t[" <<  linenum << "]\t" << strMsg << "\n";
		out			<< "DEB [" << funcname << "]\t[" <<  linenum << "]\t" << strMsg << "\n";
		break;
	case INFO:
		qInfo()		<< "INF [" << funcname << "]\t[" <<  linenum << "]\t" << strMsg << "\n";
		out			<< "INF [" << funcname << "]\t[" <<  linenum << "]\t" << strMsg << "\n";
		break;
	case WARNING:
		qWarning()	<< "WAR [" << funcname << "]\t[" <<  linenum << "]\t" << strMsg << "\n";
		out			<< "WAR [" << funcname << "]\t[" <<  linenum << "]\t" << strMsg << "\n";
		break;
	case CRITICAL:
		qCritical()	<< "CRI [" << funcname << "]\t[" <<  linenum << "]\t" << strMsg << "\n";
		out			<< "CRI [" << funcname << "]\t[" <<  linenum << "]\t" << strMsg << "\n";
		break;
	default:
		qDebug()	<< strMsg << "\n";
		out			<< strMsg << "\n";
		break;
	}

	va_end(args);

	file.close();
}
