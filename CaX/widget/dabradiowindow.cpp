#include "dabradiowindow.h"
#include "ui_dabradiowindow.h"

#include "dialog/commondialog.h"

#include "manager/dabradiomanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/caxtranslate.h"
#include "util/log.h"
#include "util/utilnovatron.h"

#include "widget/form/formplay.h"
#include "widget/form/formsort.h"

#include "widget/formTop/infoservice.h"
#include "widget/formBottom/iconservice.h"
#include "widget/formBottom/iconservicedelegate.h"

#define MAIN_TITLE	"DAB Radio"
#define RESERVE_TITLE	"Reserved record list"

DABRadioWindow::DABRadioWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new DabRadioManager),
	m_pInfoService(new InfoService(this)),
	m_pIconService(new IconService(this)),
	ui(new Ui::DABRadioWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
	Initialize();

}

DABRadioWindow::~DABRadioWindow()
{
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

	delete ui;

}

void DABRadioWindow::AddWidgetDABRadioHome()
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pIconService);
}

void DABRadioWindow::RequestList()
{
	m_pMgr->RequestList();
}

void DABRadioWindow::RequestRecordList()
{
	m_pMgr->RequestRecordList();
}

void DABRadioWindow::SlotAddWidget(QWidget *widget, QString title)
{
	emit SigAddWidget(widget, title);	// recursive
}

void DABRadioWindow::SlotRemoveWidget(QWidget *widget)
{
	emit SigRemoveWidget(widget);
}

void DABRadioWindow::SlotTopMenu()
{
	m_SelectMap = m_pIconService->GetSelectMap();
	if (m_SelectMap.count() > 0)
	{
		SetSelectOnTopMenu();
	}
	else
	{
		SetSelectOffTopMenu();
	}

}

void DABRadioWindow::SlotTopMenuAction(int menuID)
{
	switch (menuID) {
	case TOP_MENU_CLEAR_AND_SEEK_ALL:
		DoTopMenuSeekAll(true);
		break;
	case TOP_MENU_SEEK_ALL:
		DoTopMenuSeekAll(false);
		break;
	case TOP_MENU_SELECT_ALL:
		DoTopMenuSelectAll();
		break;
	case TOP_MENU_CLEAR_ALL:
		DoTopMenuClearAll();
		break;
	case TOP_MENU_DELETE:
		DoTopMenuDelete();
		break;
	case TOP_MENU_RESERVED_RECORD_LIST:
		DoTopMenuReservedRecordList();
		break;
	}
}

//void DABRadioWindow::SlotResize()
//{
//	LogDebug("click resize");
//}

void DABRadioWindow::SlotRespError(QString errMsg)
{
	CommonDialog dialog(this, STR_WARNING, errMsg);
	dialog.exec();
}

void DABRadioWindow::SlotSelectTitle(int nType)
{
	m_pMgr->RequestPlay(nType);
}

void DABRadioWindow::SlotRespList(CJsonNode node)
{

	CJsonNode result;
	if (!node.GetArray(VAL_RESULT, result) || result.ArraySize() <= 0)
	{
		QString message = node.GetString(VAL_MSG);
		SlotRespError(message.left(MSG_LIMIT_COUNT));
		return;
	}

	QList<CJsonNode> nodeList;
	for (int i = 0; i < result.ArraySize(); i++)
	{
		nodeList.append(result.GetArrayAt(i));
	}

	SetHome(nodeList);

	m_pInfoService->SetTitle(MAIN_TITLE);
	m_pIconService->SetNodeList(nodeList, IconService::ICON_SERVICE_DAB_RADIO);
}

void DABRadioWindow::SlotRespRecordList(QList<CJsonNode> list)
{
	SetHome(list);

	m_pInfoService->SetTitle(RESERVE_TITLE);
	m_pIconService->SetNodeList(list, IconService::ICON_SERVICE_DAB_RADIO_RECORD);
}

void DABRadioWindow::SlotEventDabSeek(CJsonNode node)
{
	SlotRespList(node);
}

