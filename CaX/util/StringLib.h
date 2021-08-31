#ifndef __STRING_LIB_H__
#define __STRING_LIB_H__

#include <QStringList>
#include <QByteArray>

#include "TMapDataDef.h"

#define ToAscii(str)		((str).toUtf8())
#define ToAsciiP(lpStr)		((lpStr)->toUtf8())
#define _ToUtf8(str)		(str)
#define _ToUtf8P(lpStr)		(lpStr)

#define _safeDelete(ptr)						\
	if (ptr)								\
	{									\
	delete ptr;							\
	ptr = NULL;							\
	}

int			GetHeadSpaceCnt(QString &str);

QByteArray	_Trimmed(	QByteArray &qbIn);
void		Trimmed(	QByteArray &qbIn);
QByteArray	GetTrimmed(	QByteArray qbIn);

QString	_TailTrimmed(	QString &strIn);
QString	GetTailTrimmed(	QString strIn);

QString	_Trimmed(	QString &strIn);
void	Trimmed(	QString &strIn);
QString	GetTrimmed(	QString strIn);
void	TrimmSpace(	QString &str);

void	ReverseSong(		TMapDataList &lsMdSong);
bool	MdNameLessThan(		TMapData &MdName1, TMapData &MdName2);
bool	MdNameLargerThan(	TMapData &MdName1, TMapData &MdName2);
bool	MdTitleLessThan(	TMapData &MdItem1, TMapData &MdItem2);
bool	MdTitleLargerThan(	TMapData &MdItem1, TMapData &MdItem2);
bool	MdSeqLessThan(		TMapData &MdItem1, TMapData &MdItem2);

bool	CheckUtf8(char *lpStr);
//bool	CheckUtf8(QString str);
bool	CheckUtf8(QByteArray qb);

void	SetCheckUtf8String();

QString	GetSortString(			int iSort);
QString	GetElapseTimeString(	int nSec);

QString	GetTimeStringMMSS(		int nSec);
QString	GetTimeStringDDHHMMSS(	int nSec);
QString GetHHMMSS(				int nSec);
QString GetHHMM(				int nMin, bool blAP = false);
bool	GetMinFromHHMMString(QString strTime, int &iMin);
QString ConvertMSecToHHMMSSStr(int mSec);

void	StrTrimTail(char *szStr);

bool	GetMidString(	QString strIn, QString strBegin, QString strEnd, QString &strOut, bool blBeginLast = false, bool blEndLast = false);
QString	GetMidString(	QString strIn, QString strBegin, QString strEnd, bool blBeginLast = false, bool blEndLast = false);
bool	GetRightString(	QString strIn, QString strDel, QString &strOut, bool blLast = true);
QString	GetRightString(	QString strIn, QString strDel, bool blLast = true);
bool	GetLeftString(	QString strIn, QString strDel, QString &strOut, bool blLast = true);
QString	GetLeftString(	QString strIn, QString strDel, bool blLast = true);

int		Compare(	QString str1, QString str2);
int		iCompare(	QString str1, QString str2);
//bool	Contains(	QStringList &lsStr, QString str);
bool	iContains(	QStringList &lsStr, QString str2);

int		IndexOfStr(		QString &strStr, QString strFind, int index = 0, Qt::CaseSensitivity cs = Qt::CaseSensitive);
int		IndexOf(		QString &strStr, QString strFind, int index = 0);
int		iIndexOf(		QString &strStr, QString strFind, int index = 0);
int		LastIndexOfStr(	QString &strStr, QString strFind, int index = -1, Qt::CaseSensitivity cs = Qt::CaseSensitive);
int		LastIndexOf(	QString &strStr, QString strFind, int index = -1);
int		iLastIndexOf(	QString &strStr, QString strFind, int index = -1);

//QString	GetUtf8String(	int iCode);
//QString	GetUtf16String(	int iCode);

void	VerifySqlString(	QString &str);
QString	VerifySqlStringR(	QString str);

QString	StrToFileStrR(		QString str,	bool blSlashOnly = false);		// VerifyFileStringR ->
void	StrToFileStr(		QString &str,	bool blSlashOnly = false);		// VerifyFileString ->
QString	FileStrToStrR(		QString str);
void	FileStrToStr(		QString &str);

int		RemoveStringList(	QStringList &strList, QString str,				QRegExp::PatternSyntax syntax = QRegExp::FixedString, Qt::CaseSensitivity cs = Qt::CaseInsensitive	);
int		RemoveStringList(	QStringList &strList, QStringList &lsFilter,	QRegExp::PatternSyntax syntax = QRegExp::FixedString, Qt::CaseSensitivity cs = Qt::CaseInsensitive	);

int	iIndexOfStringList(	QStringList &strList, QString str,		QStringList &strListGet, QRegExp::PatternSyntax syntax = QRegExp::Wildcard);

// KB: 2^10, MB: 2^20 GB: 2^30, TB: 2^40
// at 2015-03-06 by chpark
bool	GetFileSizeString(	int64_t llSize, QString &strGet,	bool bl1024 = true, bool blMinimal = false);
QString	GetFileSizeString(	int64_t llSize,				bool bl1024 = true, bool blMinimal = false);
QString GetNumString(int iValue, QString strSuffix = QString());
bool	GetIntPair(QString strPair, int &i, int &n, int iDefValue = 0);		// "no / count or "no parse
bool	GetIntBound(QString strInt, int &iInt, int nMaxBoundValue = 2100);	// for Year

QString	ConvertCoverArtURLToName(QString fullpath);
//QString	GetDateTimeString(	time_t t);

bool	IsNumber(	QString strNum);
bool	ToInt(		QString strNum, int32_t &iInt32);
int		ToInt(		QString strNum);
int		ToIntHex(	QString strNumHex);
bool	ToLongLong(	QString strNum, int64_t &llInt64);
int64_t	ToLongLong(	QString strNum);
bool	ToFloat(	QString strNum, float &fFloat);

bool	IsHttpUrlStrR(QString &strUrl);
bool	IsHttpUrlStr(QString strUrl);

QString	GetImageServerUrl();
QString	GetImageServerUrl(			QString strRoot,	QString strPath,	bool blPercentEncoding = true);
QString	GetImageServerUrl(			QString strRoot,	int iId);
QString	GetLoopbackImageServerUrl(	QString strRoot,	QString strPath);
QString	GetLoopbackImageServerUrl(	QString strRoot,	int iId);

bool	ToPercentEncoding(		QString &str,		bool blSpaceTrim = true);
QString	ToPercentEncodingR(		QString str,		bool blSpaceTrim = true);
bool	FromPercentEncoding(	QString &str,		bool blSpaceTrim = true);
QString	FromPercentEncodingR(	QString str,		bool blSpaceTrim = true);

QString GetUrlQueryString(QString strKey, QString strValue,	bool blStart = false);
QString GetUrlQueryString(QString strKey, int iValue,		bool blStart = false);

#endif
