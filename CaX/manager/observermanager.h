#ifndef OBSERVERMANAGER_H
#define OBSERVERMANAGER_H

#include <QObject>
#include "util/CJsonNode.h"

class ObserverClient;

class ObserverManager : public QObject
{
	Q_OBJECT

public :

	explicit ObserverManager(QObject *parent = 0);
	~ObserverManager();

	void		RequestObserverInfo(QString strAddr);
	void		RequestDisconnectObserver();


signals:

	void		SigDisconnectObserver();
	void		SigRespObserverInfo(CJsonNode node);
	void		SigRespNowPlay(CJsonNode node);
//	void		SigRespInfo(QStringList jsonValueList);

public slots:

	void		SlotRespObserverInfo(QStringList jsonValueList);
	void		SlotDisconnectObserver();


private:

	void		ConnectSigToSlot();

	ObserverClient		*m_pObsClient;

};


#endif // OBSERVERMANAGER_H
