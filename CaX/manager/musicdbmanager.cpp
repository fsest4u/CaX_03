#include "musicdbmanager.h"

#include "dialog/edittagdialog.h"

#include "util/utilnovatron.h"

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

void MusicDBManager::RequestMusicDBOverView()
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, m_pSql->GetQueryMusicDBOverview());
	RequestCommand(node, MUSICDB_OVERVIEW);
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
	QString query = m_pSql->GetQueryCategoryListFromSong(nCategory,
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

void MusicDBManager::RequestAlbumOfCategoryList(int nCategory,
												int nSort,
												bool bIncrease,
												QString artistID,
												QString genreID,
												QString composerID,
												int nFavorite,
												int nRating,
												int nStartIndex,
												int nLimitCount,
												QString catID)
{
	QString query = m_pSql->GetQueryAlbumOfCategoryListFromSong(nCategory,
																nSort,
																bIncrease,
																artistID,
																genreID,
																composerID,
																nFavorite,
																nRating,
																nStartIndex,
																nLimitCount,
																catID);

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, query);
	RequestCommand(node, MUSICDB_CATEGORY_LIST);
}

void MusicDBManager::RequestArtistOfCategoryList(int nCategory,
												 int nSort,
												 bool bIncrease,
												 QString artistID,
												 QString genreID,
												 QString composerID,
												 int nFavorite,
												 int nRating,
												 int nStartIndex,
												 int nLimitCount,
												 QString catID)
{
	QString query = m_pSql->GetQueryArtistOfCategoryListFromSong(nCategory,
																 nSort,
																 bIncrease,
																 artistID,
																 genreID,
																 composerID,
																 nFavorite,
																 nRating,
																 nStartIndex,
																 nLimitCount,
																 catID);

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, query);
	RequestCommand(node, MUSICDB_CATEGORY_LIST);
}

void MusicDBManager::RequestAlbumOfArtistOfCategoryList(int nCategory, int nSort,
														bool bIncrease,
														QString artistID,
														QString genreID,
														QString composerID,
														int nFavorite,
														int nRating,
														int nStartIndex,
														int nLimitCount,
														QString catID,
														QString catID2)
{
	QString query = m_pSql->GetQueryAlbumOfArtistOfCategoryListFromSong(nCategory,
																		nSort,
																		bIncrease,
																		artistID,
																		genreID,
																		composerID,
																		nFavorite,
																		nRating,
																		nStartIndex,
																		nLimitCount,
																		catID,
																		catID2);

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, query);
	RequestCommand(node, MUSICDB_CATEGORY_LIST);
}

void MusicDBManager::RequestCategoryOverview(int nID, int nCategory)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, m_pSql->GetQueryCategoryOverview(nID, nCategory));
	RequestCommand(node, MUSICDB_CATEGORY_OVERVIEW);
}

void MusicDBManager::RequestTrackList(int nID,
									int nCategory,
									int nSort,
									bool bIncrease,
									int nStartIndex,
									int nLimitCount)
{
	QString query = m_pSql->GetQueryTrackList(nID,
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
	RequestCommand(node, MUSICDB_TRACK_LIST);
}

void MusicDBManager::RequestTrackListForEditTag(int nID, int nCategory, int nSort, bool bIncrease, int nStartIndex, int nLimitCount)
{
	QString query = m_pSql->GetQueryTrackList(nID,
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
	RequestCommand(node, MUSICDB_TRACK_LIST_FOR_EDIT_TAG);
}

void MusicDBManager::RequestTrackListOfAlbum(int nID,
											 int nCategory,
											 int nSort,
											 bool bIncrease,
											 int nStartIndex,
											 int nLimitCount,
											 QString catID)
{
	QString query = m_pSql->GetQueryTrackListOfAlbum(nID,
													 nCategory,
													 nSort,
													 bIncrease,
													 nStartIndex,
													 nLimitCount,
													 catID);

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, query);
	RequestCommand(node, MUSICDB_TRACK_LIST);
}

void MusicDBManager::RequestTrackListOfAlbumOfArtist(int nID,
													 int nCategory,
													 int nSort,
													 bool bIncrease,
													 int nStartIndex,
													 int nLimitCount,
													 QString catID,
													 QString catID2)
{
	QString query = m_pSql->GetQueryTrackListOfAlbumOfArtist(nID,
															 nCategory,
															 nSort,
															 bIncrease,
															 nStartIndex,
															 nLimitCount,
															 catID,
															 catID2);

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, query);
	RequestCommand(node, MUSICDB_TRACK_LIST);
}

