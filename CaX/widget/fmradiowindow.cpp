#include "fmradiowindow.h"
#include "ui_fmradiowindow.h"

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

void FMRadioWindow::RequestList()
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pIconService);

	m_pMgr->RequestList();
}

void FMRadioWindow::SlotPlayTopMenu()
{
	QMap<int, bool> map = m_pIconService->GetSelectMap();
	if (map.count() > 0)
	{
		SetSelectOnTopMenu();
	}
	else
	{
		SetSelectOffTopMenu();
	}
	// for debug
	QMap<int, bool>::iterator i;
	for (i = map.begin(); i!= map.end(); i++)
	{
		LogDebug("key [%d] value [%d]", i.key(), i.value());
	}
}

void FMRadioWindow::SlotTopMenuAction(int menuID)
{
	LogDebug("click top menu [%d]", menuID);
//	LogDebug("click sub menu [%d]", nID);
//	if (InfoService::FM_SEARCH_ALL_DELETE == nID)
//	{
//		m_pMgr->RequestSeek(true);
//	}
//	else if (InfoService::FM_SEARCH_ALL == nID)
//	{
//		m_pMgr->RequestSeek(false);
//	}
//	else if (InfoService::FM_ADD == nID)
//	{
////		m_pMgr->RequestAdd();
//	}
//	else if (InfoService::FM_DELETE == nID)
//	{
////		m_pMgr->RequestDelete();
//	}
//	else if (InfoService::FM_RESERVE_LIST == nID)
//	{
//		m_pMgr->RequestRecordList();
	//	}
}

void FMRadioWindow::SlotResize()
{
	LogDebug("click resize");

}

void FMRadioWindow::SlotSelectTitle(int nType)
{
	m_pMgr->RequestPlay(nType);
}

void FMRadioWindow::SlotRespList(QList<CJsonNode> list)
{
	SetHome(list);

	m_pInfoService->SetSubtitle(MAIN_TITLE);
	m_pIconService->SetNodeList(list, IconService::ICON_SERVICE_FM_RADIO);
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

	connect(m_pMgr, SIGNAL(SigRespList(QList<CJsonNode>)), this, SLOT(SlotRespList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespRecordList(QList<CJsonNode>)), this, SLOT(SlotRespRecordList(QList<CJsonNode>)));

	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigPlayTopMenu()), this, SLOT(SlotPlayTopMenu()));
	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigTopMenuAction(int)), this, SLOT(SlotTopMenuAction(int)));
	connect(m_pInfoService->GetFormSort(), SIGNAL(SigResize()), this, SLOT(SlotResize()));

}

void FMRadioWindow::SetSelectOffTopMenu()
{
	m_TopMenu.clear();

	m_TopMenu.insert(TOP_MENU_SEARCH_ALL_N_DELETE, STR_SEARCH_ALL_N_DELETE);
	m_TopMenu.insert(TOP_MENU_SEARCH_ALL, STR_SEARCH_ALL);
	m_TopMenu.insert(TOP_MENU_ADD_ITEM, STR_ADD_ITEM);
	m_TopMenu.insert(TOP_MENU_RESERVED_RECORD_LIST, STR_RESERVE_RECORD_LIST);

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

void FMRadioWindow::SetHome(QList<CJsonNode> &list)
{
	QList<CJsonNode> tempList;
	int index = 0;
	QString strCover = "";

	foreach (CJsonNode node, list)
	{
		strCover = ":/resource/radio-img160-channelicon-n@3x.png";
		node.Add(KEY_COVER_ART, strCover);
//		node.AddInt(KEY_ID_UPPER, index);
		node.AddInt(KEY_TYPE, index);

		tempList.append(node);
		index++;
	}
	list = tempList;
}
