#include "audiocdmanager.h"

AudioCDManager::AudioCDManager()
{
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespInfo(QString, int)));

}

void AudioCDManager::RequestTrackList(int index)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_AUDIO_CD);
	node.Add	(KEY_CMD1,		VAL_LIST);
	node.AddInt	(KEY_SOURCE,	index);

	RequestCommand(node, AUDIO_CD_TRACK_LIST);
}

void AudioCDManager::RequestTrackInfo(int index)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_AUDIO_CD);
	node.Add	(KEY_CMD1,		VAL_INFO);
	node.AddInt	(KEY_TRACK,	index);

	RequestCommand(node, AUDIO_CD_TRACK_INFO);
}

void AudioCDManager::RequestTrackPlay(int index)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_AUDIO_CD);
	node.Add	(KEY_CMD1,		VAL_PLAY);
	node.AddInt	(KEY_TRACK,	index);

	RequestCommand(node, AUDIO_CD_TRACK_PLAY);
}

void AudioCDManager::RequestEject()
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0,		VAL_AUDIO_CD);
	node.Add(KEY_CMD1,		VAL_EJECT);

	RequestCommand(node, AUDIO_CD_EJECT);
}

void AudioCDManager::RequestCDRipInfo(int index, QList<int> list)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,			VAL_AUDIO_CD);
	node.Add	(KEY_CMD1,			VAL_RIP_INFO);
	if (index >= 0)
		node.AddInt	(KEY_SOURCE,	index);
	if (list.count() > 0)
	{
		node.Add	(KEY_TRACKS,	list);
	}

	RequestCommand(node, AUDIO_CD_RIP_INFO);
}

void AudioCDManager::RequestCDRip(CJsonNode node)
{
	node.Add	(KEY_CMD0,			VAL_AUDIO_CD);
	node.Add	(KEY_CMD1,			VAL_RIP);

	RequestCommand(node, AUDIO_CD_RIP);
}



void AudioCDManager::SlotRespInfo(QString json, int cmdID)
{
	CJsonNode node;
	if (!node.SetContent(json))
	{
		emit SigRespError(STR_INVALID_JSON);
		return;
	}

	LogDebug("cmdID [%d] node [%s]", cmdID, node.ToTabedByteArray().data());

	QString message = node.GetString(VAL_MSG);
	bool success = node.GetBool(VAL_SUCCESS);
	if (!success)
	{
		if (message.isEmpty())
		{
			emit SigRespError(STR_UNKNOWN_ERROR);
			return;
		}

		emit SigRespError(message.left((MSG_LIMIT_COUNT)));
		return;
	}

	switch (cmdID)
	{
	case AUDIO_CD_TRACK_LIST:
	{
		CJsonNode result = node.GetArray(VAL_RESULT);
		if (result.ArraySize() <= 0)
		{
			emit SigRespError(message.left(MSG_LIMIT_COUNT));
			return;
		}

		ParseTrackList(result);
	}
		break;
	case AUDIO_CD_TRACK_INFO:
		emit SigRespTrackInfo(node);
		break;
	case AUDIO_CD_TRACK_PLAY:
	case AUDIO_CD_EJECT:
		break;
	case AUDIO_CD_RIP_INFO:
		emit SigRespCDRipInfo(node);
		break;
	case AUDIO_CD_RIP:
		LogInfo("rip ...");
		break;
	case AUDIO_CD_MAX:
		emit SigRespError(STR_INVALID_ID);
		break;
	}
}

void AudioCDManager::ParseTrackList(CJsonNode result)
{
	m_NodeList.clear();
	for (int i = 0; i < result.ArraySize(); i++)
	{
		m_NodeList.append(result.GetArrayAt(i));
//		LogDebug("node : [%s]", m_NodeList[i].ToCompactByteArray().data());
	}

	emit SigRespTrackList(m_NodeList);
}

