#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <QWidget>

#include "util/CJsonNode.h"

class InputManager;
class InfoService;
class IconService;

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

	void SlotSelectCoverArt(int nType, QString coverArt);

private:

	void ConnectSigToSlot();

	void SetInputHome(QList<CJsonNode> &list);

	InputManager		*m_pMgr;
	InfoService			*m_pInfoService;
	IconService			*m_pIconService;

	Ui::InputWindow *ui;
};

#endif // INPUTWINDOW_H
