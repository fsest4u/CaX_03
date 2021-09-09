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

	void RequestPlay(CJsonNode srcNode);
	void RequestAdd();
	void RequestDelete();

signals:

	void SigRespError(QString errMsg);
	void SigRespLoginFail(CJsonNode node);
	void SigRespLoginSuccess();

	void SigRespList(QList<CJsonNode> nodeList);
	void SigRespGenreSubList(QList<CJsonNode> nodeList);

private slots:

	void SlotRespInfo(QString json, int nCmdID);

private:

	enum reqQobuzIndex {
		QOBUZ_LOGIN = 0,
		QOBUZ_CATEGORY,
		QOBUZ_SEARCH,
		QOBUZ_GENRE,
		QOBUZ_GENRE_SUB,
		QOBUZ_RECOMMEND,
		QOBUZ_FAVORITE,
		QOBUZ_PLAYLIST,
		QOBUZ_PLAY,
		QOBUZ_ADD,
		QOBUZ_DELETE,
		QOBUZ_MAX
	};

	void ParseList(CJsonNode node);
	void ParseGenreSubList(CJsonNode node);

};

#endif // QOBUZMANAGER_H
