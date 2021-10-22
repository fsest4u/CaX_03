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

	void SlotPlayTopMenu();
	void SlotTopMenuAction(int menuID);
	void SlotResize();

	void SlotSelectTitle(int nType);
	void SlotRespList(QList<CJsonNode> list);
	void SlotRespRecordList(QList<CJsonNode> list);

private:

	void ConnectSigToSlot();
	void SetSelectOffTopMenu();
	void SetSelectOnTopMenu();
	void SetHome(QList<CJsonNode> &list);

	FmRadioManager		*m_pMgr;
	InfoService			*m_pInfoService;
	IconService			*m_pIconService;

	QMap<int, QString>	m_TopMenu;

	Ui::FMRadioWindow *ui;
};

#endif // FMRADIOWINDOW_H
