#include <QMouseEvent>

#include "menuinfo.h"
#include "ui_menuinfo.h"

#include "dialog/submenudialog.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

MenuInfo::MenuInfo(QWidget *parent) :
	QWidget(parent),
	m_pSubmenuDlg(new SubmenuDialog(this)),
	ui(new Ui::MenuInfo)
{
	ui->setupUi(this);

	ui->framePlayAll->installEventFilter(this);
	ui->framePlayRandom->installEventFilter(this);
	ui->frameSubmenu->installEventFilter(this);
	ui->frameSort->installEventFilter(this);

}

MenuInfo::~MenuInfo()
{
	delete ui;

	if (m_pSubmenuDlg)
	{
		delete m_pSubmenuDlg;
		m_pSubmenuDlg = nullptr;
	}
}

void MenuInfo::SetTitle(const QString title)
{
	ui->labelTitle->setText(title);
}


bool MenuInfo::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->framePlayAll)
		{
			emit SigPlayAll();
		}
		else if (object == ui->framePlayRandom)
		{
			emit SigPlayRandom();
		}
		else if (object == ui->frameSubmenu)
		{
			ShowSubmenuDialog();
		}
		else if (object == ui->frameSort)
		{
			emit SigSort();
		}
	}

	return QObject::eventFilter(object, event);
}

void MenuInfo::SetSubmenuIService()
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
		node.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
		node.Add(KEY_NAME, listName.at(i));
		list.append(node);
	}
	m_pSubmenuDlg->SetItemList(list);
}

void MenuInfo::SetSubmenuFmRadio()
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
		node.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
		node.Add(KEY_NAME, listName.at(i));
		list.append(node);
	}
	m_pSubmenuDlg->SetItemList(list);
}

void MenuInfo::SetSubmenuDabRadio()
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
		node.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
		node.Add(KEY_NAME, listName.at(i));
		list.append(node);
	}
	m_pSubmenuDlg->SetItemList(list);
}

void MenuInfo::ShowSubmenuDialog()
{
	if (m_pSubmenuDlg->isHidden())
	{
		m_pSubmenuDlg->move(mapToGlobal(ui->frameSubmenu->geometry().bottomRight()).x() - m_pSubmenuDlg->width()
						, mapToGlobal(ui->frameSubmenu->geometry().bottomRight()).y());

		if (m_pSubmenuDlg->exec())
		{
			int id = m_pSubmenuDlg->GetID();
			LogDebug("sort type id [%d]", id);
			emit SigSubmenu(id);

		}
	}
	else
	{
		m_pSubmenuDlg->close();
	}
}
