#include "musicdbmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"


MusicDBManager::MusicDBManager(QObject *parent)
	: m_pSql(new SQLManager)
{
	Q_UNUSED(parent)

	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespInfo(QString, int)));
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespCoverArt(QString, int, int)), this, SLOT(SlotRespCoverArt(QString, int, int)));

	InitMusic();
}

MusicDBManager::~MusicDBManager()
{
	if (m_pSql)
	{
		delete m_pSql;
		m_pSql = nullptr;
	}
}

void MusicDBManager::RequestMusicDBInfo()
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, m_pSql->GetQueryMusicDBInfo());
	RequestCommand(node, MUSICDB_INFO);
}

void MusicDBManager::RequestCategoryList(int nCategory)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, m_pSql->GetQueryCategoryList(nCategory));
	RequestCommand(node, MUSICDB_CATEGORY_LIST);
}

void MusicDBManager::RequestCategoryInfo(int nID, int nCategory)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, m_pSql->GetQueryCategoryInfo(nID, nCategory));
	RequestCommand(node, MUSICDB_CATEGORY_INFO);
}

void MusicDBManager::RequestSongsOfCategory(int nID, int nCategory)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, m_pSql->GetQuerySongsOfCategory(nID, nCategory));
	RequestCommand(node, MUSICDB_SONGS_OF_CATEGORY);
}

void MusicDBManager::RequestPlaySong(int nID, int nWhere)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_MUSIC_DB);
	node.Add	(KEY_CMD1,		VAL_PLAY);
	node.Add	(KEY_CMD2,		VAL_SONG);
	node.AddInt	(KEY_ID_UPPER,	nID);
	node.AddInt	(KEY_WHERE,		nWhere);
	node.AddInt	(KEY_SONG_ORDER, 0);
	RequestCommand(node, MUSICDB_PLAY_SONG);

}

void MusicDBManager::RequestFavorite(int nID, int nFavorite, int nCategory)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_QUERY);
	node.Add	(KEY_CMD1,		VAL_SONG);
	node.Add	(KEY_SQL,		m_pSql->GetQueryFavorite(nID, nFavorite, nCategory));
	RequestCommand(node, MUSICDB_UPDATE_FAVORITE);
}

void MusicDBManager::RequestRating(int nID, int nRating, int nCategory)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_QUERY);
	node.Add	(KEY_CMD1,		VAL_SONG);
	node.Add	(KEY_SQL,		m_pSql->GetQueryRating(nID, nRating, nCategory));
	RequestCommand(node, MUSICDB_UPDATE_RATING);
}

QString MusicDBManager::GetCategoryName(int nCategory)
{
	QString strCat;

	switch(nCategory)
	{
	case SQLManager::CATEGORY_ALBUM:
		strCat = KEY_ALBUM;
		break;
	case SQLManager::CATEGORY_ARTIST:
		strCat = KEY_ARTIST;
		break;
	case SQLManager::CATEGORY_GENRE:
		strCat = KEY_GENRE;
		break;
	case SQLManager::CATEGORY_COMPOSER:
		strCat = KEY_COMPOSER;
		break;
	case SQLManager::CATEGORY_MOOD:
		strCat = KEY_MOOD;
		break;
	case SQLManager::CATEGORY_FOLDER:
		strCat = KEY_FOLDER;
		break;
	case SQLManager::CATEGORY_YEAR:
		strCat = KEY_YEAR;
		break;
	default:	// Song
		strCat = KEY_SONG;
		break;
	}

	return strCat;
}

void MusicDBManager::SlotRespInfo(QString json, int nCmdID)
{
	CJsonNode node;
	if (!node.SetContent(json))
	{
		emit SigRespError("invalid json");
		return;
	}

	LogDebug("cmdID [%d] node [%s]", nCmdID, node.ToTabedByteArray().data());

	QString message = node.GetString(VAL_MSG);
	bool success = node.GetBool(VAL_SUCCESS);
	if (!success)
	{
		if (message.isEmpty())
		{
			emit SigRespError("unknown error - message is empty");
			return;
		}

		emit SigRespError(message.left((MSG_LIMIT_COUNT)));
		return;
	}

	if (nCmdID == MUSICDB_PLAY_SONG
			|| nCmdID == MUSICDB_UPDATE_FAVORITE
			|| nCmdID == MUSICDB_UPDATE_RATING
			)
	{
		return;
	}

	CJsonNode result = node.GetArray(VAL_RESULT);
	if (result.ArraySize() <= 0)
	{
		emit SigRespError(message.left(MSG_LIMIT_COUNT));
		return;
	}

	switch (nCmdID)
	{
	case MUSICDB_INFO:
		ParseMusicInfo(result);
		break;
	case MUSICDB_CATEGORY_LIST:
		ParseCategoryList(result);
		break;
	case MUSICDB_CATEGORY_INFO:
		ParseCategoryInfo(result);
		break;
	case MUSICDB_SONGS_OF_CATEGORY:
		ParseSongsOfCategory(result);
		break;
	case MUSICDB_MAX:
		emit SigRespError("invalid command id");
		break;
	}

}

void MusicDBManager::SlotRespCoverArt(QString fileName, int nIndex, int mode)
{
	emit SigCoverArtUpdate(fileName, nIndex, mode);
}

void MusicDBManager::InitMusic()
{
}

void MusicDBManager::ParseMusicInfo(CJsonNode result)
{
//	LogDebug("result : [%s]", result.ToCompactByteArray().data());

	m_Node.Clear();
	for (int i = 0; i < result.ArraySize(); i++)
	{
		m_Node = result.GetArrayAt(i);
//		LogDebug("node : [%s]", m_Node.ToCompactByteArray().data());
	}

	emit SigRespMusicInfo(m_Node);
}

void MusicDBManager::ParseCategoryList(CJsonNode result)
{
//	LogDebug("result : [%s]", result.ToCompactByteArray().data());

	m_NodeList.clear();
	for (int i = 0; i < result.ArraySize(); i++)
	{
		m_NodeList.append(result.GetArrayAt(i));
//		LogDebug("node : [%s]", m_NodeList[i].ToCompactByteArray().data());
	}

	emit SigRespCategoryList(m_NodeList);
}

void MusicDBManager::ParseCategoryInfo(CJsonNode result)
{
//	LogDebug("result : [%s]", result.ToCompactByteArray().data());

	m_Node.Clear();
	for (int i = 0; i < result.ArraySize(); i++)
	{
		m_Node = result.GetArrayAt(i);
		LogDebug("node : [%s]", m_Node.ToCompactByteArray().data());

	}

	emit SigRespCategoryInfo(m_Node);
}

void MusicDBManager::ParseSongsOfCategory(CJsonNode result)
{
//	LogDebug("result : [%s]", result.ToCompactByteArray().data());

	m_NodeList.clear();
	for (int i = 0; i < result.ArraySize(); i++)
	{
		m_NodeList.append(result.GetArrayAt(i));
		LogDebug("node : [%s]", m_NodeList[i].ToCompactByteArray().data());

	}

	emit SigRespSongsOfCategory(m_NodeList);
}

