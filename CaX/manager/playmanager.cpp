#include <QImage>

#include "playmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"

PlayManager::PlayManager(QObject *parent)
{
	Q_UNUSED(parent)

	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespInfo(QString, int)));
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespCoverArt(QString)), this, SLOT(SlotRespCoverArt(QString)));

}

PlayManager::~PlayManager()
{

}

void PlayManager::RequestPlayState(int mode)
{
	// 플레이, 일시정지일 때만 사용가능
	QString strState = GetPlayMode(mode);

	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_PLAY);
	node.Add	(KEY_CMD1,		VAL_PLAY);
	node.Add	(KEY_PLAY,		strState);
	RequestCommand(node, PLAY_STATE);
}

void PlayManager::RequestSeek(int msec)
{
//	int pos = msec / 1000;
//	CJsonNode node(JSON_OBJECT);
//	node.Add	(KEY_CMD0,		VAL_PLAY);
//	node.Add	(KEY_CMD1,		VAL_SET_POSITION);
//	node.AddInt	(KEY_POSITION,	pos);
//	RequestCommand(node, PLAY_SEEK);
}

void PlayManager::RequestVolume(int value)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_PLAY);
	node.Add	(KEY_CMD1,		VAL_SET_VOLUME);
	node.AddInt	(KEY_VOLUME_CAP,	value);
	RequestCommand(node, PLAY_VOLUME);
}

void PlayManager::RequestRepeatMode()
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_REMOTE);
	node.Add	(KEY_KEY,		VAL_SHUFFLE);
	RequestCommand(node, PLAY_SHUFFLE);
}

void PlayManager::RequestMute()
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_REMOTE);
	node.Add	(KEY_KEY,		VAL_MUTE);
	RequestCommand(node, PLAY_MUTE);
}

void PlayManager::RequestSongInfo(int nID)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_MUSIC_DB);
	node.Add	(KEY_CMD1,		VAL_INFO);
	node.Add	(KEY_CMD2,		VAL_SONG);
	node.AddInt	(KEY_ID_UPPER,	nID);

	RequestCommand(node, PLAY_SONG_INFO);

}

void PlayManager::RequestQueueList(uint timestamp)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_PLAY);
	node.Add	(KEY_CMD1,		VAL_LIST);
	node.AddInt	(KEY_TIME_STAMP,timestamp);
	node.AddInt	(KEY_INDEX,		0);
	node.AddInt	(KEY_PAGE_CNT,	100);

	RequestCommand(node, PLAY_QUEUE_LIST);
}

void PlayManager::RequestQueuePlay(int nID)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_PLAY);
	node.Add	(KEY_CMD1,		VAL_INDEX);
	node.AddInt	(KEY_INDEX,		nID);

	RequestCommand(node, PLAY_QUEUE_PLAY);
}

void PlayManager::SlotRespInfo(QString json, int nCmdID)
{
	CJsonNode node;
	if (!node.SetContent(json))
	{
		emit SigRespError(STR_INVALID_JSON);
		return;
	}

//	LogDebug("node [%s]", node.ToTabedByteArray().data());

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

	if (nCmdID == PLAY_STATE
			|| nCmdID == PLAY_VOLUME
			|| nCmdID == PLAY_SEEK
			|| nCmdID == PLAY_SHUFFLE
			|| nCmdID == PLAY_MUTE
			|| nCmdID == PLAY_QUEUE_PLAY
			)
	{
		return;
	}

//	CJsonNode result;
//	if (!node.GetArray(VAL_RESULT, result))
//	{
//		emit SigRespError(strMsg.left(MSG_LIMIT_COUNT));
//		return;
//	}

	switch (nCmdID)
	{
	case PLAY_SONG_INFO:
		ParseTrackInfo(node);
		break;
	case PLAY_QUEUE_LIST:
		ParseQueueList(node);
		break;
	case PLAY_MAX:
		emit SigRespError(STR_INVALID_ID);
		break;
	}
}

void PlayManager::SlotRespCoverArt(QString fileName)
{
	emit SigCoverArtUpdate(fileName);
}

void PlayManager::ParseTrackInfo(CJsonNode node)
{
	emit SigTrackInfo(node);
}

void PlayManager::ParseQueueList(CJsonNode node)
{
	CJsonNode result;
	if (!node.GetArray(VAL_RESULT, result) || result.ArraySize() <= 0)
	{
		emit SigRespError(STR_NO_RESULT);
		return;
	}
	emit SigQueueList(result);
}

QString PlayManager::GetPlayMode(int mode)
{
	QString strMode;

	switch (mode)
	{
	case PLAY_MODE_PLAY:
		strMode = "Play";
		break;
	case PLAY_MODE_PAUSE:
		strMode = "Pause";
		break;
	case PLAY_MODE_NEXT:
		strMode = "Next";
		break;
	case PLAY_MODE_PREV:
		strMode = "Prev";
		break;
	case PLAY_MODE_STOP:
		strMode = "Stop";
		break;
	case PLAY_MODE_MAX:
	default:
		strMode = "";
		break;
	}

	return strMode;
}
