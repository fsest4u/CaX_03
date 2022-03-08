#include "browsermanager.h"

BrowserManager::BrowserManager(QObject *parent) :
	m_pSql(new SQLManager),
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
	if (m_pSql)
	{
		delete m_pSql;
		m_pSql = nullptr;
	}
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

void BrowserManager::RequestRootSetup(QString ext)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_BROWSER);
	node.Add	(KEY_CMD1,		VAL_ROOT);
	node.Add	(KEY_EXT,		ext);

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

void BrowserManager::RequestFolderSetup(QString strPath, QString ext)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_BROWSER);
	node.Add	(KEY_CMD1,		VAL_FOLDER);
	// path : folder path, cd, net, upnp ...
	node.Add	(KEY_PATH,		strPath);
	node.Add	(KEY_EXT,		ext);

	RequestCommand(node, BROWSER_FOLDER);
}

void BrowserManager::RequestFolderUpnp(CJsonNode node)
{
	node.Add	(KEY_CMD0,		VAL_BROWSER);
	node.Add	(KEY_CMD1,		VAL_FOLDER);

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

void BrowserManager::RequestRename(QString src, QString dst)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_BROWSER);
	node.Add	(KEY_CMD1,		VAL_RENAME);
	node.Add(KEY_SRC, src);
	node.Add(KEY_DST, dst);

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

void BrowserManager::RequestInfoTag(QString path)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_BROWSER);
	node.Add	(KEY_CMD1,		VAL_INFO);
	node.Add	(KEY_CMD2,		VAL_SONG);
	node.Add(KEY_PATH, path);

	RequestCommand(node, BROWSER_INFO_TAG);

}

void BrowserManager::RequestSetTag(CJsonNode node, QString path)
{
//	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_BROWSER);
	node.Add	(KEY_CMD1,		VAL_INFO);
	node.Add	(KEY_CMD2,		VAL_SET_TAG);
	node.Add(KEY_PATH, path);

	RequestCommand(node, BROWSER_SET_TAG);
}

void BrowserManager::RequestCategoryList(int nCategory)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, m_pSql->GetQueryCategoryList(nCategory));

	RequestCommand(node, BROWSER_CATEGORY_LIST);
}

void BrowserManager::RequestScanDB(QString path, int eventID)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_MUSIC_DB);
	node.Add	(KEY_CMD1,		VAL_SCAN_DB);
	node.AddInt(KEY_EVENT_ID,	eventID);
	node.Add(KEY_PATH, path);

	RequestCommand(node, BROWSER_SCAN_DB);
}

void BrowserManager::RequestRemoveDB(QString path, int eventID)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_MUSIC_DB);
	node.Add	(KEY_CMD1,		VAL_REMOVE_DB);
	node.AddInt	(KEY_EVENT_ID,	eventID);
	node.Add(KEY_PATH, path);

	RequestCommand(node, BROWSER_REMOVE_DB);
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

void BrowserManager::RequestPlaylistPlay(QString path, QStringList indexes, int where)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_BROWSER);
	node.Add	(KEY_CMD1,		VAL_PLS);
	node.Add	(KEY_CMD2,		VAL_PLAY);
	node.AddInt	(KEY_WHERE,		where);
	node.Add(KEY_PATH, path);
	if (indexes.count() > 0)
	{
		node.Add(KEY_INDEXES, indexes);
	}

	RequestCommand(node, BROWSER_PLAYLIST_PLAY);
}

void BrowserManager::RequestUpnpPlay(CJsonNode node, int eventID, int where)
{
	node.Add	(KEY_CMD0,		VAL_BROWSER);
	node.Add	(KEY_CMD1,		VAL_UPNP);
	node.Add	(KEY_CMD2,		VAL_PLAY);
	node.AddInt	(KEY_EVENT_ID,	eventID);
	node.AddInt	(KEY_WHERE,		where);

	RequestCommand(node, BROWSER_UPNP_PLAY);
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

void BrowserManager::RequestSMBWorkGroup(int eventID)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_SMB);
	node.Add(KEY_CMD1, VAL_WORK_GROUP);
	node.AddInt	(KEY_EVENT_ID,	eventID);

	RequestCommand(node, BROWSER_SMB_WORK_GROUP);
}

void BrowserManager::RequestSMBServer(int eventID, QString title)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_SMB);
	node.Add(KEY_CMD1, VAL_SERVER);
	node.Add(KEY_WORK_GROUP, title);
	node.AddInt	(KEY_EVENT_ID,	eventID);

	RequestCommand(node, BROWSER_SMB_SERVER);
}

