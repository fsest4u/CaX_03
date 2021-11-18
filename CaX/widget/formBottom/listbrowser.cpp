#include "listbrowser.h"
#include "ui_listbrowser.h"

#include "listbrowserdelegate.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"
#include "util/utilnovatron.h"

#include "widget/browser.h"

ListBrowser::ListBrowser(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_Model(new QStandardItemModel),
	m_Delegate(new ListBrowserDelegate),
	m_pLoading(new Loading(this)),
	ui(new Ui::ListBrowser)
{
	ui->setupUi(this);

	Initialize();
}

ListBrowser::~ListBrowser()
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

void ListBrowser::ClearNodeList()
{
	m_Model->clear();
	ui->gridLayout->removeWidget(m_ListView);
}

QList<CJsonNode> ListBrowser::GetNodeList() const
{
	return m_NodeList;
}

int ListBrowser::SetNodeList(const QList<CJsonNode> &NodeList, int nService)
{
	int type = 0;

	m_pLoading->Start();
	m_Model->clear();
	m_NodeList = NodeList;
	int index = 0;

	if (LIST_BROWSER_BROWSER == nService)
	{
		foreach (CJsonNode node, m_NodeList)
		{
			LogDebug("node [%s]", node.ToCompactByteArray().data());
			int nodeType = node.GetInt(KEY_TYPE);
			type = type | nodeType;

			QStandardItem *item = new QStandardItem;
			item->setData(node.GetString(KEY_PATH), ListBrowserDelegate::LIST_BROWSER_ID);
			item->setData(nodeType, ListBrowserDelegate::LIST_BROWSER_TYPE);
			item->setData(UtilNovatron::GetCoverArtIcon(SIDEMENU_BROWSER), ListBrowserDelegate::LIST_BROWSER_COVER);
			item->setData(node.GetString(KEY_PATH), ListBrowserDelegate::LIST_BROWSER_TITLE);
			item->setData(node.GetString(KEY_BOT), ListBrowserDelegate::LIST_BROWSER_SUBTITLE);
			item->setData(node.GetString(KEY_DURATION), ListBrowserDelegate::LIST_BROWSER_DURATION);
			item->setData(node.GetString(KEY_SIZE), ListBrowserDelegate::LIST_BROWSER_FILESIZE);
			item->setData(node.ToCompactString(), ListBrowserDelegate::LIST_BROWSER_RAW);
			item->setData(false, ListBrowserDelegate::LIST_BROWSER_SELECT);

			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);

			if (nodeType & iFolderType_Mask_Play_Select)
			{
				emit SigReqInfoBot(node.GetString(KEY_PATH), index);
				emit SigReqCoverArt(node.GetString(KEY_PATH), index);
			}
			index++;
		}
	}

	ui->gridLayoutList->addWidget(m_ListView);
	m_pLoading->Stop();

	return type;
}

void ListBrowser::ClearSelectMap()
{
	m_pLoading->Start();
	int count = m_Model->rowCount();

	for (int i = 0; i < count; i++)
	{
		QModelIndex index = m_Model->index(i, 0);
		QStandardItem *item = m_Model->itemFromIndex(index);
		bool bSelect = qvariant_cast<bool>(item->data(ListBrowserDelegate::LIST_BROWSER_SELECT));
		if (bSelect)
		{
			item->setData(false, ListBrowserDelegate::LIST_BROWSER_SELECT);

//			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(index);

			QString id = qvariant_cast<QString>(item->data(ListBrowserDelegate::LIST_BROWSER_ID));
			m_SelectMap.remove(id);
		}
	}
	m_pLoading->Stop();

}

void ListBrowser::SetAllSelectMap()
{
	m_pLoading->Start();
	int count = m_Model->rowCount();

	for (int i = 0; i < count; i++)
	{
		QModelIndex index = m_Model->index(i, 0);
		QStandardItem *item = m_Model->itemFromIndex(index);
		bool bSelect = qvariant_cast<bool>(item->data(ListBrowserDelegate::LIST_BROWSER_SELECT));
		if (!bSelect)
		{
			item->setData(true, ListBrowserDelegate::LIST_BROWSER_SELECT);

//			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(index);

			QString id = qvariant_cast<QString>(item->data(ListBrowserDelegate::LIST_BROWSER_ID));
			int type = qvariant_cast<int>(item->data(ListBrowserDelegate::LIST_BROWSER_TYPE));
			m_SelectMap.insert(id, type);
		}
	}
	m_pLoading->Stop();

}

QMap<QString, int> ListBrowser::GetSelectMap() const
{
	return m_SelectMap;
}

void ListBrowser::SetSelectMap(const QMap<QString, int> &SelectMap)
{
	m_SelectMap = SelectMap;
}

QStandardItemModel *ListBrowser::GetModel()
{
	return m_Model;
}

ListBrowserDelegate *ListBrowser::GetDelegate()
{
	return m_Delegate;
}

void ListBrowser::SlotDoubleClickItem(const QModelIndex &index)
{
	QStandardItem *item = m_Model->itemFromIndex(index);
	bool bSelect = !qvariant_cast<bool>(item->data(ListBrowserDelegate::LIST_BROWSER_SELECT));
	item->setData(bSelect, ListBrowserDelegate::LIST_BROWSER_SELECT);

//	QModelIndex modelIndex = m_Model->indexFromItem(item);
	m_ListView->openPersistentEditor(index);

	QString id = qvariant_cast<QString>(item->data(ListBrowserDelegate::LIST_BROWSER_ID));
	int type = qvariant_cast<int>(item->data(ListBrowserDelegate::LIST_BROWSER_TYPE));
	if (bSelect)
	{
		m_SelectMap.insert(id, type);
	}
	else
	{
		m_SelectMap.remove(id);
	}
}

void ListBrowser::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_ListView->setViewMode(QListView::ListMode);

	connect(m_ListView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(SlotDoubleClickItem(const QModelIndex&)));

}
