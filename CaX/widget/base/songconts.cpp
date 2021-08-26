#include <QThread>

#include "songconts.h"
#include "ui_songconts.h"

#include "songcontsdelegate.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

SongConts::SongConts(QWidget *parent)
	: QWidget(parent)
	, m_ListView(new QListView)
	, m_Model(new QStandardItemModel)
	, m_Delegate(new SongContsDelegate)
	, ui(new Ui::SongConts)
{
	ui->setupUi(this);

	Initialize();
}

SongConts::~SongConts()
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

void SongConts::SetContentList(QList<CJsonNode> nodeList)
{
	m_NodeList = nodeList;
	int index = 0;
	foreach (CJsonNode node, m_NodeList)
	{
		QStandardItem *item = new QStandardItem;
		int nID = node.GetString(KEY_ID).toInt();
		item->setData(nID, SongContsDelegate::SONG_ROLE_ID);
		item->setData(node.GetString(KEY_TITLE), SongContsDelegate::SONG_ROLE_TITLE);
		item->setData(node.GetString(KEY_FAVORITE), SongContsDelegate::SONG_ROLE_FAVORITE);
		item->setData(node.GetString(KEY_TIME), SongContsDelegate::SONG_ROLE_TIME);
		item->setData(node.GetString(KEY_ARTIST), SongContsDelegate::SONG_ROLE_ARTIST);
		item->setData(node.GetString(KEY_ALBUM), SongContsDelegate::SONG_ROLE_ALBUM);
		item->setData(node.GetString(KEY_GENRE), SongContsDelegate::SONG_ROLE_GENRE);
		item->setData(index, SongContsDelegate::SONG_ROLE_INDEX);
		m_Model->appendRow(item);
		QModelIndex modelIndex = m_Model->indexFromItem(item);
		m_ListView->openPersistentEditor(modelIndex);

		emit SigReqCoverArt(nID, index);
		index++;
	}
	ui->gridLayout->addWidget(m_ListView);
}

void SongConts::ClearContentList()
{
	m_Model->clear();
	ui->gridLayout->removeWidget(m_ListView);
}

QListView::ViewMode SongConts::GetViewMode()
{
	return m_ListView->viewMode();
}

void SongConts::SetViewMode(QListView::ViewMode mode)
{
	m_ListView->setViewMode(mode);
	m_Delegate->SetViewMode(mode);

}

QStandardItemModel *SongConts::GetModel()
{
	return m_Model;
}

void SongConts::SetBackgroundTask(QThread *thread)
{
	connect(thread, SIGNAL(started()), this, SLOT(SlotReqCoverArt()));
}

void SongConts::SlotReqCoverArt()
{
	int index = 0;
	foreach (CJsonNode node, m_NodeList)
	{
		int nID = node.GetString(KEY_ID).toInt();
		QThread::msleep(5);
		emit SigReqCoverArt(nID, index);
		index++;
	}
}

void SongConts::SlotSelectItem(QModelIndex index)
{
	int nID = qvariant_cast<int>(index.data(SongContsDelegate::SONG_ROLE_ID));
	QString title = qvariant_cast<QString>(index.data(SongContsDelegate::SONG_ROLE_TITLE));

	LogDebug("nID [%d] title [%s]", nID, title.toUtf8().data());

	emit SigSelectItem(nID);
}

void SongConts::SlotSelectItem(int nID)
{
	emit SigSelectItem(nID);
}

void SongConts::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	SetViewMode(QListView::ListMode);

//	connect(m_ListView, SIGNAL(clicked(QModelIndex)), this, SLOT(SlotSelectItem(QModelIndex)));
	connect(m_Delegate, SIGNAL(SigSelectPlay(int)), this, SLOT(SlotSelectItem(int)));
	connect(m_Delegate, SIGNAL(SigSelectFavorite(int)), this, SLOT(SlotSelectItem(int)));
	connect(m_Delegate, SIGNAL(SigSelectMore(int)), this, SLOT(SlotSelectItem(int)));

}
