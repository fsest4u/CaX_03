#include <limits.h>
#include <math.h>

#include <QUrl>

//#include <QDateTime>
#include <QTIme>

#include "log.h"
//#include "Global.h"		// temp_code


#include "TMapData.h"

#include "StringLib.h"

int GetHeadSpaceCnt(QString &str)
{
	int i;

	for(i = 0; i < str.size(); i++)
	{
		if( str.at(i) != ' ' )		break;
	}

	return i;
}

QByteArray _Trimmed(QByteArray &qbIn)
{
	int i;
	char ch;

	for(i = 0; i < qbIn.size(); i++)
	{
		ch = qbIn.at(i);
		if( ch != '\t' && ch != '\n' && ch != '\r' && ch != ' ' )	break;
		//if( !ch.isSpace() )						break;
	}
	if( i > 0 )		qbIn = qbIn.mid(i);

	for(i = qbIn.size() - 1; i >= 0; i--)
	{
		ch = qbIn.at(i);
		if( ch != '\t' && ch != '\n' && ch != '\r' && ch != ' ' )	break;
		//if( !ch.isSpace() )						break;
	}

	if(i != qbIn.size() - 1 )
	{
		qbIn = qbIn.left(i+1);
	}
	return qbIn;
}

void Trimmed(QByteArray &qbIn)
{
	_Trimmed(qbIn);
}

QByteArray GetTrimmed(QByteArray qbIn)
{
	return _Trimmed(qbIn);
}

QString _TailTrimmed(QString &strIn)
{
	int i;
	QChar qChar;

	for(i = strIn.size() - 1; i >= 0; i--)
	{
		qChar = strIn.at(i);
		if( qChar != '\t' && qChar != '\n' && qChar != '\r' && qChar != ' ' )	break;
		//if( !qChar.isSpace() )						break;
	}

	if(i != strIn.size() - 1 )		strIn = strIn.left(i+1);
	return strIn;
}

QString GetTailTrimmed(QString strIn)
{
	return _TailTrimmed(strIn);
}

QString _Trimmed(QString &strIn)
{
	int i;
	QChar qChar;

	for(i = 0; i < strIn.size(); i++)
	{
		qChar = strIn.at(i);
		if( qChar != '\t' && qChar != '\n' && qChar != '\r' && qChar != ' ' )	break;
		//if( !qChar.isSpace() )						break;
	}
	if( i > 0 )		strIn = strIn.mid(i);

	return _TailTrimmed(strIn);
}

void Trimmed(QString &strIn)
{
	_Trimmed(strIn);
}

QString GetTrimmed(QString strIn)
{
	return _Trimmed(strIn);
}

void TrimmSpace(QString &str)
{
	while( IndexOf(str, "  ") >= 0 )	str.replace("  ", " ");
}

void ReverseSong(TMapDataList &lsMdSong)
{
	int nCnt = lsMdSong.count();
	if( nCnt < 2 )		return;
	nCnt--;
	TMapData MdSong;
	for(int i = 0; i < nCnt; i++)
	{
		MdSong = lsMdSong.takeLast();
		lsMdSong.insert(i, MdSong);
	}
}

bool MdNameLessThan(TMapData &MdName1, TMapData &MdName2)
{
	return MapStrVal(MdName1, strKey_Name).compare(MapStrVal(MdName2, strKey_Name), Qt::CaseInsensitive) < 0;
}

bool MdNameLargerThan(TMapData &MdName1, TMapData &MdName2)
{
	return MapStrVal(MdName1, strKey_Name).compare(MapStrVal(MdName2, strKey_Name), Qt::CaseInsensitive) > 0;
}

bool MdTitleLessThan(TMapData &MdItem1, TMapData &MdItem2)
{
	return MapStrVal(MdItem1, strKey_Title).compare(MapStrVal(MdItem2, strKey_Title), Qt::CaseInsensitive) < 0;
}

bool MdTitleLargerThan(TMapData &MdItem1, TMapData &MdItem2)
{
	return MapStrVal(MdItem1, strKey_Title).compare(MapStrVal(MdItem2, strKey_Title), Qt::CaseInsensitive) > 0;
}

