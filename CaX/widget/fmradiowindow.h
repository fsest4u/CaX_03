#ifndef FMRADIOWINDOW_H
#define FMRADIOWINDOW_H

#include <QWidget>

#include "util/CJsonNode.h"

class FmRadioManager;
class InfoService;
class IconService;

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
	void SlotSelectTitle(int nType);
	void SlotRespList(QList<CJsonNode> list);
	void SlotRespRecordList(QList<CJsonNode> list);

private:

	void ConnectSigToSlot();

	void SetHome(QList<CJsonNode> &list);

	FmRadioManager	*m_pMgr;
	InfoService			*m_pInfoService;
	IconService			*m_pIconService;

	Ui::FMRadioWindow *ui;
};

#endif // FMRADIOWINDOW_H
