#include "dabradiowindow.h"
#include "ui_dabradiowindow.h"

#include "base/infoservice.h"
#include "base/iconservice.h"
#include "base/iconservicedelegate.h"

#include "manager/dabradiomanager.h"

#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"

#define MAIN_TITLE	"DAB Radio"

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

	m_pInfoService->SetSubmenuDabRadio();

}

DABRadioWindow::~DABRadioWindow()
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

void DABRadioWindow::RequestList()
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pIconService);

	m_pMgr->RequestList();
}

void DABRadioWindow::SlotSubmenu(int nID)
{
	LogDebug("click sub menu [%d]", nID);
	if (InfoService::DAB_SEARCH_ALL_DELETE == nID)
	{
		m_pMgr->RequestSeek(true);
	}
	else if (InfoService::DAB_SEARCH_ALL == nID)
	{
		m_pMgr->RequestSeek(false);
	}
	else if (InfoService::DAB_DELETE == nID)
	{
//		m_pMgr->RequestDelete();
	}
	else if (InfoService::DAB_RESERVE_LIST == nID)
	{
		m_pMgr->RequestRecordList();
	}
}

void DABRadioWindow::SlotSelectCoverArt(int nType)
{
	m_pMgr->RequestPlay(nType);
}

void DABRadioWindow::SlotRespList(QList<CJsonNode> list)
{
	SetHome(list);

	m_pInfoService->SetTitle(MAIN_TITLE);
	m_pIconService->SetNodeList(list, IconService::ICON_SERVICE_DAB_RADIO);
}

void DABRadioWindow::SlotRespRecordList(QList<CJsonNode> list)
{
//	SetHome(list);

//	m_pInfoService->SetTitle(MAIN_TITLE);
//	m_pIconService->SetNodeList(list, IconService::SERVICE_DAB_RADIO);
}

void DABRadioWindow::ConnectSigToSlot()
{
	connect(m_pInfoService, SIGNAL(SigPlayAll()), this, SLOT(SlotPlayAll()));
	connect(m_pInfoService, SIGNAL(SigPlayRandom()), this, SLOT(SlotPlayRandom()));
	connect(m_pInfoService, SIGNAL(SigSubmenu(int)), this, SLOT(SlotSubmenu(int)));
	connect(m_pInfoService, SIGNAL(SigSort()), this, SLOT(SlotSort()));

	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectCoverArt(int)), this, SLOT(SlotSelectCoverArt(int)));

	connect(m_pMgr, SIGNAL(SigRespList(QList<CJsonNode>)), this, SLOT(SlotRespList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespRecordList(QList<CJsonNode>)), this, SLOT(SlotRespRecordList(QList<CJsonNode>)));

}

void DABRadioWindow::SetHome(QList<CJsonNode> &list)
{
	QList<CJsonNode> tempList;
	int index = 0;
	QString strCover = "";

	foreach (CJsonNode node, list)
	{
		strCover = ":/resource/Icon-playbar-volume-160.png";
		node.Add(KEY_COVER_ART, strCover);
//		node.AddInt(KEY_ID_UPPER, index);
		node.AddInt(KEY_TYPE, index);

		tempList.append(node);
		index++;
	}
	list = tempList;
}