void MusicDBManager::RequestManageCategory(QString cmd1,
										   QMap<int, bool> idMap,
										   int nWhere,
										   int nCategory,
										   int eventID)
{
	QString strCat = UtilNovatron::GetCategoryName(nCategory);

	CJsonNode node(JSON_OBJECT);

	if (idMap.count() > 0)
	{
		if (SQLManager::CATEGORY_TRACK == nCategory)
		{
			node = MakeNodeTrackSelect(cmd1, idMap, nWhere, eventID);
		}
		else if (SQLManager::CATEGORY_YEAR == nCategory)
		{
			node = MakeNodeYearSelect(cmd1, idMap, nWhere, eventID);
		}
		else
		{
			node = MakeNodeCategorySelect(cmd1, idMap, nWhere, strCat, eventID);
		}
	}
	else
	{
		if (SQLManager::CATEGORY_TRACK == nCategory)
		{
			node = MakeNodeTrack(cmd1, nWhere, eventID);
		}
		else if (SQLManager::CATEGORY_YEAR == nCategory)
		{
			node = MakeNodeYear(cmd1, nWhere, strCat, eventID);
		}
		else
		{
			node = MakeNodeCategory(cmd1, nWhere, strCat, eventID);
		}
	}

	RequestCommand(node, MUSICDB_MANAGE_CATEGORY);
}

CJsonNode MusicDBManager::MakeNodeYear(QString cmd1,
									   int nWhere,
									   QString strCat,
									   int eventID)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_ETC_ORDER, QString("Song.%1").arg(strCat));
	node.AddInt	(KEY_SONG_ORDER, 0);
	node.Add	(KEY_CMD0,	VAL_MUSIC_DB);
	node.Add	(KEY_CMD1,	cmd1);
	node.Add	(KEY_CMD2,	VAL_ALL);
	if (nWhere != PLAY_NONE)
	{
		node.AddInt	(KEY_WHERE,		nWhere);
	}
	if (eventID != -1)
	{
		node.AddInt	(KEY_EVENT_ID,	eventID);
	}
	return node;
}

CJsonNode MusicDBManager::MakeNodeYearSelect(QString cmd1,
											 QMap<int, bool> idMap,
											 int nWhere,
											 int eventID )
{
	CJsonNode yearArr(JSON_ARRAY);
	QMap<int, bool>::iterator i;
	for (i = idMap.begin(); i!= idMap.end(); i++)
	{
//		LogDebug("key [%d] value [%d]", i.key(), i.value());
		yearArr.AppendArray((int64_t)i.key());
	}

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_YEARS, yearArr);
	node.AddInt	(KEY_SONG_ORDER, 0);
	node.Add	(KEY_CMD0,		VAL_MUSIC_DB);
	node.Add	(KEY_CMD1,		cmd1);
	node.Add	(KEY_CMD2,		VAL_YEAR);
	if (nWhere != PLAY_NONE)
	{
		node.AddInt	(KEY_WHERE,		nWhere);
	}
	if (eventID != -1)
	{
		node.AddInt	(KEY_EVENT_ID,	eventID);
	}
	return node;
}

CJsonNode MusicDBManager::MakeNodeTrack(QString cmd1,
										int nWhere,
										int eventID)
{
	CJsonNode node(JSON_OBJECT);
	node.AddInt	(KEY_SONG_ORDER, 0);
	node.Add	(KEY_CMD0,		VAL_MUSIC_DB);
	node.Add	(KEY_CMD1,		cmd1);
	node.Add	(KEY_CMD2,	VAL_ALL);
	if (nWhere != PLAY_NONE)
	{
		node.AddInt	(KEY_WHERE,		nWhere);
	}
	if (eventID != -1)
	{
		node.AddInt	(KEY_EVENT_ID,	eventID);
	}
	return node;
}

