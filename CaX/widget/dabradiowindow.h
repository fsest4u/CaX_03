#ifndef DABRADIOWINDOW_H
#define DABRADIOWINDOW_H

#include <QWidget>

#include "util/CJsonNode.h"

class DabRadioManager;
class InfoService;
class IconService;

namespace Ui {
class DABRadioWindow;
}

class DABRadioWindow : public QWidget
{
	Q_OBJECT

public:
	explicit DABRadioWindow(QWidget *parent = nullptr, const QString &addr = "");
	~DABRadioWindow();

	void RequestList();

private slots:

	void SlotSubmenu(int nID);
	void SlotSelectCoverArt(int nType);
	void SlotRespList(QList<CJsonNode> list);
	void SlotRespRecordList(QList<CJsonNode> list);

private:

	void ConnectSigToSlot();

	void SetHome(QList<CJsonNode> &list);

	DabRadioManager	*m_pMgr;
	InfoService		*m_pInfoService;
	IconService		*m_pIconService;

	Ui::DABRadioWindow *ui;
};

#endif // DABRADIOWINDOW_H
