#include "playmanager.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

PlayManager::PlayManager(QObject *parent)
{
	Q_UNUSED(parent)

	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespInfo(QString, int)));

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

void PlayManager::SlotRespInfo(QString json, int nCmdID)
{
	CJsonNode node;
	if (!node.SetContent(json))
	{
		LogCritical("Invalid JSON...[%s]", json.toUtf8().data());
		return;
	}

	LogDebug("node [%s]", node.ToTabedByteArray().data());

	QString strMsg;
	bool	bSuccess;
	if (!node.GetBool(KEY_SUCCESS, bSuccess))
	{
		LogCritical("Fail to response ...[%s]", json.toUtf8().data());
		return;
	}
	if (!node.GetString(KEY_MSG, strMsg) || strMsg.isEmpty())
	{
		LogCritical("Empty message ...[%s]", json.toUtf8().data());
		return;
	}

	CJsonNode result;
	if (!node.GetArray(KEY_RESULT, result))
	{
		LogCritical("Empty Result ...[%s]", json.toUtf8().data());
		return;
	}

	switch (nCmdID)
	{
	case PLAY_STATE:
		break;
	case PLAY_MAX:
		LogWarning("Invalid command ID");
		break;
	}
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