CJsonNode MusicDBManager::MakeNodeTrackSelect(QString cmd1,
											  QMap<int, bool> idMap,
											  int nWhere,
											  int eventID)
{
	CJsonNode idArr(JSON_ARRAY);
	QMap<int, bool>::iterator i;
	for (i = idMap.begin(); i!= idMap.end(); i++)
	{
//		LogDebug("key [%d] value [%d]", i.key(), i.value());
		idArr.AppendArray((int64_t)i.key());
	}

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_IDS, idArr);
	node.AddInt	(KEY_SONG_ORDER, 0);
	node.Add	(KEY_CMD0,		VAL_MUSIC_DB);
	node.Add	(KEY_CMD1,		cmd1);
	node.Add	(KEY_CMD2,		VAL_SONG);
	if (nWhere != PLAY_NONE)
	{
		node.AddInt	(KEY_WHERE,		nWhere);
	}
	if (eventID != -1)
	{
		node.AddInt	(KEY_EVENT_ID,	eventID);
	}
	return node;
}

CJsonNode MusicDBManager::MakeNodeCategory(QString cmd1,
										   int nWhere,
										   QString strCat,
										   int eventID)
{
	CJsonNode orderInfo(JSON_OBJECT);
	orderInfo.Add(KEY_CATEGORY, strCat);
	orderInfo.AddInt(KEY_ORDER, 0);

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CAT_ORDER, orderInfo);
	node.AddInt	(KEY_SONG_ORDER, 0);
	node.Add	(KEY_CMD0,	VAL_MUSIC_DB);
	node.Add	(KEY_CMD1,	cmd1);
	node.Add	(KEY_CMD2,	VAL_ALL);
	if (nWhere != PLAY_NONE)
	{
		node.AddInt	(KEY_WHERE,		nWhere);
	}
	if (eventID != -1)
	{
		node.AddInt	(KEY_EVENT_ID,	eventID);
	}
	return node;

}

CJsonNode MusicDBManager::MakeNodeCategorySelect(QString cmd1,
												 QMap<int, bool> idMap,
												 int nWhere,
												 QString strCat,
												 int eventID)
{
	CJsonNode idArr(JSON_ARRAY);
	QMap<int, bool>::iterator i;
	for (i = idMap.begin(); i!= idMap.end(); i++)
	{
//		LogDebug("key [%d] value [%d]", i.key(), i.value());
		idArr.AppendArray((int64_t)i.key());
	}

//	CJsonNode filterArr(JSON_ARRAY);
//	CJsonNode filterInfo(JSON_OBJECT);
//	filterInfo.Add(KEY_CATEGORY, strCat);
//	filterInfo.Add(KEY_IDS, idArr);
//	filterArr.AppendArray(filterInfo);

	CJsonNode node(JSON_OBJECT);
//	node.Add(KEY_FILTERS, filterArr);
	node.Add(KEY_CATEGORY, strCat);
	node.Add(KEY_IDS, idArr);
	node.AddInt(KEY_SONG_ORDER, 0);
	node.Add(KEY_CMD0, VAL_MUSIC_DB);
	node.Add(KEY_CMD1, cmd1);
	node.Add(KEY_CMD2, VAL_CATEGORY);
	if (nWhere != PLAY_NONE)
	{
		node.AddInt	(KEY_WHERE,		nWhere);
	}
	if (eventID != -1)
	{
		node.AddInt	(KEY_EVENT_ID,	eventID);
	}
	return node;
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

void MusicDBManager::RequestCategoryInfo(int id)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_MUSIC_DB);
	node.Add	(KEY_CMD1,		VAL_INFO);
	node.Add	(KEY_CMD2,		VAL_ALBUM);
	node.AddInt	(KEY_ID_UPPER,	id);

	RequestCommand(node, MUSICDB_CATEGORY_INFO);
}

void MusicDBManager::RequestCategoryInfoList(int nCategory)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_QUERY);
	node.Add(KEY_CMD1, VAL_SONG);
	node.Add(KEY_AS, true);
	node.Add(KEY_AL, false);
	node.Add(KEY_SQL, m_pSql->GetQueryCategoryList(nCategory));

	RequestCommand(node, MUSICDB_CATEGORY_INFO_LIST);

}

void MusicDBManager::RequestSetCategoryInfo(int id, int eventID, CJsonNode node)
{
	node.Add	(KEY_CMD0,		VAL_MUSIC_DB);
	node.Add	(KEY_CMD1,		VAL_INFO);
	node.Add	(KEY_CMD2,		VAL_SET_ALBUM);
	node.AddInt	(KEY_ID_UPPER,	id);
	node.AddInt	(KEY_EVENT_ID,	eventID);

	RequestCommand(node, MUSICDB_SET_CATEGORY_INFO);
}

