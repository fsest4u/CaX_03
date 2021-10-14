#include "inputwindow.h"
#include "ui_inputwindow.h"

#include "widget/form/formsort.h"

#include "widget/formTop/infoservice.h"
#include "widget/formBottom/iconservice.h"
#include "widget/formBottom/iconservicedelegate.h"

#include "manager/inputmanager.h"

#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"

#define MAIN_TITLE	"Input"


InputWindow::InputWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new InputManager),
	m_pInfoService(new InfoService(this)),
	m_pIconService(new IconService(this)),
	ui(new Ui::InputWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();

	m_pInfoService->GetFormSort()->ShowResize();

}

InputWindow::~InputWindow()
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

void InputWindow::InputHome(QList<CJsonNode> list)
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pIconService);

	SetInputHome(list);

	m_pInfoService->SetSubtitle(MAIN_TITLE);
	m_pIconService->SetNodeList(list, IconService::ICON_SERVICE_INPUT);
}

void InputWindow::SlotSelectTitle(int nType)
{
	m_pMgr->RequestInput(nType);
}

void InputWindow::SlotResize()
{
	LogDebug("click resize");

}

void InputWindow::ConnectSigToSlot()
{

//	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectPlay(int)), this, SLOT(SlotSelectPlay(int)));
	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int)), this, SLOT(SlotSelectTitle(int)));
//	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));

	connect(m_pInfoService->GetFormSort(), SIGNAL(SigResize()), this, SLOT(SlotResize()));

}

void InputWindow::SetInputHome(QList<CJsonNode> &list)
{
	QList<CJsonNode> tempList;
	int index = 0;
	QString strCover = "";

	foreach (CJsonNode node, list)
	{
		if (node.GetString(KEY_RIGHT).contains("AES/EBU"))
		{
			strCover = ":/resource/input-img160-aesebu-n@3x.png";
		}
		else if (node.GetString(KEY_RIGHT).contains("COAXIAL"))
		{
			strCover = ":/resource/input-img160-coaxial-n@3x.png";
		}
		else if (node.GetString(KEY_RIGHT).contains("TOSLINK"))
		{
			strCover = ":/resource/input-img160-toslink-n@3x.png";
		}
		else if (node.GetString(KEY_RIGHT).contains("ANALOG IN"))
		{
			strCover = ":/resource/input-img160-analogin-n@3x.png";
		}
		else if (node.GetString(KEY_RIGHT).contains("AUX IN"))
		{
			strCover = ":/resource/input-img160-auxin-n@3x.png";
		}
		else if (node.GetString(KEY_RIGHT).contains("PHONO IN"))
		{
			strCover = ":/resource/input-img160-ponoin-n@3x.png";
		}
		else
		{
			LogDebug("not found type");
			strCover = "";
			continue;
		}

		node.Add(KEY_COVER_ART, strCover);
		node.AddInt(KEY_ID_UPPER, index);
		node.AddInt(KEY_TYPE, index);

		tempList.append(node);

		index++;
	}
	list = tempList;
}
