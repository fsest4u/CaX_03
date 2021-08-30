#ifndef MYMUSICMANAGER_H
#define MYMUSICMANAGER_H

#include <QObject>

#include "basemanager.h"
#include "sqlmanager.h"

#include "util/CJsonNode.h"
#include "util/caxconstants.h"

#define	MUSIC_NONE	1000

class SQLManager;

class MyMusicManager : public BaseManager
{
	Q_OBJECT
public:
	explicit MyMusicManager(QObject *parent = nullptr);
	~MyMusicManager();

	enum reqMusicIndex {
		MUSIC_INFO = Qt::UserRole + MUSIC_NONE,
		CATEGORY_LIST,
		CATEGORY_INFO,
		SONGS_OF_CATEGORY,
		PLAY_SONG,
		UPDATE_FAVORITE,
		UPDATE_RATING,
		MUSIC_MAX
	};

	void RequestMusicInfo();
	void RequestCategoryList(int nCategory = SQLManager::CATEGORY_ALBUM);
	void RequestCategoryInfo(int nID, int nCategory = SQLManager::CATEGORY_ALBUM);
	void RequestSongsOfCategory(int nID, int nCategory = SQLManager::CATEGORY_ALBUM);

	void RequestPlaySong(int nID, int nWhere = PLAY_NOW);
	void RequestFavorite(int nID, int nFavorite, int nCategory = SQLManager::CATEGORY_ALBUM);
	void RequestRating(int nID, int nRating, int nCategory = SQLManager::CATEGORY_ALBUM);

//	int GetCntAlbum() const { return m_CntAlbum; }
//	int GetCntArtist() const { return m_CntArtist; }
//	int GetCntComposer() const { return m_CntComposer; }
//	int GetCntGenre() const { return m_CntGenre; }
//	int GetCntMood() const { return m_CntMood; }
//	int GetCntFolder() const { return m_CntFolder; }
//	int GetCntYear() const { return m_CntYear; }
//	int GetCntTrack() const { return m_CntTrack; }

	QString GetCategoryName(int nCategory);

signals:

	void SigRespMusicInfo(CJsonNode node);
	void SigRespCategoryList(QList<CJsonNode> nodeList);
	void SigRespCategoryInfo(CJsonNode node);
	void SigRespSongsOfCategory(QList<CJsonNode> nodeList);

	void SigCoverArtUpdate(QString fileName, int nIndex, int mode);
	void SigCoverArtUpdate(QString fileName);

	void SigRespError(QString errMsg);

private slots:

	void SlotRespInfo(QString json, int nCmdID);
	void SlotRespCoverArt(QString fileName, int nIndex, int mode);
	void SlotRespCoverArt(QString fileName);

private:

	void InitMusic();

	void ParseMusicInfo(CJsonNode result);
	void ParseCategoryList(CJsonNode result);
	void ParseCategoryInfo(CJsonNode result);
	void ParseSongsOfCategory(CJsonNode result);


	SQLManager	*m_pSql;

	QList<CJsonNode>	m_NodeList;
	CJsonNode			m_Node;

};

#endif // MYMUSICMANAGER_H
