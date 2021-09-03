#ifndef ISERVICEMANAGER_H
#define ISERVICEMANAGER_H

#include "basemanager.h"

class IServiceManager : public BaseManager
{
	Q_OBJECT
public:
	explicit IServiceManager(QObject *parent = nullptr);
	~IServiceManager();

	void RequestAirableLogin(int nServiceType, QString userID, QString password, bool bSaveAuth = false);
	void RequestLogout(int nServiceType, QString url);

	void RequestAirableAuth(int nServiceType);
	void RequestAirableURL(int nServiceType, QString url = "");

	void RequestPlay(int nServiceType, CJsonNode srcNode);

	// todo-dylee, record set

	// todo-dylee, action url

signals:

	void SigRespError(QString errMsg);
	void SigRespAirableLoginFail(CJsonNode node);
	void SigRespAirableLoginSuccess(int nServiceType, bool bSaveAuth);
	void SigRespAirableLogout();
	void SigRespAirableAuth(int nServiceType);

	void SigRespAirableURL(int nServiceType, QList<CJsonNode> nodeList);
	void SigCoverArtUpdate(QString fileName, int nIndex, int mode);

private slots:

	void SlotRespInfo(QString json, int nCmdID);
	void SlotRespCoverArt(QString fileName, int nIndex, int mode);

private:

	enum reqIServiceIndex {
		ISERVICE_LOGIN = 0,
		ISERVICE_LOGOUT,
		ISERVICE_AUTH,
		ISERVICE_URL,
		ISERVICE_PLAY,
		ISERVICE_MAX
	};

	void ParseAirableURL(CJsonNode node);

	int		m_ServiceType;
	bool	m_bSaveAuth;

};

#endif // ISERVICEMANAGER_H
