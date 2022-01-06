#include <QThread>

#include "searchtrack.h"
#include "ui_searchtrack.h"

#include "widget/formSearch/searchtrackdelegate.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"
#include "util/utilnovatron.h"

SearchTrack::SearchTrack(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_Model(new QStandardItemModel),
	m_Delegate(new SearchTrackDelegate),
	ui(new Ui::SearchTrack)
{
	ui->setupUi(this);

	Initialize();
}

SearchTrack::~SearchTrack()
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

void SearchTrack::ClearNodeList()
{
	m_Model->clear();
	ui->gridLayoutContent->removeWidget(m_ListView);
}

QList<CJsonNode> SearchTrack::GetNodeList() const
{
	return m_NodeList;
}

void SearchTrack::SetNodeList(const QList<CJsonNode> &NodeList, int category)
{
	m_Model->clear();
	m_NodeList = NodeList;

	int index  = 0;

	foreach (CJsonNode node, m_NodeList)
	{
//		LogDebug("node [%s]", node.ToCompactByteArray().data());
		int seconds = node.GetInt(KEY_TIME);
		QString hhmmss = UtilNovatron::CalcSecondToHMS(seconds);

		QStandardItem *item = new QStandardItem;
		int nID = node.GetString(KEY_ID_LOWER).toInt();
		item->setData(nID, SearchTrackDelegate::SEARCH_TRACKS_ID);
		item->setData(node.GetString(KEY_TITLE), SearchTrackDelegate::SEARCH_TRACKS_TITLE);
		item->setData(hhmmss, SearchTrackDelegate::SEARCH_TRACKS_TIME);
		item->setData(node.GetString(KEY_ARTIST), SearchTrackDelegate::SEARCH_TRACKS_ARTIST);
		item->setData(node.GetInt(KEY_ALBUM_ID), SearchTrackDelegate::SEARCH_TRACKS_ALBUM_ID);
		item->setData(node.GetString(KEY_ALBUM), SearchTrackDelegate::SEARCH_TRACKS_ALBUM);
		item->setData(node.GetString(KEY_GENRE), SearchTrackDelegate::SEARCH_TRACKS_GENRE);
//		item->setData(false, ListTracksDelegate::LIST_TRACKS_SELECT);

		m_Model->appendRow(item);
		QModelIndex modelIndex = m_Model->indexFromItem(item);
		m_ListView->openPersistentEditor(modelIndex);

		emit SigReqCoverArt(nID, index, category);
		index++;
	}

	ui->gridLayoutContent->addWidget(m_ListView);

}

void SearchTrack::SetTitle(QString title)
{
	ui->labelTitle->setText(title);
}

QStandardItemModel *SearchTrack::GetModel()
{
	return m_Model;
}

SearchTrackDelegate *SearchTrack::GetDelegate()
{
	return m_Delegate;
}

void SearchTrack::SetBackgroundTask(QThread *thread)
{
	connect(thread, SIGNAL(started()), this, SLOT(SlotReqCoverArt()));
	connect(thread, SIGNAL(finished()), this, SLOT(SlotFinishThread()));
}

void SearchTrack::SlotReqCoverArt()
{
//	int index = 0;
//	foreach (CJsonNode node, m_NodeList)
//	{
//		int nID = node.GetString(KEY_ID_LOWER).toInt();
//		QThread::msleep(5);
//		emit SigReqCoverArt(nID, index, QListView::ListMode);
//		index++;
//	}
}

void SearchTrack::SlotFinishThread()
{
//	LogDebug("thread finish good");
}

void SearchTrack::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_ListView->setViewMode(QListView::ListMode);
}
