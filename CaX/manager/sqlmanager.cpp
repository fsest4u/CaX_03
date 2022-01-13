#include "sqlmanager.h"

#include "util/caxkeyvalue.h"
#include "util/sqlconstatns.h"
#include "util/log.h"
#include "util/utilnovatron.h"

SQLManager::SQLManager(QObject *parent)
	: QObject(parent)
{

}

QString SQLManager::GetQueryMusicDBOverview()
{
	return SQL_MUSIC_DB_OVERVIEW;
}

QString SQLManager::GetQueryCategoryListFromSong(int nCategory,
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

	QString query;
	QString category = UtilNovatron::GetCategoryName(nCategory);
	QString column = GetColumnName(nSort);
	QString increase = GetIncrease(bIncrease);

	QString whereArtist = "";
	QString whereGenre = "";
	QString whereComposer = "";
	QString whereFavorite = "";
	QString whereRating = "";

	if (!artistID.isEmpty())
	{
		whereArtist = " and Song.ArtistID = " + artistID;
	}
	if (!genreID.isEmpty())
	{
		whereGenre = " and Song.GenreID = " + genreID;
	}
	if (!composerID.isEmpty())
	{
		whereComposer = " and Song.ComposerID = " + composerID;
	}
	if (nFavorite > 0)
	{
		whereFavorite = QString(" and %1.Favorite = %2").arg(category).arg(nFavorite);
	}
	if (nRating > 0)
	{
		whereFavorite = QString(" and %1.Rating = %2").arg(category).arg(nRating);
	}

	switch (nCategory)
	{
	case CATEGORY_ALBUM:
		query = QString(SQL_ALBUM_LIST_FROM_SONG);
		break;
	case CATEGORY_ALBUM_ARTIST:
		query = QString(SQL_ALBUM_ARTIST_LIST_FROM_SONG);
		break;
	case CATEGORY_ARTIST:
		query = QString(SQL_ARTIST_LIST_FROM_SONG);
		break;
	case CATEGORY_COMPOSER:
		query = QString(SQL_COMPOSER_LIST_FROM_SONG);
		break;
	case CATEGORY_GENRE:
		query = QString(SQL_GENRE_LIST_FROM_SONG);
		break;
	case CATEGORY_MOOD:
		query = QString(SQL_MOOD_LIST_FROM_SONG);
		break;
	case CATEGORY_FOLDER:
		query = QString(SQL_FOLDER_LIST_FROM_SONG);
		break;
	case CATEGORY_YEAR:
		query = QString(SQL_YEAR_LIST_FROM_SONG);
		column = KEY_YEAR;
		break;
	}

	query = query
			.arg(whereArtist)
			.arg(whereGenre)
			.arg(whereComposer)
			.arg(whereFavorite)
			.arg(whereRating)
			.arg(column)
			.arg(increase)
			.arg(nStartIndex)
			.arg(nLimitCount);

//	LogDebug("query [%s]", query.toUtf8().data());

	return query;
}

QString SQLManager::GetQueryAlbumOfCategoryListFromSong(int nCategory,
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
	QString query;
	QString category = UtilNovatron::GetCategoryName(nCategory);
	QString column = GetColumnName(nSort);
	QString increase = GetIncrease(bIncrease);

	QString whereArtist = "";
	QString whereGenre = "";
	QString whereComposer = "";
	QString whereFavorite = "";
	QString whereRating = "";
	QString whereCategory = "";

	if (!artistID.isEmpty())
	{
		whereArtist = " and Song.ArtistID = " + artistID;
	}
	if (!genreID.isEmpty())
	{
		whereGenre = " and Song.GenreID = " + genreID;
	}
	if (!composerID.isEmpty())
	{
		whereComposer = " and Song.ComposerID = " + composerID;
	}
	if (nFavorite > 0)
	{
		whereFavorite = QString(" and %1.Favorite = %2").arg(category).arg(nFavorite);
	}
	if (nRating > 0)
	{
		whereFavorite = QString(" and %1.Rating = %2").arg(category).arg(nRating);
	}
	if (!catID.isEmpty() && catID.compare("-1"))
	{
		whereCategory = QString (" and Song.%1ID = %2").arg(category).arg(catID);
	}

	query = QString(SQL_ALBUM_OF_CAT_LIST_FROM_SONG);

	query = query
			.arg(whereArtist)
			.arg(whereGenre)
			.arg(whereComposer)
			.arg(whereFavorite)
			.arg(whereRating)
			.arg(column)
			.arg(increase)
			.arg(nStartIndex)
			.arg(nLimitCount)
			.arg(whereCategory);

//	LogDebug("query [%s]", query.toUtf8().data());

	return query;
}

