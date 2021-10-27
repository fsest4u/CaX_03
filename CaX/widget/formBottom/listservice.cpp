#include "listservice.h"
#include "ui_listservice.h"

#include "listservicedelegate.h"

#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"

ListService::ListService(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_Model(new QStandardItemModel),
	m_Delegate(new ListServiceDelegate),
	m_pLoading(new Loading(this)),
	ui(new Ui::ListService)
{
	ui->setupUi(this);

	Initialize();
}

ListService::~ListService()
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

	if (m_pLoading)
	{
		delete m_pLoading;
		m_pLoading = nullptr;
	}
}

void ListService::ClearNodeList()
{
	m_Model->clear();
	ui->gridLayout->removeWidget(m_ListView);
}

QList<CJsonNode> ListService::GetNodeList() const
{
	return m_NodeList;
}

void ListService::SetNodeList(const QList<CJsonNode> &NodeList, int nService)
{
	m_pLoading->Start();
	m_NodeList = NodeList;
	int index = 0;

	if (LIST_SERVICE_BROWSER == nService)
	{
		foreach (CJsonNode node, m_NodeList)
		{
	//		LogDebug("type : [%d]", node.GetInt(KEY_TYPE));
	//		LogDebug("icon : [%s]", node.GetString(KEY_ICON).toUtf8().data());
	//		LogDebug("title : [%s]", node.GetString(KEY_TOP).toUtf8().data());
	//		LogDebug("url : [%s]", node.GetString(VAL_URL).toUtf8().data());
			QStandardItem *item = new QStandardItem;
			item->setData(node.GetString(KEY_ID_UPPER), ListServiceDelegate::LIST_SERVICE_ID);
			item->setData(node.GetInt(KEY_TYPE), ListServiceDelegate::LIST_SERVICE_TYPE);
			item->setData(node.GetString(KEY_ICON), ListServiceDelegate::LIST_SERVICE_COVER_ART);
			item->setData(node.GetString(KEY_PATH), ListServiceDelegate::LIST_SERVICE_TITLE);
			item->setData(node.GetString(KEY_BOT), ListServiceDelegate::LIST_SERVICE_SUBTITLE);
			item->setData(node.GetString(KEY_DURATION), ListServiceDelegate::LIST_SERVICE_DURATION);
	//		item->setData(node.GetString(KEY_ART), ListServiceDelegate::LIST_SERVICE_ART);
			item->setData(node.ToCompactString(), ListServiceDelegate::LIST_SERVICE_RAW);

			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);

			emit SigReqInfoBot(node.GetString(KEY_PATH), index);
			emit SigReqArt(node.GetString(KEY_ART), index);
			index++;
		}
	}
	else if (LIST_SERVICE_SETUP == nService)
	{
		foreach (CJsonNode node, m_NodeList)
		{
			LogDebug("type : [%d]", node.GetInt(KEY_TYPE));
	//		LogDebug("icon : [%s]", node.GetString(KEY_ICON).toUtf8().data());
			LogDebug("title : [%s]", node.GetString(KEY_TOP).toUtf8().data());
	//		LogDebug("url : [%s]", node.GetString(VAL_URL).toUtf8().data());
			QStandardItem *item = new QStandardItem;
			item->setData(node.GetString(KEY_ID_UPPER), ListServiceDelegate::LIST_SERVICE_ID);
			item->setData(node.GetInt(KEY_TYPE), ListServiceDelegate::LIST_SERVICE_TYPE);
			item->setData(node.GetString(KEY_NAME), ListServiceDelegate::LIST_SERVICE_TITLE);
//			item->setData(node.GetString(KEY_ICON), ListServiceDelegate::LIST_SERVICE_ICON);
	//		item->setData(node.GetString(KEY_ART), ListServiceDelegate::LIST_SERVICE_ART);
			item->setData(node.ToCompactString(), ListServiceDelegate::LIST_SERVICE_RAW);

			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);

			//emit SigReqArt(node.GetString(KEY_ART), index);
			index++;
		}
	}
	else
	{
		foreach (CJsonNode node, m_NodeList)
		{
	//		LogDebug("type : [%d]", node.GetInt(KEY_TYPE));
	//		LogDebug("icon : [%s]", node.GetString(KEY_ICON).toUtf8().data());
	//		LogDebug("title : [%s]", node.GetString(KEY_TOP).toUtf8().data());
	//		LogDebug("url : [%s]", node.GetString(VAL_URL).toUtf8().data());
			QStandardItem *item = new QStandardItem;
			item->setData(node.GetString(KEY_ID_UPPER), ListServiceDelegate::LIST_SERVICE_ID);
			item->setData(node.GetInt(KEY_TYPE), ListServiceDelegate::LIST_SERVICE_TYPE);
			item->setData(node.GetString(KEY_ICON), ListServiceDelegate::LIST_SERVICE_COVER_ART);
			item->setData(node.GetString(KEY_TOP), ListServiceDelegate::LIST_SERVICE_TITLE);
	//		item->setData(node.GetString(KEY_ART), ListServiceDelegate::LIST_SERVICE_ART);
			item->setData(node.ToCompactString(), ListServiceDelegate::LIST_SERVICE_RAW);

			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);

			emit SigReqArt(node.GetString(KEY_ART), index);
			index++;
		}
	}

	ui->gridLayout->addWidget(m_ListView);
	m_pLoading->Stop();

}

