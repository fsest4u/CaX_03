#include "listsetup.h"
#include "ui_listsetup.h"

#include "listsetupdelegate.h"

#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"

ListSetup::ListSetup(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_Model(new QStandardItemModel),
	m_Delegate(new ListSetupDelegate),
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

	delete ui;
}

void ListSetup::ClearNodeList()
{
	m_Model->clear();
	ui->gridLayout->removeWidget(m_ListView);
}

QList<CJsonNode> ListSetup::GetNodeList() const
{
	return m_NodeList;
}

void ListSetup::SetNodeList(const QList<CJsonNode> &NodeList)
{
	m_NodeList = NodeList;

	foreach (CJsonNode node, m_NodeList)
	{
		LogDebug("type : [%s]", node.ToCompactByteArray().data());

		QStandardItem *item = new QStandardItem;
		item->setData(node.GetString(KEY_ID_UPPER), ListSetupDelegate::LIST_SETUP_ID);
		item->setData(node.GetString(KEY_NAME_CAP), ListSetupDelegate::LIST_SETUP_TITLE);

		m_Model->appendRow(item);
		QModelIndex modelIndex = m_Model->indexFromItem(item);
		m_ListView->openPersistentEditor(modelIndex);
	}

	ui->gridLayout->addWidget(m_ListView);

}

QStandardItemModel *ListSetup::GetModel()
{
	return m_Model;
}

ListSetupDelegate *ListSetup::GetDelegate()
{
	return m_Delegate;
}

void ListSetup::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_ListView->setViewMode(QListView::ListMode);
}