bool MdSeqLessThan(TMapData &MdItem1, TMapData &MdItem2)
{
	return MapIntVal(MdItem1, iKey_Seq) < MapIntVal(MdItem2, iKey_Seq);
}

int _Utf8Len(unsigned char *utf8)
{
	//Assert(0 != utf8);
	if ((utf8[0] & 0x80) == 0)
	{
		//Logging("[%02X]\n", utf8[0]);
		return 1;
	}
	else if( (utf8[0] & 0xE0) == 0xC0 && (utf8[1] & 0xC0) == 0x80 )
	{
		if ((utf8[0] & 0xFE) == 0xC0)						return 0;		// overlong sequence check
		//Logging("[%02X, %02X]\n", utf8[0], utf8[1]);
		return 2;
	}
	else if ((utf8[0] & 0xF0) == 0xE0 && (utf8[1] & 0xC0) == 0x80 && (utf8[2] & 0xC0) == 0x80)
	{
		if( utf8[0] == 0xE0 && (utf8[1] & 0xE0) == 0x80 )	return 0;		// overlong sequence check

		// illegal surrogates check (U+D800...U+DFFF and U+FFFE...U+FFFF)
		if (utf8[0] == 0xED && (utf8[1] & 0xE0) == 0xA0)						return 0;		// D800-DFFF
		if (utf8[0] == 0xEF && utf8[1] == 0xBF && (utf8[2] & 0xFE) == 0xBE)		return 0;		// FFFE-FFFF
		//Logging("[%02X, %02X, %02X]\n", utf8[0], utf8[1], utf8[2]);
		return 3;
	}
	else if ((utf8[0] & 0xF8) == 0xF0 && (utf8[1] & 0xC0) == 0x80 && (utf8[2] & 0xC0) == 0x80 && (utf8[3] & 0xC0) == 0x80)
	{
		if (utf8[0] == 0xF0 && (utf8[1] & 0xF0) == 0x80)	return 0;		// overlong sequence check
		//Logging("[%02X, %02X, %02X %02X]\n", utf8[0], utf8[1], utf8[2], utf8[3]);
		return 4;
	}
	else if ((utf8[0] & 0xFC) == 0xF8 && (utf8[1] & 0xC0) == 0x80 && (utf8[2] & 0xC0) == 0x80 && (utf8[3] & 0xC0) == 0x80 && (utf8[4] & 0xC0) == 0x80)
	{
		if (utf8[0] == 0xF8 && (utf8[1] & 0xF8) == 0x80)	return 0;		// overlong sequence check
		return 5;
	}
	else if( (utf8[0] & 0xFE) == 0xFC && (utf8[1] & 0xC0) == 0x80 && (utf8[2] & 0xC0) == 0x80 && (utf8[3] & 0xC0) == 0x80 && (utf8[4] & 0xC0) == 0x80 && (utf8[5] & 0xC0) == 0x80 )
	{
		if (utf8[0] == 0xFC && (utf8[1] & 0xFC) == 0x80)	return 0;		// overlong sequence check
		return 6;
	}
	else
	{
		//Logging("return 0;\n");
		return 0;
	}
}

bool CheckUtf8(char *lpStr)
{
	unsigned char *sStr, *eStr;
	int n;

	sStr = (unsigned char *)lpStr;
	eStr = sStr + strlen((char *)sStr);

	while(sStr < eStr)
	{
		n = _Utf8Len(sStr);
		if( n == 0 )	return false;
#if 0
		if( n == 2 )
		{
			if( sStr[0] == 0xC2 || sStr[0] == 0xC3 )
			{
				sStr += 2;
				if( sStr >= eStr )		return true;
				n = _Utf8Len(sStr);
				if( n == 0 )			return false;
				if( n == 2 )
				{
					if( sStr[0] == 0xC2 || sStr[0] == 0xC3 )	return false;
				}
			}
		}
#endif
		sStr += n;
	}
	if( sStr == eStr )		return true;

	return false;
}