QString SQLManager::GetQueryArtistOfCategoryListFromSong(int nCategory,
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
	QString query;
	QString category = UtilNovatron::GetCategoryName(nCategory);
	QString column = GetColumnName(nSort);
	QString increase = GetIncrease(bIncrease);

	QString whereArtist = "";
	QString whereGenre = "";
	QString whereComposer = "";
	QString whereFavorite = "";
	QString whereRating = "";
	QString whereCategory = "";

	if (!artistID.isEmpty())
	{
		whereArtist = " and Song.ArtistID = " + artistID;
	}
	if (!genreID.isEmpty())
	{
		whereGenre = " and Song.GenreID = " + genreID;
	}
	if (!composerID.isEmpty())
	{
		whereComposer = " and Song.ComposerID = " + composerID;
	}
	if (nFavorite > 0)
	{
		whereFavorite = QString(" and %1.Favorite = %2").arg(category).arg(nFavorite);
	}
	if (nRating > 0)
	{
		whereFavorite = QString(" and %1.Rating = %2").arg(category).arg(nRating);
	}
	if (!catID.isEmpty() && catID.compare("-1"))
	{
		whereCategory = QString (" and Song.%1ID = %2").arg(category).arg(catID);
	}

	query = QString(SQL_ARTIST_OF_CAT_LIST_FROM_SONG);

	query = query
			.arg(whereArtist)
			.arg(whereGenre)
			.arg(whereComposer)
			.arg(whereFavorite)
			.arg(whereRating)
			.arg(column)
			.arg(increase)
			.arg(nStartIndex)
			.arg(nLimitCount)
			.arg(whereCategory);

//	LogDebug("query [%s]", query.toUtf8().data());

	return query;
}

QString SQLManager::GetQueryAlbumOfArtistOfCategoryListFromSong(int nCategory,
																int nSort,
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
	QString query;
	QString category = UtilNovatron::GetCategoryName(nCategory);
	QString column = GetColumnName(nSort);
	QString increase = GetIncrease(bIncrease);

	QString whereArtist = "";
	QString whereGenre = "";
	QString whereComposer = "";
	QString whereFavorite = "";
	QString whereRating = "";
	QString whereCategory = "";
	QString whereCategory2 = "";

	if (!artistID.isEmpty())
	{
		whereArtist = " and Song.ArtistID = " + artistID;
	}
	if (!genreID.isEmpty())
	{
		whereGenre = " and Song.GenreID = " + genreID;
	}
	if (!composerID.isEmpty())
	{
		whereComposer = " and Song.ComposerID = " + composerID;
	}
	if (nFavorite > 0)
	{
		whereFavorite = QString(" and %1.Favorite = %2").arg(category).arg(nFavorite);
	}
	if (nRating > 0)
	{
		whereFavorite = QString(" and %1.Rating = %2").arg(category).arg(nRating);
	}
	// genre id
	if (!catID.isEmpty() && catID.compare("-1"))
	{
		whereCategory = QString (" and Song.%1ID = %2").arg(category).arg(catID);
	}
	// artist id
	if (!catID2.isEmpty() && catID2.compare("-1"))
	{
		whereCategory2 = QString (" and Song.ArtistID = %1").arg(catID2);
	}

	query = QString(SQL_ALBUM_OF_ARTIST_CAT_LIST_FROM_SONG);

	query = query
			.arg(whereArtist)
			.arg(whereGenre)
			.arg(whereComposer)
			.arg(whereFavorite)
			.arg(whereRating)
			.arg(column)
			.arg(increase)
			.arg(nStartIndex)
			.arg(nLimitCount)
			.arg(whereCategory)
			.arg(whereCategory2)
			;

//	LogDebug("query [%s]", query.toUtf8().data());

	return query;
}

