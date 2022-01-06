#include "airablemanager.h"


AirableManager::AirableManager(QObject *parent)
{
	Q_UNUSED(parent)

	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespInfo(QString, int)));
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespCoverArt(QString, int, int)), this, SLOT(SlotRespCoverArt(QString, int, int)));

	m_ServiceType = -1;
	m_bSaveAuth = false;

}

AirableManager::~AirableManager()
{

}



void AirableManager::RequestLogin(int nServiceType, QString userID, QString password, bool bSaveAuth)
{
	m_ServiceType = nServiceType;
	m_bSaveAuth = bSaveAuth;

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_USER, userID);
	node.Add(KEY_PASS, password);
	node.AddInt	(KEY_TYPE,		m_ServiceType);
	if (m_bSaveAuth)
		node.Add(KEY_SAVE_AUTH, m_bSaveAuth);

	node.Add(KEY_CMD0,		VAL_AIRABLE);
	node.Add(KEY_CMD1,		VAL_URL);

	RequestCommand(node, AIRABLE_LOGIN);
}

void AirableManager::RequestLogout(int nServiceType, QString url)
{
	m_ServiceType = nServiceType;

	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_AIRABLE);
	node.Add	(KEY_CMD1,		VAL_URL);
	node.AddInt	(KEY_TYPE,		m_ServiceType);
	node.Add	(KEY_URL,		url);

	RequestCommand(node, AIRABLE_LOGOUT);
}

void AirableManager::RequestAuth(int nServiceType)
{
	m_ServiceType = nServiceType;

	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_AIRABLE);
	node.Add	(KEY_CMD1,		VAL_AUTH);
	node.AddInt	(KEY_TYPE,		m_ServiceType);

	RequestCommand(node, AIRABLE_AUTH);
}

void AirableManager::RequestURL(int nServiceType, QString url)
{
	m_ServiceType = nServiceType;

	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_AIRABLE);
	node.Add	(KEY_CMD1,		VAL_URL);
	node.AddInt	(KEY_TYPE,		m_ServiceType);
	if (!url.isEmpty())
		node.Add(KEY_URL,		url);

	RequestCommand(node, AIRABLE_URL);
}

void AirableManager::RequestPlay(int nServiceType, QMap<int, CJsonNode> nodeMap, int where)
{
	m_ServiceType = nServiceType;

	QString art = "";
	int nType = 0;

	CJsonNode nodeArr(JSON_ARRAY);
	QMap<int, CJsonNode>::iterator i;
	for (i = nodeMap.begin(); i!= nodeMap.end(); i++)
	{
//		LogDebug("key [%d] value [%d]", i.key(), i.value().ToCompactByteArray().data());
		art = i.value().GetString(KEY_ART);
		nType = i.value().GetInt(KEY_TYPE);

		i.value().Del(KEY_TYPE);
		i.value().Del(KEY_ART);
		i.value().Del(KEY_ACTS);

		nodeArr.AppendArray(i.value());
	}

	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_AIRABLE);
	node.Add	(KEY_CMD1,		VAL_PLAY);
	node.AddInt	(KEY_SVC_TYPE,	m_ServiceType);
	if (!art.isEmpty())
		node.Add(KEY_ART,		art);
	node.AddInt	(KEY_TYPE,		nType);
	node.AddInt	(KEY_WHERE,		where);

	node.Add	(KEY_TRACKS,	nodeArr);

	RequestCommand(node, AIRABLE_PLAY);
}

void AirableManager::RequestActionUrl(int nServiceType, QString url)
{
	m_ServiceType = nServiceType;

	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_AIRABLE);
	node.Add	(KEY_CMD1,		VAL_URL);
	node.Add(KEY_URL, url);
	node.AddInt(KEY_TYPE, m_ServiceType);

	RequestCommand(node, AIRABLE_ACTION_URL);

}

void AirableManager::RequestRandom()
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_REMOTE);
	node.Add(KEY_KEY, VAL_SHUFFLE);

	RequestCommand(node, AIRABLE_RANDOM);
}

void AirableManager::SlotRespInfo(QString json, int nCmdID)
{
	if (json.isEmpty())
	{
		return;
	}

	CJsonNode node;
	if (!node.SetContent(json))
	{
		emit SigRespError(STR_INVALID_JSON);
		return;
	}

//	LogDebug("node [%d] [%s]", nCmdID, node.ToTabedByteArray().data());

	QString strMsg;
	bool	bSuccess = false;
	if (!node.GetBool(VAL_SUCCESS, bSuccess) || !bSuccess)
	{
		if (!node.GetString(VAL_MSG, strMsg) || strMsg.isEmpty())
		{
			emit SigRespError(STR_UNKNOWN_ERROR);
			return;
		}

		emit SigRespError(strMsg.left(MSG_LIMIT_COUNT));
		return;
	}

	if (node.GetBool(VAL_LOGIN))
	{
		switch (nCmdID)
		{
		case AIRABLE_LOGIN:
		case AIRABLE_URL:
			node.AddInt(KEY_TYPE, m_ServiceType);
			emit SigRespLoginFail(node);
			break;
		case AIRABLE_MAX:
			emit SigRespError(STR_INVALID_ID);
			break;
		}

	}
	else
	{
		switch (nCmdID)
		{
		case AIRABLE_LOGIN:
			emit SigRespLoginSuccess(m_ServiceType, m_bSaveAuth);
			break;
		case AIRABLE_LOGOUT:
			emit SigRespLogout();
			break;
		case AIRABLE_AUTH:
			emit SigRespAuth(m_ServiceType);
			break;
		case AIRABLE_URL:
			ParseURL(node);
			break;
		case AIRABLE_ACTION_URL:
			emit SigRespError(node.GetString(VAL_DESC));
			break;
		case AIRABLE_RANDOM:
			break;
		case AIRABLE_MAX:
			emit SigRespError(STR_INVALID_ID);
			break;
		}
	}

}

void AirableManager::SlotRespCoverArt(QString fileName, int nIndex, int mode)
{
	emit SigCoverArtUpdate(fileName, nIndex, mode);
}

void AirableManager::ParseURL(CJsonNode node)
{
	bool bForm = node.GetBool(KEY_FORM);
	if (bForm)
	{
		emit SigRespForm(m_ServiceType, node);
		return;
	}

	CJsonNode result;
	if (!node.GetArray(VAL_RESULT, result) || result.ArraySize() <= 0)
	{
		emit SigRespError(STR_NO_RESULT);
		return;
	}

	QList<CJsonNode> nodeList;
	for (int i = 0; i < result.ArraySize(); i++)
	{
		nodeList.append(result.GetArrayAt(i));
	}

	QString title = node.GetString(KEY_NAME_CAP);

	emit SigRespURL(m_ServiceType, title, nodeList);
}
