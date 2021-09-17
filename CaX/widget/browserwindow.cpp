#include "browserwindow.h"
#include "ui_browserwindow.h"

#include "browser.h"

#include "manager/browsermanager.h"

#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"


#include "base/menuinfo.h"
#include "base/menuicon.h"
#include "base/menuicondelegate.h"
#include "base/menulist.h"
#include "base/menulistdelegate.h"

#include "dialog/logindialog.h"

#define BROWSER_TITLE		"Browser"

BrowserWindow::BrowserWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new BrowserManager),
	m_pMenuInfo(new MenuInfo(this)),
	m_pMenuIcon(new MenuIcon(this)),
	m_pMenuList(new MenuList(this)),
	m_pLoading(new Loading(this)),
	ui(new Ui::BrowserWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
}

BrowserWindow::~BrowserWindow()
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

	if (m_pMenuList)
	{
		delete m_pMenuList;
		m_pMenuList = nullptr;
	}

	if (m_pLoading)
	{
		delete m_pLoading;
		m_pLoading = nullptr;
	}
}

void BrowserWindow::RequestRoot()
{
	ui->gridLayoutTop->addWidget(m_pMenuInfo);
	ui->gridLayoutBottom->addWidget(m_pMenuIcon);

	m_pMgr->RequestRoot();

}

void BrowserWindow::SlotSubmenu(int nID)
{

}

void BrowserWindow::SlotSelectCoverArt(int nType)
{

}

void BrowserWindow::SlotRespList(QList<CJsonNode> list)
{

}

void BrowserWindow::ConnectSigToSlot()
{
	connect(m_pMenuInfo, SIGNAL(SigPlayAll()), this, SLOT(SlotPlayAll()));
	connect(m_pMenuInfo, SIGNAL(SigPlayRandom()), this, SLOT(SlotPlayRandom()));
	connect(m_pMenuInfo, SIGNAL(SigSubmenu(int)), this, SLOT(SlotSubmenu(int)));
	connect(m_pMenuInfo, SIGNAL(SigSort()), this, SLOT(SlotSort()));

	connect(m_pMenuIcon->GetDelegate(), SIGNAL(SigSelectCoverArt(int)), this, SLOT(SlotSelectCoverArt(int)));

	connect(m_pMgr, SIGNAL(SigRespList(QList<CJsonNode>)), this, SLOT(SlotRespList(QList<CJsonNode>)));

}