QString SQLManager::GetQueryCategoryOverview(int nID, int nCategory)
{
	QString query;

	switch (nCategory)
	{
	case CATEGORY_ALBUM:
		query = QString(SQL_ALBUM_OVERVIEW).arg(nID);
		break;
	case CATEGORY_ALBUM_ARTIST:
		query = QString(SQL_ALBUM_ARTIST_OVERVIEW).arg(nID);
		break;
	case CATEGORY_ARTIST:
		query = QString(SQL_ARTIST_OVERVIEW).arg(nID);
		break;
	case CATEGORY_COMPOSER:
		query = QString(SQL_COMPOSER_OVERVIEW).arg(nID);
		break;
	case CATEGORY_GENRE:
		query = QString(SQL_GENRE_OVERVIEW).arg(nID);
		break;
	case CATEGORY_MOOD:
		query = QString(SQL_MOOD_OVERVIEW).arg(nID);
		break;
	case CATEGORY_FOLDER:
		query = QString(SQL_FOLDER_OVERVIEW).arg(nID);
		break;
	case CATEGORY_YEAR:
		query = QString(SQL_YEAR_OVERVIEW).arg(nID);
		break;
	case CATEGORY_TRACK:
		query = SQL_TRACK_OVERVIEW;
		break;
	}

	return query;
}

QString SQLManager::GetQueryTrackList(int nID,
										int nCategory,
										int nSort,
										bool bIncrease,
										int nStartIndex,
										int nLimitCount)
{
	QString query;
	QString column = GetColumnName(nSort);
	QString increase = GetIncrease(bIncrease);

	switch (nCategory)
	{
	case CATEGORY_ALBUM:
		query = QString(SQL_ALBUM_TRACK_LIST)
				.arg(nID)
				.arg(column)
				.arg(increase)
				.arg(nStartIndex)
				.arg(nLimitCount);
		break;
	case CATEGORY_ALBUM_ARTIST:
		query = QString(SQL_ALBUM_ARTIST_TRACK_LIST)
				.arg(nID)
				.arg(column)
				.arg(increase)
				.arg(nStartIndex)
				.arg(nLimitCount);
		break;
	case CATEGORY_ARTIST:
		query = QString(SQL_ARTIST_TRACK_LIST)
				.arg(nID)
				.arg(column)
				.arg(increase)
				.arg(nStartIndex)
				.arg(nLimitCount);
		break;
	case CATEGORY_COMPOSER:
		query = QString(SQL_COMPOSER_TRACK_LIST)
				.arg(nID)
				.arg(column)
				.arg(increase)
				.arg(nStartIndex)
				.arg(nLimitCount);
		break;
	case CATEGORY_GENRE:
		query = QString(SQL_GENRE_TRACK_LIST)
				.arg(nID)
				.arg(column)
				.arg(increase)
				.arg(nStartIndex)
				.arg(nLimitCount);
		break;
	case CATEGORY_MOOD:
		query = QString(SQL_MOOD_TRACK_LIST)
				.arg(nID)
				.arg(column)
				.arg(increase)
				.arg(nStartIndex)
				.arg(nLimitCount);
		break;
	case CATEGORY_FOLDER:
		query = QString(SQL_FOLDER_TRACK_LIST)
				.arg(nID)
				.arg(column)
				.arg(increase)
				.arg(nStartIndex)
				.arg(nLimitCount);
		break;
	case CATEGORY_YEAR:
		query = QString(SQL_YEAR_TRACK_LIST)
				.arg(nID)
				.arg(column)
				.arg(increase)
				.arg(nStartIndex)
				.arg(nLimitCount);
		break;
	case CATEGORY_TRACK:
		query = QString(SQL_TRACK_LIST)
				.arg(column)
				.arg(increase)
				.arg(nStartIndex)
				.arg(nLimitCount);
		break;
	}

//	LogDebug("query [%s]", query.toUtf8().data());

	return query;
}

QString SQLManager::GetQueryTrackListOfAlbum(int nID,
											 int nCategory,
											 int nSort,
											 bool bIncrease,
											 int nStartIndex,
											 int nLimitCount,
											 QString catID)
{
	QString query;
	QString category = UtilNovatron::GetCategoryName(nCategory);
	QString column = GetColumnName(nSort);
	QString increase = GetIncrease(bIncrease);
	QString whereCategory = "";

	if (!catID.isEmpty() && catID.compare("-1"))
	{
		whereCategory = QString (" and Song.%1ID = %2").arg(category).arg(catID);
	}

	query = QString(SQL_TRACK_LIST_OF_ALBUM_OF_CAT_LIST);

	query = query
			.arg(nID)
			.arg(column)
			.arg(increase)
			.arg(nStartIndex)
			.arg(nLimitCount)
			.arg(whereCategory);

//	LogDebug("query [%s]", query.toUtf8().data());

	return query;
}

