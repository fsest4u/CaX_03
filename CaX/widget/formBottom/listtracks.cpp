#include <QThread>
#include <QScrollBar>

#include "listtracks.h"
#include "ui_listtracks.h"

#include "listtracksdelegate.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"

ListTracks::ListTracks(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_Model(new QStandardItemModel),
	m_Delegate(new ListTracksDelegate),
	m_ScrollBar(nullptr),
	m_pLoading(new Loading(this)),
	ui(new Ui::ListTracks)
{
	ui->setupUi(this);

	Initialize();
}

ListTracks::~ListTracks()
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

QList<CJsonNode> ListTracks::GetNodeList() const
{
	return m_NodeList;
}

void ListTracks::SetNodeList(QList<CJsonNode> list, int type)
{
	m_pLoading->Start();
	int index = m_NodeList.count();
	m_NodeList.append(list);
	LogDebug("list count [%d] index [%d]", m_NodeList.count(), index);

	if (LIST_TRACKS_AUDIO_CD == type)
	{
		foreach (CJsonNode node, list)
		{
			LogDebug("node [%s]", node.ToCompactByteArray().data());
			QStandardItem *item = new QStandardItem;
			int nID = node.GetString(KEY_TRACK).toInt();
			item->setData(nID, ListTracksDelegate::LIST_TRACKS_ID);
			item->setData(":/resource/playlist-img160-albumart-h@3x.png", ListTracksDelegate::LIST_TRACKS_COVER);
			item->setData(node.GetString(KEY_TOP), ListTracksDelegate::LIST_TRACKS_TITLE);
			item->setData(node.GetInt(KEY_TIME_CAP), ListTracksDelegate::LIST_TRACKS_TIME);
			item->setData(node.GetString(KEY_ARTIST), ListTracksDelegate::LIST_TRACKS_ARTIST);
			item->setData(node.GetString(KEY_ALBUM), ListTracksDelegate::LIST_TRACKS_ALBUM);
			item->setData(node.GetString(KEY_GENRE), ListTracksDelegate::LIST_TRACKS_GENRE);
			item->setData(false, ListTracksDelegate::LIST_TRACKS_SELECT);

			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);

//			emit SigReqCoverArt(nID, index, QListView::ListMode);
			index++;
		}
	}
	else
	{
		foreach (CJsonNode node, list)
		{
			LogDebug("node [%s]", node.ToCompactByteArray().data());
			QStandardItem *item = new QStandardItem;
			int nID = node.GetString(KEY_ID_LOWER).toInt();
			item->setData(nID, ListTracksDelegate::LIST_TRACKS_ID);
			item->setData(node.GetString(KEY_TITLE), ListTracksDelegate::LIST_TRACKS_TITLE);
			if (node.GetInt(KEY_FAVORITE) >= 0)
			{
				item->setData(node.GetInt(KEY_FAVORITE), ListTracksDelegate::LIST_TRACKS_FAVORITE);
			}
			item->setData(node.GetString(KEY_TIME), ListTracksDelegate::LIST_TRACKS_TIME);
			item->setData(node.GetString(KEY_ARTIST), ListTracksDelegate::LIST_TRACKS_ARTIST);
			item->setData(node.GetString(KEY_ALBUM), ListTracksDelegate::LIST_TRACKS_ALBUM);
			item->setData(node.GetString(KEY_GENRE), ListTracksDelegate::LIST_TRACKS_GENRE);
			item->setData(false, ListTracksDelegate::LIST_TRACKS_SELECT);

			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);

			emit SigReqCoverArt(nID, index, QListView::ListMode);
			index++;
		}
	}

	ui->gridLayout->addWidget(m_ListView);
	m_pLoading->Stop();
}

void ListTracks::ClearNodeList()
{
	m_Model->clear();
	m_NodeList.clear();
	m_SelectMap.clear();
	ui->gridLayout->removeWidget(m_ListView);
}

QListView::ViewMode ListTracks::GetViewMode()
{
	return m_ListView->viewMode();
}

void ListTracks::SetViewMode(QListView::ViewMode mode)
{
	m_ListView->setViewMode(mode);
	m_Delegate->SetViewMode(mode);
}

