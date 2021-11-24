#include "listdevice.h"
#include "ui_listdevice.h"

#include "listdevicedelegate.h"

#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"

ListDevice::ListDevice(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_Model(new QStandardItemModel),
	m_Delegate(new ListDeviceDelegate),
	m_pLoading(new Loading(this)),
	ui(new Ui::ListDevice)
{
	ui->setupUi(this);

	Initialize();
}

ListDevice::~ListDevice()
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

	if (m_pLoading)
	{
		delete m_pLoading;
		m_pLoading = nullptr;
	}

	delete ui;

}

QStandardItemModel *ListDevice::GetModel() const
{
	return m_Model;
}

void ListDevice::SetModel(QStandardItemModel *Model)
{
	m_Model = Model;
}

ListDeviceDelegate *ListDevice::GetDelegate() const
{
	return m_Delegate;
}

void ListDevice::SetDelegate(ListDeviceDelegate *Delegate)
{
	m_Delegate = Delegate;
}

void ListDevice::ClearNodeList()
{
	m_Model->clear();
	ui->gridLayout->removeWidget(m_ListView);
}

QList<CJsonNode> ListDevice::GetNodeList() const
{
	return m_NodeList;
}

void ListDevice::SetNodeList(const QList<CJsonNode> &NodeList)
{
	m_pLoading->Start();
	m_Model->clear();
	m_NodeList = NodeList;

	foreach (CJsonNode node, m_NodeList)
	{
		QStandardItem *item = new QStandardItem;
		item->setData(node.GetString(KEY_MAC), ListDeviceDelegate::LIST_DEVICE_MAC);
		item->setData(node.GetString(KEY_ADDR), ListDeviceDelegate::LIST_DEVICE_ADDR);
		item->setData(node.GetString(KEY_VAL), ListDeviceDelegate::LIST_DEVICE_VAL);
		item->setData(node.GetString(KEY_DEV), ListDeviceDelegate::LIST_DEVICE_DEV);

		m_Model->appendRow(item);
		QModelIndex modelIndex = m_Model->indexFromItem(item);
		m_ListView->openPersistentEditor(modelIndex);
	}

	ui->gridLayout->addWidget(m_ListView);
	m_pLoading->Stop();

}

void ListDevice::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_ListView->setViewMode(QListView::ListMode);


}
