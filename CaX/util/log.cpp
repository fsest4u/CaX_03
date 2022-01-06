#include <QFile>
#include <QCoreApplication>
#include <QDateTime>

#include "log.h"

void LogCax(int nLevel, const char *filename, const char *funcname, const int linenum, const char *fmt, ...)
{
	Q_UNUSED(filename)

	va_list args;
	va_start(args, fmt);

	QString strMsg;
	strMsg.vsprintf(fmt, args);

	switch (nLevel)
	{
	case DEBUG:
		qDebug()	<< "DEB [" << funcname << "]\t[" <<  linenum << "]\t" << strMsg << "\n";
		break;
	case INFO:
		qInfo()		<< "INF [" << funcname << "]\t[" <<  linenum << "]\t" << strMsg << "\n";
		break;
	case WARNING:
		qWarning()	<< "WAR [" << funcname << "]\t[" <<  linenum << "]\t" << strMsg << "\n";
		break;
	case CRITICAL:
		qCritical()	<< "CRI [" << funcname << "]\t[" <<  linenum << "]\t" << strMsg << "\n";
		break;
	default:
		qDebug()	<< strMsg << "\n";
		break;
	}

#if 1	// for log file
	QString appPath = QCoreApplication::applicationDirPath();
	QFile file(appPath + "/log.txt");
	if(!file.open(QIODevice::Append | QIODevice::Text))
	{
		return;
	}

	QString curtime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

	QTextStream out(&file);

	switch (nLevel)
	{
	case DEBUG:
		out			<< "DEB [" << curtime << funcname << "]\t[" <<  linenum << "]\t" << strMsg << "\n";
		break;
	case INFO:
		out			<< "INF [" << curtime << funcname << "]\t[" <<  linenum << "]\t" << strMsg << "\n";
		break;
	case WARNING:
		out			<< "WAR [" << curtime << funcname << "]\t[" <<  linenum << "]\t" << strMsg << "\n";
		break;
	case CRITICAL:
		out			<< "CRI [" << curtime << funcname << "]\t[" <<  linenum << "]\t" << strMsg << "\n";
		break;
	default:
		out			<< strMsg << "\n";
		break;
	}

	file.close();
#endif

	va_end(args);

}
