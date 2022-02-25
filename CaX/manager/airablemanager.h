#ifndef AIRABLEMANAGER_H
#define AIRABLEMANAGER_H

#include "basemanager.h"

class AirableManager : public BaseManager
{
	Q_OBJECT
public:
	explicit AirableManager(QObject *parent = nullptr);
	~AirableManager();

	void RequestLogin(int nServiceType, QString userID, QString password, bool bSaveAuth = false);
	void RequestLogout(int nServiceType, QString url);

	void RequestAuth(int nServiceType);
	void RequestURL(int nServiceType, QString url = "");

	void RequestPlay(int nServiceType, QMap<int, CJsonNode> nodeMap, int where);
	void RequestActionUrl(int nServiceType, QString url);

	// todo-dylee, record set

	void RequestRandom();

signals:

	void SigRespError(QString errMsg);
	void SigRespLoginFail(CJsonNode node);
	void SigRespLoginSuccess(int nServiceType, bool bSaveAuth);
	void SigRespLogout();
	void SigRespAuth(int nServiceType);
	void SigRespURL(int nServiceType, QList<CJsonNode> nodeList, QString title, QString nextUrl);
	void SigRespForm(int nServiceType, CJsonNode node);
	void SigCoverArtUpdate(QString fileName, int nIndex, int mode);

private slots:

	void SlotRespInfo(QString json, int nCmdID);
	void SlotRespCoverArt(QString fileName, int nIndex, int mode);

private:

	enum {
		AIRABLE_LOGIN = 0,
		AIRABLE_LOGOUT,
		AIRABLE_AUTH,
		AIRABLE_URL,
		AIRABLE_PLAY,
		AIRABLE_ACTION_URL,
		AIRABLE_RANDOM,
		AIRABLE_MAX
	};

	void ParseURL(CJsonNode node);

	int		m_ServiceType;
	bool	m_bSaveAuth;

};

#endif // AIRABLEMANAGER_H
