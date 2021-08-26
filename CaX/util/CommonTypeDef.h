#ifndef __COMMON_TYPE_DEF_H__
#define __COMMON_TYPE_DEF_H__

#include <stdint.h>

#include <QStringList>
#include <QList>
#include <QSet>
#include <QByteArray>

typedef QList<QStringList>	QStrStrList;

typedef QList<int32_t>		QIntList;
typedef QList<int16_t>		QInt16List;
typedef QList<int64_t>		QInt64List;
typedef QList<QByteArray>	QQbList;

typedef QSet<int32_t>		QIntSet;
typedef QSet<int16_t>		QInt16Set;
typedef QSet<int64_t>		QInt64Set;

typedef union IpAddr_s
{
	unsigned int iIp;
	unsigned char acIp[4];
} TIpAddr;

#endif
