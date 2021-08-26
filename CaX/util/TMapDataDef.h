#ifndef __TMAPDATA_DEF_H__
#define __TMAPDATA_DEF_H__

#include <QList>
#include <QString>
#include <QMap>

typedef struct
{
	QMap<int, int64_t>	mapInt;		// iKey_???
	QMap<int, QString>	mapStr;		// strKey_???
	QMap<int, char>		mapChar;	// key : blKey_??? or cKey_???,		value : cVal_??? or any char
} TMapData;

typedef QList<TMapData>		TMapDataList;

#endif	// __TMAPDATA_DEF_H__
