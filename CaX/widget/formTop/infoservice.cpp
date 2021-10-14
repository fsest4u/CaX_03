#include <QMouseEvent>

#include "infoservice.h"
#include "ui_infoservice.h"

#include "widget/form/formtitle.h"
#include "widget/form/formplay.h"
#include "widget/form/formsort.h"

#include "dialog/submenudialog.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

InfoService::InfoService(QWidget *parent) :
	QWidget(parent),
	m_pSubmenuDlg(new SubmenuDialog(this)),
	m_pFormTitle(new FormTitle(this)),
	m_pFormPlay(new FormPlay(this)),
	m_pFormSort(new FormSort(this)),
	ui(new Ui::InfoService)
{
	ui->setupUi(this);

	ConnectSigToSlot();

}

InfoService::~InfoService()
{
	if (m_pSubmenuDlg)
	{
		delete m_pSubmenuDlg;
		m_pSubmenuDlg = nullptr;
	}

	if (m_pFormTitle)
	{
		delete m_pFormTitle;
		m_pFormTitle = nullptr;
	}

	if (m_pFormPlay)
	{
		delete m_pFormPlay;
		m_pFormPlay = nullptr;
	}

	if (m_pFormSort)
	{
		delete m_pFormSort;
		m_pFormSort = nullptr;
	}

	delete ui;

}

void InfoService::SetTitle(const QString title)
{
	m_pFormTitle->SetTitleFontSize(42);
	m_pFormTitle->SetTitle(title);
}

void InfoService::SetSubtitle(const QString title)
{
	m_pFormTitle->SetSubtitleFontSize(42);
	m_pFormTitle->SetSubtitle(title);
}

void InfoService::SlotTitle()
{
	LogDebug("good choice title");
}

void InfoService::SlotSubtitle()
{
	LogDebug("good choice subtitle");
}


void InfoService::ConnectSigToSlot()
{
	ui->gridLayoutFormTitle->addWidget(m_pFormTitle);
	ui->gridLayoutFormPlay->addWidget(m_pFormPlay);
	ui->gridLayoutFormSort->addWidget(m_pFormSort);

	connect(m_pFormTitle, SIGNAL(SigTitle()), this, SLOT(SlotTitle()));
	connect(m_pFormTitle, SIGNAL(SigSubtitle()), this, SLOT(SlotSubtitle()));
}

void InfoService::SetSubmenuIService()
{
	QList<CJsonNode> list;

	QList<int> listID = {
		ISERVICE_NONE
	};
	QList<QString> listName = {
		"Test"
	};

	CJsonNode node(JSON_OBJECT);
	for (int i = 0; i < listID.count(); i++)
	{
		node.AddInt(KEY_ID_UPPER, listID.at(i));
		node.Add(KEY_COVER_ART, ":/resource/Icon-playbar-volume-160.png");
		node.Add(KEY_NAME, listName.at(i));
		list.append(node);
	}
	m_pSubmenuDlg->SetItemList(list);
}

void InfoService::SetSubmenuFmRadio()
{
	QList<CJsonNode> list;

	QList<int> listID = {
		FM_SEARCH_ALL_DELETE,
		FM_SEARCH_ALL,
		FM_ADD,
		FM_DELETE,
		FM_RESERVE_LIST
	};
	QList<QString> listName = {
		"Delete and search all",
		"Search all",
		"Add",
		"Delete",
		"Reserved list"
	};

	CJsonNode node(JSON_OBJECT);
	for (int i = 0; i < listID.count(); i++)
	{
		node.AddInt(KEY_ID_UPPER, listID.at(i));
		node.Add(KEY_COVER_ART, ":/resource/Icon-playbar-volume-160.png");
		node.Add(KEY_NAME, listName.at(i));
		list.append(node);
	}
	m_pSubmenuDlg->SetItemList(list);
}

void InfoService::SetSubmenuDabRadio()
{
	QList<CJsonNode> list;

	QList<int> listID = {
		DAB_SEARCH_ALL_DELETE,
		DAB_SEARCH_ALL,
//		DAB_ADD,
		DAB_DELETE,
		DAB_RESERVE_LIST
	};
	QList<QString> listName = {
		"Delete and search all",
		"Search all",
		"Add",
		"Delete",
		"Reserved list"
	};

	CJsonNode node(JSON_OBJECT);
	for (int i = 0; i < listID.count(); i++)
	{
		node.AddInt(KEY_ID_UPPER, listID.at(i));
		node.Add(KEY_COVER_ART, ":/resource/Icon-playbar-volume-160.png");
		node.Add(KEY_NAME, listName.at(i));
		list.append(node);
	}
	m_pSubmenuDlg->SetItemList(list);
}

FormPlay *InfoService::GetFormPlay()
{
	return m_pFormPlay;
}

FormSort *InfoService::GetFormSort()
{
	return m_pFormSort;
}

void InfoService::ShowSubmenuDialog()
{
//	if (m_pSubmenuDlg->isHidden())
//	{
//		m_pSubmenuDlg->move(mapToGlobal(ui->frameSubmenu->geometry().bottomRight()).x() - m_pSubmenuDlg->width()
//						, mapToGlobal(ui->frameSubmenu->geometry().bottomRight()).y());

//		if (m_pSubmenuDlg->exec())
//		{
//			int id = m_pSubmenuDlg->GetID();
//			LogDebug("sort type id [%d]", id);
//			emit SigSubmenu(id);

//		}
//	}
//	else
//	{
//		m_pSubmenuDlg->close();
//	}
}
