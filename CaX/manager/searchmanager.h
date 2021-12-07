#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H

#include "basemanager.h"

class SQLManager;

class SearchManager : public BaseManager
{
	Q_OBJECT
public:
	SearchManager();
	~SearchManager();

	void RequestSearchAlbumList(QString keyword);
	void RequestSearchArtistList(QString keyword);
//	void RequestSearchCategoryList(QString keyword);
	void RequestSearchTrackList(QString keyword);
	void RequestPlayTrack(QMap<int, bool> idMap, int nWhere);

	SQLManager *GetSqlMgr() const;

signals:

	void SigRespError(QString msg);
	void SigRespAlbumList(QList<CJsonNode> list);
	void SigRespArtistList(QList<CJsonNode> list);
	void SigRespTrackList(QList<CJsonNode> list);

	void SigCoverArtUpdate(QString fileName, int nIndex, int mode);

private slots:

	void SlotRespInfo(QString json, int cmdID);
	void SlotRespCoverArt(QString fileName, int nIndex, int mode);

private:

	enum {
		SEARCH_ALBUM = 0,
		SEARCH_ARTIST,
		SEARCH_TRACK,
		SEARCH_PLAY_TRACK,
		SEARCH_MAX
	};

	void ParseAlbumList(CJsonNode result);
	void ParseArtistList(CJsonNode result);
	void ParseTrackList(CJsonNode result);

	SQLManager	*m_pSql;


};

#endif // SEARCHMANAGER_H
