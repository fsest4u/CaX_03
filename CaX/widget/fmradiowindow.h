#ifndef FMRADIOWINDOW_H
#define FMRADIOWINDOW_H

#include <QWidget>

#include "util/CJsonNode.h"

class FmRadioManager;
class InfoService;
class IconService;
class ProgressFmDialog;

namespace Ui {
class FMRadioWindow;
}

class FMRadioWindow : public QWidget
{
	Q_OBJECT

public:
	explicit FMRadioWindow(QWidget *parent = nullptr, const QString &addr = "", const int &eventID = -1);
	~FMRadioWindow();

	void AddWidgetFMRadioHome();
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

	void SlotEventFmSeeking(CJsonNode node);
	void SlotEventFmSeek(CJsonNode node);
	void SlotEventFmSeekStop(CJsonNode node);
	void SlotEventFmAdd(CJsonNode node);
	void SlotEventFmDel(CJsonNode node);
	void SlotEventFmSet(CJsonNode node);

private:

	void ConnectSigToSlot();
	void Initialize();
	void SetSelectOffTopMenu();
	void SetSelectOnTopMenu();

	void DoTopMenuSeekAll(bool bDelete);
	void DoTopMenuSelectAll();
	void DoTopMenuClearAll();
	void DoTopMenuAddNewStation();
	void DoTopMenuDelete();
	void DoTopMenuEdit();
	void DoTopMenuReservedRecordList();
	void DoTopMenuSetupReservedRecord();

	void SetHome(QList<CJsonNode> &list);

	FmRadioManager		*m_pMgr;
	InfoService			*m_pInfoService;
	IconService			*m_pIconService;
	ProgressFmDialog	*m_ProgressDialog;

	QMap<int, QString>	m_TopMenuMap;
	QMap<int, bool>		m_SelectMap;

	QList<CJsonNode>	m_NodeList;

	int					m_EventID;

	int					m_FreqMax;
	int					m_FreqMin;
	int					m_FreqStep;

	Ui::FMRadioWindow *ui;
};

#endif // FMRADIOWINDOW_H
