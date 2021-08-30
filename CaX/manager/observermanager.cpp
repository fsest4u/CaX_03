#include "observermanager.h"

#include "network/observerclient.h"

#include "util/CJsonNode.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"

ObserverManager::ObserverManager(QObject *parent)
	: QObject(parent)
	, m_pObsClient(new ObserverClient)
{
	ConnectSigToSlot();
}

ObserverManager::~ObserverManager()
{
	if (m_pObsClient)
	{
		delete m_pObsClient;
		m_pObsClient = nullptr;
	}
}

void ObserverManager::ConnectSigToSlot()
{
	connect(m_pObsClient, SIGNAL(SigConnect(QStringList)), this, SLOT(SlotRespObserverInfo(QStringList)));
	connect(m_pObsClient, SIGNAL(SigDisconnect()), this, SLOT(SlotDisconnectObserver()));
}



///////////////////////////////////////////////////////////
// Slots
///////////////////////////////////////////////////////////

void ObserverManager::RequestObserverInfo(QString strAddr)
{
	CJsonNode rootNode(JSON_OBJECT);
	rootNode.Add(KEY_CMD0, VAL_OBSERVE);

	QByteArray jsonData = rootNode.ToCompactByteArray();
	m_pObsClient->Connect(strAddr, jsonData);

}

void ObserverManager::RequestDisconnectObserver()
{
	m_pObsClient->Disconnect();
}

void ObserverManager::SlotRespObserverInfo(QStringList jsonValueList)
{
	CJsonNode node;

//	LogDebug("@@@@ JSON List Count [%d]", jsonValueList.count());

	foreach(QString jsonValue, jsonValueList)
	{
		if (!node.SetContent(jsonValue))
		{
			LogCritical("invalid json ..............");
			continue;
		}

		QString strCmd0;
		if (!node.GetString(KEY_CMD0, strCmd0) || strCmd0.isEmpty()) { continue; }

		LogDebug("*****************************************");
		LogDebug("command [%s]", strCmd0.toUtf8().data());
		LogDebug("*****************************************");
		LogDebug("info    [%s]", node.ToTabedByteArray().data());
		LogDebug("*****************************************");

		if (strCmd0.contains("Dev"))
		{
			emit SigRespObserverInfo(node);
		}
		// todo-dylee
//		else if (strCmd0.contains("NowPlaying"))
//		{
//			emit SigNowPlaying(node);
//		}
//		else if (strCmd0.contains("Progress"))
//		{
//			// todo-dylee, 4.2 "MusicDB" -> "Info" -> "SetAlbum"

//			// todo-dylee, cd ripping success & fail

//			// todo-dylee, browser folder copy / move / delete

//			// todo-dylee, browser convert / set replay gain / clear replay gain


//			// setup - music db scan status
//			emit SigProgress(node);

//		}
//		else if (strCmd0.contains("Task"))
//		{


////			emit SigObserveInfo();

//		}
//		else if (strCmd0.contains("Swap"))
//		{


////			emit SigObserveInfo();

//		}
//		else if (strCmd0.contains(KEY_FM_RADIO))
//		{
//			if (!ParseFmRadioEvent(node))	{	continue;	}
//		}
//		else if (strCmd0.contains("Dab"))
//		{
//			if (!ParseDabEvent(node))		{	continue;	}
//		}
//		else if (strCmd0.contains(KEY_GROUP_PLAY))
//		{
//			if (!ParseGroupPlayEvent(node))	{	continue;	}
//		}
//		else
//		{
//			LoggingError("====================================");
//			LoggingError("This command is not supported [%s]", strCmd0.toUtf8().data());
//			LoggingError("====================================");
//		}
	}
}

void ObserverManager::SlotDisconnectObserver()
{
	emit SigDisconnectObserver();
}
