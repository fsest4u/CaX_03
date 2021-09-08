#include "inputwindow.h"
#include "ui_inputwindow.h"

#include "base/menuinfo.h"
#include "base/menuicon.h"
#include "base/menuicondelegate.h"

#include "manager/inputmanager.h"

#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"

#define INPUT_TITLE	"Input"


InputWindow::InputWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new InputManager),
	m_pMenuInfo(new MenuInfo(this)),
	m_pMenuIcon(new MenuIcon(this)),
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

void InputWindow::InputHome(QList<CJsonNode> list)
{
	ui->gridLayoutTop->addWidget(m_pMenuInfo);
	ui->gridLayoutBottom->addWidget(m_pMenuIcon);

	SetInputHome(list);

	m_pMenuInfo->SetTitle(INPUT_TITLE);
	m_pMenuIcon->SetNodeList(list);
}

void InputWindow::SlotSelectCoverArt(int nType)
{
	m_pMgr->RequestInput(nType);
}

void InputWindow::ConnectSigToSlot()
{

	connect(m_pMenuIcon->GetDelegate(), SIGNAL(SigSelectCoverArt(int)), this, SLOT(SlotSelectCoverArt(int)));

}

void InputWindow::SetInputHome(QList<CJsonNode> &srclist)
{
	QList<CJsonNode> dstList;
	int nID = -1;
	QString strCover = "";

	foreach (CJsonNode node, srclist)
	{
		if (node.GetString(KEY_RIGHT).contains("AES/EBU"))
		{
			nID = 0;
			strCover = ":/resource/baseline_search_black_24dp.png";
		}
		else if (node.GetString(KEY_RIGHT).contains("COAXIAL"))
		{
			nID = 1;
			strCover = ":/resource/baseline_search_black_24dp.png";
		}
		else if (node.GetString(KEY_RIGHT).contains("TOSLINK"))
		{
			nID = 2;
			strCover = ":/resource/baseline_search_black_24dp.png";
		}
		else if (node.GetString(KEY_RIGHT).contains("ANALOG IN"))
		{
			nID = 3;
			strCover = ":/resource/baseline_search_black_24dp.png";
		}
		else if (node.GetString(KEY_RIGHT).contains("AUX IN"))
		{
			nID = 4;
			strCover = ":/resource/baseline_search_black_24dp.png";
		}
		else if (node.GetString(KEY_RIGHT).contains("PHONO IN"))
		{
			nID = 5;
			strCover = ":/resource/baseline_search_black_24dp.png";
		}
		else
		{
			LogDebug("not found type");
			continue;
		}
		node.AddInt(KEY_ID_UPPER, nID);
		node.AddInt(KEY_TYPE, nID);
		node.Add(KEY_COVER_ART, strCover);
		node.Add(KEY_NAME, node.GetString(KEY_LEFT) + "/" + node.GetString(KEY_RIGHT));

		dstList.append(node);
	}
	srclist = dstList;
}
