#include "listservice.h"
#include "ui_listservice.h"

#include "listservicedelegate.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/utilnovatron.h"

ListService::ListService(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_Model(new QStandardItemModel),
	m_Delegate(new ListServiceDelegate),
	ui(new Ui::ListService)
{
	ui->setupUi(this);

	Initialize();
}

ListService::~ListService()
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

QList<CJsonNode> ListService::GetNodeList() const
{
	return m_NodeList;
}

void ListService::SetNodeList(const QList<CJsonNode> &NodeList, int nService)
{
	Loading *loading = UtilNovatron::LoadingStart(parentWidget()->parentWidget());

	m_NodeList = NodeList;
	int index = 0;

	if (LIST_SERVICE_SETUP == nService)
	{
		foreach (CJsonNode node, m_NodeList)
		{
//			LogDebug("type : [%d]", node.GetInt(KEY_TYPE));
//			LogDebug("cover art : [%s]", node.GetString(KEY_COVER_ART).toUtf8().data());
//			LogDebug("title : [%s]", node.GetString(KEY_TOP).toUtf8().data());
//			LogDebug("url : [%s]", node.GetString(VAL_URL).toUtf8().data());
			QStandardItem *item = new QStandardItem;
			item->setData(node.GetString(KEY_ID_UPPER), ListServiceDelegate::LIST_SERVICE_ID);
			item->setData(node.GetInt(KEY_TYPE), ListServiceDelegate::LIST_SERVICE_TYPE);
			item->setData(node.GetString(KEY_NAME_CAP), ListServiceDelegate::LIST_SERVICE_TITLE);
			item->setData(node.ToCompactString(), ListServiceDelegate::LIST_SERVICE_RAW);
			item->setData(index, ListServiceDelegate::LIST_SERVICE_INDEX);
			item->setData(false, ListServiceDelegate::LIST_SERVICE_SELECT);

			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);

			index++;
		}
	}

	UtilNovatron::LoadingStop(loading);
	delete loading;
	loading = nullptr;
}

void ListService::ClearNodeList()
{
	m_Model->clear();
	m_NodeList.clear();
	m_SelectMap.clear();
}

void ListService::ClearSelectMap()
{
	int count = m_Model->rowCount();

	for (int i = 0; i < count; i++)
	{
		QModelIndex modelIndex = m_Model->index(i, 0);
//		bool bSelect = qvariant_cast<bool>(item->data(ListServiceDelegate::LIST_SERVICE_SELECT));
//		if (bSelect)
		{
			m_Model->setData(modelIndex, false, ListServiceDelegate::LIST_SERVICE_SELECT);

//			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);

			int id = qvariant_cast<int>(modelIndex.data(ListServiceDelegate::LIST_SERVICE_ID));
			m_SelectMap.remove(id);
		}
	}
}

void ListService::SetAllSelectMap()
{
	int count = m_Model->rowCount();

	for (int i = 0; i < count; i++)
	{
		QModelIndex modelIndex = m_Model->index(i, 0);
		bool bSelect = qvariant_cast<bool>(modelIndex.data(ListServiceDelegate::LIST_SERVICE_SELECT));
		if (!bSelect)
		{
			m_Model->setData(modelIndex, true, ListServiceDelegate::LIST_SERVICE_SELECT);

//			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);

			int id = qvariant_cast<int>(modelIndex.data(ListServiceDelegate::LIST_SERVICE_ID));
			m_SelectMap.insert(id, true);
		}
	}
}

QMap<int, bool> ListService::GetSelectMap() const
{
	return m_SelectMap;
}

void ListService::SetSelectMap(const QMap<int, bool> &SelectMap)
{
	m_SelectMap = SelectMap;
}

QStandardItemModel *ListService::GetModel()
{
	return m_Model;
}

ListServiceDelegate *ListService::GetDelegate()
{
	return m_Delegate;
}

void ListService::SlotSelectCoverArt(int index)
{
	QModelIndex modelIndex = m_Model->index(index, 0);
	bool bSelect = !qvariant_cast<bool>(modelIndex.data(ListServiceDelegate::LIST_SERVICE_SELECT));
	m_Model->setData(modelIndex, bSelect, ListServiceDelegate::LIST_SERVICE_SELECT);

	m_ListView->openPersistentEditor(modelIndex);

	int id = qvariant_cast<int>(modelIndex.data(ListServiceDelegate::LIST_SERVICE_ID));
	if (bSelect)
	{
		m_SelectMap.insert(id, bSelect);
	}
	else
	{
		m_SelectMap.remove(id);
	}
}

void ListService::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_ListView->setViewMode(QListView::ListMode);

	connect(m_Delegate, SIGNAL(SigSelectCoverArt(int)), this, SLOT(SlotSelectCoverArt(int)));

	ui->gridLayout->addWidget(m_ListView);

}