void MusicDBManager::RequestRenameCategory(int id, QString name, int nCategory, int eventID)
{
	QString strCat = UtilNovatron::GetCategoryName(nCategory);

	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_MUSIC_DB);
	node.Add	(KEY_CMD1,		VAL_RENAME);
	node.Add	(KEY_CMD2,		VAL_CATEGORY);
	node.AddInt	(KEY_ID_UPPER,	id);
	node.Add	(KEY_NAME_CAP,		name);
	node.Add	(KEY_CATEGORY,	strCat);
	node.AddInt	(KEY_EVENT_ID,	eventID);

	RequestCommand(node, MUSICDB_RENAME_CATEGORY);
}

void MusicDBManager::RequestTrackInfo(int id)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_MUSIC_DB);
	node.Add	(KEY_CMD1,		VAL_INFO);
	node.Add	(KEY_CMD2,		VAL_SONG);
	node.AddInt	(KEY_ID_UPPER,	id);

	RequestCommand(node, MUSICDB_TRACK_INFO);
}

void MusicDBManager::RequestSetTrackInfo(int id, int eventID, CJsonNode node)
{
	Q_UNUSED(eventID)
//	LogDebug("node [%s]", node.ToCompactByteArray().data());

	node.Add	(KEY_CMD0,		VAL_MUSIC_DB);
	node.Add	(KEY_CMD1,		VAL_INFO);
	node.Add	(KEY_CMD2,		VAL_SET_TAG);
	node.AddInt	(KEY_ID_UPPER,	id);

	RequestCommand(node, MUSICDB_SET_TRACK_INFO);
}

void MusicDBManager::RequestSetTrackInfoAll(QMap<QStringList, QString> updateMap, QStandardItemModel *updateModel, int eventID)
{
	QMap<QStringList, QString>::iterator i;
	for (i = updateMap.begin(); i != updateMap.end(); i++)
	{
		QStringList valueList;
		int row = i.key().at(0).toInt();
		int colCount = updateModel->columnCount();
		for (int col = 0; col < colCount; col++)
		{
			QModelIndex modelIndex = updateModel->index(row, col);
			valueList.append(qvariant_cast<QString>(modelIndex.data(Qt::DisplayRole)));
//			LogDebug("row [%d] col [%d] value [%s]", row, col, valueList.at(col).toUtf8().data());
		}

		CJsonNode node(JSON_OBJECT);
		node.Add(KEY_TITLE_CAP, valueList.at(EditTagDialog::EDIT_TAG_TITLE));
		node.Add(KEY_ARTIST, valueList.at(EditTagDialog::EDIT_TAG_ARTIST));
		node.Add(KEY_ALBUM, valueList.at(EditTagDialog::EDIT_TAG_ALBUM));
		node.Add(KEY_GENRE, valueList.at(EditTagDialog::EDIT_TAG_GENRE));
		node.Add(KEY_ALBUM_ARTIST, valueList.at(EditTagDialog::EDIT_TAG_ALBUM_ARTIST));
		node.Add(KEY_COMPOSER, valueList.at(EditTagDialog::EDIT_TAG_COMPOSER));
		node.Add(KEY_YEAR, valueList.at(EditTagDialog::EDIT_TAG_YEAR));
		node.Add(KEY_MOOD, valueList.at(EditTagDialog::EDIT_TAG_MOOD));

		int id = valueList.at(EditTagDialog::EDIT_TAG_ID).toInt();
		RequestSetTrackInfo(id, eventID, node);
		RequestUpdateTrackFavorite(id, valueList.at(EditTagDialog::EDIT_TAG_FAVORITE).toInt());
	}

	ParseRefresh();
}

void MusicDBManager::RequestRenameTrack(int id, QString name, int eventID)
{
	CJsonNode node(JSON_OBJECT);
	node.Add	(KEY_CMD0,		VAL_MUSIC_DB);
	node.Add	(KEY_CMD1,		VAL_RENAME);
	node.Add	(KEY_CMD2,		VAL_SONG);
	node.AddInt	(KEY_ID_UPPER,	id);
	node.Add	(KEY_NAME_CAP,		name);
	node.AddInt	(KEY_EVENT_ID,	eventID);

	RequestCommand(node, MUSICDB_RENAME_TRACK);
}