//bool CheckUtf8(QString str)
bool CheckUtf8(QByteArray qb)
{
	return CheckUtf8(qb.data());
}

QString GetSortString(int iSort)
{
	if( iSort != 1 && iSort != 2 )		return QString();

	QString strSort = " (";
	strSort.append( (iSort == 1)? 'A' : 'Z' );
	strSort.append(QChar(0x2192));		// right arrow
	strSort.append( (iSort == 1)? 'Z' : 'A' );
	strSort.append(")");

	LogDebug("strSort = [%s]\n", strSort.toUtf8().data());
	return strSort;
}

QString GetElapseTimeString(int nSec)
{
	if( nSec < 3600 )	return GetTimeStringMMSS(nSec);

	int nBack = nSec;
	nSec /= 3600;

	QString strTime;
	if( nSec < 24 )
	{
		strTime.sprintf("%d.%02dh", nSec, (nBack % 3600) / 60);
		return strTime;
	}

	nBack = nSec;
	nSec /= 24;
	if( nSec < 30 )
	{
		strTime.sprintf("%d.%02dd", nSec, nBack % 24);
		return strTime;
	}

	nBack = nSec;
	nSec /= 30;
	if( nSec < 12 )
	{
		strTime.sprintf("%d.%02dm", nSec, nBack % 30);
		return strTime;
	}

	nBack = nSec;
	nSec /= 12;
	if( nSec < 100 )
	{
		strTime.sprintf("%d.%02dy", nSec, nBack % 12);
		return strTime;
	}

	return QString("999 Y");
}

QString GetTimeStringMMSS(int nSec)
{
	if( nSec < 0 )		nSec = 0;

	QString strTime;
	strTime.sprintf("%02d:%02d", nSec / 60, nSec % 60);
	return strTime;
}

QString GetTimeStringDDHHMMSS(int nSec)
{
	if( nSec < 0 )		nSec = 0;

	QString strTime;

	if( nSec < 3600 )			strTime.sprintf("%02d:%02d", nSec / 60, nSec % 60);
	else if( nSec < 86400 )		strTime.sprintf("%02d:%02d:%02d", nSec/3600, (nSec/60)%60, nSec % 60);
	else						strTime.sprintf("%dd %02d:%02d:%02d", nSec/86400, (nSec%86400)/3600, (nSec/60)%60, nSec % 60);

	return strTime;
}

QString GetHHMMSS(int nSec)
{
	if( nSec < 0 )		nSec = 0;

	QString strTime;
	strTime.sprintf("%02d:%02d:%02d", nSec/3600, (nSec/60)%60, nSec%60);
	return strTime;
}

QString GetHHMM(int nMin, bool blAP)
{
	if( nMin < 0 || nMin >= 1440)	nMin = 0;

	if( blAP )
	{
		QTime qTime;
		qTime.setHMS( (nMin / 60) % 24, nMin % 60, 0 );
		return qTime.toString("AP hh:mm");
	}

	QString strTime;
	strTime.sprintf("%02d:%02d", nMin / 60, nMin % 60);
	return strTime;
}

bool GetMinFromHHMMString(QString strTime, int &iMin)
{
	iMin = -1;
	QStringList lsStr = strTime.split(":");
	if( lsStr.size() != 2 )			return false;

	int iInt = ToInt(lsStr[0]);
	if( iInt < 0 || iInt >= 24 )	return false;
	iMin = iInt * 60;

	iInt = ToInt(lsStr[1]);
	if( iInt < 0 || iInt >= 60 )	return false;
	iMin += iInt;

	return true;
}

QString ConvertMSecToHHMMSSStr(int mSec)
{
	if (mSec < 0) mSec = 0;

	QTime time(0, 0, 0, 0);
	time = time.addMSecs(mSec);
	QString strTime = QString("%1:%2:%3")
			.arg(time.hour(), 2, 10, QChar('0'))
			.arg(time.minute(), 2, 10, QChar('0'))
			.arg(time.second(), 2, 10, QChar('0'));

	return strTime;
}

