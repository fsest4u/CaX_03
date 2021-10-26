#include <QMessageBox>

#include "fmradiowindow.h"
#include "ui_fmradiowindow.h"

#include "dialog/addradiodialog.h"

#include "widget/form/formplay.h"
#include "widget/form/formsort.h"

#include "widget/formTop/infoservice.h"
#include "widget/formBottom/iconservice.h"
#include "widget/formBottom/iconservicedelegate.h"

#include "manager/fmradiomanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"

#define MAIN_TITLE	"FM Radio"

FMRadioWindow::FMRadioWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new FmRadioManager),
	m_pInfoService(new InfoService(this)),
	m_pIconService(new IconService(this)),
	m_FreqMax(0),
	m_FreqMin(0),
	m_FreqStep(0),
	ui(new Ui::FMRadioWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();

	m_pInfoService->SetSubmenuFmRadio();
	m_pInfoService->GetFormPlay()->ShowPlayTopMenu();
	m_pInfoService->GetFormSort()->ShowResize();

}

FMRadioWindow::~FMRadioWindow()
{
	delete ui;

	if (m_pMgr)
	{
		delete m_pMgr;
		m_pMgr = nullptr;
	}

	if (m_pInfoService)
	{
		delete m_pInfoService;
		m_pInfoService = nullptr;
	}

	if (m_pIconService)
	{
		delete m_pIconService;
		m_pIconService = nullptr;
	}
}

void FMRadioWindow::AddWidgetFMRadioHome()
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pIconService);
}

void FMRadioWindow::RequestList()
{
	m_pMgr->RequestList();
}

void FMRadioWindow::SlotPlayTopMenu()
{
	m_SelectItem = m_pIconService->GetSelectMap();
	if (m_SelectItem.count() > 0)
	{
		SetSelectOnTopMenu();
	}
	else
	{
		SetSelectOffTopMenu();
	}
}

void FMRadioWindow::SlotTopMenuAction(int menuID)
{
	switch (menuID) {
	case TOP_MENU_SEARCH_ALL:
		DoTopMenuSearchAll(false);
		break;
	case TOP_MENU_SELECT_ALL:
		DoTopMenuSelectAll();
		break;
	case TOP_MENU_UNSELECT:
		DoTopMenuUnselect();
		break;
	case TOP_MENU_ADD_ITEM:
		DoTopMenuAddItem();
		break;
	case TOP_MENU_DELETE_ITEM:
		DoTopMenuDeleteItem();
		break;
	case TOP_MENU_RESERVED_RECORD_LIST:
		DoTopMenuReservedRecordList();
		break;
	}

}

void FMRadioWindow::SlotResize()
{
	LogDebug("click resize");

}

void FMRadioWindow::SlotRespError(QString errMsg)
{
	QMessageBox::warning(this, "Warning", errMsg);
}

void FMRadioWindow::SlotSelectTitle(int nType)
{
	m_pMgr->RequestPlay(nType);
}

void FMRadioWindow::SlotRespList(CJsonNode node)
{
	CJsonNode result;
	if (!node.GetArray(VAL_RESULT, result) || result.ArraySize() <= 0)
	{
		SlotRespError("there is no result");
		return;
	}

	m_FreqMax = node.GetInt(KEY_FREQ_MAX);
	m_FreqMin = node.GetInt(KEY_FREQ_MIN);
	m_FreqStep = node.GetInt(KEY_FREQ_STEP);

	QList<CJsonNode> nodeList;
	for (int i = 0; i < result.ArraySize(); i++)
	{
		nodeList.append(result.GetArrayAt(i));
	}

	SetHome(nodeList);

	m_pInfoService->SetSubtitle(MAIN_TITLE);
	m_pIconService->SetNodeList(nodeList, IconService::ICON_SERVICE_FM_RADIO);
}

