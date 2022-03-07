#include "audiocdmanager.h"

AudioCDManager::AudioCDManager() :
	m_pSql(new SQLManager)
{
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespInfo(QString, int)));
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespCoverArt(QString, int, int)), this, SLOT(SlotRespCoverArt(QString, int, int)));
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespCoverArt(QString)), this, SLOT(SlotRespCoverArt(QString)));

}

AudioCDManager::~AudioCDManager()
{
	if (m_pSql)
	{
		delete m_pSql;
		m_pSql = nullptr;
	}
}

void AudioCDManager::RequestTrackList(int index)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_AUDIO_CD);
	node.Add	(KEY_CMD1,		VAL_LIST);
	node.AddInt	(KEY_SOURCE,	index);

	RequestCommand(node, AUDIO_CD_TRACK_LIST);
}

void AudioCDManager::RequestTrackInfo(int id)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_AUDIO_CD);
	node.Add	(KEY_CMD1,		VAL_INFO);
	node.AddInt	(KEY_TRACK,		id);

	RequestCommand(node, AUDIO_CD_TRACK_INFO);
}

void AudioCDManager::RequestTrackPlay(int id)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_AUDIO_CD);
	node.Add	(KEY_CMD1,		VAL_PLAY);
	node.AddInt	(KEY_TRACK,		id);

	RequestCommand(node, AUDIO_CD_TRACK_PLAY);
}

void AudioCDManager::RequestEject()
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0,		VAL_AUDIO_CD);
	node.Add(KEY_CMD1,		VAL_EJECT);

	RequestCommand(node, AUDIO_CD_EJECT);
}

void AudioCDManager::RequestCDRipInfo(int index, QMap<int, bool> idMap)
{
	CJsonNode node(JSON_OBJECT);

	if (idMap.count() > 0)
	{
		CJsonNode idArr(JSON_ARRAY);
		QMap<int, bool>::iterator i;
		for (i = idMap.begin(); i!= idMap.end(); i++)
		{
//			LogDebug("key [%d] value [%d]", i.key(), i.value());
			idArr.AppendArray((int64_t)i.key());
		}
		node.Add(KEY_TRACKS, idArr);

	}

	node.Add	(KEY_CMD0,			VAL_AUDIO_CD);
	node.Add	(KEY_CMD1,			VAL_RIP_INFO);
	if (index >= 0)
		node.AddInt	(KEY_SOURCE,	index);

	RequestCommand(node, AUDIO_CD_RIP_INFO);
}

void AudioCDManager::RequestCDRip(CJsonNode node)
{
	node.Add	(KEY_CMD0,			VAL_AUDIO_CD);
	node.Add	(KEY_CMD1,			VAL_RIP);

	RequestCommand(node, AUDIO_CD_RIP);
}

void AudioCDManager::RequestCategoryList(int nCategory)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, m_pSql->GetQueryCategoryList(nCategory));

	RequestCommand(node, AUDIO_CD_CATEGORY_LIST);
}

void AudioCDManager::RequestRandom()
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_REMOTE);
	node.Add(KEY_KEY, VAL_SHUFFLE);

	RequestCommand(node, AUDIO_CD_RANDOM);
}

void AudioCDManager::SlotRespInfo(QString json, int cmdID)
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

//	LogDebug("cmdID [%d] node [%s]", cmdID, node.ToTabedByteArray().data());

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

	if (cmdID == AUDIO_CD_RANDOM)
	{
		return;
	}

	switch (cmdID)
	{
	case AUDIO_CD_TRACK_LIST:
		ParseTrackList(node);
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
	case AUDIO_CD_CATEGORY_LIST:
		ParseCategoryList(node);
		break;
	case AUDIO_CD_MAX:
		emit SigRespError(STR_INVALID_ID);
		break;
	}
}

void AudioCDManager::SlotRespCoverArt(QString filename, int nIndex, int mode)
{
	emit SigCoverArtUpdate(filename, nIndex, mode);
}

void AudioCDManager::SlotRespCoverArt(QString filename)
{
	emit SigCoverArtUpdate(filename);
}

void AudioCDManager::ParseTrackList(CJsonNode node)
{
	emit SigRespTrackList(node);
}

void AudioCDManager::ParseCategoryList(CJsonNode node)
{
	emit SigRespCategoryList(node);
}

