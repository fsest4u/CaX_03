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
//	void		SigRespInfo(QStringList jsonValueList);

	void		SigEventProgress(CJsonNode node);

	void		SigEventNowPlay(CJsonNode node);

	void		SigEventFmSeeking		(CJsonNode node);
	void		SigEventFmSeek			(CJsonNode node);
	void		SigEventFmSeekStop		(CJsonNode node);
	void		SigEventFmAdd			(CJsonNode node);
	void		SigEventFmDel			(CJsonNode node);
	void		SigEventFmSet			(CJsonNode node);

//	void		SigEventDabSeeking		(CJsonNode node);
	void		SigEventDabSeek			(CJsonNode node);
	void		SigEventDabSeekStop		(CJsonNode node);
	void		SigEventDabDel			(CJsonNode node);
	void		SigEventDabSet			(CJsonNode node);

	void		SigEventGroupPlayUpdate();


public slots:

	void		SlotRespObserverInfo(QStringList jsonValueList);
	void		SlotDisconnectObserver();


private:

	void		ConnectSigToSlot();

	bool		ParseFmRadioEvent(CJsonNode node);
	bool		ParseDabRadioEvent(CJsonNode node);
	bool		ParseGroupPlayEvent(CJsonNode rootNode);


	ObserverClient		*m_pObsClient;

};


#endif // OBSERVERMANAGER_H