void BrowserManager::RequestSMBShare(int eventID, CJsonNode node)
{
	node.Add(KEY_CMD0, VAL_SMB);
	node.Add(KEY_CMD1, VAL_SHARE);
	node.AddInt	(KEY_EVENT_ID,	eventID);

	RequestCommand(node, BROWSER_SMB_SHARE);
}

void BrowserManager::RequestSMBSet(int eventID, CJsonNode node)
{
	node.Add(KEY_CMD0, VAL_SMB);
	node.Add(KEY_CMD1, VAL_SET);
	node.AddInt	(KEY_EVENT_ID,	eventID);

	RequestCommand(node, BROWSER_SMB_SET);
}

void BrowserManager::RequestSMBInfo(int eventID, QString title)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_SMB);
	node.Add(KEY_CMD1, VAL_INFO);
	node.Add(KEY_NAME_CAP, title);
	node.AddInt	(KEY_EVENT_ID,	eventID);

	RequestCommand(node, BROWSER_SMB_INFO);
}

void BrowserManager::RequestSMBDelete(int eventID, QString title)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_SMB);
	node.Add(KEY_CMD1, VAL_DEL);
	node.Add(KEY_NAME_CAP, title);
	node.AddInt	(KEY_EVENT_ID,	eventID);

	RequestCommand(node, BROWSER_SMB_DELETE);
}

void BrowserManager::SlotRespInfo(QString json, int nCmdID, int nIndex)
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

	QString message = node.GetString(VAL_MSG);
	bool	bSuccess = false;
	if (!node.GetBool(VAL_SUCCESS, bSuccess) || !bSuccess)
	{
		if (message.isEmpty())
		{
			emit SigRespError(STR_UNKNOWN_ERROR);
			return;
		}

		emit SigRespError(message.left(MSG_LIMIT_COUNT));
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
	case BROWSER_CONVERT_FORMAT:
	case BROWSER_CREATE:
	case BROWSER_RENAME:
	case BROWSER_DELETE:
	case BROWSER_COPY:
	case BROWSER_MOVE:
	case BROWSER_SCAN_DB:
	case BROWSER_REMOVE_DB:
	case BROWSER_SMB_SET:
	case BROWSER_SMB_DELETE:
		emit SigListUpdate();
		break;
	case BROWSER_INFO_TAG:
		ParseInfoTag(node);
		break;
	case BROWSER_CATEGORY_LIST:
		ParseCategoryList(node);
		break;
	case BROWSER_SMB_WORK_GROUP:
		ParseSMBWorkGroup(node);
		break;
	case BROWSER_SMB_SERVER:
		ParseSMBServer(node);
		break;
	case BROWSER_SMB_SHARE:
		ParseSMBShare(node);
		break;
	case BROWSER_SMB_INFO:
		ParseSMBInfo(node);
		break;
	case BROWSER_TRACK_PLAY:
	case BROWSER_PLAYLIST_PLAY:
	case BROWSER_REPLAYGAIN:
	case BROWSER_SET_ART:
	case BROWSER_SET_TAG:
		break;
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

void BrowserManager::ParseInfoTag(CJsonNode node)
{
	emit SigInfoTagUpdate(node);
}

void BrowserManager::ParseCategoryList(CJsonNode node)
{
	m_NodeList.clear();

	CJsonNode result = node.GetArray(VAL_RESULT);
	if (result.ArraySize() <= 0)
	{
//		emit SigRespError(message.left(MSG_LIMIT_COUNT));
		return;
	}

	for (int i = 0; i < result.ArraySize(); i++)
	{
		m_NodeList.append(result.GetArrayAt(i));
//		LogDebug("node : [%s]", m_NodeList[i].ToCompactByteArray().data());
	}

	emit SigRespCategoryList(m_NodeList);
}

void BrowserManager::ParseSMBWorkGroup(CJsonNode node)
{
	QStringList list = node.GetStringList(KEY_RESULT);

	emit SigRespSMBWorkGroup(list);
}

void BrowserManager::ParseSMBServer(CJsonNode node)
{
	m_NodeList.clear();

	CJsonNode result = node.GetArray(VAL_RESULT);
	if (result.ArraySize() <= 0)
	{
//		emit SigRespError(message.left(MSG_LIMIT_COUNT));
		return;
	}

	for (int i = 0; i < result.ArraySize(); i++)
	{
		m_NodeList.append(result.GetArrayAt(i));
//		LogDebug("node : [%s]", m_NodeList[i].ToCompactByteArray().data());
	}

	emit SigRespSMBServer(m_NodeList);
}

void BrowserManager::ParseSMBShare(CJsonNode node)
{
	QStringList list = node.GetStringList(KEY_RESULT);

	emit SigRespSMBShare(list);
}

void BrowserManager::ParseSMBInfo(CJsonNode node)
{
	emit SigRespSMBInfo(node);
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

