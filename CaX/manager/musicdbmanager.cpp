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

void MusicDBManager::RequestCategoryList(int nCategory,
										 int nSort,
										 bool bIncrease,
										 QString artistID,
										 QString genreID,
										 QString composerID,
										 int nFavorite,
										 int nRating,
										 int nStartIndex,
										 int nLimitCount)
{
	QString query = m_pSql->GetQueryCategoryList(nCategory,
												 nSort,
												 bIncrease,
												 artistID,
												 genreID,
												 composerID,
												 nFavorite,
												 nRating,
												 nStartIndex,
												 nLimitCount);

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, query);
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

void MusicDBManager::RequestSongsOfCategory(int nID,
											int nCategory,
											int nSort,
											bool bIncrease,
											int nStartIndex,
											int nLimitCount)
{
	QString query = m_pSql->GetQuerySongsOfCategory(nID,
													nCategory,
													nSort,
													bIncrease,
													nStartIndex,
													nLimitCount);

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, query);
	RequestCommand(node, MUSICDB_SONGS_OF_CATEGORY);
}

void MusicDBManager::RequestPlayCategoryItems(int nWhere, int nCategory)
{
	QString strCat = m_pSql->GetCategoryName(nCategory);

	CJsonNode node(JSON_OBJECT);

	if (SQLManager::CATEGORY_YEAR == nCategory)
	{
		node.Add(KEY_ETC_ORDER, QString("Song.%1").arg(strCat));
	}
	else
	{
		CJsonNode orderInfo(JSON_OBJECT);
		orderInfo.Add(VAL_CATEGORY, strCat);
		orderInfo.AddInt(KEY_ORDER, 0);
		node.Add(KEY_CAT_ORDER, orderInfo);
	}

	node.AddInt	(KEY_SONG_ORDER, 0);
	node.AddInt	(KEY_WHERE, nWhere);
	node.Add	(KEY_CMD0,	VAL_MUSIC_DB);
	node.Add	(KEY_CMD1,	VAL_PLAY);
	node.Add	(KEY_CMD2,	VAL_ALL);

	RequestCommand(node, MUSICDB_PLAY_CATEGORY_ITEMS);
}

void MusicDBManager::RequestPlayCategoryItem(int nID, int nWhere, int nCategory)
{
	QString strCat = m_pSql->GetCategoryName(nCategory);

	CJsonNode node(JSON_OBJECT);

	if (SQLManager::CATEGORY_YEAR == nCategory)
	{
		node.Add	(KEY_CMD2,		VAL_YEAR);
		CJsonNode yearArr(JSON_ARRAY);
		yearArr.AppendArray((int64_t)nID);
		node.Add(KEY_YEARS, yearArr);
	}
	else if (SQLManager::CATEGORY_TRACK == nCategory)
	{
		node.Add	(KEY_CMD2,		VAL_ALL);
	}
	else
	{
		node.Add	(KEY_CMD2,		VAL_ALL);

		CJsonNode filterArr(JSON_ARRAY);
		CJsonNode filterInfo(JSON_OBJECT);
		filterInfo.Add(KEY_CATEGORY, strCat);
		filterInfo.AddInt(KEY_ID_UPPER, nID);
		filterArr.AppendArray(filterInfo);
		node.Add(KEY_FILTERS, filterArr);
	}

	node.AddInt	(KEY_SONG_ORDER, 0);
	node.AddInt	(KEY_WHERE, nWhere);
	node.Add	(KEY_CMD0,		VAL_MUSIC_DB);
	node.Add	(KEY_CMD1,		VAL_PLAY);

	RequestCommand(node, MUSICDB_PLAY_CATEGORY_ITEM);
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

void MusicDBManager::RequestUpdateFavorite(int nID, int nFavorite, int nCategory)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_QUERY);
	node.Add	(KEY_CMD1,		VAL_SONG);
	node.Add	(KEY_SQL,		m_pSql->GetQueryUpdateCatFavorite(nID, nFavorite, nCategory));
	RequestCommand(node, MUSICDB_UPDATE_CATEGORY_FAVORITE);
}

