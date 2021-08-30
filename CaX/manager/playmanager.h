#ifndef PLAYMANAGER_H
#define PLAYMANAGER_H

#include <QObject>

#include "basemanager.h"

#include "util/CJsonNode.h"

#define	PLAY_NONE	1000


class PlayManager : public BaseManager
{
	Q_OBJECT
public:
	explicit PlayManager(QObject *parent = nullptr);
	~PlayManager();

	enum reqPlayIndex {
		PLAY_STATE = Qt::UserRole + PLAY_NONE,
		PLAY_MAX
	};

	enum playModeIndex {
		PLAY_MODE_UNKNOWN		= -1,
		PLAY_MODE_PLAY			= 0,
		PLAY_MODE_PAUSE			= 1,
		PLAY_MODE_NEXT			= 2,
		PLAY_MODE_PREV			= 3,
		PLAY_MODE_STOP			= 4,
		PLAY_MODE_RECORD		= 5,
		PLAY_MODE_MAX			= 10
	};

	void RequestPlayState(int mode);	// play, pause, next, prev, stop

signals:

	void SigRespError(QString errMsg);

private slots:

	void SlotRespInfo(QString json, int nCmdID);


private:

	QString GetPlayMode(int mode);


};

#endif // PLAYMANAGER_H