void StrTrimTail(char *szStr)	// wslim
{
	if (szStr == NULL)		return;

	for (int i = strlen(szStr) - 1; i >= 0; i--)
	{
		//Logging("szStr[%d]=[0x%X]\n", i, szStr[i]);
		if( isspace(szStr[i]) )		szStr[i] = 0;
		else						return;
	}
}

bool GetMidString(QString strIn, QString strBegin, QString strEnd, QString &strOut, bool blBeginLast, bool blEndLast)
{
	if( !GetRightString(strIn, strBegin, strOut, blBeginLast) )		return false;

	if( !GetLeftString(strOut, strEnd, strOut, blEndLast) )
	{
		strOut.clear();
		return false;
	}

	return true;
}

QString GetMidString(QString strIn, QString strBegin, QString strEnd, bool blBeginLast, bool blEndLast)
{
	QString strOut;

	if( !GetMidString(strIn, strBegin, strEnd, strOut, blBeginLast, blEndLast) )	return QString();

	return strOut;
}

bool GetRightString(QString strIn, QString strDel, QString &strOut, bool blLast)
{
	int index;

	strOut = strIn;
	if( blLast )	index = strIn.lastIndexOf(strDel);
	else			index = IndexOf(strIn, strDel);

	if( index < 0 )		return false;

	strOut = strIn.right(strIn.length() - strDel.length() - index);
	//Logging("strOut = \"%s\"\n", strOut.toUtf8().data());
	return true;
}

QString GetRightString(QString strIn, QString strDel, bool blLast)
{
	QString strOut;
	if( !GetRightString(strIn, strDel, strOut, blLast) )	return QString();

	return strOut;
}

bool GetLeftString(QString strIn, QString strDel, QString &strOut, bool blLast)
{
	int index;

	strOut = strIn;
	if( blLast )	index = strIn.lastIndexOf(strDel);
	else			index = IndexOf(strIn, strDel);

	if( index < 0 )		return false;

	strOut = strIn.left(index);
	//Logging("strOut = \"%s\"\n", strOut.toUtf8().data());
	return true;
}

QString GetLeftString(QString strIn, QString strDel, bool blLast)
{
	QString strOut;
	if( !GetLeftString(strIn, strDel, strOut, blLast) )		return QString();

	return strOut;
}

int Compare(QString str1, QString str2)
{
	return str1.compare(str2, Qt::CaseSensitive);
}

int iCompare(QString str1, QString str2)
{
	return str1.compare(str2, Qt::CaseInsensitive);
}

#if 0
bool Contains(QStringList &lsStr, QString str)
{
	return lsStr.contains(str, Qt::CaseSensitive);
}
#endif

bool iContains(QStringList &lsStr, QString str)
{
	return lsStr.contains(str, Qt::CaseInsensitive);
}

int IndexOfStr(QString &strStr, QString strFind, int index, Qt::CaseSensitivity cs)
{
	return strStr.indexOf(strFind, index, cs);
}

int IndexOf(QString &strStr, QString strFind, int index)
{
	return IndexOfStr(strStr, strFind, index);
}

int iIndexOf(QString &strStr, QString strFind, int index)
{
	return IndexOfStr(strStr, strFind, index, Qt::CaseInsensitive);
}

int LastIndexOfStr(QString &strStr, QString strFind, int index, Qt::CaseSensitivity cs)
{
	return strStr.lastIndexOf(strFind, index, cs);
}

int LastIndexOf(QString &strStr, QString strFind, int index)
{
	return LastIndexOfStr(strStr, strFind, index);
}

int iLastIndexOf(QString &strStr, QString strFind, int index)
{
	return LastIndexOfStr(strStr, strFind, index, Qt::CaseInsensitive);
}

#if 0
QString GetUtf8String(int iCode)
{
	return QString(QChar(iCode)).toUtf8();
}

QString GetUtf16String(int iCode)
{
	return QString(QChar(iCode));
}
#endif

void VerifySqlString(QString &str)
{
	int from = 0;

	while( (from = IndexOf(str, "'", from)) >= 0 )
	{
		str.insert(from, "'");
		from += 2;
	}
}

