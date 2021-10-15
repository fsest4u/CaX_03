#include "sqlmanager.h"

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
										 QString composerID)
{
	// todo-dylee
	Q_UNUSED(nSort)
	QString query;
	QString sort = GetSort(nSort);
	QString increase = GetIncrease(bIncrease);

	QString whereArtist = "";
	QString whereGenre = "";
	QString whereComposer = "";

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

	switch (nCategory)
	{
	case CATEGORY_ALBUM:
		query = QString(SQL_ALBUM_LIST)
				.arg(whereArtist)
				.arg(whereGenre)
				.arg(whereComposer)
				.arg(sort)
				.arg(increase);
		break;
	case CATEGORY_ARTIST:
		query = QString(SQL_ARTIST_LIST)
				.arg(whereArtist)
				.arg(whereGenre)
				.arg(whereComposer)
				.arg(sort)
				.arg(increase);
		break;
	case CATEGORY_COMPOSER:
		query = QString(SQL_COMPOSER_LIST)
				.arg(whereArtist)
				.arg(whereGenre)
				.arg(whereComposer)
				.arg(sort)
				.arg(increase);
		break;
	case CATEGORY_GENRE:
		query = QString(SQL_GENRE_LIST)
				.arg(whereArtist)
				.arg(whereGenre)
				.arg(whereComposer)
				.arg(sort)
				.arg(increase);
		break;
	case CATEGORY_MOOD:
		query = QString(SQL_MOOD_LIST)
				.arg(whereArtist)
				.arg(whereGenre)
				.arg(whereComposer)
				.arg(sort)
				.arg(increase);
		break;
	case CATEGORY_FOLDER:
		query = SQL_FOLDER_LIST;
		query = QString(SQL_ALBUM_LIST)
				.arg(whereArtist)
				.arg(whereGenre)
				.arg(whereComposer)
				.arg(sort)
				.arg(increase);
		break;
	case CATEGORY_YEAR:
		query = QString(SQL_YEAR_LIST)
				.arg(whereArtist)
				.arg(whereGenre)
				.arg(whereComposer)
				.arg(sort)
				.arg(increase);
		break;
	}

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
	QString sort = GetSort(nSort);
	QString increase = GetIncrease(bIncrease);

	switch (nCategory)
	{
	case CATEGORY_ALBUM:
		query = QString(SQL_SONGS_OF_ALBUM)
				.arg(nID)
				.arg(sort)
				.arg(increase);
		break;
	case CATEGORY_ARTIST:
		query = QString(SQL_SONGS_OF_ARTIST)
				.arg(nID)
				.arg(sort)
				.arg(increase);
		break;
	case CATEGORY_COMPOSER:
		query = QString(SQL_SONGS_OF_COMPOSER)
				.arg(nID)
				.arg(sort)
				.arg(increase);
		break;
	case CATEGORY_GENRE:
		query = QString(SQL_SONGS_OF_GENRE)
				.arg(nID)
				.arg(sort)
				.arg(increase);
		break;
	case CATEGORY_MOOD:
		query = QString(SQL_SONGS_OF_MOOD)
				.arg(nID)
				.arg(sort)
				.arg(increase);
		break;
	case CATEGORY_FOLDER:
		query = QString(SQL_SONGS_OF_FOLDER)
				.arg(nID)
				.arg(sort)
				.arg(increase);
		break;
	case CATEGORY_YEAR:
		query = QString(SQL_SONGS_OF_YEAR)
				.arg(nID)
				.arg(sort)
				.arg(increase);
		break;
	case CATEGORY_TRACK:
		query = QString(SQL_SONGS_OF_TRACK)
				.arg(sort)
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

QString SQLManager::GetQueryFavorite(int nID, int nFavorite, int nCategory)
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

QString SQLManager::GetQueryRating(int nID, int nRating, int nCategory)
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

QString SQLManager::GetSort(int nSort)
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
