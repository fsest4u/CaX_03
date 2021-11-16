#include "browsermanager.h"

BrowserManager::BrowserManager(QObject *parent) :
	m_OptPlaySubDir(true),
	m_OptOverwrite(true)
{
	Q_UNUSED(parent)
//	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespInfo(QString, int)));
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int, int)), this, SLOT(SlotRespInfo(QString, int, int)));
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespCoverArt(QString, int, int)), this, SLOT(SlotRespCoverArt(QString, int, int)));

}

BrowserManager::~BrowserManager()
{

}

void BrowserManager::RequestRoot(bool dirOnly)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_BROWSER);
	node.Add	(KEY_CMD1,		VAL_ROOT);
	node.Add	(KEY_DIR_ONLY,	dirOnly);
	node.Add	(KEY_IMAGE,		false);

	RequestCommand(node, BROWSER_ROOT);
}

void BrowserManager::RequestFolder(QString strPath, bool dirOnly, bool imageOnly)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_BROWSER);
	node.Add	(KEY_CMD1,		VAL_FOLDER);
	// path : folder path, cd, net, upnp ...
	node.Add	(KEY_PATH,		strPath);
	node.Add	(KEY_DIR_ONLY,	dirOnly);
	node.Add	(KEY_IMAGE,		imageOnly);

	RequestCommand(node, BROWSER_FOLDER);
}

void BrowserManager::RequestInfoBot(QString strPath, int nIndex)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_BROWSER);
	node.Add	(KEY_CMD1,		VAL_INFO);
	node.Add	(KEY_CMD2,		VAL_BOT);
	node.Add	(KEY_PATH,		strPath);

	RequestCommand(node, BROWSER_INFO_BOT, nIndex);

}

void BrowserManager::RequestCreate(QString path)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_BROWSER);
	node.Add	(KEY_CMD1,		VAL_CREATE_FOLDER);
	node.Add	(KEY_PATH,		path);

	RequestCommand(node, BROWSER_CREATE);
}

void BrowserManager::RequestRename()
{
	CJsonNode node(JSON_OBJECT);

	RequestCommand(node, BROWSER_RENAME);
}

void BrowserManager::RequestDelete(QString root, QStringList paths, int eventID)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_BROWSER);
	node.Add	(KEY_CMD1,		VAL_COPY);
	node.Add(KEY_DELETE, true);
	node.AddInt(KEY_EVENT_ID, eventID);
	node.Add(KEY_SRC_PATH, root);
	node.Add(KEY_SRC_PATHS, paths);

	RequestCommand(node, BROWSER_DELETE);
}

void BrowserManager::RequestCopy(QString srcRoot, QStringList srcPaths, QString dstRoot, bool move, int eventID)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_BROWSER);
	node.Add	(KEY_CMD1,		VAL_COPY);
	if (move)
	{
		node.Add(KEY_MOVE, true);
	}
	node.AddInt(KEY_EVENT_ID, eventID);
	node.Add(KEY_OVERWRITE, m_OptOverwrite);
	node.Add(KEY_SRC_PATH, srcRoot);
	node.Add(KEY_SRC_PATHS, srcPaths);
	node.Add(KEY_DST_PATH, dstRoot);

	RequestCommand(node, BROWSER_COPY);
}

void BrowserManager::RequestSetArt(QString root, QStringList files, QString image, QString thumb, int eventID)
{
	CJsonNode coverArt(JSON_OBJECT);
	coverArt.Add(KEY_IMAGE_URL, image);
	coverArt.Add(KEY_THUMB_URL, thumb);

	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_BROWSER);
	node.Add	(KEY_CMD1,		VAL_SET_ART);
	node.AddInt(KEY_EVENT_ID, eventID);
	node.Add(KEY_ROOT, root);
	node.Add(KEY_SONG_FILES, files);
	node.Add(KEY_COVER_ART, coverArt);

	RequestCommand(node, BROWSER_SET_ART);

}