QString SQLManager::GetQueryTrackListOfAlbumOfArtist(int nID,
													 int nCategory,
													 int nSort,
													 bool bIncrease,
													 int nStartIndex,
													 int nLimitCount,
													 QString catID,
													 QString catID2)
{
	QString query;
	QString category = UtilNovatron::GetCategoryName(nCategory);
	QString column = GetColumnName(nSort);
	QString increase = GetIncrease(bIncrease);
	QString whereCategory = "";
	QString whereCategory2 = "";

	if (!catID.isEmpty() && catID.compare("-1"))
	{
		whereCategory = QString (" and Song.%1ID = %2").arg(category).arg(catID);
	}
	if (!catID2.isEmpty() && catID2.compare("-1"))
	{
//		whereCategory2 = QString (" and Song.%1ID = %2").arg(category).arg(catID2);
		whereCategory2 = QString (" and Song.ArtistID = %1").arg(catID2);
	}

	query = QString(SQL_TRACK_LIST_OF_ALBUM_OF_ARTIST_OF_CAT_LIST);

	query = query
			.arg(nID)
			.arg(column)
			.arg(increase)
			.arg(nStartIndex)
			.arg(nLimitCount)
			.arg(whereCategory)
			.arg(whereCategory2);

//	LogDebug("query [%s]", query.toUtf8().data());

	return query;
}

QString SQLManager::GetQueryPlaylist()
{
	return SQL_PLAYLIST;
}

QString SQLManager::GetQueryPlaylistInfo(int nID)
{
	return QString(SQL_PLAYLIST_OVERVIEW).arg(nID);
}

QString SQLManager::GetQueryPlaylistTrackList(int nID)
{
	return QString(SQL_PLAYLIST_TRACK_LIST).arg(nID);
}

QString SQLManager::GetQueryUpdateCatFavorite(int nID, int nFavorite, int nCategory)
{
	QString query;

	switch (nCategory)
	{
	case CATEGORY_ALBUM:
		query = QString(SQL_UPDATE_FAVORITE_OF_ALBUM).arg(nFavorite).arg(nID);
		break;
	case CATEGORY_ALBUM_ARTIST:
		query = QString(SQL_UPDATE_FAVORITE_OF_ALBUM_ARTIST).arg(nFavorite).arg(nID);
		break;
	case CATEGORY_ARTIST:
		query = QString(SQL_UPDATE_FAVORITE_OF_ARTIST).arg(nFavorite).arg(nID);
		break;
	case CATEGORY_COMPOSER:
		query = QString(SQL_UPDATE_FAVORITE_OF_COMPOSER).arg(nFavorite).arg(nID);
		break;
	case CATEGORY_GENRE:
		query = QString(SQL_UPDATE_FAVORITE_OF_GENRE).arg(nFavorite).arg(nID);
		break;
	case CATEGORY_MOOD:
		query = QString(SQL_UPDATE_FAVORITE_OF_MOOD).arg(nFavorite).arg(nID);
		break;
	case CATEGORY_FOLDER:
		query = QString(SQL_UPDATE_FAVORITE_OF_FOLDER).arg(nFavorite).arg(nID);
		break;

	default:
		break;
	}
	return query;

}

QString SQLManager::GetQueryUpdateCatRating(int nID, int nRating, int nCategory)
{
	QString query;

	switch (nCategory)
	{
	case CATEGORY_ALBUM:
		query = QString(SQL_UPDATE_RATING_OF_ALBUM).arg(nRating).arg(nID);
		break;
	case CATEGORY_ALBUM_ARTIST:
		query = QString(SQL_UPDATE_RATING_OF_ALBUM_ARTIST).arg(nRating).arg(nID);
		break;
	case CATEGORY_ARTIST:
		query = QString(SQL_UPDATE_RATING_OF_ARTIST).arg(nRating).arg(nID);
		break;
	case CATEGORY_COMPOSER:
		query = QString(SQL_UPDATE_RATING_OF_COMPOSER).arg(nRating).arg(nID);
		break;
	case CATEGORY_GENRE:
		query = QString(SQL_UPDATE_RATING_OF_GENRE).arg(nRating).arg(nID);
		break;
	case CATEGORY_MOOD:
		query = QString(SQL_UPDATE_RATING_OF_MOOD).arg(nRating).arg(nID);
		break;
	case CATEGORY_FOLDER:
		query = QString(SQL_UPDATE_RATING_OF_FOLDER).arg(nRating).arg(nID);
		break;

	default:
		break;
	}
	return query;
}

