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
//		else if (strCmd0.contains("Task"))
//		{
////			emit SigObserveInfo();
//		}
//		else if (strCmd0.contains("Swap"))
//		{
////			emit SigObserveInfo();
//		}
		else if (strCmd0.contains("Progress"))
		{
			// setup - music db scan status
			// todo-dylee, 4.2 "MusicDB" -> "Info" -> "SetAlbum"

			// audio cd - cd rip
			// browser folder copy / move / delete
			// browser convert / set replay gain / clear replay gain

			emit SigEventProgress(node);

		}
		else if (strCmd0.contains(KEY_NOW_PLAY))
		{
			emit SigEventNowPlay(node);
		}
		else if (strCmd0.contains(KEY_FM_RADIO))
		{
			if (!ParseFmRadioEvent(node))
			{
				continue;
			}
		}
		else if (strCmd0.contains(KEY_DAB))
		{
			if (!ParseDabRadioEvent(node))
			{
				continue;
			}
		}
		else if (strCmd0.contains(KEY_GROUP_PLAY))
		{
			if (!ParseGroupPlayEvent(node))
			{
				continue;
			}
		}
		else
		{
			LogWarning("====================================");
			LogWarning("This command is not supported [%s]", strCmd0.toUtf8().data());
			LogWarning("====================================");
		}
	}
}

void ObserverManager::SlotDisconnectObserver()
{
	emit SigDisconnectObserver();
}

void ObserverManager::ConnectSigToSlot()
{
	connect(m_pObsClient, SIGNAL(SigConnect(QStringList)), this, SLOT(SlotRespObserverInfo(QStringList)));
	connect(m_pObsClient, SIGNAL(SigDisconnect()), this, SLOT(SlotDisconnectObserver()));
}

bool ObserverManager::ParseFmRadioEvent(CJsonNode node)
{
	QString strCmd1 = node.GetString(KEY_CMD1);

	if (strCmd1.contains("SeekStop"))
	{
		SigEventFmSeekStop(node);
	}
	else if (strCmd1.contains("Seek"))
	{
		SigEventFmSeek(node);
	}
	else if (strCmd1.contains("Add"))
	{
		SigEventFmAdd(node);
	}
	else if (strCmd1.contains("Del"))
	{
		SigEventFmDel(node);
	}
	else if (strCmd1.contains("Edit"))
	{
		SigEventFmSet(node);
	}
	else if (strCmd1.contains("Progress"))
	{
		SigEventFmSeeking(node);
	}
	else
	{
		LogWarning("====================================");
		LogWarning("This command is not supported [%s]", strCmd1.toUtf8().data());
		LogWarning("====================================");
	}

	return true;
}

bool ObserverManager::ParseDabRadioEvent(CJsonNode node)
{
	QString strCmd1 = node.GetString(KEY_CMD1);

	if (strCmd1.contains("SeekStop"))
	{
		SigEventDabSeekStop(node);
	}
	if (strCmd1.contains("Add") || strCmd1.contains("Seek"))
	{
		// the same behavior with seek of fm radio
		SigEventDabSeek(node);
	}
	else if (strCmd1.contains("Del"))
	{
		SigEventDabDel(node);
	}
	else if (strCmd1.contains("Rename"))
	{
		// the same behavior with edit of fm radio
		SigEventDabSet(node);
	}
//	else if (strCmd1.contains("Progress"))
//	{
//		SigDabEventSeeking(rootNode);
//	}
	else
	{
		LogWarning("====================================");
		LogWarning("This command is not supported [%s]", strCmd1.toUtf8().data());
		LogWarning("====================================");
	}

	return true;
}

bool ObserverManager::ParseGroupPlayEvent(CJsonNode rootNode)
{
	if (rootNode.GetBool(KEY_DEV_LIST_UPDATE))
	{
		emit SigEventGroupPlayUpdate();
	}
	else
	{
		LogWarning("====================================");
		LogWarning("This command is not supported [%s]", KEY_DEV_LIST_UPDATE);
		LogWarning("====================================");
	}

	return true;
}