void BrowserManager::RequestTrackPlay(QString root, QStringList dirs, QStringList files, int where)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_BROWSER);
	node.Add	(KEY_CMD1,		VAL_PLAY);
	node.AddInt	(KEY_WHERE,		where);
	node.Add(KEY_SUB_DIR, m_OptPlaySubDir);
	node.Add(KEY_ROOT, root);
	if (dirs.count() > 0)
	{
		node.Add(KEY_DIRS, dirs);
	}
	if (files.count() > 0)
	{
		node.Add(KEY_FILES, files);
	}

	RequestCommand(node, BROWSER_TRACK_PLAY);
}

void BrowserManager::RequestPlaylistPlay(QString path, QList<int> indexes)
{

}

void BrowserManager::RequestReplayGain(QString root, QStringList paths, QString gain, int eventID)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_BROWSER);
	node.Add	(KEY_CMD1,		VAL_REPLAY_GAIN);
	node.Add	(KEY_CMD2,		gain);
	node.AddInt	(KEY_EVENT_ID,	eventID);
	node.Add(KEY_ROOT, root);
	node.Add(KEY_PATHS, paths);

	RequestCommand(node, BROWSER_REPLAYGAIN);
}

void BrowserManager::RequestConvertFormat(QString root, QStringList paths, QString format, int eventID)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_BROWSER);
	node.Add	(KEY_CMD1,		VAL_CONVERT);
	node.Add	(KEY_FORMAT,	format);
	node.AddInt	(KEY_EVENT_ID,	eventID);
	node.Add(KEY_ROOT, root);
	node.Add(KEY_PATHS, paths);

	RequestCommand(node, BROWSER_CONVERT_FORMAT);
}

void BrowserManager::RequestRandom()
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_REMOTE);
	node.Add(KEY_KEY, VAL_SHUFFLE);

	RequestCommand(node, BROWSER_RANDOM);
}

void BrowserManager::SlotRespInfo(QString json, int nCmdID, int nIndex)
{
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

	switch (nCmdID)
	{
	case BROWSER_ROOT:
	case BROWSER_FOLDER:
		ParseFolder(node);
		break;
	case BROWSER_INFO_BOT:
		ParseInfoBot(node, nIndex);
		break;
	case BROWSER_TRACK_PLAY:
		break;
	case BROWSER_PLAYLIST_PLAY:
		break;
	case BROWSER_CREATE:
	case BROWSER_RENAME:
	case BROWSER_DELETE:
	case BROWSER_COPY:
	case BROWSER_MOVE:
//	case BROWSER_INFO:
//	case BROWSER_SET_ART:
//	case BROWSER_CONVERT:
//	case BROWSER_REPLAYGAIN_SET:
//	case BROWSER_REPLAYGAIN_CLEAR:
//	case BROWSER_IMPORT:
//	case BROWSER_UPNP_FOLDER:
//	case BROWSER_UPNP_PLAY:
	case BROWSER_MAX:
		emit SigRespError(STR_INVALID_ID);
		break;
	}
}

void BrowserManager::SlotRespCoverArt(QString fileName, int nIndex, int mode)
{
	emit SigCoverArtUpdate(fileName, nIndex, mode);
}

void BrowserManager::ParseFolder(CJsonNode node)
{
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

	emit SigRespList(nodeList);
}

void BrowserManager::ParseInfoBot(CJsonNode node, int nIndex)
{
	emit SigInfoBotUpdate(node, nIndex);
}

bool BrowserManager::GetOptOverwrite() const
{
	return m_OptOverwrite;
}

void BrowserManager::SetOptOverwrite(bool OptOverwrite)
{
	m_OptOverwrite = OptOverwrite;
}

bool BrowserManager::GetOptPlaySubDir() const
{
	return m_OptPlaySubDir;
}

void BrowserManager::SetOptPlaySubDir(bool OptPlaySubDir)
{
	m_OptPlaySubDir = OptPlaySubDir;
}

