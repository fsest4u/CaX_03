#include <QThread>
#include <QScrollBar>

#include "icontracks.h"
#include "ui_icontracks.h"

#include "icontracksdelegate.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"
#include "util/utilnovatron.h"

IconTracks::IconTracks(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_Model(new QStandardItemModel),
	m_Delegate(new IconTracksDelegate),
	m_ScrollBar(nullptr),
//	m_pLoading(new Loading(this)),
	ui(new Ui::IconTracks)
{
	ui->setupUi(this);

	Initialize();
}

IconTracks::~IconTracks()
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

//	if (m_pLoading)
//	{
//		delete m_pLoading;
//		m_pLoading = nullptr;
//	}

	delete ui;

}

QList<CJsonNode> IconTracks::GetNodeList() const
{
	return m_NodeList;
}

void IconTracks::SetNodeList(QList<CJsonNode> &list, int service)
{
//	m_pLoading->Start();
	int index = m_NodeList.count();
	m_NodeList.append(list);

	if (SIDEMENU_AUDIO_CD == service)
	{
//		int totalTime = 0;
		foreach (CJsonNode node, list)
		{
			LogDebug("node [%s]", node.ToCompactByteArray().data());
//			int seconds = node.GetInt(KEY_TIME_CAP);
//			QString hhmmss = UtilNovatron::CalcSecondToHMS(seconds);

			QStandardItem *item = new QStandardItem;
			item->setData(node.GetString(KEY_TRACK), IconTracksDelegate::ICON_TRACKS_ID);
			item->setData(UtilNovatron::GetCoverArtIcon(SIDEMENU_AUDIO_CD), IconTracksDelegate::ICON_TRACKS_COVER);
			item->setData(node.GetString(KEY_TOP), IconTracksDelegate::ICON_TRACKS_TITLE);
			item->setData(node.GetString(KEY_SUBTITLE), IconTracksDelegate::ICON_TRACKS_SUBTITLE);
			item->setData(node.GetString(KEY_COUNT), IconTracksDelegate::ICON_TRACKS_COUNT);
			item->setData(false, IconTracksDelegate::ICON_TRACKS_SELECT);
			item->setData(index, IconTracksDelegate::ICON_TRACKS_INDEX);
			item->setData(-1, IconTracksDelegate::ICON_TRACKS_FAVORITE);
			item->setData(-1, IconTracksDelegate::ICON_TRACKS_RATING);

			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);

//			totalTime += seconds;
			index++;
		}
//		emit SigCalcTotalTime(totalTime);
	}
	else
	{
		foreach (CJsonNode node, list)
		{
//			LogDebug("node [%s]", node.ToCompactByteArray().data());

			QStandardItem *item = new QStandardItem;
			int nID = node.GetString(KEY_ID_LOWER).toInt();
			item->setData(nID, IconTracksDelegate::ICON_TRACKS_ID);
			item->setData(node.GetString(KEY_TITLE), IconTracksDelegate::ICON_TRACKS_TITLE);
			item->setData(node.GetString(KEY_SUBTITLE), IconTracksDelegate::ICON_TRACKS_SUBTITLE);
			item->setData(node.GetString(KEY_COUNT), IconTracksDelegate::ICON_TRACKS_COUNT);
			item->setData(false, IconTracksDelegate::ICON_TRACKS_SELECT);
			item->setData(index, IconTracksDelegate::ICON_TRACKS_INDEX);
			if (SIDEMENU_MUSIC_DB == service)
			{
				item->setData(node.GetString(KEY_FAVORITE), IconTracksDelegate::ICON_TRACKS_FAVORITE);
				item->setData(node.GetString(KEY_RATING), IconTracksDelegate::ICON_TRACKS_RATING);
			}
			else if (ICON_TRACKS_MUSIC_DB_TRACK == service)
			{
				item->setData(node.GetString(KEY_FAVORITE), IconTracksDelegate::ICON_TRACKS_FAVORITE);
				item->setData(-1, IconTracksDelegate::ICON_TRACKS_RATING);
			}
			else
			{
				item->setData(-1, IconTracksDelegate::ICON_TRACKS_FAVORITE);
				item->setData(-1, IconTracksDelegate::ICON_TRACKS_RATING);
			}

			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);

//			emit SigReqCoverArt(nID, index, QListView::IconMode);
			index++;
		}
	}

	ui->gridLayout->addWidget(m_ListView);