void ListTracks::ClearSelectMap()
{
	m_pLoading->Start();
	int count = m_Model->rowCount();

	for (int i = 0; i < count; i++)
	{
		QModelIndex index = m_Model->index(i, 0);
		QStandardItem *item = m_Model->itemFromIndex(index);
		bool bSelect = qvariant_cast<bool>(item->data(ListTracksDelegate::LIST_TRACKS_SELECT));
		if (bSelect)
		{
			item->setData(false, ListTracksDelegate::LIST_TRACKS_SELECT);

//			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(index);

			int id = qvariant_cast<int>(item->data(ListTracksDelegate::LIST_TRACKS_ID));
			m_SelectMap.remove(id);
		}
	}

	m_pLoading->Stop();
}

void ListTracks::SetAllSelectMap()
{
	m_pLoading->Start();
	int count = m_Model->rowCount();
	for (int i = 0; i < count; i++)
	{
		QModelIndex index = m_Model->index(i, 0);
		QStandardItem *item = m_Model->itemFromIndex(index);
		bool bSelect = qvariant_cast<bool>(item->data(ListTracksDelegate::LIST_TRACKS_SELECT));
		if (!bSelect)
		{
			item->setData(true, ListTracksDelegate::LIST_TRACKS_SELECT);

//			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(index);

			int id = qvariant_cast<int>(item->data(ListTracksDelegate::LIST_TRACKS_ID));
			m_SelectMap.insert(id, true);
		}
	}
	m_pLoading->Stop();
}

QMap<int, bool> ListTracks::GetSelectMap() const
{
	return m_SelectMap;
}

void ListTracks::SetSelectMap(const QMap<int, bool> &SelectMap)
{
	m_SelectMap = SelectMap;
}

void ListTracks::SetResize(int resize)
{
	m_Delegate->SetResize(resize);
	m_ListView->setGridSize(QSize(resize * 1.25, resize * 1.375));

	m_pLoading->Start();
	int count = m_Model->rowCount();

	for (int i = 0; i < count; i++)
	{
		QModelIndex index = m_Model->index(i, 0);
		m_ListView->openPersistentEditor(index);
	}
	m_pLoading->Stop();
}

QStandardItemModel *ListTracks::GetModel()
{
	return m_Model;
}

ListTracksDelegate *ListTracks::GetDelegate()
{
	return m_Delegate;
}

void ListTracks::SetBackgroundTask(QThread *thread)
{
	connect(thread, SIGNAL(started()), this, SLOT(SlotReqCoverArt()));
}

void ListTracks::SlotReqCoverArt()
{
	int index = 0;
	foreach (CJsonNode node, m_NodeList)
	{
		int nID = node.GetString(KEY_ID_LOWER).toInt();
		QThread::msleep(5);
		emit SigReqCoverArt(nID, index, QListView::ListMode);
		index++;
	}
}

void ListTracks::SlotScrollValueChanged(int value)
{
	int min = m_ScrollBar->minimum();
	int max = m_ScrollBar->maximum();
	LogDebug("value [%d] min [%d] max [%d]", value, min, max);
	if (value >= max)
	{
		emit SigAppendList();
	}
}

void ListTracks::SlotDoubleClickItem(const QModelIndex &index)
{
	QStandardItem *item = m_Model->itemFromIndex(index);
	bool bSelect = !qvariant_cast<bool>(item->data(ListTracksDelegate::LIST_TRACKS_SELECT));
	item->setData(bSelect, ListTracksDelegate::LIST_TRACKS_SELECT);

//	QModelIndex modelIndex = m_Model->indexFromItem(item);
	m_ListView->openPersistentEditor(index);

	int id = qvariant_cast<int>(item->data(ListTracksDelegate::LIST_TRACKS_ID));
	if (bSelect)
	{
		m_SelectMap.insert(id, bSelect);
	}
	else
	{
		m_SelectMap.remove(id);
	}
}

void ListTracks::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	SetViewMode(QListView::ListMode);
	SetResize(LIST_HEIGHT_MIN);

	m_ScrollBar = m_ListView->verticalScrollBar();
	connect(m_ScrollBar, SIGNAL(valueChanged(int)), this, SLOT(SlotScrollValueChanged(int)));
	connect(m_ListView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(SlotDoubleClickItem(const QModelIndex&)));

}
