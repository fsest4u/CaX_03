#include "listsetup.h"
#include "ui_listsetup.h"

#include "listsetupdelegate.h"

#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"

ListSetup::ListSetup(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_ListViewSub(new QListView),
	m_ListViewDetail(new QListView),
	m_Model(new QStandardItemModel),
	m_ModelSub(new QStandardItemModel),
	m_ModelDetail(new QStandardItemModel),
	m_Delegate(new ListSetupDelegate),
	m_DelegateSub(new ListSetupDelegate),
	m_DelegateDetail(new ListSetupDelegate),
	ui(new Ui::ListSetup)
{
	ui->setupUi(this);

	Initialize();
}

ListSetup::~ListSetup()
{
	if (m_ListView)
	{
		delete m_ListView;
		m_ListView = nullptr;
	}
	if (m_ListViewSub)
	{
		delete m_ListViewSub;
		m_ListViewSub = nullptr;
	}
	if (m_ListViewDetail)
	{
		delete m_ListViewDetail;
		m_ListViewDetail = nullptr;
	}
	if (m_Model)
	{
		delete m_Model;
		m_Model = nullptr;
	}
	if (m_ModelSub)
	{
		delete m_ModelSub;
		m_ModelSub = nullptr;
	}
	if (m_ModelDetail)
	{
		delete m_ModelDetail;
		m_ModelDetail = nullptr;
	}
	if (m_Delegate)
	{
		delete m_Delegate;
		m_Delegate = nullptr;
	}
	if (m_DelegateSub)
	{
		delete m_DelegateSub;
		m_DelegateSub = nullptr;
	}
	if (m_DelegateDetail)
	{
		delete m_DelegateDetail;
		m_DelegateDetail = nullptr;
	}

	delete ui;
}

QList<CJsonNode> ListSetup::GetNodeList() const
{
	return m_NodeList;
}

QList<CJsonNode> ListSetup::GetNodeListSub() const
{
	return m_NodeListSub;
}

QList<CJsonNode> ListSetup::GetNodeListDetail() const
{
	return m_NodeListDetail;
}

void ListSetup::SetNodeList(const QList<CJsonNode> &NodeList)
{
	LogDebug("setup main menu ========================");

	int index = 0;
	m_NodeList = NodeList;

	foreach (CJsonNode node, m_NodeList)
	{
		LogDebug("type : [%s]", node.ToCompactByteArray().data());

		QStandardItem *item = new QStandardItem;
		item->setData(node.GetString(KEY_ID_UPPER), ListSetupDelegate::LIST_SETUP_ID);
		item->setData(node.GetString(KEY_NAME_CAP), ListSetupDelegate::LIST_SETUP_TITLE);
		item->setData(index, ListSetupDelegate::LIST_SETUP_INDEX);

		m_Model->appendRow(item);

		index++;
	}

}

void ListSetup::SetNodeListSub(const QList<CJsonNode> &NodeList)
{
	LogDebug("setup sub menu ========================");

	int index = 0;
	m_NodeListSub = NodeList;

	foreach (CJsonNode node, m_NodeListSub)
	{
		LogDebug("type : [%s]", node.ToCompactByteArray().data());

		QStandardItem *item = new QStandardItem;
		item->setData(node.GetString(KEY_ID_UPPER), ListSetupDelegate::LIST_SETUP_ID);
		item->setData(node.GetString(KEY_TYPE), ListSetupDelegate::LIST_SETUP_TYPE);
		item->setData(node.GetString(KEY_NAME_CAP), ListSetupDelegate::LIST_SETUP_TITLE);
		item->setData(node.GetString(KEY_VALUE), ListSetupDelegate::LIST_SETUP_SUBTITLE);
		item->setData(node.GetStringList(KEY_VALUES), ListSetupDelegate::LIST_SETUP_VALUES);
		item->setData(node.GetStringList(KEY_KEYS), ListSetupDelegate::LIST_SETUP_KEYS);
		item->setData(node.GetString(KEY_FORM), ListSetupDelegate::LIST_SETUP_FORM);
		item->setData(index, ListSetupDelegate::LIST_SETUP_INDEX);

		m_ModelSub->appendRow(item);

		index++;
	}
}

void ListSetup::SetNodeListDetail(const QList<CJsonNode> &NodeList)
{
	LogDebug("setup detail menu ========================");

	int index = 0;
	m_NodeListDetail = NodeList;

	foreach (CJsonNode node, m_NodeListDetail)
	{
		LogDebug("type : [%s]", node.ToCompactByteArray().data());

		QStandardItem *item = new QStandardItem;
		item->setData(node.GetString(KEY_ID_UPPER), ListSetupDelegate::LIST_SETUP_ID);
		item->setData(node.GetString(KEY_NAME_CAP), ListSetupDelegate::LIST_SETUP_TITLE);
		item->setData(index, ListSetupDelegate::LIST_SETUP_INDEX);

		m_ModelDetail->appendRow(item);

		index++;
	}
}

void ListSetup::ClearNodeList()
{
	m_Model->clear();
	m_NodeList.clear();
}

void ListSetup::ClearNodeListSub()
{
	m_ModelSub->clear();
	m_NodeListSub.clear();
}

void ListSetup::ClearNodeListDetail()
{
	m_ModelDetail->clear();
	m_NodeListDetail.clear();
}

//void ListSetup::SetEditor(int index)
//{
//	QModelIndex modelIndex = m_Model->index(index, 0);
//	QString nothing = qvariant_cast<QString>(modelIndex.data(ListSetupDelegate::LIST_SETUP_MAX));
//	m_Model->setData(modelIndex, nothing + " ", ListSetupDelegate::LIST_SETUP_MAX);

//	m_ListView->openPersistentEditor(modelIndex);
//}

QListView *ListSetup::GetListView()
{
	return m_ListView;
}

QListView *ListSetup::GetListViewSub()
{
	return m_ListViewSub;
}

QListView *ListSetup::GetListViewDetail()
{
	return m_ListViewDetail;
}

QStandardItemModel *ListSetup::GetModel()
{
	return m_Model;
}

QStandardItemModel *ListSetup::GetModelSub()
{
	return m_ModelSub;
}

QStandardItemModel *ListSetup::GetModelDetail()
{
	return m_ModelDetail;
}

ListSetupDelegate *ListSetup::GetDelegate()
{
	return m_Delegate;
}

ListSetupDelegate *ListSetup::GetDelegateSub()
{
	return m_DelegateSub;
}

ListSetupDelegate *ListSetup::GetDelegateDetail()
{
	return m_DelegateDetail;
}

void ListSetup::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_ListView->setViewMode(QListView::ListMode);

	m_ListViewSub->setItemDelegate(m_DelegateSub);
	m_ListViewSub->setModel(m_ModelSub);
	m_ListViewSub->setResizeMode(QListView::Adjust);
	m_ListViewSub->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_ListViewSub->setViewMode(QListView::ListMode);

	m_ListViewDetail->setItemDelegate(m_DelegateDetail);
	m_ListViewDetail->setModel(m_ModelDetail);
	m_ListViewDetail->setResizeMode(QListView::Adjust);
	m_ListViewDetail->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_ListViewDetail->setViewMode(QListView::ListMode);

	ui->gridLayoutSetup->addWidget(m_ListView);
	ui->gridLayoutSetupSub->addWidget(m_ListViewSub);
	ui->gridLayoutSetupDetail->addWidget(m_ListViewDetail);
}
