#include "dabradiowindow.h"
#include "ui_dabradiowindow.h"

#include "base/menuinfo.h"
#include "base/menuicon.h"
#include "base/menuicondelegate.h"

#include "manager/dabradiomanager.h"

#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"

#define MAIN_TITLE	"DAB Radio"

DABRadioWindow::DABRadioWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new DabRadioManager),
	m_pMenuInfo(new MenuInfo(this)),
	m_pMenuIcon(new MenuIcon(this)),
	ui(new Ui::DABRadioWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();

	m_pMenuInfo->SetSubmenuDabRadio();

}

DABRadioWindow::~DABRadioWindow()
{
	delete ui;

	if (m_pMgr)
	{
		delete m_pMgr;
		m_pMgr = nullptr;
	}

	if (m_pMenuInfo)
	{
		delete m_pMenuInfo;
		m_pMenuInfo = nullptr;
	}

	if (m_pMenuIcon)
	{
		delete m_pMenuIcon;
		m_pMenuIcon = nullptr;
	}
}

void DABRadioWindow::RequestList()
{
	ui->gridLayoutTop->addWidget(m_pMenuInfo);
	ui->gridLayoutBottom->addWidget(m_pMenuIcon);

	m_pMgr->RequestList();
}

void DABRadioWindow::SlotSubmenu(int nID)
{
	LogDebug("click sub menu [%d]", nID);
	if (MenuInfo::DAB_SEARCH_ALL_DELETE == nID)
	{
		m_pMgr->RequestSeek(true);
	}
	else if (MenuInfo::DAB_SEARCH_ALL == nID)
	{
		m_pMgr->RequestSeek(false);
	}
	else if (MenuInfo::DAB_DELETE == nID)
	{
//		m_pMgr->RequestDelete();
	}
	else if (MenuInfo::DAB_RESERVE_LIST == nID)
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

	m_pMenuInfo->SetTitle(MAIN_TITLE);
	m_pMenuIcon->SetNodeList(list, MenuIcon::MENU_DAB_RADIO);
}

void DABRadioWindow::SlotRespRecordList(QList<CJsonNode> list)
{
//	SetHome(list);

//	m_pMenuInfo->SetTitle(MAIN_TITLE);
//	m_pMenuIcon->SetNodeList(list, MenuIcon::MENU_DAB_RADIO);
}

void DABRadioWindow::ConnectSigToSlot()
{
	connect(m_pMenuInfo, SIGNAL(SigPlayAll()), this, SLOT(SlotPlayAll()));
	connect(m_pMenuInfo, SIGNAL(SigPlayRandom()), this, SLOT(SlotPlayRandom()));
	connect(m_pMenuInfo, SIGNAL(SigSubmenu(int)), this, SLOT(SlotSubmenu(int)));
	connect(m_pMenuInfo, SIGNAL(SigSort()), this, SLOT(SlotSort()));

	connect(m_pMenuIcon->GetDelegate(), SIGNAL(SigSelectCoverArt(int)), this, SLOT(SlotSelectCoverArt(int)));

	connect(m_pMgr, SIGNAL(SigRespList(QList<CJsonNode>)), this, SLOT(SlotRespList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespRecordList(QList<CJsonNode>)), this, SLOT(SlotRespRecordList(QList<CJsonNode>)));

}

void DABRadioWindow::SetHome(QList<CJsonNode> &srclist)
{
	QList<CJsonNode> dstList;
	int index = 0;
	QString strCover = "";

	foreach (CJsonNode node, srclist)
	{
		strCover = ":/resource/baseline_search_black_24dp.png";
		node.Add(KEY_COVER_ART, strCover);
//		node.AddInt(KEY_ID_UPPER, index);
		node.AddInt(KEY_TYPE, index);

		dstList.append(node);
		index++;
	}
	srclist = dstList;
}