QString VerifySqlStringR(QString str)
{
	VerifySqlString(str);
	return str;
}

QString StrToFileStrR(QString str, bool blSlashOnly)	// VerifyFileStringR ->
{
	StrToFileStr(str, blSlashOnly);
	return str;
}

void StrToFileStr(QString &str, bool blSlashOnly)		// VerifyFileString ->
{
	str.replace('/',	QChar(0xFF0F));
	str.replace('\\',	QChar(0xFF3C));

	if( !blSlashOnly )
	{
		//str.replace('.',	QChar(0xFF0E));
		str.replace('?',	QChar(0xFF1F));
		str.replace('*',	QChar(0xFF0A));
		str.replace('\"',	QChar(0xFF02));
		str.replace('<',	QChar(0xFF1C));
		str.replace('>',	QChar(0xFF1E));
		str.replace('|',	QChar(0xFF5C));
		str.replace(':',	QChar(0xFF1A));
		//str.replace('&',	QChar(0xFF06));
	}
}

QString FileStrToStrR(QString str)
{
	FileStrToStr(str);
	return str;
}

void FileStrToStr(QString &str)
{
	str.replace(QChar(0xFF0F), '/');
	//str.replace(QChar(0xFF0E), '.');
	str.replace(QChar(0xFF3C), '\\');
	str.replace(QChar(0xFF1F), '?');
	str.replace(QChar(0xFF0A), '*');
	str.replace(QChar(0xFF02), '\"');
	str.replace(QChar(0xFF1C), '<');
	str.replace(QChar(0xFF1E), '>');
	str.replace(QChar(0xFF5C), '|');
	str.replace(QChar(0xFF1A), ':');
	//str.replace(QChar(0xFF06), '&');
}

int RemoveStringList(QStringList &strList, QString str, QRegExp::PatternSyntax syntax, Qt::CaseSensitivity cs)
{
	QRegExp regExp(str, cs, syntax);
	int nCnt = 0;
	int index;

	while( (index = strList.indexOf(regExp)) >= 0 )
	{
		nCnt++;
		strList.removeAt(index);
	}
	return nCnt;
}

int RemoveStringList(QStringList &strList, QStringList &lsFilter, QRegExp::PatternSyntax syntax, Qt::CaseSensitivity cs)
{
	int nCnt = 0;
	for(int i = 0; i < lsFilter.size(); i++)	nCnt += RemoveStringList(strList, lsFilter.at(i), syntax, cs);

	return nCnt;
}

int _IndexOfStringList(QStringList &strList, QString str, QStringList &strListGet, Qt::CaseSensitivity cs, QRegExp::PatternSyntax syntax)
{
	QRegExp regExp(str, cs, syntax);
	int index = 0;

	strListGet.clear();

	while( (index = strList.indexOf(regExp, index)) >= 0 )
	{
		strListGet.append(strList.at(index));
		if( ++index >= strList.size() )		break;
	}

	return strListGet.size();
}

int iIndexOfStringList(QStringList &strList, QString str, QStringList &strListGet, QRegExp::PatternSyntax syntax)
{
	return _IndexOfStringList(strList, str, strListGet, Qt::CaseInsensitive, syntax);
}

bool GetFileSizeString(int64_t llSize, QString &strGet, bool bl1024, bool blMinimal)
{
	if( llSize < 0 )	return false;

	if( llSize == 0 )
	{
		//strGet = QString("0 <FONT COLOR=white>B</FONT>");
		strGet = QString("0 B");
		return true;
	}

	float fUnitK = bl1024? 1024. : 1000.;

	int iUnit = 0;
	float fSize = (float)llSize;

	while( fSize >= fUnitK )
	{
		fSize /= fUnitK;
		iUnit++;
	}

	//fSize = (fSize / fUnitK) * 1000.;

	int nLow = 0;
	if( iUnit > 0 )
	{
		nLow = ( (fSize >= 100.)? 1 : ( (fSize >= 10.)? 2 : 3 ) ) - ( blMinimal? 1 : 0 );
	}

	//Logging("%s : %.4f\n", QString("%.%1f ").arg(nLow).toUtf8().data(), fSize);
	strGet.sprintf(QString("%.%1f ").arg(nLow).toUtf8().data(), fSize);
	if( iUnit == 0 )		strGet.append("B");
	else if( iUnit == 1 )	strGet.append("K");
	else if( iUnit == 2 )	strGet.append("M");
	else if( iUnit == 3 )	strGet.append("G");
	else if( iUnit == 4 )	strGet.append("T");

	return true;
}

