#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "basemanager.h"

class InputManager : public BaseManager
{
	Q_OBJECT
public:
	InputManager();
	~InputManager();

	void RequestInput(int index);

signals:

	void SigRespError(QString errMsg);

private slots:

	void SlotRespInfo(QString json, int nCmdID);

private:

	enum {
		INPUT_NORMAL = 0,
		INPUT_MAX
	};

};

#endif // INPUTMANAGER_H
