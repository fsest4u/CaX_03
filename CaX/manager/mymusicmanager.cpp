#include "mymusicmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"


MyMusicManager::MyMusicManager(QObject *parent)
	: m_pSql(new SQLManager)
{
	Q_UNUSED(parent)

	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespInfo(QString, int)));
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespCoverArt(QString, int, int)), this, SLOT(SlotRespCoverArt(QString, int, int)));
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespCoverArt(QString)), this, SLOT(SlotRespCoverArt(QString)));

	InitMusic();
}

MyMusicManager::~MyMusicManager()
{
	if (m_pSql)
	{
		delete m_pSql;
		m_pSql = nullptr;
	}
}

void MyMusicManager::RequestMusicInfo()
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, m_pSql->GetQueryMyMusicInfo());
	RequestCommand(node, MUSIC_INFO);
}

void MyMusicManager::RequestCategoryList(int nCategory)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, m_pSql->GetQueryCategoryList(nCategory));
	RequestCommand(node, CATEGORY_LIST);
}

void MyMusicManager::RequestCategoryInfo(int nID, int nCategory)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, m_pSql->GetQueryCategoryInfo(nID, nCategory));
	RequestCommand(node, CATEGORY_INFO);
}

void MyMusicManager::RequestSongsOfCategory(int nID, int nCategory)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, m_pSql->GetQuerySongsOfCategory(nID, nCategory));
	RequestCommand(node, SONGS_OF_CATEGORY);
}

void MyMusicManager::RequestPlaySong(int nID, int nWhere)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_MUSIC_DB);
	node.Add	(KEY_CMD1,		VAL_PLAY);
	node.Add	(KEY_CMD2,		VAL_SONG);
	node.AddInt	(KEY_ID2,		nID);
	node.AddInt	(KEY_WHERE,		nWhere);
	node.AddInt	(KEY_SONG_ORDER, 0);
	RequestCommand(node, PLAY_SONG);

}

void MyMusicManager::RequestFavorite(int nID, int nFavorite, int nCategory)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_QUERY);
	node.Add	(KEY_CMD1,		VAL_SONG);
	node.Add	(KEY_SQL,		m_pSql->GetQueryFavorite(nID, nFavorite, nCategory));
	RequestCommand(node, UPDATE_FAVORITE);
}

void MyMusicManager::RequestRating(int nID, int nRating, int nCategory)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_QUERY);
	node.Add	(KEY_CMD1,		VAL_SONG);
	node.Add	(KEY_SQL,		m_pSql->GetQueryRating(nID, nRating, nCategory));
	RequestCommand(node, UPDATE_RATING);
}

QString MyMusicManager::GetCategoryName(int nCategory)
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

void MyMusicManager::SlotRespInfo(QString json, int nCmdID)
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

	if (nCmdID == PLAY_SONG)
	{
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
	case MUSIC_INFO:
		ParseMusicInfo(result);
		break;
	case CATEGORY_LIST:
		ParseCategoryList(result);
		break;
	case CATEGORY_INFO:
		ParseCategoryInfo(result);
		break;
	case SONGS_OF_CATEGORY:
		ParseSongsOfCategory(result);
		break;
	default:
		LogWarning("Invalid command ID");
		break;
	}

}

void MyMusicManager::SlotRespCoverArt(QString fileName, int nIndex, int mode)
{
	emit SigCoverArtUpdate(fileName, nIndex, mode);
}

void MyMusicManager::SlotRespCoverArt(QString fileName)
{
	emit SigCoverArtUpdate(fileName);
}

void MyMusicManager::InitMusic()
{


}

void MyMusicManager::ParseMusicInfo(CJsonNode result)
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

void MyMusicManager::ParseCategoryList(CJsonNode result)
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

void MyMusicManager::ParseCategoryInfo(CJsonNode result)
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

void MyMusicManager::ParseSongsOfCategory(CJsonNode result)
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

