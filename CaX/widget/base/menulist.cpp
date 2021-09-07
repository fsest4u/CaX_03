#include "menulist.h"
#include "ui_menulist.h"

#include "menulistdelegate.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

MenuList::MenuList(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_Model(new QStandardItemModel),
	m_Delegate(new MenuListDelegate),
	ui(new Ui::MenuList)
{
	ui->setupUi(this);

	Initialize();
}

MenuList::~MenuList()
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

QListView::ViewMode MenuList::GetViewMode()
{
	return m_ListView->viewMode();
}

void MenuList::SetViewMode(QListView::ViewMode mode)
{
	m_ListView->setViewMode(mode);
	m_Delegate->SetViewMode(mode);
}

QStandardItemModel *MenuList::GetModel()
{
	return m_Model;
}

MenuListDelegate *MenuList::GetDelegate()
{
	return m_Delegate;
}

void MenuList::ClearNodeList()
{
	m_Model->clear();
	ui->gridLayout->removeWidget(m_ListView);
}

QList<CJsonNode> MenuList::GetNodeList() const
{
	return m_NodeList;
}

void MenuList::SetNodeList(const QList<CJsonNode> &NodeList)
{
	m_NodeList = NodeList;
	int index = 0;
	foreach (CJsonNode node, m_NodeList)
	{
//		LogDebug("type : [%d]", node.GetInt(KEY_TYPE));
//		LogDebug("icon : [%s]", node.GetString(KEY_ICON).toUtf8().data());
//		LogDebug("title : [%s]", node.GetString(KEY_TOP).toUtf8().data());
//		LogDebug("url : [%s]", node.GetString(VAL_URL).toUtf8().data());
		QStandardItem *item = new QStandardItem;
		item->setData(node.GetString(KEY_ID_UPPER), MenuListDelegate::MENU_LIST_ROLE_ID);
		item->setData(node.GetInt(KEY_TYPE), MenuListDelegate::MENU_LIST_ROLE_TYPE);
		item->setData(node.GetString(KEY_TOP), MenuListDelegate::MENU_LIST_ROLE_TITLE);
		item->setData(node.GetString(KEY_ICON), MenuListDelegate::MENU_LIST_ROLE_ICON);
//		item->setData(node.GetString(KEY_ART), MenuListDelegate::MENU_LIST_ROLE_ART);
		item->setData(node.ToCompactString(), MenuListDelegate::MENU_LIST_ROLE_RAW);

		m_Model->appendRow(item);
		QModelIndex modelIndex = m_Model->indexFromItem(item);
		m_ListView->openPersistentEditor(modelIndex);

		emit SigReqArt(node.GetString(KEY_ART), index);
		index++;
	}

	ui->gridLayout->addWidget(m_ListView);
}

void MenuList::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	SetViewMode(QListView::ListMode);
}

