#include <limits.h>

#include "log.h"

#include "StringLib.h"
//#include "StringLib_g.h"
//#include "FileSystemLib.h"
//#include "QTrans.h"
#include "TMapData.h"
//#include "StrDef.h"

#include "CJsonNode.h"

#ifndef NO_LOGGING
	//#define __INC_DEC_REF_LOG__
#endif

#ifdef __INC_DEC_REF_LOG__
	#define LoggingIncRef(...)		LoggingWarning(__VA_ARGS__)
	#define LoggingDecRef(...)		LoggingError(__VA_ARGS__)
#else
	#define LoggingIncRef(...)
	#define LoggingDecRef(...)
#endif

bool CheckAndSetMapDataStr(CJsonNode &JsonItem, TMapData &MdInfo, bool &blSet, QString strJsonKey, int iStrKey)
{
	QString strGet = JsonItem.GetString(strJsonKey);
	if( strGet.isEmpty() || MapStrVal(MdInfo, iStrKey) == strGet )		return false;
	MapStrAdd(MdInfo, iStrKey, strGet);
	blSet = true;

	return true;
}

bool CheckAndSetMapDataStr(CJsonNode &JsonItem, TMapData &MdInfo, QString strJsonKey, int iStrKey)
{
	bool blSet;
	return CheckAndSetMapDataStr(JsonItem, MdInfo, blSet, strJsonKey, iStrKey);
}

bool CheckAndSetMapDataStrA(CJsonNode JsonItem, TMapData &MdInfo, QString strJsonKey, int iStrKey)
{
	bool blSet;
	return CheckAndSetMapDataStr(JsonItem, MdInfo, blSet, strJsonKey, iStrKey);
}

bool CheckAndSetMapDataInt(CJsonNode &JsonItem, TMapData &MdInfo, bool &blSet, QString strJsonKey, int iKey)
{
	int iGet = JsonItem.GetInt(strJsonKey);
	if( iGet <= 0 || MapIntVal(MdInfo, iKey) == iGet )		return false;
	MapIntAdd(MdInfo, iKey, iGet);
	blSet = true;

	return true;
}

bool CheckAndSetMapDataInt(CJsonNode &JsonItem, TMapData &MdInfo, QString strJsonKey, int iKey)
{
	bool blSet;
	return CheckAndSetMapDataInt(JsonItem, MdInfo, blSet, strJsonKey, iKey);
}

bool CheckAndSetMapDataCdNumber(CJsonNode &JsonItem, TMapData &MdInfo, bool &blSet)
{
	int i, n;
	//QString strGet = JsonItem.GetString(TR_CD_Number);
	QString strGet = JsonItem.GetString("CD-Number");		// temp_code
	if( strGet.isEmpty() || !GetIntPair(strGet, i, n) )		return false;
	if( i == MapIntVal(MdInfo, iKey_CdNumber) && n == MapIntVal(MdInfo, iKey_CdTotal) )		return false;
	MapIntAdd(MdInfo, iKey_CdNumber, i);
	MapIntAdd(MdInfo, iKey_CdTotal, n);
	blSet = true;

	return true;
}

CJsonNode& CJsonNode::operator = (const CJsonNode& JsonNode)
{
	Clear();
	m_lpJson	= json_copy(JsonNode.m_lpJson);
	m_blDecRef	= ( m_lpJson != NULL );

#ifdef __INC_DEC_REF_LOG__
	if( m_blDecRef )	LoggingIncRef("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< %p\n", m_lpJson);
#endif

	return *this;
}

CJsonNode::CJsonNode(const CJsonNode &JsonNode)
{
	m_lpJson	= json_copy(JsonNode.m_lpJson);
	m_blDecRef	= ( m_lpJson != NULL );

#ifdef __INC_DEC_REF_LOG__
	if( m_blDecRef )	LoggingIncRef("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< %p\n", m_lpJson);
#endif
}

CJsonNode::CJsonNode(json_t *lpJson)
{
	m_lpJson	= lpJson;
	m_blDecRef	= false;
}

CJsonNode::CJsonNode()
{
	m_lpJson	= NULL;
	m_blDecRef	= false;

#ifdef __INC_DEC_REF_LOG__
	if( m_blDecRef )	LoggingIncRef("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< %p\n", m_lpJson);
#endif
}

