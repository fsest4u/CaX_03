#include <QThread>

#include "listtracks.h"
#include "ui_listtracks.h"

#include "listtracksdelegate.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

ListTracks::ListTracks(QWidget *parent)
	: QWidget(parent)
	, m_ListView(new QListView)
	, m_Model(new QStandardItemModel)
	, m_Delegate(new ListTracksDelegate)
	, ui(new Ui::ListTracks)
{
	ui->setupUi(this);

	Initialize();
}

ListTracks::~ListTracks()
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

void ListTracks::SetContentList(QList<CJsonNode> nodeList)
{
	m_NodeList = nodeList;
	int index = 0;
	foreach (CJsonNode node, m_NodeList)
	{
		QStandardItem *item = new QStandardItem;
		int nID = node.GetString(KEY_ID_LOWER).toInt();
		item->setData(nID, ListTracksDelegate::LIST_TRACKS_ID);
		item->setData(node.GetString(KEY_TITLE), ListTracksDelegate::LIST_TRACKS_TITLE);
		item->setData(node.GetString(KEY_TIME), ListTracksDelegate::LIST_TRACKS_TIME);
		item->setData(node.GetString(KEY_ARTIST), ListTracksDelegate::LIST_TRACKS_ARTIST);
		item->setData(node.GetString(KEY_ALBUM), ListTracksDelegate::LIST_TRACKS_ALBUM);
		item->setData(node.GetString(KEY_GENRE), ListTracksDelegate::LIST_TRACKS_GENRE);
		item->setData(index, ListTracksDelegate::LIST_TRACKS_INDEX);
		m_Model->appendRow(item);
		QModelIndex modelIndex = m_Model->indexFromItem(item);
		m_ListView->openPersistentEditor(modelIndex);

		emit SigReqCoverArt(nID, index);
		index++;
	}
	ui->gridLayout->addWidget(m_ListView);
}

void ListTracks::ClearContentList()
{
	m_Model->clear();
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
		emit SigReqCoverArt(nID, index);
		index++;
	}
}

void ListTracks::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	SetViewMode(QListView::ListMode);
}
