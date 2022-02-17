#include "listsetup.h"
#include "ui_listsetup.h"

#include "listsetupdelegate.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"

ListSetup::ListSetup(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_ListViewSub(new QListView),
//	m_ListViewDetail(new QListView),
	m_Model(new QStandardItemModel),
	m_ModelSub(new QStandardItemModel),
//	m_ModelDetail(new QStandardItemModel),
	m_Delegate(new ListSetupDelegate),
	m_DelegateSub(new ListSetupDelegate),
//	m_DelegateDetail(new ListSetupDelegate),
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
//	if (m_ListViewDetail)
//	{
//		delete m_ListViewDetail;
//		m_ListViewDetail = nullptr;
//	}
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
//	if (m_ModelDetail)
//	{
//		delete m_ModelDetail;
//		m_ModelDetail = nullptr;
//	}
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
//	if (m_DelegateDetail)
//	{
//		delete m_DelegateDetail;
//		m_DelegateDetail = nullptr;
//	}

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

//QList<CJsonNode> ListSetup::GetNodeListDetail() const
//{
//	return m_NodeListDetail;
//}

void ListSetup::SetNodeList(const QList<CJsonNode> &NodeList)
{
	int index = 0;
	m_NodeList = NodeList;

	foreach (CJsonNode node, m_NodeList)
	{
//		LogDebug("node main : [%s]", node.ToCompactByteArray().data());

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
	int index = 0;
	m_NodeListSub = NodeList;

	foreach (CJsonNode node, m_NodeListSub)
	{
//		LogDebug("node sub : [%s]", node.ToCompactByteArray().data());
		QString id = node.GetString(KEY_ID_UPPER);
		if (!id.compare(ID_RIP_FREE_DB_INSTALL)
				|| !id.compare(ID_RIP_FREE_DB_UPDATE)
				|| !id.compare(ID_RIP_FREE_DB_DELETE))
		{
			continue;
		}

		QString subtitle = node.GetString(KEY_VALUE);
		if (subtitle.isEmpty())
		{
			subtitle = node.GetString(KEY_VOLUME_CAP);
		}

		QStandardItem *item = new QStandardItem;
		item->setData(id, ListSetupDelegate::LIST_SETUP_ID);
		item->setData(node.GetString(KEY_TYPE), ListSetupDelegate::LIST_SETUP_TYPE);
		item->setData(node.GetString(KEY_NAME_CAP), ListSetupDelegate::LIST_SETUP_TITLE);
		item->setData(subtitle, ListSetupDelegate::LIST_SETUP_SUBTITLE);
		item->setData(node.ToCompactByteArray().data(), ListSetupDelegate::LIST_SETUP_RAW);
		item->setData(index, ListSetupDelegate::LIST_SETUP_INDEX);

		m_ModelSub->appendRow(item);

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

QListView *ListSetup::GetListView()
{
	return m_ListView;
}

QListView *ListSetup::GetListViewSub()
{
	return m_ListViewSub;
}

QStandardItemModel *ListSetup::GetModel()
{
	return m_Model;
}

QStandardItemModel *ListSetup::GetModelSub()
{
	return m_ModelSub;
}

ListSetupDelegate *ListSetup::GetDelegate()
{
	return m_Delegate;
}

ListSetupDelegate *ListSetup::GetDelegateSub()
{
	return m_DelegateSub;
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

	ui->gridLayoutSetup->addWidget(m_ListView);
	ui->gridLayoutSetupSub->addWidget(m_ListViewSub);
}
