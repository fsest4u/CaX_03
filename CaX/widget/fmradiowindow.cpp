#include "fmradiowindow.h"
#include "ui_fmradiowindow.h"

#include "base/menuinfo.h"
#include "base/menuicon.h"
#include "base/menuicondelegate.h"

#include "manager/fmradiomanager.h"

#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"

#define MAIN_TITLE	"FM Radio"

FMRadioWindow::FMRadioWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new FmRadioManager),
	m_pMenuInfo(new MenuInfo(this)),
	m_pMenuIcon(new MenuIcon(this)),
	ui(new Ui::FMRadioWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
}

FMRadioWindow::~FMRadioWindow()
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

void FMRadioWindow::RequestList()
{
	ui->gridLayoutTop->addWidget(m_pMenuInfo);
	ui->gridLayoutBottom->addWidget(m_pMenuIcon);

	m_pMgr->RequestList();
}

void FMRadioWindow::SlotSelectCoverArt(int nType)
{
	m_pMgr->RequestPlay(nType);
}

void FMRadioWindow::SlotRespList(QList<CJsonNode> list)
{
	SetFmRadioHome(list);

	m_pMenuInfo->SetTitle(MAIN_TITLE);
	m_pMenuIcon->SetNodeList(list, MenuIcon::MENU_FM_RADIO);
}

void FMRadioWindow::ConnectSigToSlot()
{
	connect(m_pMenuInfo, SIGNAL(SigPlayAll()), this, SLOT(SlotPlayAll()));
	connect(m_pMenuInfo, SIGNAL(SigPlayRandom()), this, SLOT(SlotPlayRandom()));
	connect(m_pMenuInfo, SIGNAL(SigSubmenu()), this, SLOT(SlotSubmenu()));
	connect(m_pMenuInfo, SIGNAL(SigSort()), this, SLOT(SlotSort()));

	connect(m_pMenuIcon->GetDelegate(), SIGNAL(SigSelectCoverArt(int)), this, SLOT(SlotSelectCoverArt(int)));

	connect(m_pMgr, SIGNAL(SigRespList(QList<CJsonNode>)), this, SLOT(SlotRespList(QList<CJsonNode>)));
}

void FMRadioWindow::SetFmRadioHome(QList<CJsonNode> &srclist)
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