void MusicDBManager::RequestAddToPlaylist(int id, QMap<int, bool> idMap, int category)
{
	QString strCat = UtilNovatron::GetCategoryName(category);

	CJsonNode idArr(JSON_ARRAY);
	QMap<int, bool>::iterator i;
	for (i = idMap.begin(); i!= idMap.end(); i++)
	{
		idArr.AppendArray((int64_t)i.key());
	}

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_PLAYLIST);
	node.Add(KEY_CMD1, VAL_ADD);
	node.AddInt(KEY_PLS_ID, id);
	node.AddInt(KEY_SONG_ORDER, 0);
	if (category >= 0)
	{
		CJsonNode orderInfo(JSON_OBJECT);
		orderInfo.Add(KEY_CATEGORY, strCat);
		orderInfo.AddInt(KEY_ORDER, 0);
		node.Add(KEY_CAT_ORDER, orderInfo);

		node.Add(KEY_CMD2, VAL_CATEGORY);
		node.Add(KEY_CATEGORY, strCat);
	}
	else
	{
		node.Add(KEY_CMD2, VAL_SONG);
	}
	node.Add(KEY_IDS, idArr);

	RequestCommand(node, MUSICDB_ADD_TO_PLAYLIST);
}

void MusicDBManager::RequestSetCategoryCoverArt(int id, int category, int eventID, QString image, QString thumb)
{
	m_ID = id;
	m_Category = category;

	QString strCat = UtilNovatron::GetCategoryName(category);

	CJsonNode coverArt(JSON_OBJECT);
	coverArt.Add(KEY_IMAGE_URL, image);
	coverArt.Add(KEY_THUMB_URL, thumb);

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_MUSIC_DB);
	node.Add(KEY_CMD1, VAL_SET_ART);
	node.Add(KEY_CMD2, VAL_CATEGORY);
	node.Add(KEY_CATEGORY, strCat);
	node.AddInt(KEY_EVENT_ID, eventID);
	node.Add(KEY_COVER_ART, coverArt);
	node.AddInt(KEY_ID_UPPER, id);

	RequestCommand(node, MUSICDB_SET_CATEGORY_COVER_ART);
}

void MusicDBManager::RequestSetTrackCoverArt(int id, int category, int eventID, QString image, QString thumb)
{
	m_ID = id;
	m_Category = category;

	CJsonNode coverArt(JSON_OBJECT);
	coverArt.Add(KEY_IMAGE_URL, image);
	coverArt.Add(KEY_THUMB_URL, thumb);

	CJsonNode idArr(JSON_ARRAY);
	idArr.AppendArray((int64_t)id);

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_MUSIC_DB);
	node.Add(KEY_CMD1, VAL_SET_ART);
	node.Add(KEY_CMD2, VAL_SONG);
	node.AddInt(KEY_EVENT_ID, eventID);
	node.Add(KEY_COVER_ART, coverArt);
	node.Add(KEY_IDS, idArr);

	RequestCommand(node, MUSICDB_SET_TRACK_COVER_ART);
}

//void MusicDBManager::RequestCheckCategory(int id, int category, int updateCategory, QString updateName)
//{
//	m_ID = id;
//	m_Category = category;
//	m_UpdateCategory = updateCategory;
//	m_UpdateName = updateName;

//	CJsonNode node(JSON_OBJECT);
//	node.Add	(KEY_CMD0,		VAL_QUERY);
//	node.Add	(KEY_CMD1,		VAL_SONG);
//	node.Add(KEY_AS, true);
//	node.Add(KEY_AL, false);
//	node.Add	(KEY_SQL,		m_pSql->GetQueryCheckCategory(m_UpdateCategory, m_UpdateName));

//	RequestCommand(node, MUSICDB_CHECK_CATEGORY);
//}

//void MusicDBManager::RequestInsertCategory(int updateCategory, QString updateName)
//{
//	CJsonNode node(JSON_OBJECT);
//	node.Add	(KEY_CMD0,		VAL_QUERY);
//	node.Add	(KEY_CMD1,		VAL_SONG);
//	node.Add(KEY_AS, true);
//	node.Add(KEY_AL, false);
//	node.Add	(KEY_SQL,		m_pSql->GetQueryInsertCategory(updateCategory, updateName));

//	RequestCommand(node, MUSICDB_INSERT_CATEGORY);
//}

//void MusicDBManager::RequestUpdateCategory(int id, int category, int updateCategory, int updateId)
//{
//	if (updateCategory == category)
//	{
//		m_ID = updateId;
//	}
//	else
//	{
//		m_ID = -1;
//	}