void DABRadioWindow::SlotEventDabSeekStop(CJsonNode node)
{
	SlotRespList(node);
}

void DABRadioWindow::SlotEventDabDel(CJsonNode node)
{
	SlotRespList(node);
}

void DABRadioWindow::SlotEventDabSet(CJsonNode node)
{
	SlotRespList(node);
}

void DABRadioWindow::ConnectSigToSlot()
{
	connect(this, SIGNAL(SigAddWidget(QWidget*, QString)), parent(), SLOT(SlotAddWidget(QWidget*, QString)));
	connect(this, SIGNAL(SigRemoveWidget(QWidget*)), parent(), SLOT(SlotRemoveWidget(QWidget*)));

	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespList(CJsonNode)), this, SLOT(SlotRespList(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespRecordList(QList<CJsonNode>)), this, SLOT(SlotRespRecordList(QList<CJsonNode>)));

//	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectPlay(int)), this, SLOT(SlotSelectPlay(int)));
	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int)), this, SLOT(SlotSelectTitle(int)));
//	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));

	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigMenu()), this, SLOT(SlotTopMenu()));
	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigMenuAction(int)), this, SLOT(SlotTopMenuAction(int)));
//	connect(m_pInfoService->GetFormSort(), SIGNAL(SigResize()), this, SLOT(SlotResize()));

}

void DABRadioWindow::Initialize()
{

	m_pInfoService->GetFormPlay()->ShowMenu();
//	m_pInfoService->GetFormSort()->ShowResize();

	m_TopMenuMap.clear();
	m_SelectMap.clear();
}

void DABRadioWindow::SetSelectOffTopMenu()
{
	m_TopMenuMap.clear();

	m_TopMenuMap.insert(TOP_MENU_CLEAR_AND_SEEK_ALL, STR_CLEAR_AND_SEEK_ALL);
	m_TopMenuMap.insert(TOP_MENU_SEEK_ALL, STR_SEEK_ALL);
	m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
//	m_TopMenuMap.insert(TOP_MENU_RESERVED_RECORD_LIST, STR_RESERVE_RECORD_LIST);

	m_pInfoService->GetFormPlay()->ClearMenu();
	m_pInfoService->GetFormPlay()->SetMenu(m_TopMenuMap);
}

void DABRadioWindow::SetSelectOnTopMenu()
{
	m_TopMenuMap.clear();

	m_TopMenuMap.insert(TOP_MENU_CLEAR_ALL, STR_CLEAR_ALL);
	m_TopMenuMap.insert(TOP_MENU_DELETE, STR_DELETE);

	m_pInfoService->GetFormPlay()->ClearMenu();
	m_pInfoService->GetFormPlay()->SetMenu(m_TopMenuMap);
}

void DABRadioWindow::DoTopMenuSeekAll(bool bDelete)
{
	m_pMgr->RequestSeek(bDelete);
}

void DABRadioWindow::DoTopMenuSelectAll()
{
	m_pIconService->SetAllSelectMap();
}

void DABRadioWindow::DoTopMenuClearAll()
{
	m_pIconService->ClearSelectMap();
}

void DABRadioWindow::DoTopMenuDelete()
{
	if (m_SelectMap.count() > 0)
	{
		m_pMgr->RequestDelete(m_SelectMap);
	}
}

void DABRadioWindow::DoTopMenuReservedRecordList()
{
	DABRadioWindow *widget = new DABRadioWindow(this, m_pMgr->GetAddr());
	widget->AddWidgetDABRadioHome();
	emit widget->SigAddWidget(widget, STR_DAB_RADIO);
	widget->RequestRecordList();
}

void DABRadioWindow::SetHome(QList<CJsonNode> &list)
{
	QList<CJsonNode> tempList;
	int index = 0;

	foreach (CJsonNode node, list)
	{
		node.Add(KEY_COVER_ART, UtilNovatron::GetCoverArtIcon(SIDEMENU_DAB_RADIO));
//		node.AddInt(KEY_ID_UPPER, index);
		node.AddInt(KEY_TYPE, index);

		tempList.append(node);
		index++;
	}
	list = tempList;
}
