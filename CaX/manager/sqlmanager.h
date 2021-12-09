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
		SORT_MOOD,
		SORT_FOLDER,
		SORT_YEAR,
		SORT_MAX
	};

	QString GetQueryMusicDBOverview();
	QString GetQueryMusicDBCategoryList(int nCategory = CATEGORY_ALBUM,
								 int nSort = SORT_NAME,
								 bool bIncrease = true,
								 QString artistID = "",
								 QString genreID = "",
								 QString composerID = "",
								 int nFavorite = 0,
								 int nRating = 0,
								 int nStartIndex = 0,
								 int nLimitCount = 100);
	QString GetQueryMusicDBCategoryOverview(int nID, int nCategory = CATEGORY_ALBUM);
	QString GetQueryMusicDBTrackList(int nID,
									int nCategory = CATEGORY_ALBUM,
									int nSort = SORT_NAME,
									bool bIncrease = true,
									int nStartIndex = 0,
									int nLimitCount = 100);

	QString GetQueryPlaylist();
	QString GetQueryPlaylistInfo(int nID);
	QString GetQueryPlaylistTrackList(int nID);

	QString GetQueryUpdateCatFavorite(int nID, int nFavorite, int nCategory = CATEGORY_ALBUM);
	QString GetQueryUpdateCatRating(int nID, int nRating, int nCategory = CATEGORY_ALBUM);
	QString GetQueryUpdateTrackFavorite(int nID, int nFavorite);

	QString GetQueryClassifyArtist(int nCategory);
	QString GetQueryCategoryList(int nCategory);

	QString GetCategoryName(int nCategory);
	QString GetColumnName(int nSort);
	QString GetIncrease(bool bIncrease);

	QString GetQuerySearchList(int nCategory, QString keyword);
	QString GetQueryQueueCategoryInfo(int nID);


signals:

};

#endif // SQLMANAGER_H