//	CJsonNode node(JSON_OBJECT);
//	node.Add	(KEY_CMD0,		VAL_QUERY);
//	node.Add	(KEY_CMD1,		VAL_SONG);
//	node.Add(KEY_AS, true);
//	node.Add(KEY_AL, false);
//	node.Add	(KEY_SQL,		m_pSql->GetQueryUpdateCategory(id, category, updateCategory, updateId));

//	RequestCommand(node, MUSICDB_UPDATE_CATEGORY);
//}

//void MusicDBManager::RequestInsertIgnoreCategoryAll(QMap<QStringList, QString> updateMap)
//{
//	QStringList insertQuery;
//	QMap<QStringList, QString>::iterator i;
//	for (i = updateMap.begin(); i != updateMap.end(); i++)
//	{
//		int updateCategory = -1;
//		QString updateName = i.value();

//		int col = i.key().at(1).toInt();
////		LogDebug("col [%d] value [%s]", col, updateName.toUtf8().data());
//		switch (col)
//		{
//		case EditTagDialog::EDIT_TAG_TITLE:
//			break;
//		case EditTagDialog::EDIT_TAG_FAVORITE:
//			break;
//		case EditTagDialog::EDIT_TAG_ARTIST:
//			updateCategory = SQLManager::CATEGORY_ARTIST;
//			break;
//		case EditTagDialog::EDIT_TAG_ALBUM:
//			updateCategory = SQLManager::CATEGORY_ALBUM;
//			break;
//		case EditTagDialog::EDIT_TAG_GENRE:
//			updateCategory = SQLManager::CATEGORY_GENRE;
//			break;
//		case EditTagDialog::EDIT_TAG_ALBUM_ARTIST:
//			updateCategory = SQLManager::CATEGORY_ALBUM_ARTIST;
//			break;
//		case EditTagDialog::EDIT_TAG_COMPOSER:
//			updateCategory = SQLManager::CATEGORY_COMPOSER;
//			break;
//		case EditTagDialog::EDIT_TAG_YEAR:
//			break;
//		case EditTagDialog::EDIT_TAG_MOOD:
//			updateCategory = SQLManager::CATEGORY_MOOD;
//			break;
////		case EditTagDialog::EDIT_TAG_RATING:
////			break;
//		}
//		QString query = m_pSql->GetQueryInsertIgnoreCategoryAll(updateCategory, updateName);
//		insertQuery.append(query);
//	}

//	// dummy
//	insertQuery.append("SELECT ROWID AS id FROM Album ORDER BY ROWID DESC LIMIT 1;");
////	LogDebug("query [%s]", insertQuery.join(";").toUtf8().data());

//	CJsonNode node(JSON_OBJECT);
//	node.Add	(KEY_CMD0,		VAL_QUERY);
//	node.Add	(KEY_CMD1,		VAL_SONG);
//	node.Add(KEY_AS, false);
//	node.Add(KEY_AL, false);
//	node.Add	(KEY_SQL,		insertQuery.join(""));

//	RequestCommand(node, MUSICDB_INSERT_REPLACE_CATEGORY_ALL);
//}

//void MusicDBManager::RequestUpdateCategoryAll(QMap<QStringList, QString> updateMap, QStandardItemModel *updateModel)
//{
//	QStringList updateQuery;

//	QMap<QStringList, QString>::iterator i;
//	for (i = updateMap.begin(); i != updateMap.end(); i++)
//	{
//		QStringList valueList;
//		int row = i.key().at(0).toInt();
//		int colCount = updateModel->columnCount();
//		for (int col = 0; col < colCount; col++)
//		{
//			QModelIndex modelIndex = updateModel->index(row, col);
//			valueList.append(qvariant_cast<QString>(modelIndex.data(Qt::DisplayRole)));
////			LogDebug("row [%d] col [%d] value [%s]", row, col, valueList.at(col).toUtf8().data());
//		}

//		QString query = m_pSql->GetQueryUpdateCategoryAll(valueList.at(EditTagDialog::EDIT_TAG_ID),
//														  valueList.at(EditTagDialog::EDIT_TAG_TITLE),
//														  valueList.at(EditTagDialog::EDIT_TAG_FAVORITE),
//														  valueList.at(EditTagDialog::EDIT_TAG_ARTIST),
//														  valueList.at(EditTagDialog::EDIT_TAG_ALBUM),
//														  valueList.at(EditTagDialog::EDIT_TAG_GENRE),
//														  valueList.at(EditTagDialog::EDIT_TAG_ALBUM_ARTIST),
//														  valueList.at(EditTagDialog::EDIT_TAG_COMPOSER),
//														  valueList.at(EditTagDialog::EDIT_TAG_YEAR),
//														  valueList.at(EditTagDialog::EDIT_TAG_MOOD));
//		updateQuery.append(query);
//	}

