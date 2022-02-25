#ifndef QOBUZMANAGER_H
#define QOBUZMANAGER_H

#include "basemanager.h"

class QobuzManager : public BaseManager
{
	Q_OBJECT
public:
	explicit QobuzManager(QObject *parent = nullptr);
	~QobuzManager();

	void RequestLogin(QString userID = "", QString password = "");
	void RequestSearch(int nType, QString keyword, int nStart, int nCount);
	void RequestGenre(QString strID = "");
	void RequestRecommend(int nType, QString strID, int nStart, int nCount, QString strGenreID = "");
	void RequestFavorite(int nType, int nStart, int nCount);
	void RequestPlaylist(int nStart, int nCount);
	void RequestCategory(int nType, QString strID, int nStart, int nCount);

	void RequestPlay(QMap<int, CJsonNode> nodeMap, int nWhere);
	void RequestAddToFavorite(int nType, QMap<int, CJsonNode> nodeMap);
	void RequestDeleteToFavorite(int nType, QMap<int, CJsonNode> nodeMap);
	void RequestAddPlaylist(QString name, QString desc);
	void RequestDeletePlaylist(QString strID);
	void RequestDeleteTrack(QString playlistID, QMap<int, CJsonNode> nodeMap);

	void RequestRandom();

signals:

	void SigRespError(QString errMsg);
	void SigRespLoginFail(CJsonNode node);
	void SigRespLoginSuccess();

	void SigRespList(QList<CJsonNode> nodeList, bool genre, int start);
	void SigListUpdate();


private slots:

	void SlotRespInfo(QString json, int nCmdID);

private:

	enum {
		QOBUZ_LOGIN = 0,
		QOBUZ_CATEGORY,
		QOBUZ_SEARCH,
		QOBUZ_GENRE,
		QOBUZ_GENRE_SUB,
		QOBUZ_RECOMMEND,
		QOBUZ_FAVORITE,
		QOBUZ_PLAYLIST,
		QOBUZ_PLAY,
		QOBUZ_ADD_TO_FAVORITE,
		QOBUZ_DELETE_TO_FAVORITE,
		QOBUZ_ADD_PLAYLIST,
		QOBUZ_DELETE_PLAYLIST,
		QOBUZ_DELETE_TRACK,
		QOBUZ_RANDOM,
		QOBUZ_MAX
	};

	void ParseList(CJsonNode node, bool genre);

};

#endif // QOBUZMANAGER_H
