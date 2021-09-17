#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QWidget>

#include "util/CJsonNode.h"

class BrowserManager;
class MenuInfo;
class MenuIcon;
class MenuList;
class Loading;

namespace Ui {
class BrowserWindow;
}

class BrowserWindow : public QWidget
{
	Q_OBJECT

public:
	explicit BrowserWindow(QWidget *parent = nullptr, const QString &addr = "");
	~BrowserWindow();

	void RequestRoot();
	void RequestFolder();

private slots:

	void SlotSubmenu(int nID);
	void SlotSelectCoverArt(int nType);
	void SlotRespList(QList<CJsonNode> list);

private:

	void ConnectSigToSlot();

	BrowserManager		*m_pMgr;
	MenuInfo			*m_pMenuInfo;
	MenuIcon			*m_pMenuIcon;
	MenuList			*m_pMenuList;

	Loading				*m_pLoading;

	Ui::BrowserWindow *ui;
};

#endif // BROWSERWINDOW_H
