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

	void AddWidgetDABRadioHome();
	void RequestList();
	void RequestRecordList();

signals:

	void SigAddWidget(QWidget *widget, QString title);

private slots:

	void SlotPlayTopMenu();
	void SlotTopMenuAction(int menuID);
	void SlotResize();

	void SlotRespError(QString errMsg);
	void SlotSelectTitle(int nType);
	void SlotRespList(QList<CJsonNode> list);
	void SlotRespRecordList(QList<CJsonNode> list);

private:

	void ConnectSigToSlot();
	void SetSelectOffTopMenu();
	void SetSelectOnTopMenu();

	void DoTopMenuSearchAll(bool bDelete);
	void DoTopMenuSelectAll();
	void DoTopMenuUnselect();
	void DoTopMenuDeleteItem();
	void DoTopMenuReservedRecordList();

	void SetHome(QList<CJsonNode> &list);

	DabRadioManager	*m_pMgr;
	InfoService		*m_pInfoService;
	IconService		*m_pIconService;

	QMap<int, QString>	m_TopMenu;
	QMap<int, bool>		m_SelectItem;

	Ui::DABRadioWindow *ui;
};

#endif // DABRADIOWINDOW_H
