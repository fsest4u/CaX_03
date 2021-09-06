#ifndef QOBUZMANAGER_H
#define QOBUZMANAGER_H

#include "basemanager.h"

class QobuzManager : public BaseManager
{
	Q_OBJECT
public:
	explicit QobuzManager(QObject *parent = nullptr);
	~QobuzManager();

	void RequestQobuzLogin(QString userID = "", QString password = "");

signals:

	void SigRespError(QString errMsg);
	void SigRespQobuzLoginFail(CJsonNode node);
	void SigRespQobuzLoginSuccess();

private slots:

	void SlotRespInfo(QString json, int nCmdID);

private:

	enum reqIQobuzIndex {
		QOBUZ_LOGIN = 0,
		QOBUZ_MAX
	};

	//	void ParseQobuzLogin(CJsonNode node);



};

#endif // QOBUZMANAGER_H