void FMRadioWindow::SlotRespRecordList(QList<CJsonNode> list)
{
//	SetHome(list);

//	m_pInfoService->SetSubtitle(MAIN_TITLE);
//	m_pIconService->SetNodeList(list, IconService::SERVICE_FM_RADIO);
}

void FMRadioWindow::ConnectSigToSlot()
{

//	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectPlay(int)), this, SLOT(SlotSelectPlay(int)));
	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int)), this, SLOT(SlotSelectTitle(int)));
//	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));

	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespList(CJsonNode)), this, SLOT(SlotRespList(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespRecordList(QList<CJsonNode>)), this, SLOT(SlotRespRecordList(QList<CJsonNode>)));

	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigPlayTopMenu()), this, SLOT(SlotPlayTopMenu()));
	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigTopMenuAction(int)), this, SLOT(SlotTopMenuAction(int)));
	connect(m_pInfoService->GetFormSort(), SIGNAL(SigResize()), this, SLOT(SlotResize()));

}

void FMRadioWindow::SetSelectOffTopMenu()
{
	m_TopMenu.clear();

//	m_TopMenu.insert(TOP_MENU_SEARCH_ALL_N_DELETE, STR_SEARCH_ALL_N_DELETE);
	m_TopMenu.insert(TOP_MENU_SEARCH_ALL, STR_SEARCH_ALL);
	m_TopMenu.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
	m_TopMenu.insert(TOP_MENU_ADD_ITEM, STR_ADD_ITEM);
//	m_TopMenu.insert(TOP_MENU_RESERVED_RECORD_LIST, STR_RESERVE_RECORD_LIST);

	m_pInfoService->GetFormPlay()->ClearTopMenu();
	m_pInfoService->GetFormPlay()->SetTopMenu(m_TopMenu);
}

void FMRadioWindow::SetSelectOnTopMenu()
{
	m_TopMenu.clear();

	m_TopMenu.insert(TOP_MENU_UNSELECT, STR_UNSELECT);
	m_TopMenu.insert(TOP_MENU_DELETE_ITEM, STR_DELETE_ITEM);

	m_pInfoService->GetFormPlay()->ClearTopMenu();
	m_pInfoService->GetFormPlay()->SetTopMenu(m_TopMenu);
}

void FMRadioWindow::DoTopMenuSearchAll(bool bDelete)
{
	m_pMgr->RequestSeek(bDelete);
}

void FMRadioWindow::DoTopMenuSelectAll()
{
	m_pIconService->SetAllSelectMap();
}

void FMRadioWindow::DoTopMenuUnselect()
{
	m_pIconService->ClearSelectMap();
}

void FMRadioWindow::DoTopMenuAddItem()
{
	AddRadioDialog dialog;
	dialog.SetName(tr("My Radio"));
	dialog.SetRange(m_FreqMin/100.0, m_FreqMax/100.0, m_FreqStep/100.0);

	if (dialog.exec() == QDialog::Accepted)
	{
		QString name = dialog.GetName();
		double freq = dialog.GetFrequency();
		m_pMgr->RequestAdd(freq, name);

		//refresh
//		RequestList();
	}
}

void FMRadioWindow::DoTopMenuDeleteItem()
{
	if (m_SelectItem.count() > 0)
	{
		m_pMgr->RequestDelete(m_SelectItem);
	}
}

void FMRadioWindow::DoTopMenuReservedRecordList()
{
	m_pMgr->RequestRecordList();
}

void FMRadioWindow::SetHome(QList<CJsonNode> &list)
{
	QList<CJsonNode> tempList;
	int index = 0;
	QString strCover = "";

	foreach (CJsonNode node, list)
	{
		strCover = ":/resource/radio-img160-channelicon-n@3x.png";
		node.Add(KEY_COVER_ART, strCover);
		node.AddInt(KEY_ID_UPPER, index);
		node.AddInt(KEY_TYPE, index);

		tempList.append(node);
		index++;
	}
	list = tempList;
}
