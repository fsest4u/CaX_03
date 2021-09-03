#include "menuicon.h"
#include "ui_menuicon.h"

#include "menuicondelegate.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

MenuIcon::MenuIcon(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_Model(new QStandardItemModel),
	m_Delegate(new MenuIconDelegate),
	ui(new Ui::MenuIcon)
{
	ui->setupUi(this);

	Initialize();
}

MenuIcon::~MenuIcon()
{
	delete ui;

	if (m_ListView)
	{
		delete m_ListView;
		m_ListView = nullptr;
	}
	if (m_Model)
	{
		delete m_Model;
		m_Model = nullptr;
	}
	if (m_Delegate)
	{
		delete m_Delegate;
		m_Delegate = nullptr;
	}
}

void MenuIcon::ClearNodeList()
{
	m_Model->clear();
	ui->gridLayout->removeWidget(m_ListView);
}

QList<CJsonNode> MenuIcon::GetNodeList() const
{
	return m_NodeList;
}

void MenuIcon::SetNodeList(const QList<CJsonNode> &NodeList)
{
	m_NodeList = NodeList;
	foreach (CJsonNode node, m_NodeList)
	{
		QStandardItem *item = new QStandardItem;
		item->setData(node.GetInt(KEY_TYPE), MenuIconDelegate::MENU_ICON_ROLE_ID);
		item->setData(node.GetString(KEY_COVER_ART), MenuIconDelegate::MENU_ICON_ROLE_COVER);
		item->setData(node.GetString(KEY_NAME), MenuIconDelegate::MENU_ICON_ROLE_TITLE);
		m_Model->appendRow(item);
		QModelIndex modelIndex = m_Model->indexFromItem(item);
		m_ListView->openPersistentEditor(modelIndex);
	}

	ui->gridLayout->addWidget(m_ListView);
}

QListView::ViewMode MenuIcon::GetViewMode()
{
	return m_ListView->viewMode();
}

void MenuIcon::SetViewMode(QListView::ViewMode mode)
{
	m_ListView->setViewMode(mode);
	m_Delegate->SetViewMode(mode);

}

QStandardItemModel *MenuIcon::GetModel()
{
	return m_Model;
}

MenuIconDelegate *MenuIcon::GetDelegate()
{
	return m_Delegate;
}

void MenuIcon::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	SetViewMode(QListView::IconMode);
}