CJsonNode::CJsonNode(QString strContent)
{
	//m_lpJson	= NULL;
	//m_blDecRef	= false;
	SetContent(strContent);
}

bool CJsonNode::_JsonObjectSet(QString &strKey, json_t *lpJson)
{
	if( !lpJson )	return false;
	if( !m_lpJson )
	{
		json_decref(lpJson);
		return false;
	}

	bool blSuccess = ( json_object_set(m_lpJson, ToAscii(strKey).data(), lpJson) == 0 );
	json_decref(lpJson);

	return blSuccess;
}

CJsonNode::CJsonNode(QString strKey, const char *szValue)
{
	m_lpJson	= json_object();
	m_blDecRef	= true;

	_JsonObjectSet(strKey, json_string(szValue));
	LoggingIncRef("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< %p\n", m_lpJson);
}

CJsonNode::CJsonNode(QString strKey, QString strValue)
{
	m_lpJson	= json_object();
	m_blDecRef	= true;

	_JsonObjectSet(strKey, json_string(ToAscii(strValue).data()));
	LoggingIncRef("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< %p\n", m_lpJson);
}

CJsonNode::CJsonNode(QString strKey, int64_t llValue)
{
	m_lpJson	= json_object();
	m_blDecRef	= true;

	_JsonObjectSet(strKey, json_integer(llValue));
	LoggingIncRef("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< %p\n", m_lpJson);
}

CJsonNode::CJsonNode(QString strKey, double dValue)
{
	m_lpJson	= json_object();
	m_blDecRef	= true;

	_JsonObjectSet(strKey, json_real(dValue));
	LoggingIncRef("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< %p\n", m_lpJson);
}

CJsonNode::CJsonNode(json_type eJsonType)
{
	m_lpJson	= NULL;
	m_blDecRef	= false;

	if( eJsonType == JSON_ARRAY )			m_lpJson = json_array();
	else if( eJsonType == JSON_OBJECT )		m_lpJson = json_object();
	else if( eJsonType == JSON_TRUE )		m_lpJson = json_true();
	else if( eJsonType == JSON_FALSE )		m_lpJson = json_false();

	if( m_lpJson )
	{
		m_blDecRef = true;
		LoggingIncRef("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< %p\n", m_lpJson);
	}
}

CJsonNode::CJsonNode(QString strKey, json_type eJsonType)
{
	m_lpJson		= NULL;
	m_blDecRef		= false;
	json_t *lpJson	= NULL;

	if( eJsonType == JSON_ARRAY )			lpJson = json_array();
	else if( eJsonType == JSON_OBJECT )		lpJson = json_object();
	else if( eJsonType == JSON_TRUE )		lpJson = json_true();
	else if( eJsonType == JSON_FALSE )		lpJson = json_false();

	if( lpJson )
	{
		m_lpJson	= json_object();
		m_blDecRef	= true;
		_JsonObjectSet(strKey, lpJson);
		LoggingIncRef("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< %p\n", m_lpJson);
	}
}

CJsonNode::~CJsonNode()
{
	Clear();
}

#if 0
CJsonNode CJsonNode::GetRef()
{
	return CJsonNode(m_lpJson);
}
#endif

