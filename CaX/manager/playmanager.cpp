#include "playmanager.h"

#include "util/caxconstants.h"
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
		emit SigRespError("invalid json");
		return;
	}

	LogDebug("node [%s]", node.ToTabedByteArray().data());

	QString strMsg;
	bool	bSuccess = false;
	if (!node.GetBool(KEY_SUCCESS, bSuccess) || !bSuccess)
	{
		if (!node.GetString(KEY_MSG, strMsg) || strMsg.isEmpty())
		{
			emit SigRespError("unknown error");
			return;
		}

		emit SigRespError(strMsg.left(MSG_LIMIT_COUNT));
		return;
	}

	CJsonNode result;
	if (!node.GetArray(KEY_RESULT, result))
	{
		emit SigRespError(strMsg.left(MSG_LIMIT_COUNT));
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