QString GetFileSizeString(int64_t llSize, bool bl1024, bool blMinimal)
{
	QString strGet;
	if( !GetFileSizeString(llSize, strGet, bl1024, blMinimal) )		return QString();

	return strGet;
}

QString GetNumString(int iValue, QString strSuffix)
{
	QString strValue;

	if( iValue < 1000 )
	{
		strValue = QString::number(iValue);
		if( !strSuffix.isEmpty() )		strValue.append(" " + strSuffix);
	}
	else
	{
		for(int iDiv = 1000; iDiv > 0; iDiv /= 10)
		{
			strValue.append(QString::number(iValue / iDiv));
			iValue %= iDiv;

			//LoggingError("strValue = [ %s ]\n", strValue.toUtf8().data());

			if( iValue <= 0 )		break;
			if( iDiv == 1000 )		strValue.append(".");
		}


		strValue.append(" K" + strSuffix);
	}

	return strValue;
}

bool GetIntPair(QString strPair, int &i, int &n, int iDefValue)		// "no / count" or "no" parse
{
	i = n = iDefValue;

	QStringList lsStr = strPair.split("/");
	if( lsStr.size() >= 2 )
	{
		i = ToInt(GetTrimmed(lsStr.at(0)));
		n = ToInt(GetTrimmed(lsStr.at(1)));
	}
	else
	{
		i = ToInt(GetTrimmed(strPair));
	}

	if( i > 0 )
	{
		if( n < i )		n = i;
		return true;
	}

	i = n = iDefValue;
	return false;
}

bool GetIntBound(QString strInt, int &iInt, int nMaxBoundValue)		// for Year
{
	iInt = ToInt(strInt.left(4));
	if( iInt > 0 )
	{
		if( nMaxBoundValue <= 0 )		return true;
		if( iInt < nMaxBoundValue )		return true;
	}

	iInt = 0;
	return false;
}

#if 0
QString GetDateTimeString(time_t t)
{
	QDateTime DateTime;

	DateTime.setTime_t(t);
	//DateTime = DateTime.toLocalTime();
	return DateTime.toString("AP hh:mm yyyy.MM.dd");
}
#endif

bool IsNumber(QString strNum)
{
	if( strNum.isEmpty() )		return false;

	for(int i = strNum.size() - 1; i >= 0; i--)
	{
		if( strNum.at(i) < '0' || strNum.at(i) > '9' )		return false;
	}

	return true;
}

bool ToInt(QString strNum, int32_t &iInt32)
{
	bool blOk;
	iInt32 = strNum.toInt(&blOk);

	return blOk;
}

int ToInt(QString strNum)
{
	int32_t iInt32;
	if( !ToInt(strNum, iInt32) )	return INT_MIN;

	return iInt32;
}

int ToIntHex(QString strNumHex)
{
	bool blOk;

	int rc = strNumHex.toInt(&blOk, 16);

	if( !blOk )			return INT_MIN;

	return rc;
}

bool ToLongLong(QString strNum, int64_t &llInt64)
{
	bool blOk;
	llInt64 = strNum.toLongLong(&blOk);

	return blOk;
}

int64_t ToLongLong(QString strNum)
{
	int64_t llInt64;
	if( !ToLongLong(strNum, llInt64) )		return LLONG_MIN;

	return llInt64;
}

bool ToFloat(QString strNum, float &fFloat)
{
	bool blOk;
	fFloat = strNum.toFloat(&blOk);

	return blOk;
}

bool IsHttpUrlStrR(QString &strUrl)
{
	return strUrl.startsWith("http://", Qt::CaseInsensitive) || strUrl.startsWith("https://", Qt::CaseInsensitive);
}

