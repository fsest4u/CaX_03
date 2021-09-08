#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <QWidget>

#include "util/CJsonNode.h"

class InputManager;
class MenuInfo;
class MenuIcon;

namespace Ui {
class InputWindow;
}

class InputWindow : public QWidget
{
	Q_OBJECT

public:
	explicit InputWindow(QWidget *parent = nullptr, const QString &addr = "");
	~InputWindow();

	void InputHome(QList<CJsonNode> list);

private slots:

	void SlotSelectCoverArt(int nType);

private:

	void ConnectSigToSlot();

	void SetInputHome(QList<CJsonNode> &srclist);

	InputManager		*m_pMgr;
	MenuInfo			*m_pMenuInfo;
	MenuIcon			*m_pMenuIcon;

	Ui::InputWindow *ui;
};

#endif // INPUTWINDOW_H
