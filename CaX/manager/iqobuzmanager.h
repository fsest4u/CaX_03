#ifndef IQOBUZMANAGER_H
#define IQOBUZMANAGER_H

#include "basemanager.h"

class IQobuzManager : public BaseManager
{
	Q_OBJECT
public:
	explicit IQobuzManager(QObject *parent = nullptr);
	~IQobuzManager();

	void RequestQobuzLogin(QString userID = "", QString password = "");

signals:

	void SigRespError(QString errMsg);
	void SigRespQobuzLoginFail(CJsonNode node);
	void SigRespQobuzLoginSuccess();

private slots:

	void SlotRespInfo(QString json, int nCmdID);

private:

	enum reqIQobuzIndex {
		IQOBUZ_LOGIN = 0,
		IQOBUZ_MAX
	};

	//	void ParseQobuzLogin(CJsonNode node);



};

#endif // IQOBUZMANAGER_H