QString SQLManager::GetQueryUpdateTrackFavorite(int nID, int nFavorite)
{
	return QString(SQL_UPDATE_FAVORITE_OF_SONG).arg(nFavorite).arg(nID);
}

QString SQLManager::GetQueryClassifyArtist(int nCategory)
{
	QString query;

	switch (nCategory)
	{
	case CATEGORY_ARTIST:
		query = QString(SQL_CLASSIFY).arg("Artist");
		break;
	case CATEGORY_GENRE:
		query = QString(SQL_CLASSIFY).arg("Genre");
		break;
	case CATEGORY_COMPOSER:
		query = QString(SQL_CLASSIFY).arg("Composer");
		break;
	}

	return query;
}

QString SQLManager::GetQueryCategoryList(int nCategory)
{
	QString query;

	switch (nCategory)
	{
	case CATEGORY_ALBUM:
		query = SQL_CATEGORY_ALBUM_LIST;
		break;
	case CATEGORY_ALBUM_ARTIST:
		query = SQL_CATEGORY_ALBUM_ARTIST_LIST;
		break;
	case CATEGORY_ARTIST:
		query = SQL_CATEGORY_ARTIST_LIST;
		break;
	case CATEGORY_GENRE:
		query = SQL_CATEGORY_GENRE_LIST;
		break;
	case CATEGORY_COMPOSER:
		query = SQL_CATEGORY_COMPOSER_LIST;
		break;
	case CATEGORY_MOOD:
		query = SQL_CATEGORY_MOOD_LIST;
		break;
	}

	return query;
}

QString SQLManager::GetColumnName(int nSort)
{
	QString sort;

	switch (nSort)
	{
	case SORT_IMPORTED_DATE:
		sort = "rowid";
		break;
	case SORT_ALPHABET:
		sort = "name";
		break;
	case SORT_FAVORITE:
		sort = "favorite";
		break;
	case SORT_RATING:
		sort = "rating";
		break;
	default:
		sort = "rowid";
	}

	return sort;

}

QString SQLManager::GetIncrease(bool bIncrease)
{
	QString increase;
	if (bIncrease)
	{
		increase = "asc";
	}
	else
	{
		increase = "desc";
	}

	return increase;
}

QString SQLManager::GetQuerySearchList(int nCategory, QString keyword)
{
	QString query;

	switch (nCategory)
	{
	case CATEGORY_ALBUM:
		query = QString(SQL_SEARCH_ALBUM_LIST).arg(keyword);
		break;
	case CATEGORY_ALBUM_ARTIST:
		query = QString(SQL_SEARCH_ALBUM_LIST).arg(keyword);
		break;
	case CATEGORY_ARTIST:
		query = QString(SQL_SEARCH_ARTIST_LIST).arg(keyword);
		break;
	case CATEGORY_GENRE:
		query = QString(SQL_SEARCH_ALBUM_LIST).arg(keyword);
		break;
	case CATEGORY_COMPOSER:
		query = QString(SQL_SEARCH_ALBUM_LIST).arg(keyword);
		break;
	case CATEGORY_MOOD:
		query = QString(SQL_SEARCH_ALBUM_LIST).arg(keyword);
		break;
	case CATEGORY_TRACK:
		query = QString(SQL_SEARCH_TRACK_LIST).arg(keyword);
		break;
	}

	return query;
}

QString SQLManager::GetQueryQueueCategoryInfo(int nID)
{
	QString query;

	query = QString(SQL_QUEUE_CATEGORY_INFO).arg(nID);

	return query;
}

//QString SQLManager::GetQueryCheckCategory(int updateCategory, QString updateName)
//{
//	QString query;

//	switch (updateCategory)
//	{
//	case CATEGORY_ALBUM:
//		query = QString(SQL_CHECK_ALBUM).arg(updateName);
//		break;
//	case CATEGORY_ALBUM_ARTIST:
//		query = QString(SQL_CHECK_ALBUM).arg(updateName);
//		break;
//	case CATEGORY_ARTIST:
//		query = QString(SQL_CHECK_ARTIST).arg(updateName);
//		break;
//	case CATEGORY_GENRE:
//		query = QString(SQL_CHECK_GENRE).arg(updateName);
//		break;
//	case CATEGORY_COMPOSER:
//		query = QString(SQL_CHECK_ALBUM).arg(updateName);
//		break;
//	case CATEGORY_MOOD:
//		query = QString(SQL_CHECK_ALBUM).arg(updateName);
//		break;
//	case CATEGORY_TRACK:
//		query = QString(SQL_CHECK_ALBUM).arg(updateName);
//		break;
//	}

