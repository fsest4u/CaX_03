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

	void AddWidgetFMRadioHome();
	void RequestList();
	void RequestRecordList();

signals:

	void SigAddWidget(QWidget *widget, QString title);


private slots:

	void SlotTopMenu();
	void SlotTopMenuAction(int menuID);
	void SlotResize();

	void SlotRespError(QString errMsg);
	void SlotSelectTitle(int nType);
	void SlotRespList(CJsonNode node);
	void SlotRespRecordList(QList<CJsonNode> list);

private:

	void ConnectSigToSlot();
	void SetSelectOffTopMenu();
	void SetSelectOnTopMenu();

	void DoTopMenuSearchAll(bool bDelete);
	void DoTopMenuSelectAll();
	void DoTopMenuClearAll();
	void DoTopMenuAddItem();
	void DoTopMenuDeleteItem();
	void DoTopMenuReservedRecordList();

	void SetHome(QList<CJsonNode> &list);

	FmRadioManager		*m_pMgr;
	InfoService			*m_pInfoService;
	IconService			*m_pIconService;

	QMap<int, QString>	m_TopMenuMap;
	QMap<int, bool>		m_SelectMap;

	int					m_FreqMax;
	int					m_FreqMin;
	int					m_FreqStep;

	Ui::FMRadioWindow *ui;
};

#endif // FMRADIOWINDOW_H
