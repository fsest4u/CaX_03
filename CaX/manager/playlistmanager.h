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
	void RequestTrackList();
	void RequestNew();
	void RequestRename();
	void RequestDelete();
	void RequestAddTrack();
	void RequestDelTrack();
	void RequestSwap();
	void RequestPlay();
	void RequestPlayTracks();
	void RequestExport();

signals:

	void SigRespError(QString msg);
	void SigRespPlaylist(QList<CJsonNode> list);

	void SigCoverArtUpdate(QString fileName, int nIndex, int mode);

private slots:

	void SlotRespInfo(QString json, int cmdID);
	void SlotRespCoverArt(QString fileName, int nIndex, int mode);


private:

	enum {
		PLAYLIST_LIST = 0,
		PLAYLIST_TRACK_LIST,
		PLAYLIST_NEW,
		PLAYLIST_RENAME,
		PLAYLIST_DELETE,
		PLAYLIST_ADD_TRACK,
		PLAYLIST_DEL_TRACK,
		PLAYLIST_SWAP,
		PLAYLIST_PLAY,
		PLAYLIST_PLAY_TRACKS,
		PLAYLIST_EXPORT,
		PLAYLIST_MAX
	};

	void ParsePlaylist(CJsonNode result);

	SQLManager	*m_pSql;
	QList<CJsonNode>	m_NodeList;
	CJsonNode			m_Node;


};

#endif // PLAYLISTMANAGER_H
