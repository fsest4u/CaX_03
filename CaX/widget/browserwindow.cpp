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

BrowserWindow::BrowserWindow(QWidget *parent, const QString &addr, const QString &root) :
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
	m_Root = root;

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
//	ui->gridLayoutTop->addWidget(m_pMenuInfo);
//	ui->gridLayoutBottom->addWidget(m_pMenuIcon);

	m_pMgr->RequestRoot();

}

void BrowserWindow::RequestFolder(QString strPath)
{
//	ui->gridLayoutTop->addWidget(m_pMenuInfo);
//	ui->gridLayoutBottom->addWidget(m_pMenuIcon);

	if (!m_Root.isEmpty())
		strPath = m_Root + "/" + strPath;

	m_Root = strPath;
	m_pMgr->RequestFolder(strPath);

}

void BrowserWindow::SlotAddWidget(QWidget *widget)
{
	emit SigAddWidget(widget);		// recursive
}

void BrowserWindow::SlotSubmenu(int nID)
{

}

void BrowserWindow::SlotSelectCoverArt(int nType, QString rawData)
{

	CJsonNode node;
	if (!node.SetContent(rawData))
	{
		LogCritical("invalid json");
		return;
	}

	LogDebug("nType [%d]", nType);
	if ( iFolderType_Mask_Dir & nType
		 && iFolderType_Mask_Sub & nType )
	{
		LogDebug("iFolderType_Mask_Sub");

		BrowserWindow *widget = new BrowserWindow(this, m_pMgr->GetAddr(), m_Root);
		emit SigAddWidget(widget);

		QString strPath = node.GetString(KEY_PATH);
		widget->RequestFolder(strPath);
	}
	else if (iFolderType_Mask_Song & nType
			 && (iFolderType_Mask_Play_Top & nType
			 || iFolderType_Mask_Play_Option & nType
			 || iFolderType_Mask_Play_Check & nType
			 || iFolderType_Mask_Play_Select & nType ))
	{
		LogDebug("play...");

	}
}

void BrowserWindow::SlotRespList(QList<CJsonNode> list)
{
	CJsonNode node = list[0];
	int nType = node.GetInt(KEY_TYPE);

	if ( iFolderType_Mask_Root & nType
		 || iFolderType_Mask_Dir & nType)
	{
		ui->gridLayoutTop->addWidget(m_pMenuInfo);
		ui->gridLayoutBottom->addWidget(m_pMenuIcon);

		SetCoverArt(list);

		m_pMenuInfo->SetTitle(m_Root);
		m_pMenuIcon->SetNodeList(list, MenuIcon::MENU_BROWSER);
	}
	// song
	else
	{
		ui->gridLayoutTop->addWidget(m_pMenuInfo);
		ui->gridLayoutBottom->addWidget(m_pMenuList);

		m_pMenuInfo->SetTitle(m_Root);
		m_pMenuList->SetNodeList(list, MenuList::MENU_BROWSER);

		// get play time
	}



}

void BrowserWindow::SlotReqInfoBot(QString strPath, int nIndex)
{
	strPath = m_Root + "/" + strPath;
	m_pMgr->RequestInfoBot(strPath, nIndex);
}

void BrowserWindow::SlotRespNodeUpdate(CJsonNode node, int nIndex)
{
	if (nIndex < 0)
		return;

	QStandardItem *item = m_pMenuList->GetModel()->item(nIndex);
	item->setData(node.GetString(KEY_BOT), MenuListDelegate::MENU_LIST_ROLE_BOT);
	item->setData(node.GetString(KEY_DURATION), MenuListDelegate::MENU_LIST_ROLE_DURATION);
	m_pMenuList->GetModel()->setItem(nIndex, item);
}

void BrowserWindow::ConnectSigToSlot()
{
	connect(this, SIGNAL(SigAddWidget(QWidget*)), parent(), SLOT(SlotAddWidget(QWidget*)));		// recursive


	connect(m_pMenuInfo, SIGNAL(SigPlayAll()), this, SLOT(SlotPlayAll()));
	connect(m_pMenuInfo, SIGNAL(SigPlayRandom()), this, SLOT(SlotPlayRandom()));
	connect(m_pMenuInfo, SIGNAL(SigSubmenu(int)), this, SLOT(SlotSubmenu(int)));
	connect(m_pMenuInfo, SIGNAL(SigSort()), this, SLOT(SlotSort()));

//	connect(m_pMenuIcon->GetDelegate(), SIGNAL(SigSelectCoverArt(int)), this, SLOT(SlotSelectCoverArt(int)));
	connect(m_pMenuIcon->GetDelegate(), SIGNAL(SigSelectCoverArt(int, QString)), this, SLOT(SlotSelectCoverArt(int, QString)));
	connect(m_pMenuList->GetDelegate(), SIGNAL(SigSelectIcon(QString)), this, SLOT(SlotSelectURL(QString)));
	connect(m_pMenuList->GetDelegate(), SIGNAL(SigSelectTitle(QString)), this, SLOT(SlotSelectURL(QString)));
	connect(m_pMenuList, SIGNAL(SigReqArt(QString, int)), this, SLOT(SlotReqArt(QString, int)));
	connect(m_pMenuList, SIGNAL(SigReqInfoBot(QString, int)), this, SLOT(SlotReqInfoBot(QString, int)));

	connect(m_pMgr, SIGNAL(SigRespList(QList<CJsonNode>)), this, SLOT(SlotRespList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespNodeUpdate(CJsonNode, int)), this, SLOT(SlotRespNodeUpdate(CJsonNode, int)));

}

void BrowserWindow::SetCoverArt(QList<CJsonNode> &list)
{
	QList<CJsonNode> tempList;
	int index = 0;
	QString strCover = "";

	foreach (CJsonNode node, list)
	{
		strCover = ":/resource/Icon-playbar-volume-160.png";
		node.Add(KEY_COVER_ART, strCover);
//		node.AddInt(KEY_ID_UPPER, index);
//		node.AddInt(KEY_TYPE, index);

		tempList.append(node);
		index++;
	}
	list = tempList;
}
