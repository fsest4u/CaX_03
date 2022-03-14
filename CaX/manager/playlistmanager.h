#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include "basemanager.h"

class SQLManager;

class PlaylistManager : public BaseManager
{
	Q_OBJECT
public:
	PlaylistManager();
	~PlaylistManager();

	void RequestPlaylist();
	void RequestPlaylistInfo(int id);
	void RequestTrackList(int id);
	void RequestPlayTrack(QMap<int, bool> idMap, int nWhere);
	void RequestPlayPlaylist(QMap<int, bool> idMap, int nWhere);
	void RequestAddPlaylist(QString name);
	void RequestRenamePlaylist(int id, QString name);
	void RequestDeletePlaylist(QMap<int, bool> idMap);
	void RequestAddCategoryFromPlaylist(int id, QMap<int, bool> idMap, int category);
	void RequestAddTrackFromPlaylist(int id, QMap<int, bool> idMap);
	void RequestDelTrack(int id, QMap<int, bool> idMap);
	void RequestUpdateTrackFavorite(int id, int favorite);
	void RequestRandom();

signals:

	void SigRespError(QString msg);
	void SigRespPlaylist(QList<CJsonNode> list);
	void SigRespPlaylistInfo(CJsonNode node);
	void SigRespTrackList(QList<CJsonNode> list);
	void SigCoverArtUpdate(QString fileName, int nIndex, int mode);
	void SigRefresh(CJsonNode node);

private slots:

	void SlotRespInfo(QString json, int cmdID);
	void SlotRespCoverArt(QString fileName, int nIndex, int mode);


private:

	enum {
		PLAYLIST_LIST = 0,
		PLAYLIST_TRACK_LIST,
		PLAYLIST_INFO,
		PLAYLIST_PLAY_PLAYLIST,
		PLAYLIST_ADD_PLAYLIST,
		PLAYLIST_RENAME_PLAYLIST,
		PLAYLIST_DELETE_PLAYLIST,
		PLAYLIST_PLAY_TRACK,
		PLAYLIST_DEL_TRACK,
		PLAYLIST_ADD_CATEGORY_FROM_PLAYLIST,
		PLAYLIST_ADD_TRACK_FROM_PLAYLIST,
		PLAYLIST_UPDATE_TRACK_FAVORITE,
		PLAYLIST_RANDOM,
		PLAYLIST_MAX
	};

	void ParsePlaylist(CJsonNode node);
	void ParseTrackList(CJsonNode node);
	void ParsePlaylistInfo(CJsonNode node);

	SQLManager	*m_pSql;
	QList<CJsonNode>	m_NodeList;
	CJsonNode			m_Node;


};

#endif // PLAYLISTMANAGER_H
