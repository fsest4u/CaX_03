#include "inputwindow.h"
#include "ui_inputwindow.h"

#include "base/menuinfo.h"
#include "base/menuicon.h"
#include "base/menuicondelegate.h"

#include "manager/inputmanager.h"

#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"

#define MAIN_TITLE	"Input"


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

	m_pMenuInfo->SetTitle(MAIN_TITLE);
	m_pMenuIcon->SetNodeList(list, MenuIcon::MENU_INPUT);
}

void InputWindow::SlotSelectCoverArt(int nType)
{
	m_pMgr->RequestInput(nType);
}

void InputWindow::ConnectSigToSlot()
{

	connect(m_pMenuIcon->GetDelegate(), SIGNAL(SigSelectCoverArt(int)), this, SLOT(SlotSelectCoverArt(int)));

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
