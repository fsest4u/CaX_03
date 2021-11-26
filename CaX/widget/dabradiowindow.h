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

	void SlotTopMenu();
	void SlotTopMenuAction(int menuID);
//	void SlotResize();

	void SlotRespError(QString errMsg);
	void SlotSelectTitle(int nType);
	void SlotRespList(CJsonNode node);
	void SlotRespRecordList(QList<CJsonNode> list);

	void SlotEventDabSeek(CJsonNode node);
	void SlotEventDabSeekStop(CJsonNode node);
	void SlotEventDabDel(CJsonNode node);
	void SlotEventDabSet(CJsonNode node);

private:

	void ConnectSigToSlot();
	void SetSelectOffTopMenu();
	void SetSelectOnTopMenu();

	void DoTopMenuSearchAll(bool bDelete);
	void DoTopMenuSelectAll();
	void DoTopMenuClearAll();
	void DoTopMenuDelete();
	void DoTopMenuReservedRecordList();

	void SetHome(QList<CJsonNode> &list);

	DabRadioManager	*m_pMgr;
	InfoService		*m_pInfoService;
	IconService		*m_pIconService;

	QMap<int, QString>	m_TopMenuMap;
	QMap<int, bool>		m_SelectMap;

	Ui::DABRadioWindow *ui;
};

#endif // DABRADIOWINDOW_H