void CJsonNode::Clear()
{
	if( m_blDecRef && m_lpJson )
	{
		LoggingDecRef(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> %p\n", m_lpJson);
		json_decref(m_lpJson);
	}

	m_lpJson	= NULL;
	m_blDecRef	= false;
}

bool CJsonNode::SetContent(QString strContent)
{
	Clear();

	json_error_t error;

	m_lpJson = json_loads(ToAscii(strContent).data(), JSON_DISABLE_EOF_CHECK, &error);

	if( m_lpJson )		m_blDecRef = true;
#ifndef NO_LOGGING
	else				LogCritical(" json error on line %d: %s\n", error.line, error.text);
#endif

#ifdef __INC_DEC_REF_LOG__
	if( m_lpJson )		LoggingIncRef("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< %p\n", m_lpJson);
#endif


	return m_blDecRef;
}

bool CJsonNode::Load(QString strJsonPath)
{

	//Clear();

	//QString strRead;
	//if( !ReadFile(strJsonPath, strRead) )	return false;

	//return SetContent(strRead);
	return false;		// temp_code
}

QString CJsonNode::_ToString(bool blCompact)
{
	if( !m_lpJson )		return QString();

	char *szJson = json_dumps(m_lpJson, blCompact? JSON_COMPACT : JSON_INDENT(2));

	if( !szJson )		return QString();

	QString strJsonString = QString(szJson);

	free(szJson);

	return strJsonString;
}

QByteArray CJsonNode::_ToByteArray(bool blCompact)
{
	if( !m_lpJson)		return QByteArray();

	char *szJson = json_dumps(m_lpJson, blCompact? JSON_COMPACT : JSON_INDENT(2));

	if( !szJson )		return QByteArray();

	QByteArray baJson = QByteArray(szJson);

	free(szJson);

	return baJson;
}

int CJsonNode::GetObjectList(QStringList &lsKey, QList<CJsonNode> &lsJsonMode)
{
	lsKey.clear();
	lsJsonMode.clear();

	if( !m_lpJson )		return -1;

	const char *szKey;
	json_t *lpJson;

	json_object_foreach(m_lpJson, szKey, lpJson)
	{
		lsKey.append(szKey);
		lsJsonMode.append(CJsonNode(lpJson));
	}

	return lsKey.size();
}

int CJsonNode::GetKeyStrValueList(QStringList &lsKey, QStringList &lsValue)
{
	lsKey.clear();
	lsValue.clear();

	if( !m_lpJson )		return -1;

	QString strValue;
	const char *szKey;
	json_t *lpJson;

	json_object_foreach(m_lpJson, szKey, lpJson)
	{
		if( json_typeof(lpJson) == JSON_STRING )			strValue = json_string_value(lpJson);
		else if( json_typeof(lpJson) == JSON_INTEGER )		strValue = QString::number(json_integer_value(lpJson));
		else if( json_typeof(lpJson) == JSON_TRUE )			strValue = "true";
		else if( json_typeof(lpJson) == JSON_FALSE )		strValue = "false";
		else
		{
			lsKey.clear();
			lsValue.clear();
			return -1;
		}

		lsKey.append(szKey);
		lsValue.append(strValue);
	}

	return lsKey.size();
}

bool CJsonNode::GetInt64(QString strKey, int64_t &llValue)
{
	if( !m_lpJson )		return false;

	json_t *lpJson = json_object_get(m_lpJson, ToAscii(strKey).data());
	if( !lpJson )		return false;
	if( json_typeof(lpJson) == JSON_INTEGER )
	{
		//LoggingMax("sizeof(json_int_t) = %d\n", sizeof(json_int_t));
		llValue = json_integer_value(lpJson);
		return true;
	}
	else if( json_typeof(lpJson) == JSON_STRING )
	{
		if( (llValue = ToLongLong(QString(json_string_value(lpJson)))) == LLONG_MIN )	return false;
		return true;
	}

	return false;
}

int64_t CJsonNode::GetInt64(QString strKey)
{
	int64_t llValue;
	if( !GetInt64(strKey, llValue) )	return LLONG_MIN;

	return llValue;
}

bool CJsonNode::GetInt(QString strKey, int &iValue)
{
	int64_t llValue;
	if( !GetInt64(strKey, llValue) )	return false;

	if( llValue < INT_MIN || llValue > INT_MAX )
	{
		LogCritical("int is exceed range !!!\n");
		return false;
	}

	iValue = (int)llValue;

	return true;
}

int CJsonNode::GetInt(QString strKey)
{
	int iValue;
	if( !GetInt(strKey, iValue) )	return INT_MIN;

	return iValue;
}

bool CJsonNode::GetString(QString strKey, QString &strValue)
{
	if( !m_lpJson )		return false;

	json_t *lpJson = json_object_get(m_lpJson, ToAscii(strKey).data());
	if( !lpJson )		return false;
	if( json_typeof(lpJson) == JSON_STRING )
	{
		strValue = json_string_value(lpJson);
		return true;
	}
	else if( json_typeof(lpJson) == JSON_INTEGER )
	{
		strValue = QString::number(json_integer_value(lpJson));
		return true;
	}
	else if( json_typeof(lpJson) == JSON_TRUE )
	{
		strValue = "true";
		return true;
	}
	else if( json_typeof(lpJson) == JSON_FALSE )
	{
		strValue = "false";
		return true;
	}

	return false;
}

QString CJsonNode::GetString(QString strKey)
{
	QString strValue;

	if( !GetString(strKey, strValue) )		return QString();

	return strValue;
}

QString CJsonNode::GetString()
{
	if( !m_lpJson )		return QString();

	if( json_typeof(m_lpJson) == JSON_STRING )		return json_string_value(m_lpJson);
	if( json_typeof(m_lpJson) == JSON_INTEGER )		return QString::number(json_integer_value(m_lpJson));
	if( json_typeof(m_lpJson) == JSON_TRUE )		return "true";
	if( json_typeof(m_lpJson) == JSON_FALSE )		return "false";

	return QString();
}

bool CJsonNode::GetBool(QString strKey, bool &blValue)
{
	blValue = false;
	if( !m_lpJson )		return false;

	json_t *lpJson = json_object_get(m_lpJson, ToAscii(strKey).data());
	if( !lpJson )		return false;

	if( json_typeof(lpJson) == JSON_TRUE )				blValue = true;
	else if( json_typeof(lpJson) == JSON_FALSE )		blValue = false;
	else if( json_typeof(lpJson) == JSON_STRING )
	{
		QString strValue = json_string_value(lpJson);
		if( iCompare(strValue, "true") == 0 )			blValue = true;
		else if( iCompare(strValue, "false") == 0 )		blValue = false;
		else											return false;
	}
	else												return false;

	return true;
}

bool CJsonNode::GetBool(QString strKey)
{
	bool blValue;

	if( !GetBool(strKey, blValue) )		return false;

	return blValue;
}

bool CJsonNode::IsString(QString strKey)
{
	QString strValue;

	return GetString(strKey, strValue);
}

QStringList CJsonNode::GetStringList(QString strKey)
{
	QStringList lsStr;
	GetStringList(strKey, lsStr);
	return lsStr;
}

int CJsonNode::GetStringList(QString strKey, QStringList &lsStr)
{
	lsStr.clear();
	if( !m_lpJson )		return -1;

	json_t *lpJson = json_object_get(m_lpJson, ToAscii(strKey).data());
	if( !lpJson )								return -1;
	if( json_typeof(lpJson) != JSON_ARRAY )		return -1;

	int nCnt = json_array_size(lpJson);

	json_t *lpJsonItem;
	for(int i = 0; i < nCnt; i++)
	{
		lpJsonItem = json_array_get(lpJson, i);
		if( json_typeof(lpJsonItem) == JSON_STRING )		lsStr.append(json_string_value(lpJsonItem));
		else if( json_typeof(lpJsonItem) == JSON_INTEGER )	lsStr.append(QString::number(json_integer_value(lpJsonItem)));
		else if( json_typeof(lpJsonItem) == JSON_TRUE )		lsStr.append("true");
		else if( json_typeof(lpJsonItem) == JSON_FALSE )	lsStr.append("false");
	}

	return lsStr.size();
}

QStringList *CJsonNode::GetStringListP(QString strKey)
{
	if( !m_lpJson )		return NULL;

	json_t *lpJson = json_object_get(m_lpJson, ToAscii(strKey).data());
	if( !lpJson )								return NULL;
	if( json_typeof(lpJson) != JSON_ARRAY )		return NULL;

	int nCnt = json_array_size(lpJson);

	json_t *lpJsonItem;
	QStringList *lplsStr = new QStringList();
	for(int i = 0; i < nCnt; i++)
	{
		lpJsonItem = json_array_get(lpJson, i);
		if( json_typeof(lpJsonItem) == JSON_STRING )			lplsStr->append(json_string_value(lpJsonItem));
		else if( json_typeof(lpJsonItem) == JSON_INTEGER )		lplsStr->append(QString::number(json_integer_value(lpJsonItem)));
	}

	if( lplsStr->isEmpty() )	_safeDelete(lplsStr);

	return lplsStr;
}

QIntList CJsonNode::GetIntList(QString strKey, int iMin)
{
	QIntList lsInt;
	GetIntList(strKey, lsInt, iMin);
	return lsInt;
}

int CJsonNode::GetIntList(QString strKey, QIntList &lsInt, int iMin)
{
	lsInt.clear();
	if( !m_lpJson )		return -1;

	json_t *lpJson = json_object_get(m_lpJson, ToAscii(strKey).data());
	if( !lpJson )								return -1;
	if( json_typeof(lpJson) != JSON_ARRAY )		return -1;

	int nCnt = json_array_size(lpJson);

	json_t *lpJsonItem;
	int iInt;
	for(int i = 0; i < nCnt; i++)
	{
		lpJsonItem = json_array_get(lpJson, i);
		iInt = iMin;
		if( json_typeof(lpJsonItem) == JSON_STRING )			iInt = ToInt(QString(json_string_value(lpJsonItem)));
		else if( json_typeof(lpJsonItem) == JSON_INTEGER )		iInt = json_integer_value(lpJsonItem);

		if( iInt > iMin )	lsInt.append(iInt);
	}

	return lsInt.size();
}

QIntList *CJsonNode::GetIntListP(QString strKey, int iMin)
{
	if( !m_lpJson )		return NULL;

	json_t *lpJson = json_object_get(m_lpJson, ToAscii(strKey).data());
	if( !lpJson )								return NULL;
	if( json_typeof(lpJson) != JSON_ARRAY )		return NULL;

	int nCnt = json_array_size(lpJson);

	json_t *lpJsonItem;
	int iInt;
	QIntList *lplsInt = new QIntList();
	for(int i = 0; i < nCnt; i++)
	{
		lpJsonItem = json_array_get(lpJson, i);
		iInt = iMin;
		if( json_typeof(lpJsonItem) == JSON_STRING )			iInt = ToInt(QString(json_string_value(lpJsonItem)));
		else if( json_typeof(lpJsonItem) == JSON_INTEGER )		iInt = json_integer_value(lpJsonItem);

		if( iInt > iMin )	lplsInt->append(iInt);
	}

	if( lplsInt->isEmpty() )	_safeDelete(lplsInt);

	return lplsInt;
}

CJsonNode CJsonNode::GetObject(QString strKey)
{
	do
	{
		if( !m_lpJson || strKey.isEmpty() )			break;

		json_t *lpJson = json_object_get(m_lpJson, ToAscii(strKey).data());
		if( !lpJson )								break;
		if( json_typeof(lpJson) != JSON_OBJECT )	break;

		return CJsonNode(lpJson);
	} while(false);

	//LoggingError("Not found Json Object [ %s ] :\n%s\n", ToAscii(strKey).data(), ToAscii(toString).data());
	return CJsonNode();
}

bool CJsonNode::GetObject(QString strKey, CJsonNode &JsonNode)
{
	JsonNode = GetObject(strKey);

	return !JsonNode.IsNull();
}

CJsonNode CJsonNode::GetArray(QString strKey)
{
	do
	{
		if( !m_lpJson )		break;

		json_t *lpJson = json_object_get(m_lpJson, ToAscii(strKey).data());
		if( !lpJson )								break;
		if( json_typeof(lpJson) != JSON_ARRAY )		break;

		return CJsonNode(lpJson);
	} while(false);

	//LoggingError("Not found Json Array [ %s ] :\n%s\n", ToAscii(strKey).data(), ToAscii(toString()).data());
	return CJsonNode();
}

bool CJsonNode::GetArray(QString strKey, CJsonNode &JsonNode)
{
	JsonNode = GetArray(strKey);

	return !JsonNode.IsNull();
}

CJsonNode CJsonNode::GetArrayAt(int index)
{
	do
	{
		if( index < 0 )		break;

		json_t *lpJson = json_array_get(m_lpJson, index);
		if( !lpJson )		break;

		return CJsonNode(lpJson);
	} while(false);

	//LoggingError("Not found Json Array Item (index = %d) :\n%s\n", index, ToAscii(toString()).data());
	return CJsonNode();
}

bool CJsonNode::GetArrayAt(int index, CJsonNode &JsonNode)
{
	JsonNode = GetArrayAt(index);

	return !JsonNode.IsNull();
}

int CJsonNode::ArraySize()
{
	if( !m_lpJson )								return -1;
	if( json_typeof(m_lpJson) != JSON_ARRAY )	return -1;

	return json_array_size(m_lpJson);
}

bool CJsonNode::Del(QString strKey)
{
	if( !m_lpJson || strKey.isEmpty() )			return false;

	return ( json_object_del(m_lpJson, ToAscii(strKey).data()) == 0 );
}

bool CJsonNode::Add(QString strKey, CJsonNode JsonNode)
{
	if( !m_lpJson || !JsonNode.m_lpJson || strKey.isEmpty() )	return false;

	return ( json_object_set(m_lpJson, ToAscii(strKey).data(), JsonNode.m_lpJson) == 0 );
}

bool CJsonNode::Add(QString strKey, const char *szValue)
{
	if( !m_lpJson || strKey.isEmpty() )		return false;

	return _JsonObjectSet(strKey, json_string(szValue));
}

bool CJsonNode::Add(QString strKey, QString strValue)
{
	return Add(strKey, (const char *)ToAscii(strValue).data());
}

bool CJsonNode::Add(QString strKey, int64_t llValue)
{
	if( !m_lpJson || strKey.isEmpty() )		return false;

	return _JsonObjectSet(strKey, json_integer(llValue));
}

bool CJsonNode::Add(QString strKey, double dValue)
{
	if( !m_lpJson || strKey.isEmpty() )		return false;

	return _JsonObjectSet(strKey, json_real(dValue));
}

bool CJsonNode::Add(QString strKey, bool blValue)
{
	if( !m_lpJson || strKey.isEmpty() )		return false;

	return _JsonObjectSet(strKey, blValue? json_true() : json_false());
}

bool CJsonNode::Add(QString strKey, QStringList &lsStr)
{
	if( !m_lpJson || strKey.isEmpty() || lsStr.isEmpty() )		return false;

	json_t *lpJsonArray = json_array();

	foreach(QString str, lsStr)
	{
		json_t *lpJson = json_string(ToAscii(str).data());
		if( json_array_append(lpJsonArray, lpJson) != 0 )
		{
			json_decref(lpJson);
			json_decref(lpJsonArray);
			return false;
		}
		json_decref(lpJson);
	}

	return _JsonObjectSet(strKey, lpJsonArray);
}

bool CJsonNode::Add(QString strKey, QIntList &lsInt)
{
	if( !m_lpJson || strKey.isEmpty() || lsInt.isEmpty() )		return false;

	json_t *lpJsonArray = json_array();

	foreach(int32_t i, lsInt)
	{
		json_t *lpJson = json_integer(i);
		if( json_array_append(lpJsonArray, lpJson) != 0 )
		{
			json_decref(lpJson);
			json_decref(lpJsonArray);
			return false;
		}
		json_decref(lpJson);
	}

	return _JsonObjectSet(strKey, lpJsonArray);
}

bool CJsonNode::AddString(QString strKey, QString strValue)
{
	if( strValue.isEmpty() )	return false;

	return Add(strKey, (const char *)ToAscii(strValue).data());
}

bool CJsonNode::AddString(QString strKey, QString strValue1, QString strValue2)
{
	if( !strValue1.isEmpty() )	return Add(strKey, (const char *)ToAscii(strValue1).data());

	if( strValue2.isEmpty() )	return false;

	return Add(strKey, (const char *)ToAscii(strValue2).data());
}

bool CJsonNode::AddInt(QString strKey, int64_t llValue)
{
//	if( llValue <= 0 )						return false;

	if( !m_lpJson || strKey.isEmpty() )		return false;

	return _JsonObjectSet(strKey, json_integer(llValue));
}

bool CJsonNode::AppendArray(CJsonNode JsonNode)
{
	if( !JsonNode.m_lpJson )	return false;
	if( !m_lpJson )
	{
		m_lpJson	= json_array();
		m_blDecRef	= true;
		LoggingIncRef("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< %p\n", m_lpJson);
	}

	if( json_typeof(m_lpJson) != JSON_ARRAY )	return false;

	return ( json_array_append(m_lpJson, JsonNode.m_lpJson) == 0 );
}

bool CJsonNode::AppendArray(const char *szValue)
{
	return _AppendArray(json_string(szValue));
}

bool CJsonNode::AppendArray(QString strValue)
{
	return _AppendArray(json_string(ToAscii(strValue).data()));
}

bool CJsonNode::AppendArray(int64_t llValue)
{
	return _AppendArray(json_integer(llValue));
}

bool CJsonNode::AppendArray(double dValue)
{
	return _AppendArray(json_real(dValue));
}

bool CJsonNode::AppendArray(bool blValue)
{
	return _AppendArray(blValue? json_true() : json_false());
}

bool CJsonNode::InsertArray(size_t index, CJsonNode &JsonNode)
{
	if( !JsonNode.m_lpJson )	return false;
	if( !m_lpJson )
	{
		m_lpJson	= json_array();
		m_blDecRef	= true;
		LoggingIncRef("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< %p\n", m_lpJson);
	}

	if( json_typeof(m_lpJson) != JSON_ARRAY )	return false;

	return ( json_array_insert(m_lpJson, index, JsonNode.m_lpJson) == 0 );
}

bool CJsonNode::InsertArray(size_t index, const char *szValue)
{
	return _InsertArray(index, json_string(szValue));
}

bool CJsonNode::InsertArray(size_t index, QString strValue)
{
	return _InsertArray(index, json_string(ToAscii(strValue).data()));
}

bool CJsonNode::InsertArray(size_t index, int64_t llValue)
{
	return _InsertArray(index, json_integer(llValue));
}

bool CJsonNode::InsertArray(size_t index, double dValue)
{
	return _InsertArray(index, json_real(dValue));
}

bool CJsonNode::InsertArray(size_t index, bool blValue)
{
	return _InsertArray(index, blValue? json_true() : json_false());
}

bool CJsonNode::ReplaceArray(size_t index, CJsonNode &JsonNode)
{
	if( !JsonNode.m_lpJson )	return false;
	if( !m_lpJson )				return false;

	if( json_typeof(m_lpJson) != JSON_ARRAY )		return false;
	if( index >= json_array_size(m_lpJson) )		return false;
	if( json_array_remove(m_lpJson, index) != 0 )	return false;

	return ( json_array_insert(m_lpJson, index, JsonNode.m_lpJson) == 0 );
}

bool CJsonNode::ReplaceArray(size_t index, const char *szValue)
{
	return _ReplaceArray(index, json_string(szValue));
}

bool CJsonNode::ReplaceArray(size_t index, QString strValue)
{
	return _ReplaceArray(index, json_string(ToAscii(strValue).data()));
}

bool CJsonNode::ReplaceArray(size_t index, int64_t llValue)
{
	return _ReplaceArray(index, json_integer(llValue));
}

bool CJsonNode::ReplaceArray(size_t index, double dValue)
{
	return _ReplaceArray(index, json_real(dValue));
}

bool CJsonNode::ReplaceArray(size_t index, bool blValue)
{
	return _ReplaceArray(index, blValue? json_true() : json_false());
}

bool CJsonNode::RemoveArray(size_t index)
{
	if( !m_lpJson )								return false;
	if( json_typeof(m_lpJson) != JSON_ARRAY )	return false;

	return ( json_array_remove(m_lpJson, index) == 0 );
}

bool CJsonNode::ClearArray()
{
	if( !m_lpJson )								return false;
	if( json_typeof(m_lpJson) != JSON_ARRAY )	return false;

	return ( json_array_clear(m_lpJson) == 0 );
}

bool CJsonNode::_AppendArray(json_t *lpJson)
{
	if( !lpJson )	return false;

	if( !m_lpJson )
	{
		m_lpJson	= json_array();
		m_blDecRef	= true;
		LoggingIncRef("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< %p\n", m_lpJson);
	}

	bool blSuccess = false;
	do
	{
		if( json_typeof(m_lpJson) != JSON_ARRAY )	break;

		blSuccess = ( json_array_append(m_lpJson, lpJson) == 0 );
	} while(false);

	json_decref(lpJson);
	return blSuccess;
}

bool CJsonNode::_InsertArray(size_t index, json_t *lpJson)
{
	if( !lpJson )	return false;

	if( !m_lpJson )
	{
		m_lpJson	= json_array();
		m_blDecRef	= true;
		LoggingIncRef("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< %p\n", m_lpJson);
	}

	bool blSuccess = false;
	do
	{
		if( json_typeof(m_lpJson) != JSON_ARRAY )	break;

		blSuccess = ( json_array_insert(m_lpJson, index, lpJson) == 0 );
	} while(false);

	json_decref(lpJson);
	return blSuccess;
}

bool CJsonNode::_ReplaceArray(size_t index, json_t *lpJson)
{
	if( !lpJson )		return false;
	if( !m_lpJson )		return false;

	bool blSuccess = false;
	do
	{
		if( json_typeof(m_lpJson) != JSON_ARRAY )	break;
		if( index >= json_array_size(m_lpJson) )	break;

		if( json_array_remove(m_lpJson, index) != 0 )	break;

		blSuccess = ( json_array_insert(m_lpJson, index, lpJson) == 0 );
	} while(false);

	json_decref(lpJson);
	return blSuccess;
}

#ifndef NO_LOGGING
void JsonTest()
{
	LogDebug("json_int_t : %d\n", sizeof(json_int_t));
	CJsonNode JsonString(	"str",	"str1");
	LogDebug("JsonString = \n%s\n", JsonString.ToTabedByteArray().data());

	CJsonNode JsonInteger(	"int",	100LL);
	LogDebug("JsonInteger = \n%s\n", JsonInteger.ToTabedByteArray().data());

	CJsonNode JsonReal(		"real",	1.5);
	LogDebug("JsonReal = \n%s\n", JsonReal.ToTabedByteArray().data());

	CJsonNode JsonTrue(		JSON_TRUE);
	LogDebug("JsonTrue = \n%s\n", JsonTrue.ToTabedByteArray().data());

	CJsonNode JsonTrueKey(	"true", JSON_TRUE);
	LogDebug("JsonTrueKey = \n%s\n", JsonTrueKey.ToTabedByteArray().data());

	CJsonNode JsonFalse(	JSON_FALSE);
	LogDebug("JsonFalse = \n%s\n", JsonFalse.ToTabedByteArray().data());

	CJsonNode JsonFalseKey(	"false", JSON_FALSE);
	LogDebug("JsonFalseKey = \n%s\n", JsonFalseKey.ToTabedByteArray().data());

	CJsonNode JsonArray(	JSON_ARRAY);
	LogDebug("JsonArray = \n%s\n", JsonArray.ToTabedByteArray().data());

	CJsonNode JsonArrayKey(	"array", JSON_ARRAY);
	LogDebug("JsonArrayKey = \n%s\n", JsonArrayKey.ToTabedByteArray().data());

	CJsonNode JsonObj(		JSON_OBJECT);
	LogDebug("JsonObj = \n%s\n", JsonObj.ToTabedByteArray().data());

	CJsonNode JsonObjKey(	"obj", JSON_OBJECT);
	LogDebug("JsonObjKey = \n%s\n", JsonObjKey.ToTabedByteArray().data());

	JsonArray.AppendArray("array1");
	JsonArray.AppendArray(200LL);
	JsonArray.AppendArray(3.141592);
	JsonArray.AppendArray(true);
	JsonArray.AppendArray(false);
	LogDebug("JsonArray = \n%s\n", JsonArray.ToTabedByteArray().data());

	JsonObj.Add("strobj", JsonString);
	JsonObj.Add("intobj", JsonInteger);
	JsonObj.Add("realobj", JsonReal);
	JsonObj.Add("tureobj", JsonTrue);
	JsonObj.Add("truekeyobj", JsonTrueKey);
	JsonObj.Add("falseobj", JsonFalse);
	JsonObj.Add("falseKeyobj", JsonFalseKey);
	JsonObj.Add("arrayobj", JsonArray);
	JsonObj.Add("arraykeyobj", JsonArrayKey);
	JsonObj.Add("jsonkeyobj", JsonObjKey);
	LogDebug("JsonObj = \n%s\n", JsonObj.ToTabedByteArray().data());

	JsonObj.GetObject("intobj").Add("inssub", JsonArray);
	LogDebug("JsonObj = \n%s\n", JsonObj.ToTabedByteArray().data());

	QStringList		lsKey;
	QList<CJsonNode>	lsJsonNode;

	JsonObj.GetObjectList(lsKey, lsJsonNode);
	for(int i = 0; i < lsKey.size(); i++)	LogDebug("%02d : %s :\n%s\n", i, ToAscii(lsKey.at(i)).data(), lsJsonNode.value(i).ToTabedByteArray().data());
}
#endif
