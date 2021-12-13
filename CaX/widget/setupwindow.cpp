#include "setupwindow.h"
#include "ui_setupwindow.h"

#include "widget/formTop/infoservice.h"
#include "widget/formBottom/listsetup.h"
#include "widget/formBottom/listsetupdelegate.h"

#include "manager/setupmanager.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

#include "widget/setup.h"

#define MAIN_TITLE	"Setup"


SetupWindow::SetupWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new SetupManager),
	m_pInfoService(new InfoService(this)),
	m_pListSetup(new ListSetup(this)),
	ui(new Ui::SetupWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
}

SetupWindow::~SetupWindow()
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

	if (m_pListSetup)
	{
		delete m_pListSetup;
		m_pListSetup = nullptr;
	}

	delete ui;

}

void SetupWindow::SetupHome(QList<CJsonNode> list, int eventID)
{
	m_EventID = eventID;

	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pListSetup);

//	SetSetupHome(list);

	m_pInfoService->SetTitle(MAIN_TITLE);
	m_pListSetup->SetNodeList(list);
}

void SetupWindow::SlotSelectTitle(QString strID)
{
	LogDebug("good choice title [%s]", strID.toUtf8().data());
	m_pMgr->RequestSetupGroup(m_EventID, strID);
}

void SetupWindow::SlotRespList(QList<CJsonNode> list)
{
	foreach (CJsonNode node, list)
	{
		LogDebug("node [%s]", node.ToCompactByteArray().data());
	}
}

void SetupWindow::ConnectSigToSlot()
{
	connect(m_pListSetup->GetDelegate(), SIGNAL(SigSelectTitle(QString)), this, SLOT(SlotSelectTitle(QString)));
	connect(m_pMgr, SIGNAL(SigRespList(QList<CJsonNode>)), this, SLOT(SlotRespList(QList<CJsonNode>)));
}

//void SetupWindow::SetSetupHome(QList<CJsonNode> &list)
//{
//	QList<CJsonNode> tempList;
//	int index = 0;
//	QString strCover = "";

//	foreach (CJsonNode node, list)
//	{
//		node.Add(KEY_COVER_ART, strCover);
//		node.AddInt(KEY_TYPE, index);

//		tempList.append(node);
//		index++;
//	}

//	list = tempList;
//}