void MusicDBManager::RequestUpdateRating(int nID, int nRating, int nCategory)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_QUERY);
	node.Add	(KEY_CMD1,		VAL_SONG);
	node.Add	(KEY_SQL,		m_pSql->GetQueryUpdateCatRating(nID, nRating, nCategory));
	RequestCommand(node, MUSICDB_UPDATE_CATEGORY_RATING);
}

void MusicDBManager::RequestUpdateTrackFavorite(int nID, int nFavorite)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_QUERY);
	node.Add	(KEY_CMD1,		VAL_SONG);
	node.Add	(KEY_SQL,		m_pSql->GetQueryUpdateTrackFavorite(nID, nFavorite));
	RequestCommand(node, MUSICDB_UPDATE_TRACK_FAVORITE);
}

void MusicDBManager::RequestClassifyList(int nCategory)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_QUERY);
	node.Add	(KEY_CMD1,		VAL_SONG);
	node.Add	(KEY_AS,		true);
	node.Add	(KEY_SQL,		m_pSql->GetQueryClassifyArtist(nCategory));

	int nCmdID;
	switch (nCategory)
	{
	case SQLManager::CATEGORY_ARTIST:
		nCmdID = MUSICDB_CLASSIFY_ARTIST;
		break;
	case SQLManager::CATEGORY_GENRE:
		nCmdID = MUSICDB_CLASSIFY_GENRE;
		break;
	case SQLManager::CATEGORY_COMPOSER:
		nCmdID = MUSICDB_CLASSIFY_COMPOSER;
		break;
	}
	RequestCommand(node, nCmdID);
}

void MusicDBManager::RequestRandom()
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_REMOTE);
	node.Add(KEY_KEY, VAL_SHUFFLE);

	RequestCommand(node, MUSICDB_RANDOM);
}

SQLManager *MusicDBManager::GetSqlMgr() const
{
	return m_pSql;
}

void MusicDBManager::SetSqlMgr(SQLManager *pSql)
{
	m_pSql = pSql;
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

	if (nCmdID == MUSICDB_PLAY_CATEGORY_ITEMS
			|| nCmdID == MUSICDB_PLAY_SONG
			|| nCmdID == MUSICDB_UPDATE_CATEGORY_FAVORITE
			|| nCmdID == MUSICDB_UPDATE_CATEGORY_RATING
			|| nCmdID == MUSICDB_UPDATE_TRACK_FAVORITE
			|| nCmdID == MUSICDB_RANDOM
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
//	case MUSICDB_PLAY_CATEGORY:
//	case MUSICDB_PLAY_SONG:
//		break;
	case MUSICDB_CLASSIFY_ARTIST:
		ParseClassifyArtist(result);
		break;
	case MUSICDB_CLASSIFY_GENRE:
		ParseClassifyGenre(result);
		break;
	case MUSICDB_CLASSIFY_COMPOSER:
		ParseClassifyComposer(result);
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
	QList<CJsonNode> list = ParseResultNode(result);
	emit SigRespSongsOfCategory(list);
}

void MusicDBManager::ParseClassifyArtist(CJsonNode result)
{
	QList<CJsonNode> list = ParseResultNode(result);
	emit SigRespClassifyArtist(list);
}

void MusicDBManager::ParseClassifyGenre(CJsonNode result)
{
	QList<CJsonNode> list = ParseResultNode(result);
	emit SigRespClassifyGenre(list);
}

void MusicDBManager::ParseClassifyComposer(CJsonNode result)
{
	QList<CJsonNode> list = ParseResultNode(result);
	emit SigRespClassifyComposer(list);
}

QList<CJsonNode> MusicDBManager::ParseResultNode(CJsonNode result)
{
	QList<CJsonNode> list;
	list.clear();
	for (int i = 0; i < result.ArraySize(); i++)
	{
		list.append(result.GetArrayAt(i));
		LogDebug("node : [%s]", list[i].ToCompactByteArray().data());
	}

	return list;
}

