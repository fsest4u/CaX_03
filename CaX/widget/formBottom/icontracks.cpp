#include <QThread>
#include <QScrollBar>

#include "icontracks.h"
#include "ui_icontracks.h"

#include "icontracksdelegate.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"


IconTracks::IconTracks(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_Model(new QStandardItemModel),
	m_Delegate(new IconTracksDelegate),
	m_ScrollBar(nullptr),
	m_pLoading(new Loading(this)),
	ui(new Ui::IconTracks)
{
	ui->setupUi(this);

	Initialize();
}

IconTracks::~IconTracks()
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

QList<CJsonNode> IconTracks::GetNodeList() const
{
	return m_NodeList;
}

void IconTracks::SetNodeList(QList<CJsonNode> &list, int type)
{
	m_pLoading->Start();

	int index = m_NodeList.count();
	m_NodeList.append(list);
	LogDebug("list count [%d] index [%d]", m_NodeList.count(), index);

	if (ICON_TRACKS_MUSIC_DB == type)
	{
		foreach (CJsonNode node, list)
		{
			QStandardItem *item = new QStandardItem;
			int nID = node.GetString(KEY_ID_LOWER).toInt();
			item->setData(nID, IconTracksDelegate::ICON_TRACKS_ID);
			item->setData(node.GetString(KEY_TITLE), IconTracksDelegate::ICON_TRACKS_TITLE);
			item->setData(node.GetString(KEY_SUBTITLE), IconTracksDelegate::ICON_TRACKS_SUBTITLE);
			item->setData(node.GetString(KEY_COUNT), IconTracksDelegate::ICON_TRACKS_COUNT);
			item->setData(node.GetString(KEY_FAVORITE), IconTracksDelegate::ICON_TRACKS_FAVORITE);
			item->setData(node.GetString(KEY_RATING), IconTracksDelegate::ICON_TRACKS_RATING);
			item->setData(false, IconTracksDelegate::ICON_TRACKS_SELECT);

			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);

			emit SigReqCoverArt(nID, index, QListView::IconMode);
			index++;
		}
	}
	else if (ICON_TRACKS_AUDIO_CD == type)
	{
		int totalTime = 0;
		foreach (CJsonNode node, list)
		{
			int time = node.GetInt(KEY_TIME_CAP);
			QStandardItem *item = new QStandardItem;
			int nID = node.GetString(KEY_TRACK).toInt();
			item->setData(nID, IconTracksDelegate::ICON_TRACKS_ID);
			item->setData(":/resource/playlist-img160-albumart-h@3x.png", IconTracksDelegate::ICON_TRACKS_COVER);
			item->setData(node.GetString(KEY_TOP), IconTracksDelegate::ICON_TRACKS_TITLE);
			item->setData(time, IconTracksDelegate::ICON_TRACKS_SUBTITLE);
			item->setData("", IconTracksDelegate::ICON_TRACKS_COUNT);
			item->setData(-1, IconTracksDelegate::ICON_TRACKS_FAVORITE);
			item->setData(-1, IconTracksDelegate::ICON_TRACKS_RATING);
			item->setData(false, IconTracksDelegate::ICON_TRACKS_SELECT);

			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);

			index++;

			totalTime += time;
		}
		emit SigCalcTotalTime(totalTime);
	}
	else if (ICON_TRACKS_PLAYLIST == type)
	{
		foreach (CJsonNode node, list)
		{
			QStandardItem *item = new QStandardItem;
			int nID = node.GetString(KEY_ID_LOWER).toInt();
			item->setData(nID, IconTracksDelegate::ICON_TRACKS_ID);
			item->setData(node.GetString(KEY_TITLE), IconTracksDelegate::ICON_TRACKS_TITLE);
			item->setData("", IconTracksDelegate::ICON_TRACKS_SUBTITLE);
			item->setData(node.GetString(KEY_COUNT), IconTracksDelegate::ICON_TRACKS_COUNT);
			item->setData(-1, IconTracksDelegate::ICON_TRACKS_FAVORITE);
			item->setData(-1, IconTracksDelegate::ICON_TRACKS_RATING);
			item->setData(false, IconTracksDelegate::ICON_TRACKS_SELECT);

			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);

			emit SigReqCoverArt(nID, index, QListView::IconMode);
			index++;
		}
	}

	ui->gridLayout->addWidget(m_ListView);

	m_pLoading->Stop();

}

void IconTracks::ClearNodeList()
{
	m_Model->clear();
	ui->gridLayout->removeWidget(m_ListView);
}

QListView::ViewMode IconTracks::GetViewMode()
{
	return m_ListView->viewMode();
}

void IconTracks::SetViewMode(QListView::ViewMode mode)
{
	m_ListView->setViewMode(mode);
	m_Delegate->SetViewMode(mode);

}

QMap<int, bool> IconTracks::GetSelectMap() const
{
	return m_SelectMap;
}

void IconTracks::SetSelectMap(const QMap<int, bool> &SelectMap)
{
	m_SelectMap = SelectMap;
}

QStandardItemModel *IconTracks::GetModel()
{
	return m_Model;
}

IconTracksDelegate *IconTracks::GetDelegate()
{
	return m_Delegate;
}

void IconTracks::SetBackgroundTask(QThread *thread)
{
	connect(thread, SIGNAL(started()), this, SLOT(SlotReqCoverArt()));
}

void IconTracks::SlotReqCoverArt()
{
	int index = 0;
	foreach (CJsonNode node, m_NodeList)
	{
		int nID = node.GetString(KEY_ID_LOWER).toInt();
		QThread::msleep(5);
		emit SigReqCoverArt(nID, index, QListView::IconMode);
		index++;
	}
}

void IconTracks::SlotScrollValueChanged(int value)
{
	int min = m_ScrollBar->minimum();
	int max = m_ScrollBar->maximum();
	LogDebug("value [%d] min [%d] max [%d]", value, min, max);
	if (value >= max)
	{
		emit SigAppendIconList();
	}
}

void IconTracks::SlotDoubleClickItem(const QModelIndex &index)
{
	QStandardItem *item = m_Model->itemFromIndex(index);
	bool bSelect = !qvariant_cast<bool>(item->data(IconTracksDelegate::ICON_TRACKS_SELECT));
	item->setData(bSelect, IconTracksDelegate::ICON_TRACKS_SELECT);

	QModelIndex modelIndex = m_Model->indexFromItem(item);
	m_ListView->openPersistentEditor(modelIndex);

	int row = index.row();
	if (bSelect)
	{
		m_SelectMap.insert(row, bSelect);
	}
	else
	{
		m_SelectMap.remove(row);
	}

//	// for debug
//	QMap<int, bool>::iterator i;
//	for (i = m_SelectMap.begin(); i!= m_SelectMap.end(); i++)
//	{
//		LogDebug("key [%d] value [%d]", i.key(), i.value());
//	}
}

void IconTracks::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_ListView->setGridSize(QSize(ICON_ITEM_WIDTH, ICON_ITEM_HEIGHT));
//	m_ListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	SetViewMode(QListView::IconMode);

	m_ScrollBar = m_ListView->verticalScrollBar();
	connect(m_ScrollBar, SIGNAL(valueChanged(int)), this, SLOT(SlotScrollValueChanged(int)));
	connect(m_ListView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(SlotDoubleClickItem(const QModelIndex&)));
}

