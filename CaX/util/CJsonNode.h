#ifndef __JSON_PARSER_H__
#define __JSON_PARSER_H__

#include <QString>
#include <QStringList>
#include <QByteArray>
#include <jansson.h>

#include "CommonTypeDef.h"
#include "TMapDataDef.h"

class CJsonNode
{
	private:
		CJsonNode(json_t *lpJson);

	public:
		json_t	*m_lpJson;
		bool	m_blDecRef;

	public:
		CJsonNode& operator = (const CJsonNode& JsonNode);	// Copy CJsonNode

	public:
		CJsonNode();
		CJsonNode(const CJsonNode &JsonNode);				// Copy CJsonNode
		CJsonNode(QString strContent);
		CJsonNode(QString strKey, const char *szValue);		// Create string type
		CJsonNode(QString strKey, QString strValue);		// Create string type
		CJsonNode(QString strKey, int64_t llValue);			// Create integer type
		CJsonNode(QString strKey, double dValue);			// Create real type
		CJsonNode(json_type eJsonType);						// Create JSON_ARRAY, JSON_OBJECT, JSON_TRUE, JSON_FALSE
		CJsonNode(QString strKey, json_type eJsonType);		// Create JSON_ARRAY, JSON_OBJECT, JSON_TRUE, JSON_FALSE by Key

		~CJsonNode();

		//CJsonNode GetRef();

		bool IsNull()						{	return (m_lpJson == NULL);	};
		QString		ToCompactString()		{	return _ToString(true);		};
		QString		ToTabedString()			{	return _ToString(false);	};
		QByteArray	ToCompactByteArray()	{	return _ToByteArray(true);	};
		QByteArray	ToTabedByteArray()		{	return _ToByteArray(false);	};

		void		Clear();
		bool		SetContent(QString strContent);
		bool		Load(QString strJsonPath);

		int			GetObjectList(QStringList &lsKey, QList<CJsonNode> &lsJsonMode);
		int			GetKeyStrValueList(QStringList &lsKey, QStringList &lsValue);

		bool		GetInt64(QString strKey, int64_t &llValue);
		int64_t		GetInt64(QString strKey);
		bool		GetInt(QString strKey, int &iValue);
		int			GetInt(QString strKey);
		bool		GetString(QString strKey, QString &strValue);
		QString		GetString(QString strKey);
		QString		GetString();
		bool		GetBool(QString strKey, bool &blValue);
		bool		GetBool(QString strKey);

		bool		IsString(QString strKey);

		QStringList 	GetStringList(QString strKey);
		int				GetStringList(QString strKey, QStringList &lsStr);
		QStringList		*GetStringListP(QString strKey);
		QIntList		GetIntList(QString strKey, int iMin = 0);
		QIntList		*GetIntListP(QString strKey, int iMin = 0);
		int				GetIntList(QString strKey, QIntList &lsInt, int iMin = 0);

		CJsonNode 	GetObject(QString strKey);							// Get referenced CJsonNode
		bool		GetObject(QString strKey, CJsonNode &JsonNode);		// Get Copyed CJsonNode
		CJsonNode 	GetArray(QString strKey);							// Get referenced CJsonNode
		bool		GetArray(QString strKey, CJsonNode &JsonNode);		// Get Copyed CJsonNode
		CJsonNode 	GetArrayAt(int index);								// Get referenced CJsonNode
		bool		GetArrayAt(int index, CJsonNode &JsonNode);			// Get Copyed CJsonNode
		int			ArraySize();

	public:
		bool	Del(QString strKey);
		bool	Add(QString strKey, CJsonNode JsonNode);
		bool	Add(QString strKey, const char *szValue);			// Add string type
		bool	Add(QString strKey, QString strValue);				// Add string type
		bool	Add(QString strKey, int64_t llValue);				// Add integer type
		bool	Add(QString strKey, double dValue);					// Add real type
		bool	Add(QString strKey, bool blValue);					// Add bool type
		bool	Add(QString strKey, QStringList &lsStr);			// Add String Array type
		bool	Add(QString strKey, QIntList &lsInt);				// Add Int Array type

		bool	AddString(QString strKey, QString strValue);		// Add string type : if empty then not add
		bool	AddString(QString strKey, QString strValue1, QString strValue2);		// Add string type : if empty then not add
		bool	AddInt(QString strKey, int64_t llValue);			// Add integer type : if <= 0 then not add

		bool	AppendArray(CJsonNode JsonNode);
		bool	AppendArray(const char *strValue);					// Append Array string type
		bool	AppendArray(QString strValue);						// Append Array string type
		bool	AppendArray(int64_t llValue);						// Append Array integer type
		bool	AppendArray(double dValue);							// Append Array real type
		bool	AppendArray(bool blValue);							// Append Array bool type

		bool	InsertArray(size_t index, CJsonNode &JsonNode);
		bool	InsertArray(size_t index, const char *szValue);		// Insert Array string type
		bool	InsertArray(size_t index, QString strValue);		// Insert Array string type
		bool	InsertArray(size_t index, int64_t llValue);			// Insert Array integer type
		bool	InsertArray(size_t index, double dValue);			// Insert Array real type
		bool	InsertArray(size_t index, bool blValue);			// Insert Array real type

		bool	ReplaceArray(size_t index, CJsonNode &JsonNode);
		bool	ReplaceArray(size_t index, const char *szValue);	// Insert Array string type
		bool	ReplaceArray(size_t index, QString strValue);		// Insert Array string type
		bool	ReplaceArray(size_t index, int64_t llValue);		// Insert Array integer type
		bool	ReplaceArray(size_t index, double dValue);			// Insert Array real type
		bool	ReplaceArray(size_t index, bool blValue);			// Insert Array real type

		bool	RemoveArray(size_t index);
		bool	ClearArray();

	private:
		bool	_JsonObjectSet(QString &strKey, json_t *lpJson);
		bool	_AppendArray(json_t *lpJson);
		bool	_InsertArray(size_t index, json_t *lpJson);
		bool	_ReplaceArray(size_t index, json_t *lpJson);

	private:
		QString		_ToString(bool blCompact);
		QByteArray	_ToByteArray(bool blCompact);
};

bool	CheckAndSetMapDataStr(		CJsonNode &JsonItem, TMapData &MdInfo, bool &blSet, QString strJsonKey, int iStrKey );
bool	CheckAndSetMapDataStr(		CJsonNode &JsonItem, TMapData &MdInfo, QString strJsonKey, int iStrKey );
bool	CheckAndSetMapDataStrA(		CJsonNode JsonItem, TMapData &MdInfo, QString strJsonKey, int iStrKey );

bool	CheckAndSetMapDataInt(		CJsonNode &JsonItem, TMapData &MdInfo, bool &blSet, QString strJsonKey, int iKey );
bool	CheckAndSetMapDataInt(		CJsonNode &JsonItem, TMapData &MdInfo, QString strJsonKey, int iKey );

bool	CheckAndSetMapDataCdNumber(	CJsonNode &JsonItem, TMapData &MdInfo, bool &blSet );

#endif
