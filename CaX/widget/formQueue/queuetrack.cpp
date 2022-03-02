#include "queuetrack.h"
#include "ui_queuetrack.h"

#include "queuetrackdelegate.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/utilnovatron.h"

QueueTrack::QueueTrack(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_Model(new QStandardItemModel),
	m_Delegate(new QueueTrackDelegate),
	ui(new Ui::QueueTrack)
{
	ui->setupUi(this);

	Initialize();
}

QueueTrack::~QueueTrack()
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

QList<CJsonNode> QueueTrack::GetNodeList() const
{
	return m_NodeList;
}

int QueueTrack::SetNodeList(QList<CJsonNode> list)
{
	Loading *loading = UtilNovatron::LoadingStart(parentWidget()->parentWidget());

	m_NodeList = list;

	int index = 0;
	int totalTime = 0;

	foreach (CJsonNode node, list)
	{
//		LogDebug("node [%s]", node.ToCompactByteArray().data());
		int seconds = node.GetInt(KEY_DURATION);
		QString hhmmss = UtilNovatron::CalcSecondToHMS(seconds);

		QStandardItem *item = new QStandardItem;
		int nID = node.GetString(KEY_SONG).toInt();
		item->setData(nID, QueueTrackDelegate::QUEUE_TRACKS_ID);
		item->setData(node.GetString(KEY_TOP), QueueTrackDelegate::QUEUE_TRACKS_TITLE);
		item->setData(hhmmss, QueueTrackDelegate::QUEUE_TRACKS_TIME);
		item->setData(node.GetString(KEY_BOT), QueueTrackDelegate::QUEUE_TRACKS_ARTIST);
		item->setData(index, QueueTrackDelegate::QUEUE_TRACKS_INDEX);

		m_Model->appendRow(item);

		totalTime += seconds;
		index++;
	}

	UtilNovatron::LoadingStop(loading);

	return totalTime;
}

void QueueTrack::ClearNodeList()
{
	m_Model->clear();
	m_NodeList.clear();
}

void QueueTrack::SetCurrentIndex(int index)
{
	QModelIndex modelIndex = m_Model->index(index, 0);
	m_ListView->setCurrentIndex(modelIndex);
}

QStandardItemModel *QueueTrack::GetModel()
{
	return m_Model;
}

QueueTrackDelegate *QueueTrack::GetDelegate()
{
	return m_Delegate;
}

void QueueTrack::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_ListView->setViewMode(QListView::ListMode);
	m_ListView->setGridSize(QSize(ICON_ITEM_WIDTH, LIST_HEIGHT_MIN));
	m_ListView->setEditTriggers(QAbstractItemView::EditTrigger::AllEditTriggers);
	m_ListView->setMouseTracking(true);

	ui->gridLayout->addWidget(m_ListView);

}