bool IsHttpUrlStr(QString strUrl)
{
	return IsHttpUrlStrR(strUrl);
}

QString GetImageServerUrl()
{
	//return QString("http://%1:%2/").arg(g_Global.strLocalAddress).arg(CA_IMAGE_SERVER_PORT);
	return "";		// temp_code
}

QString GetImageServerUrl(QString strRoot, QString strPath, bool blPercentEncoding)
{
	QString strUrl = QString("%1%3").arg(GetImageServerUrl()).arg(strRoot);
	if( !strPath.startsWith("/") )		strUrl.append("/");

	if( blPercentEncoding )		strUrl.append(ToPercentEncodingR(strPath));
	else						strUrl.append(strPath);

	return strUrl;
	//return QUrl::fromEncoded(ToAscii(strUrl)).toString();
}

QString GetImageServerUrl(QString strRoot, int iId)
{
	return GetImageServerUrl(strRoot, QString::number(iId));
}

QString GetLoopbackImageServerUrl(QString strRoot, QString strPath)
{
	//QString strUrl = QString("http://127.0.0.1:%1/%2").arg(CA_IMAGE_SERVER_PORT).arg(strRoot);
	//if( !strPath.startsWith("/") )		strUrl.append("/");
	//strUrl.append(ToPercentEncodingR(strPath));

	//return strUrl;
	////return QUrl::fromEncoded(ToAscii(strUrl)).toString();
	return "";		// temp_code
}

QString GetLoopbackImageServerUrl(QString strRoot, int iId)
{
	return GetLoopbackImageServerUrl(strRoot, QString::number(iId));
}

QString ToPercentEncodingR(QString str, bool blSpaceTrim)
{
	if( blSpaceTrim )	TrimmSpace(str);

#if 0
	str.replace("%", "%25");
	str.replace(" ", "%20");
	str.replace("#", "%23");
	str.replace("&", "%26");
	str.replace("?", "%3F");
	str.replace("=", "%3D");

	return str;
#endif

	//return QUrl::toPercentEncoding(str,	"~`!@$^*()_-+{[}]|\\:;\"'<,>,/",	"% #&?=");
	//return QUrl::toPercentEncoding(str,	"~`!@X$X^X*()_-+={[}]|\\:;\"'<,>,X/");
	//return QUrl::toPercentEncoding(str,	"~`!@#$%^&*()_-+={[}]|\\:;\"'<,>.?/"
	//return QUrl::toPercentEncoding(str,	":/[]@!$()*+,;");
	return QUrl::toPercentEncoding(str,		":/[]@$()*,;");
	//return QUrl::toPercentEncoding(str,	":/[]@!$()*+,;#?=");
}

bool ToPercentEncoding(QString &str, bool blSpaceTrim)
{
	str = ToPercentEncodingR(str, blSpaceTrim);

	return !str.isEmpty();
}

QString FromPercentEncodingR(QString str, bool blSpaceTrim)
{
	if( blSpaceTrim )	TrimmSpace(str);

	return QUrl::fromPercentEncoding(ToAscii(str));
}

bool FromPercentEncoding(QString &str, bool blSpaceTrim)
{
	str = FromPercentEncodingR(str, blSpaceTrim);

	return !str.isEmpty();
}

QString GetUrlQueryString(QString strKey, QString strValue, bool blStart)
{
	return QString("%1%2=%3").arg(blStart? "?" : "&").arg(strKey).arg(ToPercentEncodingR(strValue));
}

QString GetUrlQueryString(QString strKey, int iValue, bool blStart)
{
	return QString("%1%2=%3").arg(blStart? "?" : "&").arg(strKey).arg(iValue);
}

QString ConvertCoverArtURLToName(QString fullpath)
{
	QString filename = fullpath;
	int colon = filename.length() - filename.lastIndexOf(":") - 1;
	filename = filename.right(colon);
	colon = filename.length() - filename.indexOf("/") - 1;
	filename = filename.right(colon);
	filename.replace("/", "_");
	filename = filename + ".jpg";

	return filename;
}
