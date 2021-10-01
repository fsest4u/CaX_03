#include <QThread>

#include "icontracks.h"
#include "ui_icontracks.h"

#include "icontracksdelegate.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"


IconTracks::IconTracks(QWidget *parent)
	: QWidget(parent)
	, m_ListView(new QListView)
	, m_Model(new QStandardItemModel)
	, m_Delegate(new IconTracksDelegate)
	, ui(new Ui::IconTracks)
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
}

QList<CJsonNode> IconTracks::GetNodeList() const
{
	return m_NodeList;
}

void IconTracks::SetNodeList(QList<CJsonNode> &list, int type)
{
	m_NodeList = list;
	int index = 0;

	if (ICON_TRACKS_MUSIC_DB == type)
	{
		foreach (CJsonNode node, m_NodeList)
		{
			QStandardItem *item = new QStandardItem;
			int nID = node.GetString(KEY_ID_LOWER).toInt();
			item->setData(nID, IconTracksDelegate::ICON_TRACKS_ID);
			item->setData(node.GetString(KEY_TITLE), IconTracksDelegate::ICON_TRACKS_TOP);
			item->setData(node.GetString(KEY_SUBTITLE), IconTracksDelegate::ICON_TRACKS_BOTTOM);
			item->setData(node.GetString(KEY_COUNT), IconTracksDelegate::ICON_TRACKS_COUNT);
			item->setData(node.GetString(KEY_FAVORITE), IconTracksDelegate::ICON_TRACKS_FAVORITE);
			item->setData(node.GetString(KEY_RATING), IconTracksDelegate::ICON_TRACKS_RATING);
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
		foreach (CJsonNode node, m_NodeList)
		{
			int time = node.GetInt(KEY_TIME_CAP);
			QStandardItem *item = new QStandardItem;
			int nID = node.GetString(KEY_TRACK).toInt();
			item->setData(nID, IconTracksDelegate::ICON_TRACKS_ID);
			item->setData(":/resource/Icon-playbar-volume-160.png", IconTracksDelegate::ICON_TRACKS_COVER);
			item->setData(node.GetString(KEY_TOP), IconTracksDelegate::ICON_TRACKS_TOP);
			item->setData(time, IconTracksDelegate::ICON_TRACKS_BOTTOM);
			item->setData("", IconTracksDelegate::ICON_TRACKS_COUNT);
			item->setData(-1, IconTracksDelegate::ICON_TRACKS_FAVORITE);
			item->setData(-1, IconTracksDelegate::ICON_TRACKS_RATING);
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
		foreach (CJsonNode node, m_NodeList)
		{
			QStandardItem *item = new QStandardItem;
			int nID = node.GetString(KEY_ID_LOWER).toInt();
			item->setData(nID, IconTracksDelegate::ICON_TRACKS_ID);
			item->setData(node.GetString(KEY_TITLE), IconTracksDelegate::ICON_TRACKS_TOP);
			item->setData("", IconTracksDelegate::ICON_TRACKS_BOTTOM);
			item->setData(node.GetString(KEY_COUNT), IconTracksDelegate::ICON_TRACKS_COUNT);
			item->setData(-1, IconTracksDelegate::ICON_TRACKS_FAVORITE);
			item->setData(-1, IconTracksDelegate::ICON_TRACKS_RATING);
			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);

			emit SigReqCoverArt(nID, index, QListView::IconMode);
			index++;
		}
	}

	ui->gridLayout->addWidget(m_ListView);
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

void IconTracks::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	SetViewMode(QListView::IconMode);

}