//	return query;
//}

//QString SQLManager::GetQueryInsertCategory(int updateCategory, QString updateName)
//{
//	QString query;

//	switch (updateCategory)
//	{
//	case CATEGORY_ALBUM:
//		query = QString(SQL_INSERT_ALBUM).arg(updateName);
//		break;
//	case CATEGORY_ALBUM_ARTIST:
//		query = QString(SQL_INSERT_ALBUM).arg(updateName);
//		break;
//	case CATEGORY_ARTIST:
//		query = QString(SQL_INSERT_ARTIST).arg(updateName);
//		break;
//	case CATEGORY_GENRE:
//		query = QString(SQL_INSERT_GENRE).arg(updateName);
//		break;
//	case CATEGORY_COMPOSER:
//		query = QString(SQL_INSERT_ALBUM).arg(updateName);
//		break;
//	case CATEGORY_MOOD:
//		query = QString(SQL_INSERT_ALBUM).arg(updateName);
//		break;
//	case CATEGORY_TRACK:
//		query = QString(SQL_INSERT_ALBUM).arg(updateName);
//		break;
//	}

//	return query;
//}

//QString SQLManager::GetQueryUpdateCategory(int id, int category, int updateCategory, int updateId)
//{
//	QString query;

//	QString categoryName = UtilNovatron::GetCategoryName(category);


//	switch (updateCategory)
//	{
//	case CATEGORY_ALBUM:
//		query = QString(SQL_UPDATE_ALBUM).arg(updateId).arg(categoryName).arg(id);
//		break;
//	case CATEGORY_ALBUM_ARTIST:
//		query = QString(SQL_UPDATE_ALBUM).arg(updateId).arg(categoryName).arg(id);
//		break;
//	case CATEGORY_ARTIST:
//		query = QString(SQL_UPDATE_ARTIST).arg(updateId).arg(categoryName).arg(id);
//		break;
//	case CATEGORY_GENRE:
//		query = QString(SQL_UPDATE_GENRE).arg(updateId).arg(categoryName).arg(id);
//		break;
//	case CATEGORY_COMPOSER:
//		query = QString(SQL_UPDATE_ALBUM).arg(updateId).arg(categoryName).arg(id);
//		break;
//	case CATEGORY_MOOD:
//		query = QString(SQL_UPDATE_ALBUM).arg(updateId).arg(categoryName).arg(id);
//		break;
//	case CATEGORY_TRACK:
//		query = QString(SQL_UPDATE_ALBUM).arg(updateId).arg(categoryName).arg(id);
//		break;
//	}

//	return query;
//}

//QString SQLManager::GetQueryInsertIgnoreCategoryAll(int updateCategory, QString updateName)
//{
//	QString query;

//	switch (updateCategory)
//	{
//	case CATEGORY_ALBUM:
//		query = QString(SQL_INSERT_OR_IGNORE_ALBUM).arg(updateName);
//		break;
//	case CATEGORY_ALBUM_ARTIST:
//		query = QString(SQL_INSERT_OR_IGNORE_ALBUM_ARTIST).arg(updateName);
//		break;
//	case CATEGORY_ARTIST:
//		query = QString(SQL_INSERT_OR_IGNORE_ARTIST).arg(updateName);
//		break;
//	case CATEGORY_GENRE:
//		query = QString(SQL_INSERT_OR_IGNORE_GENRE).arg(updateName);
//		break;
//	case CATEGORY_COMPOSER:
//		query = QString(SQL_INSERT_OR_IGNORE_COMPOSER).arg(updateName);
//		break;
//	case CATEGORY_MOOD:
//		query = QString(SQL_INSERT_OR_IGNORE_MOOD).arg(updateName);
//		break;
//	}

//	return query;
//}

//QString SQLManager::GetQueryUpdateCategoryAll(QString id,
//											  QString name,
//											  QString favorite,
//											  QString artist,
//											  QString album,
//											  QString genre,
//											  QString albumArtist,
//											  QString composer,
//											  QString year,
//											  QString mood)
//{
//	QString query;

//	query = QString(SQL_UPDATE_ALL)
//			.arg(id)
//			.arg(name)
//			.arg(favorite)
//			.arg(artist)
//			.arg(album)
//			.arg(genre)
//			.arg(albumArtist)
//			.arg(composer)
//			.arg(year)
//			.arg(mood);


//	return query;
//}
