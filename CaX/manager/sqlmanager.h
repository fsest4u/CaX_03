#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <QObject>

#define CATEGORY_NONE	2000
#define SORT_NONE		3000

class SQLManager : public QObject
{
	Q_OBJECT
public:
	explicit SQLManager(QObject *parent = nullptr);

	enum categoryType {
		CATEGORY_ALBUM = Qt::UserRole + CATEGORY_NONE,
		CATEGORY_ALBUMARTIST,
		CATEGORY_ARTIST,
		CATEGORY_COMPOSER,
		CATEGORY_GENRE,
		CATEGORY_MOOD,
		CATEGORY_FOLDER,
		CATEGORY_YEAR,
		CATEGORY_MAX
	};

	enum sortType {
		SORT_TRACK = Qt::UserRole + SORT_NONE,
		SORT_GENRE,
		SORT_MOOD,
		SORT_FOLDER,
		SORT_YEAR,
		SORT_RATING,
		SORT_SAMPLE_RATE,
		SORT_MAX
	};

	QString GetQueryMyMusicInfo();
	QString GetQueryCategoryList(int nCategory = CATEGORY_ALBUM, int nSort = SORT_TRACK);
	QString GetQueryCategoryInfo(int nID, int nCategory = CATEGORY_ALBUM);
	QString GetQuerySongsOfCategory(int nID, int nCategory = CATEGORY_ALBUM, int nSort = SORT_TRACK);

	QString GetQueryPlaylist();
	QString GetQueryPlaylistInfo(int nID);
	QString GetQuerySongsOfPlaylist(int nID);

	QString GetQueryFavorite(int nID, int nFavorite, int nCategory = CATEGORY_ALBUM);
	QString GetQueryRating(int nID, int nRating, int nCategory = CATEGORY_ALBUM);


signals:

};

#endif // SQLMANAGER_H
