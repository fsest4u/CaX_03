#ifndef MUSICDBMANAGER_H
#define MUSICDBMANAGER_H

#include <QObject>

#include "basemanager.h"
#include "sqlmanager.h"

#include "util/CJsonNode.h"
#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"


class SQLManager;

class MusicDBManager : public BaseManager
{
	Q_OBJECT
public:
	explicit MusicDBManager(QObject *parent = nullptr);
	~MusicDBManager();

	enum {
		MUSICDB_INFO = 0,
		MUSICDB_CATEGORY_LIST,
		MUSICDB_CATEGORY_INFO,
		MUSICDB_SONGS_OF_CATEGORY,
//		MUSICDB_PLAY_CATEGORY_ITEMS,
//		MUSICDB_PLAY_CATEGORY_ITEM,
//		MUSICDB_PLAY_SONG,
		MUSICDB_MANAGE_CATEGORY,
		MUSICDB_UPDATE_CATEGORY_FAVORITE,
		MUSICDB_UPDATE_CATEGORY_RATING,
		MUSICDB_UPDATE_TRACK_FAVORITE,
		MUSICDB_CLASSIFY_ARTIST,
		MUSICDB_CLASSIFY_GENRE,
		MUSICDB_CLASSIFY_COMPOSER,
		MUSICDB_RANDOM,
		MUSICDB_MAX
	};

	void RequestMusicDBInfo();
	void RequestCategoryList(int nCategory = SQLManager::CATEGORY_ALBUM,
							 int nSort = SQLManager::SORT_NAME,
							 bool bIncrease = true,
							 QString artistID = "",
							 QString genreID = "",
							 QString composerID = "",
							 int nFavorite = 0,
							 int nRating = 0,
							 int nStartIndex = 0,
							 int nLimitCount = 100);
	void RequestCategoryInfo(int nID, int nCategory = SQLManager::CATEGORY_ALBUM);
	void RequestSongsOfCategory(int nID,
								int nCategory = SQLManager::CATEGORY_ALBUM,
								int nSort = SQLManager::SORT_NAME,
								bool bIncrease = true,
								int nStartIndex = 0,
								int nLimitCount = 100);

	void RequestManageCategory(QString cmd1,
							   QMap<int, bool> idMap,
							   int nWhere = PLAY_NONE,
							   int nCategory = SQLManager::CATEGORY_ALBUM,
							   int eventID = -1);

	CJsonNode MakeNodeYear(QString cmd1, int nWhere, QString strCat, int eventID = -1);
	CJsonNode MakeNodeYearSelect(QString cmd1, QMap<int, bool> idMap, int nWhere, int eventID = -1);
	CJsonNode MakeNodeTrack(QString cmd1, int nWhere, int eventID = -1);
	CJsonNode MakeNodeTrackSelect(QString cmd1, QMap<int, bool> idMap, int nWhere, int eventID = -1);
	CJsonNode MakeNodeCategory(QString cmd1, int nWhere, QString strCat, int eventID = -1);
	CJsonNode MakeNodeCategorySelect(QString cmd1, QMap<int, bool> idMap, int nWhere, QString strCat, int eventID = -1);

	void RequestUpdateFavorite(int nID, int nFavorite, int nCategory = SQLManager::CATEGORY_ALBUM);
	void RequestUpdateRating(int nID, int nRating, int nCategory = SQLManager::CATEGORY_ALBUM);

	void RequestUpdateTrackFavorite(int nID, int nFavorite);

	void RequestClassifyList(int nCategory);

	void RequestRandom();

//	int GetCntAlbum() const { return m_CntAlbum; }
//	int GetCntArtist() const { return m_CntArtist; }
//	int GetCntComposer() const { return m_CntComposer; }
//	int GetCntGenre() const { return m_CntGenre; }
//	int GetCntMood() const { return m_CntMood; }
//	int GetCntFolder() const { return m_CntFolder; }
//	int GetCntYear() const { return m_CntYear; }
//	int GetCntTrack() const { return m_CntTrack; }

	SQLManager *GetSqlMgr() const;
	void SetSqlMgr(SQLManager *pSql);

signals:

	void SigRespMusicInfo(CJsonNode node);
	void SigRespCategoryList(QList<CJsonNode> nodeList);
	void SigRespCategoryInfo(CJsonNode node);
	void SigRespSongsOfCategory(QList<CJsonNode> nodeList);
	void SigRespClassifyArtist(QList<CJsonNode> list);
	void SigRespClassifyGenre(QList<CJsonNode> list);
	void SigRespClassifyComposer(QList<CJsonNode> list);


	void SigCoverArtUpdate(QString fileName, int nIndex, int mode);

	void SigRespError(QString errMsg);

private slots:

	void SlotRespInfo(QString json, int nCmdID);
	void SlotRespCoverArt(QString fileName, int nIndex, int mode);

private:

	void InitMusic();

	void ParseMusicInfo(CJsonNode result);
	void ParseCategoryList(CJsonNode result);
	void ParseCategoryInfo(CJsonNode result);
	void ParseSongsOfCategory(CJsonNode result);
	void ParseClassifyArtist(CJsonNode result);
	void ParseClassifyGenre(CJsonNode result);
	void ParseClassifyComposer(CJsonNode result);

	QList<CJsonNode> ParseResultNode(CJsonNode result);



	SQLManager	*m_pSql;

	QList<CJsonNode>	m_NodeList;
	CJsonNode			m_Node;

};

#endif // MUSICDBMANAGER_H
