#include "setupwindow.h"
#include "ui_setupwindow.h"

#include "widget/formTop/infoservice.h"
#include "widget/formBottom/listservice.h"
#include "widget/formBottom/listservicedelegate.h"

#include "manager/setupmanager.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

#include "widget/setup.h"

#define MAIN_TITLE	"Setup"


SetupWindow::SetupWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new SetupManager),
	m_pInfoService(new InfoService(this)),
	m_pListService(new ListService(this)),
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

	if (m_pListService)
	{
		delete m_pListService;
		m_pListService = nullptr;
	}

	delete ui;

}

void SetupWindow::SetupHome(QList<CJsonNode> list, int eventID)
{
	m_nEventID = eventID;

	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pListService);

	SetSetupHome(list);

	m_pInfoService->SetSubtitle(MAIN_TITLE);
	m_pListService->SetNodeList(list, ListService::LIST_SERVICE_SETUP);
}

void SetupWindow::SlotSelectCoverArt(QString rawData)
{
	LogDebug("good choice icon [%s]", rawData.toUtf8().data());
}

void SetupWindow::SlotSelectTitle(QString rawData)
{
//	LogDebug("good choice title [%s]", rawData.toUtf8().data());
	CJsonNode node;
	if (!node.SetContent(rawData))
	{
		return;
	}
	QString id = node.GetString(KEY_ID_UPPER);
	m_pMgr->RequestSetupGroup(m_nEventID, id);
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
	connect(m_pListService->GetDelegate(), SIGNAL(SigSelectCoverArt(QString)), this, SLOT(SlotSelectCoverArt(QString)));
	connect(m_pListService->GetDelegate(), SIGNAL(SigSelectTitle(QString)), this, SLOT(SlotSelectTitle(QString)));

	connect(m_pMgr, SIGNAL(SigRespList(QList<CJsonNode>)), this, SLOT(SlotRespList(QList<CJsonNode>)));

}

void SetupWindow::SetSetupHome(QList<CJsonNode> &list)
{
	QList<CJsonNode> tempList;
	int index = 0;
	QString strCover = "";

	foreach (CJsonNode node, list)
	{
		node.Add(KEY_COVER_ART, strCover);
		node.AddInt(KEY_TYPE, index);

		tempList.append(node);
		index++;
	}

	list = tempList;
}
