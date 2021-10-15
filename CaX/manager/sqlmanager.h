#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <QObject>

class SQLManager : public QObject
{
	Q_OBJECT
public:
	explicit SQLManager(QObject *parent = nullptr);

	enum {
		CATEGORY_ALBUM = 0,
		CATEGORY_ALBUMARTIST,
		CATEGORY_ARTIST,
		CATEGORY_COMPOSER,
		CATEGORY_GENRE,
		CATEGORY_MOOD,
		CATEGORY_FOLDER,
		CATEGORY_YEAR,
		CATEGORY_TRACK,
		CATEGORY_MAX
	};

	enum {
		SORT_NAME = 0,
		SORT_ALBUM,
		SORT_ALBUM_ARTIST,
		SORT_ARTIST,
		SORT_GENRE,
		SORT_COMPOSER,
		SORT_YEAR,
		SORT_MAX
	};

	QString GetQueryMusicDBInfo();
	QString GetQueryCategoryList(int nCategory = CATEGORY_ALBUM,
								 int nSort = SORT_NAME,
								 bool bIncrease = true,
								 QString artistID = "",
								 QString genreID = "",
								 QString composerID = "");
	QString GetQueryCategoryInfo(int nID, int nCategory = CATEGORY_ALBUM);
	QString GetQuerySongsOfCategory(int nID,
									int nCategory = CATEGORY_ALBUM,
									int nSort = SORT_NAME,
									bool bIncrease = true);

	QString GetQueryPlaylist();
	QString GetQueryPlaylistInfo(int nID);
	QString GetQuerySongsOfPlaylist(int nID);

	QString GetQueryFavorite(int nID, int nFavorite, int nCategory = CATEGORY_ALBUM);
	QString GetQueryRating(int nID, int nRating, int nCategory = CATEGORY_ALBUM);

	QString GetQueryClassifyArtist(int nCategory);

	QString GetSort(int nSort);
	QString GetIncrease(bool bIncrease);


signals:

};

#endif // SQLMANAGER_H
