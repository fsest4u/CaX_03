#ifndef FMRADIOWINDOW_H
#define FMRADIOWINDOW_H

#include <QWidget>

#include "util/CJsonNode.h"

class FmRadioManager;
class MenuInfo;
class MenuIcon;

namespace Ui {
class FMRadioWindow;
}

class FMRadioWindow : public QWidget
{
	Q_OBJECT

public:
	explicit FMRadioWindow(QWidget *parent = nullptr, const QString &addr = "");
	~FMRadioWindow();

	void RequestList();

private slots:

	void SlotSubmenu(int nID);
	void SlotSelectCoverArt(int nType);
	void SlotRespList(QList<CJsonNode> list);
	void SlotRespRecordList(QList<CJsonNode> list);

private:

	void ConnectSigToSlot();

	void SetHome(QList<CJsonNode> &list);

	FmRadioManager	*m_pMgr;
	MenuInfo			*m_pMenuInfo;
	MenuIcon			*m_pMenuIcon;

	Ui::FMRadioWindow *ui;
};

#endif // FMRADIOWINDOW_H
