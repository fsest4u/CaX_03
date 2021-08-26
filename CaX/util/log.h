#ifndef LOG_H
#define LOG_H

#include <QDebug>

#define DEBUG_ENABLE 1
#define LOG_NONE	0

enum logType {
	DEBUG = Qt::UserRole + LOG_NONE,
	INFO,
	WARNING,
	CRITICAL
};

#if DEBUG_ENABLE
#define LogDebug(...)		LogCax(DEBUG,		__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LogInfo(...)		LogCax(INFO,		__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LogWarning(...)		LogCax(WARNING,		__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LogCritical(...)	LogCax(CRITICAL,	__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#else
#define LogDebug(...)
#define LogInfo(...)
#define LogWarning(...)
#define LogCritical(...)
#endif

void LogCax(int nLevel, const char *filename, const char *funcname, const int linenum, const char *fmt, ...);

#endif // LOG_H
