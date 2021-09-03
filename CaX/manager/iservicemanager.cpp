#include "iservicemanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"


IServiceManager::IServiceManager(QObject *parent)
{
	Q_UNUSED(parent)

	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespInfo(QString, int)));
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespCoverArt(QString, int, int)), this, SLOT(SlotRespCoverArt(QString, int, int)));

	m_ServiceType = -1;
	m_bSaveAuth = false;

}

IServiceManager::~IServiceManager()
{

}



void IServiceManager::RequestAirableLogin(int nServiceType, QString userID, QString password, bool bSaveAuth)
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

	RequestCommand(node, ISERVICE_LOGIN);
}

void IServiceManager::RequestLogout(int nServiceType, QString url)
{
	m_ServiceType = nServiceType;

	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_AIRABLE);
	node.Add	(KEY_CMD1,		VAL_URL);
	node.AddInt	(KEY_TYPE,		m_ServiceType);
	node.Add	(KEY_URL,		url);

	RequestCommand(node, ISERVICE_LOGOUT);
}

void IServiceManager::RequestAirableAuth(int nServiceType)
{
	m_ServiceType = nServiceType;

	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_AIRABLE);
	node.Add	(KEY_CMD1,		VAL_AUTH);
	node.AddInt	(KEY_TYPE,		m_ServiceType);

	RequestCommand(node, ISERVICE_AUTH);
}

void IServiceManager::RequestAirableURL(int nServiceType, QString url)
{
	m_ServiceType = nServiceType;

	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_AIRABLE);
	node.Add	(KEY_CMD1,		VAL_URL);
	node.AddInt	(KEY_TYPE,		m_ServiceType);
	if (!url.isEmpty())
		node.Add(KEY_URL,		url);

	RequestCommand(node, ISERVICE_URL);
}

void IServiceManager::RequestPlay(int nServiceType, CJsonNode srcNode)
{
	m_ServiceType = nServiceType;

	QString art = srcNode.GetString(KEY_ART);
	int nType = srcNode.GetInt(KEY_TYPE);

	srcNode.Del(KEY_TYPE);
	srcNode.Del(KEY_ICON);
	srcNode.Del(KEY_ACTS);

	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_AIRABLE);
	node.Add	(KEY_CMD1,		VAL_PLAY);
	node.AddInt	(KEY_SVC_TYPE,	m_ServiceType);
	if (!art.isEmpty())
		node.Add(KEY_ART,		art);
	node.AddInt	(KEY_TYPE,		nType);
	node.AddInt	(KEY_WHERE,		3);	// play new

	CJsonNode trackNode(JSON_ARRAY);
	trackNode.AppendArray(srcNode);
	node.Add	(KEY_TRACKS,	trackNode);

	RequestCommand(node, ISERVICE_PLAY);
}

void IServiceManager::SlotRespInfo(QString json, int nCmdID)
{
	CJsonNode node;
	if (!node.SetContent(json))
	{
		emit SigRespError("invalid json");
		return;
	}

	LogDebug("node [%d] [%s]", nCmdID, node.ToTabedByteArray().data());

	QString strMsg;
	bool	bSuccess = false;
	if (!node.GetBool(VAL_SUCCESS, bSuccess) || !bSuccess)
	{
		if (!node.GetString(VAL_MSG, strMsg) || strMsg.isEmpty())
		{
			emit SigRespError("unknown error");
			return;
		}

		emit SigRespError(strMsg.left(MSG_LIMIT_COUNT));
		return;
	}

	if (node.GetBool(VAL_LOGIN))
	{
		switch (nCmdID)
		{
		case ISERVICE_LOGIN:
		case ISERVICE_URL:
			node.AddInt(KEY_TYPE, m_ServiceType);
			emit SigRespAirableLoginFail(node);
			break;
		case ISERVICE_MAX:
			LogWarning("Invalid command ID");
			break;
		}

	}
	else
	{
		switch (nCmdID)
		{
		case ISERVICE_LOGIN:
			emit SigRespAirableLoginSuccess(m_ServiceType, m_bSaveAuth);
			break;
		case ISERVICE_LOGOUT:
			emit SigRespAirableLogout();
			break;
		case ISERVICE_AUTH:
			emit SigRespAirableAuth(m_ServiceType);
			break;
		case ISERVICE_URL:
			ParseAirableURL(node);
			break;
		case ISERVICE_MAX:
			LogWarning("Invalid command ID");
			break;
		}
	}

}

void IServiceManager::SlotRespCoverArt(QString fileName, int nIndex, int mode)
{
	emit SigCoverArtUpdate(fileName, nIndex, mode);
}

//void IServiceManager::ParseQobuzLogin(CJsonNode node)
//{
//	if (node.GetBool(VAL_LOGIN))
//	{
//		// fail to login
//		emit SigRespQobuzLoginFail(node);
//	}
//	else
//	{
//		// success to login
//		emit SigRespQobuzLoginSuccess();
//	}
//}

void IServiceManager::ParseAirableURL(CJsonNode node)
{
	CJsonNode result;
	if (!node.GetArray(VAL_RESULT, result))
	{
		emit SigRespError("there is no result");
		return;
	}

	QList<CJsonNode> nodeList;
	for (int i = 0; i < result.ArraySize(); i++)
	{
		nodeList.append(result.GetArrayAt(i));
	}

	emit SigRespAirableURL(m_ServiceType, nodeList);
}