QListView::ViewMode ListService::GetViewMode()
{
	return m_ListView->viewMode();
}

void ListService::SetViewMode(QListView::ViewMode mode)
{
	m_ListView->setViewMode(mode);
	m_Delegate->SetViewMode(mode);
}

void ListService::ClearSelectMap()
{
	m_pLoading->Start();
	int count = m_Model->rowCount();

	for (int i = 0; i < count; i++)
	{
		QModelIndex index = m_Model->index(i, 0);
		QStandardItem *item = m_Model->itemFromIndex(index);
		bool bSelect = qvariant_cast<bool>(item->data(ListServiceDelegate::LIST_SERVICE_SELECT));
		if (bSelect)
		{
			item->setData(false, ListServiceDelegate::LIST_SERVICE_SELECT);

//			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(index);

			int id = qvariant_cast<int>(item->data(ListServiceDelegate::LIST_SERVICE_ID));
			m_SelectMap.remove(id);
		}
	}
	m_pLoading->Stop();
}

void ListService::SetAllSelectMap()
{
	m_pLoading->Start();
	int count = m_Model->rowCount();

	for (int i = 0; i < count; i++)
	{
		QModelIndex index = m_Model->index(i, 0);
		QStandardItem *item = m_Model->itemFromIndex(index);
		bool bSelect = qvariant_cast<bool>(item->data(ListServiceDelegate::LIST_SERVICE_SELECT));
		if (!bSelect)
		{
			item->setData(true, ListServiceDelegate::LIST_SERVICE_SELECT);

//			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(index);

			int id = qvariant_cast<int>(item->data(ListServiceDelegate::LIST_SERVICE_ID));
			m_SelectMap.insert(id, true);
		}
	}
	m_pLoading->Stop();
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

void ListService::SlotDoubleClickItem(const QModelIndex &index)
{
	QStandardItem *item = m_Model->itemFromIndex(index);
	bool bSelect = !qvariant_cast<bool>(item->data(ListServiceDelegate::LIST_SERVICE_SELECT));
	item->setData(bSelect, ListServiceDelegate::LIST_SERVICE_SELECT);

//	QModelIndex modelIndex = m_Model->indexFromItem(item);
	m_ListView->openPersistentEditor(index);

	int id = qvariant_cast<int>(item->data(ListServiceDelegate::LIST_SERVICE_ID));
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
	SetViewMode(QListView::ListMode);

	connect(m_ListView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(SlotDoubleClickItem(const QModelIndex&)));
}