//	m_pLoading->Stop();
}

void IconTracks::ClearNodeList()
{
	m_Model->clear();
	m_NodeList.clear();
	m_SelectMap.clear();
	ui->gridLayout->removeWidget(m_ListView);
}

void IconTracks::ClearSelectMap()
{
//	m_pLoading->Start();
	int count = m_Model->rowCount();

	for (int i = 0; i < count; i++)
	{
		QModelIndex index = m_Model->index(i, 0);
		QStandardItem *item = m_Model->itemFromIndex(index);
//		bool bSelect = qvariant_cast<bool>(item->data(IconTracksDelegate::ICON_TRACKS_SELECT));
//		if (bSelect)
		{
			item->setData(false, IconTracksDelegate::ICON_TRACKS_SELECT);

//			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(index);

			int id = qvariant_cast<int>(item->data(IconTracksDelegate::ICON_TRACKS_ID));
			m_SelectMap.remove(id);
		}
	}
//	m_pLoading->Stop();
}

void IconTracks::SetAllSelectMap()
{
//	m_pLoading->Start();
	int count = m_Model->rowCount();

	for (int i = 0; i < count; i++)
	{
		QModelIndex index = m_Model->index(i, 0);
		QStandardItem *item = m_Model->itemFromIndex(index);
		bool bSelect = qvariant_cast<bool>(item->data(IconTracksDelegate::ICON_TRACKS_SELECT));
		if (!bSelect)
		{
			item->setData(true, IconTracksDelegate::ICON_TRACKS_SELECT);

//			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(index);

			int id = qvariant_cast<int>(item->data(IconTracksDelegate::ICON_TRACKS_ID));
			m_SelectMap.insert(id, true);
		}
	}
//	m_pLoading->Stop();
}

QMap<int, bool> IconTracks::GetSelectMap() const
{
	return m_SelectMap;
}

void IconTracks::SetSelectMap(const QMap<int, bool> &SelectMap)
{
	m_SelectMap = SelectMap;
}

void IconTracks::SetResize(int resize)
{
	m_Delegate->SetResize(resize);
	m_ListView->setGridSize(QSize(resize * 1.25, resize * 1.375));

//	m_pLoading->Start();
	int count = m_Model->rowCount();

	for (int i = 0; i < count; i++)
	{
		QModelIndex index = m_Model->index(i, 0);
		m_ListView->openPersistentEditor(index);
	}
//	m_pLoading->Stop();
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
	connect(thread, SIGNAL(finished()), this, SLOT(SlotFinishThread()));
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

void IconTracks::SlotFinishThread()
{
	LogDebug("thread finish good");
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

void IconTracks::SlotSelectCoverArt(int index)
{
	QStandardItem *item = m_Model->item(index);
	bool bSelect = !qvariant_cast<bool>(item->data(IconTracksDelegate::ICON_TRACKS_SELECT));
	item->setData(bSelect, IconTracksDelegate::ICON_TRACKS_SELECT);

	QModelIndex modelIndex = m_Model->indexFromItem(item);
	m_ListView->openPersistentEditor(modelIndex);

	int id = qvariant_cast<int>(item->data(IconTracksDelegate::ICON_TRACKS_ID));
	if (bSelect)
	{
		m_SelectMap.insert(id, bSelect);
	}
	else
	{
		m_SelectMap.remove(id);
	}
}

void IconTracks::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
//	m_ListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	m_ListView->setViewMode(QListView::IconMode);
	SetResize(ICON_HEIGHT_MAX);

	m_ScrollBar = m_ListView->verticalScrollBar();
	connect(m_ScrollBar, SIGNAL(valueChanged(int)), this, SLOT(SlotScrollValueChanged(int)));
	connect(m_Delegate, SIGNAL(SigSelectCoverArt(int)), this, SLOT(SlotSelectCoverArt(int)));
}