//	// dummy
//	updateQuery.append("SELECT ROWID AS id FROM Album ORDER BY ROWID DESC LIMIT 1;");
////	LogDebug("query [%s]", updateQuery.join(";").toUtf8().data());

//	CJsonNode node(JSON_OBJECT);
//	node.Add	(KEY_CMD0,		VAL_QUERY);
//	node.Add	(KEY_CMD1,		VAL_SONG);
//	node.Add(KEY_AS, false);
//	node.Add(KEY_AL, false);
//	node.Add	(KEY_SQL,		updateQuery.join(""));

//	RequestCommand(node, MUSICDB_UPDATE_CATEGORY_ALL);
//}

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

//	LogDebug("cmdID [%d] node [%s]", nCmdID, node.ToTabedByteArray().data());

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

	if (nCmdID == MUSICDB_MANAGE_CATEGORY
			|| nCmdID == MUSICDB_UPDATE_CATEGORY_FAVORITE
			|| nCmdID == MUSICDB_UPDATE_CATEGORY_RATING
			|| nCmdID == MUSICDB_UPDATE_TRACK_FAVORITE
			|| nCmdID == MUSICDB_SET_CATEGORY_INFO
			|| nCmdID == MUSICDB_RENAME_CATEGORY
			|| nCmdID == MUSICDB_RENAME_TRACK
			|| nCmdID == MUSICDB_SET_TRACK_INFO
			|| nCmdID == MUSICDB_RANDOM
			)
	{
		return;
	}

	switch (nCmdID)
	{
	case MUSICDB_CATEGORY_INFO:
		ParseCategoryInfo(node);
		return;
	case MUSICDB_TRACK_INFO:
		ParseTrackInfo(node);
		return;
	case MUSICDB_SET_CATEGORY_COVER_ART:
	case MUSICDB_SET_TRACK_COVER_ART:
		ParseSetCoverArt();
		break;
//	case MUSICDB_CHECK_CATEGORY:
//		ParseCheckCategory(node);
//		return;
//	case MUSICDB_INSERT_CATEGORY:
//		ParseInsertCategory(node);
//		return;
//	case MUSICDB_UPDATE_CATEGORY:
//		ParseUpdateCategory(node);
//		return;
//	case MUSICDB_INSERT_REPLACE_CATEGORY_ALL:
//		ParseInsertReplaceCategoryAll(node);
//		break;
//	case MUSICDB_UPDATE_CATEGORY_ALL:
//		ParseRefresh();
//		break;
	case MUSICDB_MAX:
		emit SigRespError(STR_INVALID_ID);
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
	case MUSICDB_OVERVIEW:
		ParseMusicOverview(result);
		break;
	case MUSICDB_CATEGORY_LIST:
		ParseCategoryList(result);
		break;
	case MUSICDB_CATEGORY_OVERVIEW:
		ParseCategoryOverview(result);
		break;
	case MUSICDB_TRACK_LIST:
		ParseTrackList(result);
		break;
	case MUSICDB_TRACK_LIST_FOR_EDIT_TAG:
		ParseTrackListForEditTag(result);
		break;
	case MUSICDB_CLASSIFY_ARTIST:
		ParseClassifyArtist(result);
		break;
	case MUSICDB_CLASSIFY_GENRE:
		ParseClassifyGenre(result);
		break;
	case MUSICDB_CLASSIFY_COMPOSER:
		ParseClassifyComposer(result);
		break;
	case MUSICDB_CATEGORY_INFO_LIST:
		ParseCategoryInfoList(result);
		break;
	case MUSICDB_MAX:
		emit SigRespError(STR_INVALID_ID);
		break;
	}

}

void MusicDBManager::SlotRespCoverArt(QString fileName, int nIndex, int mode)
{
	emit SigCoverArtUpdate(fileName, nIndex, mode);
}

void MusicDBManager::InitMusic()
{
	m_ID = -1;
	m_Category = -1;
	m_UpdateCategory = -1;
	m_UpdateName = "";
}

