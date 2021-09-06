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
	void RequestCategory();
	void RequestSearch();
	void RequestGenre();
	void RequestRecommend();
	void RequestFavorite();
	void RequestPlaylist();
	void RequestPlay();
	void RequestAdd();
	void RequestDelete();

signals:

	void SigRespError(QString errMsg);
	void SigRespLoginFail(CJsonNode node);
	void SigRespLoginSuccess();

private slots:

	void SlotRespInfo(QString json, int nCmdID);

private:

	enum reqQobuzIndex {
		QOBUZ_LOGIN = 0,
		QOBUZ_CATEGORY,
		QOBUZ_SEARCH,
		QOBUZ_GENRE,
		QOBUZ_RECOMMEND,
		QOBUZ_FAVORITE,
		QOBUZ_PLAYLIST,
		QOBUZ_PLAY,
		QOBUZ_ADD,
		QOBUZ_DELETE,
		QOBUZ_MAX
	};

	//	void ParseQobuzLogin(CJsonNode node);



};

#endif // QOBUZMANAGER_H
