#include "sqlmanager.h"

#include "util/caxkeyvalue.h"
#include "util/sqlconstatns.h"

SQLManager::SQLManager(QObject *parent)
	: QObject(parent)
{

}

QString SQLManager::GetQueryMusicDBOverview()
{
	return SQL_MUSIC_DB_OVERVIEW;
}

QString SQLManager::GetQueryMusicDBCategoryList(int nCategory,
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
	QString category = GetCategoryName(nCategory);
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
		query = QString(SQL_ALBUM_LIST);
		break;
	case CATEGORY_ARTIST:
		query = QString(SQL_ARTIST_LIST);
		break;
	case CATEGORY_COMPOSER:
		query = QString(SQL_COMPOSER_LIST);
		break;
	case CATEGORY_GENRE:
		query = QString(SQL_GENRE_LIST);
		break;
	case CATEGORY_MOOD:
		query = QString(SQL_MOOD_LIST);
		break;
	case CATEGORY_FOLDER:
		query = QString(SQL_FOLDER_LIST);
		break;
	case CATEGORY_YEAR:
		query = QString(SQL_YEAR_LIST);
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

	return query;
}

QString SQLManager::GetQueryMusicDBCategoryOverview(int nID, int nCategory)
{
	QString query;

	switch (nCategory)
	{
	case CATEGORY_ALBUM:
		query = QString(SQL_ALBUM_OVERVIEW).arg(nID);
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

QString SQLManager::GetQueryMusicDBTrackList(int nID,
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
	case CATEGORY_ALBUMARTIST:
		query = QString(SQL_UPDATE_FAVORITE_OF_ALBUMARTIST).arg(nFavorite).arg(nID);
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
	case CATEGORY_ALBUMARTIST:
		query = QString(SQL_UPDATE_RATING_OF_ALBUMARTIST).arg(nRating).arg(nID);
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
	case CATEGORY_ALBUMARTIST:
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

QString SQLManager::GetCategoryName(int nCategory)
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

QString SQLManager::GetColumnName(int nSort)
{
	QString sort;

	switch (nSort)
	{
	case SORT_NAME:
		sort = "Name";
		break;
	case SORT_ALBUM:
		sort = "AlbumID";
		break;
	case SORT_ALBUM_ARTIST:
		sort = "AlbumArtistID";
		break;
	case SORT_ARTIST:
		sort = "ArtistID";
		break;
	case SORT_GENRE:
		sort = "GenreID";
		break;
	case SORT_COMPOSER:
		sort = "ComposerID";
		break;
	case SORT_MOOD:
		sort = "MoodID";
		break;
	case SORT_FOLDER:
		sort = "FolderID";
		break;
	case SORT_YEAR:
		sort = "Year";
		break;
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
	case CATEGORY_ALBUMARTIST:
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

QString SQLManager::GetQueryCheckCategory(int updateCategory, QString updateName)
{
	QString query;

	switch (updateCategory)
	{
	case CATEGORY_ALBUM:
		query = QString(SQL_CHECK_ALBUM).arg(updateName);
		break;
	case CATEGORY_ALBUMARTIST:
		query = QString(SQL_CHECK_ALBUM).arg(updateName);
		break;
	case CATEGORY_ARTIST:
		query = QString(SQL_CHECK_ARTIST).arg(updateName);
		break;
	case CATEGORY_GENRE:
		query = QString(SQL_CHECK_GENRE).arg(updateName);
		break;
	case CATEGORY_COMPOSER:
		query = QString(SQL_CHECK_ALBUM).arg(updateName);
		break;
	case CATEGORY_MOOD:
		query = QString(SQL_CHECK_ALBUM).arg(updateName);
		break;
	case CATEGORY_TRACK:
		query = QString(SQL_CHECK_ALBUM).arg(updateName);
		break;
	}

	return query;
}

QString SQLManager::GetQueryInsertCategory(int updateCategory, QString updateName)
{
	QString query;

	switch (updateCategory)
	{
	case CATEGORY_ALBUM:
		query = QString(SQL_INSERT_ALBUM).arg(updateName);
		break;
	case CATEGORY_ALBUMARTIST:
		query = QString(SQL_INSERT_ALBUM).arg(updateName);
		break;
	case CATEGORY_ARTIST:
		query = QString(SQL_INSERT_ARTIST).arg(updateName);
		break;
	case CATEGORY_GENRE:
		query = QString(SQL_INSERT_GENRE).arg(updateName);
		break;
	case CATEGORY_COMPOSER:
		query = QString(SQL_INSERT_ALBUM).arg(updateName);
		break;
	case CATEGORY_MOOD:
		query = QString(SQL_INSERT_ALBUM).arg(updateName);
		break;
	case CATEGORY_TRACK:
		query = QString(SQL_INSERT_ALBUM).arg(updateName);
		break;
	}

	return query;
}

QString SQLManager::GetQueryUpdateCategory(int id, int category, int updateCategory, int updateId)
{
	QString query;

	QString categoryName = GetCategoryName(category);


	switch (updateCategory)
	{
	case CATEGORY_ALBUM:
		query = QString(SQL_UPDATE_ALBUM).arg(updateId).arg(categoryName).arg(id);
		break;
	case CATEGORY_ALBUMARTIST:
		query = QString(SQL_UPDATE_ALBUM).arg(updateId).arg(categoryName).arg(id);
		break;
	case CATEGORY_ARTIST:
		query = QString(SQL_UPDATE_ARTIST).arg(updateId).arg(categoryName).arg(id);
		break;
	case CATEGORY_GENRE:
		query = QString(SQL_UPDATE_GENRE).arg(updateId).arg(categoryName).arg(id);
		break;
	case CATEGORY_COMPOSER:
		query = QString(SQL_UPDATE_ALBUM).arg(updateId).arg(categoryName).arg(id);
		break;
	case CATEGORY_MOOD:
		query = QString(SQL_UPDATE_ALBUM).arg(updateId).arg(categoryName).arg(id);
		break;
	case CATEGORY_TRACK:
		query = QString(SQL_UPDATE_ALBUM).arg(updateId).arg(categoryName).arg(id);
		break;
	}

	return query;
}