void MusicDBManager::ParseMusicOverview(CJsonNode result)
{
//	LogDebug("result : [%s]", result.ToCompactByteArray().data());

	m_Node.Clear();
	for (int i = 0; i < result.ArraySize(); i++)
	{
		m_Node = result.GetArrayAt(i);
//		LogDebug("node : [%s]", m_Node.ToCompactByteArray().data());
	}

	emit SigRespMusicOverview(m_Node);
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

void MusicDBManager::ParseCategoryOverview(CJsonNode result)
{
//	LogDebug("result : [%s]", result.ToCompactByteArray().data());

	m_Node.Clear();
	for (int i = 0; i < result.ArraySize(); i++)
	{
		m_Node = result.GetArrayAt(i);
//		LogDebug("node : [%s]", m_Node.ToCompactByteArray().data());

	}

	emit SigRespCategoryOverview(m_Node);
}

void MusicDBManager::ParseTrackList(CJsonNode result)
{
	QList<CJsonNode> list = ParseResultNode(result);
	emit SigRespTrackList(list);
}

void MusicDBManager::ParseTrackListForEditTag(CJsonNode result)
{
	QList<CJsonNode> list = ParseResultNode(result);
	emit SigRespTrackListForEditTag(list);
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

void MusicDBManager::ParseCategoryInfo(CJsonNode node)
{
	emit SigRespCategoryInfo(node);
}

void MusicDBManager::ParseCategoryInfoList(CJsonNode result)
{
	QList<CJsonNode> list = ParseResultNode(result);
	emit SigRespCategoryInfoList(list);
}

void MusicDBManager::ParseTrackInfo(CJsonNode node)
{
	emit SigRespTrackInfo(node);
}

void MusicDBManager::ParseSearchCoverArt(CJsonNode node)
{
	emit SigRespSearchCoverArt(node);
}

void MusicDBManager::ParseSetCoverArt()
{
	emit SigRespSetCoverArt(m_ID, m_Category);
}

//void MusicDBManager::ParseCheckCategory(CJsonNode node)
//{
//	CJsonNode result = node.GetArray(VAL_RESULT);
//	if (result.ArraySize() <= 0)
//	{
////		LogDebug("insert updateCategory[%d], updateName[%s]", m_UpdateCategory, m_UpdateName.toUtf8().data());
//		RequestInsertCategory(m_UpdateCategory, m_UpdateName);
//	}
//	else
//	{
//		int updateId = -1;
//		QList<CJsonNode> list = ParseResultNode(result);
//		foreach (CJsonNode node, list)
//		{
//			updateId = node.GetString(KEY_ID_LOWER).toInt();
//		}

////		LogDebug("exist & update id[%d] category[%d], updateId[%d]", m_ID, m_Category, updateId);
//		RequestUpdateCategory(m_ID, m_Category, m_UpdateCategory, updateId);
//	}
//}

//void MusicDBManager::ParseInsertCategory(CJsonNode node)
//{
//	CJsonNode result = node.GetArray(VAL_RESULT);
//	if (result.ArraySize() <= 0)
//	{
//		LogDebug("error insert updateCategory[%d], updateName[%s]", m_UpdateCategory, m_UpdateName.toUtf8().data());
////		emit SigRespError("error insert category");
//	}
//	else
//	{
//		int updateId = -1;
//		QList<CJsonNode> list = ParseResultNode(result);
//		foreach (CJsonNode node, list)
//		{
//			updateId = node.GetString(KEY_ID_LOWER).toInt();
//		}

////		LogDebug("update category id[%d] category[%d], updateId[%d]", m_ID, m_Category, updateId);
//		RequestUpdateCategory(m_ID, m_Category, m_UpdateCategory, updateId);
//	}
//}

//void MusicDBManager::ParseUpdateCategory(CJsonNode node)
//{
////	LogDebug("node [%s]", node.ToCompactByteArray().data());
//	emit SigRespUpdateCategory(m_ID);
//}

//void MusicDBManager::ParseInsertReplaceCategoryAll(CJsonNode node)
//{
//	Q_UNUSED(node)

//	emit SigRespInsertReplaceCategoryAll();
//}

void MusicDBManager::ParseRefresh()
{
	emit SigRespRefresh();
}

QList<CJsonNode> MusicDBManager::ParseResultNode(CJsonNode result)
{
	QList<CJsonNode> list;
	list.clear();
	for (int i = 0; i < result.ArraySize(); i++)
	{
		list.append(result.GetArrayAt(i));
//		LogDebug("node : [%s]", list[i].ToCompactByteArray().data());
	}

	return list;
}

