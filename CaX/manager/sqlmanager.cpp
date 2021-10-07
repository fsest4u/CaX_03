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

QString SQLManager::GetQueryCategoryList(int nCategory, int nSort)
{
	// todo-dylee
	Q_UNUSED(nSort)

	QString query;

	switch (nCategory)
	{
	case CATEGORY_ALBUM:
		query = SQL_ALBUM_LIST;
		break;
	case CATEGORY_ARTIST:
		query = SQL_ARTIST_LIST;
		break;
	case CATEGORY_COMPOSER:
		query = SQL_COMPOSER_LIST;
		break;
	case CATEGORY_GENRE:
		query = SQL_GENRE_LIST;
		break;
	case CATEGORY_MOOD:
		query = SQL_MOOD_LIST;
		break;
	case CATEGORY_FOLDER:
		query = SQL_FOLDER_LIST;
		break;
	case CATEGORY_YEAR:
		query = SQL_YEAR_LIST;
		break;
	default:
		query = "";
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
	default:
		query = SQL_TRACK_INFO;
		break;
	}

	return query;
}

QString SQLManager::GetQuerySongsOfCategory(int nID, int nCategory, int nSort)
{
	// todo-dylee
	Q_UNUSED(nSort)

	QString query;

	switch (nCategory)
	{
	case CATEGORY_ALBUM:
		query = QString(SQL_SONGS_OF_ALBUM).arg(nID);
		break;
	case CATEGORY_ARTIST:
		query = QString(SQL_SONGS_OF_ARTIST).arg(nID);
		break;
	case CATEGORY_COMPOSER:
		query = QString(SQL_SONGS_OF_COMPOSER).arg(nID);
		break;
	case CATEGORY_GENRE:
		query = QString(SQL_SONGS_OF_GENRE).arg(nID);
		break;
	case CATEGORY_MOOD:
		query = QString(SQL_SONGS_OF_MOOD).arg(nID);
		break;
	case CATEGORY_FOLDER:
		query = QString(SQL_SONGS_OF_FOLDER).arg(nID);
		break;
	case CATEGORY_YEAR:
		query = QString(SQL_SONGS_OF_YEAR).arg(nID);
		break;
	default:
		query = SQL_SONGS_OF_TRACK;
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
