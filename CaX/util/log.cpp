
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
		qDebug()	<< "DEB [" << funcname << "]\t[" <<  linenum << "]\t" << strMsg;
		break;
	case INFO:
		qInfo()		<< "INF [" << funcname << "]\t[" <<  linenum << "]\t" << strMsg;
		break;
	case WARNING:
		qWarning()	<< "WAR [" << funcname << "]\t[" <<  linenum << "]\t" << strMsg;
		break;
	case CRITICAL:
		qCritical()	<< "CRI [" << funcname << "]\t[" <<  linenum << "]\t" << strMsg;
		break;
	default:
		qDebug()	<< strMsg;
		break;
	}

	va_end(args);
}
