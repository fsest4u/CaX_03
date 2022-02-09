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

QList<CJsonNode> ListSetup::GetNodeList() const
{
	return m_NodeList;
}

void ListSetup::SetNodeList(const QList<CJsonNode> &NodeList)
{
	int index = 0;
	m_NodeList = NodeList;

	foreach (CJsonNode node, m_NodeList)
	{
//		LogDebug("type : [%s]", node.ToCompactByteArray().data());

		QStandardItem *item = new QStandardItem;
		item->setData(node.GetString(KEY_ID_UPPER), ListSetupDelegate::LIST_SETUP_ID);
		item->setData(node.GetString(KEY_NAME_CAP), ListSetupDelegate::LIST_SETUP_TITLE);
		item->setData(index, ListSetupDelegate::LIST_SETUP_INDEX);

		m_Model->appendRow(item);
		QModelIndex modelIndex = m_Model->indexFromItem(item);
		m_ListView->openPersistentEditor(modelIndex);

		index++;
	}

}

void ListSetup::ClearNodeList()
{
	m_Model->clear();
	m_NodeList.clear();
}

void ListSetup::SetEditor(int index)
{
	QModelIndex modelIndex = m_Model->index(index, 0);
	QString nothing = qvariant_cast<QString>(modelIndex.data(ListSetupDelegate::LIST_SETUP_MAX));
	m_Model->setData(modelIndex, nothing + " ", ListSetupDelegate::LIST_SETUP_MAX);

	m_ListView->openPersistentEditor(modelIndex);
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

	ui->gridLayout->addWidget(m_ListView);

}
