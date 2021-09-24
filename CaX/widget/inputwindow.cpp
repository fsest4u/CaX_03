#include "inputwindow.h"
#include "ui_inputwindow.h"

#include "base/infoservice.h"
#include "base/iconservice.h"
#include "base/iconservicedelegate.h"

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

	m_pInfoService->SetTitle(MAIN_TITLE);
	m_pIconService->SetNodeList(list, IconService::ICON_SERVICE_INPUT);
}

void InputWindow::SlotSelectCoverArt(int nType)
{
	m_pMgr->RequestInput(nType);
}

void InputWindow::ConnectSigToSlot()
{

	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectCoverArt(int)), this, SLOT(SlotSelectCoverArt(int)));

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
			strCover = ":/resource/Icon-playbar-volume-160.png";
		}
		else if (node.GetString(KEY_RIGHT).contains("COAXIAL"))
		{
			strCover = ":/resource/Icon-playbar-volume-160.png";
		}
		else if (node.GetString(KEY_RIGHT).contains("TOSLINK"))
		{
			strCover = ":/resource/Icon-playbar-volume-160.png";
		}
		else if (node.GetString(KEY_RIGHT).contains("ANALOG IN"))
		{
			strCover = ":/resource/Icon-playbar-volume-160.png";
		}
		else if (node.GetString(KEY_RIGHT).contains("AUX IN"))
		{
			strCover = ":/resource/Icon-playbar-volume-160.png";
		}
		else if (node.GetString(KEY_RIGHT).contains("PHONO IN"))
		{
			strCover = ":/resource/Icon-playbar-volume-160.png";
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
