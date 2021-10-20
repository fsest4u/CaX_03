#include "sqlmanager.h"

#include "util/caxkeyvalue.h"
#include "util/sqlconstatns.h"

SQLManager::SQLManager(QObject *parent)
	: QObject(parent)
{

}

QString SQLManager::GetQueryMusicDBInfo()
{
	return SQL_MUSIC_DB_INFO;
}

QString SQLManager::GetQueryCategoryList(int nCategory,
										 int nSort,
										 bool bIncrease,
										 QString artistID,
										 QString genreID,
										 QString composerID,
										 int nFavorite,
										 int nRating)
{
	// todo-dylee
	Q_UNUSED(nSort)
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
			.arg(increase);

	return query;
}

QString SQLManager::GetQueryCategoryInfo(int nID, int nCategory)
{
	QString query;

	switch (nCategory)
	{
	case CATEGORY_ALBUM:
		query = QString(SQL_ALBUM_INFO).arg(nID);
		break;
	case CATEGORY_ARTIST:
		query = QString(SQL_ARTIST_INFO).arg(nID);
		break;
	case CATEGORY_COMPOSER:
		query = QString(SQL_COMPOSER_INFO).arg(nID);
		break;
	case CATEGORY_GENRE:
		query = QString(SQL_GENRE_INFO).arg(nID);
		break;
	case CATEGORY_MOOD:
		query = QString(SQL_MOOD_INFO).arg(nID);
		break;
	case CATEGORY_FOLDER:
		query = QString(SQL_FOLDER_INFO).arg(nID);
		break;
	case CATEGORY_YEAR:
		query = QString(SQL_YEAR_INFO).arg(nID);
		break;
	case CATEGORY_TRACK:
		query = SQL_TRACK_INFO;
		break;
	}

	return query;
}

QString SQLManager::GetQuerySongsOfCategory(int nID,
											int nCategory,
											int nSort,
											bool bIncrease)
{
	QString query;
	QString column = GetColumnName(nSort);
	QString increase = GetIncrease(bIncrease);

	switch (nCategory)
	{
	case CATEGORY_ALBUM:
		query = QString(SQL_SONGS_OF_ALBUM)
				.arg(nID)
				.arg(column)
				.arg(increase);
		break;
	case CATEGORY_ARTIST:
		query = QString(SQL_SONGS_OF_ARTIST)
				.arg(nID)
				.arg(column)
				.arg(increase);
		break;
	case CATEGORY_COMPOSER:
		query = QString(SQL_SONGS_OF_COMPOSER)
				.arg(nID)
				.arg(column)
				.arg(increase);
		break;
	case CATEGORY_GENRE:
		query = QString(SQL_SONGS_OF_GENRE)
				.arg(nID)
				.arg(column)
				.arg(increase);
		break;
	case CATEGORY_MOOD:
		query = QString(SQL_SONGS_OF_MOOD)
				.arg(nID)
				.arg(column)
				.arg(increase);
		break;
	case CATEGORY_FOLDER:
		query = QString(SQL_SONGS_OF_FOLDER)
				.arg(nID)
				.arg(column)
				.arg(increase);
		break;
	case CATEGORY_YEAR:
		query = QString(SQL_SONGS_OF_YEAR)
				.arg(nID)
				.arg(column)
				.arg(increase);
		break;
	case CATEGORY_TRACK:
		query = QString(SQL_SONGS_OF_TRACK)
				.arg(column)
				.arg(increase);
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
	return QString(SQL_PLAYLIST_INFO).arg(nID);
}

QString SQLManager::GetQuerySongsOfPlaylist(int nID)
{
	return QString(SQL_SONGS_OF_PLAYLIST).arg(nID);
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
