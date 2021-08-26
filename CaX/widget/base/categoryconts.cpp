#include <QThread>

#include "categoryconts.h"
#include "ui_categoryconts.h"

#include "categorycontsdelegate.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"


CategoryConts::CategoryConts(QWidget *parent)
	: QWidget(parent)
	, m_ListView(new QListView)
	, m_Model(new QStandardItemModel)
	, m_Delegate(new CategoryContsDelegate)
	, ui(new Ui::CategoryConts)
{
	ui->setupUi(this);

	Initialize();
}

CategoryConts::~CategoryConts()
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


void CategoryConts::SetContentList(QList<CJsonNode> nodeList)
{
	m_NodeList = nodeList;
	int index = 0;
	foreach (CJsonNode node, m_NodeList)
	{
		QStandardItem *item = new QStandardItem;
		int nID = node.GetString(KEY_ID).toInt();
		item->setData(nID, CategoryContsDelegate::CATEGORY_ROLE_ID);
		item->setData(node.GetString(KEY_TITLE), CategoryContsDelegate::CATEGORY_ROLE_TITLE);
		item->setData(node.GetString(KEY_SUBTITLE), CategoryContsDelegate::CATEGORY_ROLE_SUBTITLE);
		item->setData(node.GetString(KEY_COUNT), CategoryContsDelegate::CATEGORY_ROLE_COUNT);
		item->setData(index, CategoryContsDelegate::CATEGORY_ROLE_INDEX);
		m_Model->appendRow(item);
		QModelIndex modelIndex = m_Model->indexFromItem(item);
		m_ListView->openPersistentEditor(modelIndex);

		emit SigReqCoverArt(nID, index);
		index++;
	}

	ui->gridLayout->addWidget(m_ListView);
}

void CategoryConts::ClearContentList()
{
	m_Model->clear();
	ui->gridLayout->removeWidget(m_ListView);
}

QListView::ViewMode CategoryConts::GetViewMode()
{
	return m_ListView->viewMode();
}

void CategoryConts::SetViewMode(QListView::ViewMode mode)
{
	m_ListView->setViewMode(mode);
	m_Delegate->SetViewMode(mode);

}

QStandardItemModel *CategoryConts::GetModel()
{
	return m_Model;
}

void CategoryConts::SetBackgroundTask(QThread *thread)
{
	connect(thread, SIGNAL(started()), this, SLOT(SlotReqCoverArt()));
}

void CategoryConts::SlotReqCoverArt()
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

void CategoryConts::SlotSelectItem(QModelIndex index)
{
	int nID = qvariant_cast<int>(index.data(CategoryContsDelegate::CATEGORY_ROLE_ID));
	QString title = qvariant_cast<QString>(index.data(CategoryContsDelegate::CATEGORY_ROLE_TITLE));

	LogDebug("nID [%d] title [%s]", nID, title.toUtf8().data());

	emit SigSelectItem(nID);
}

void CategoryConts::SlotSelectItem(int nID)
{
	emit SigSelectItem(nID);
}

void CategoryConts::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	SetViewMode(QListView::IconMode);

//	connect(m_ListView, SIGNAL(clicked(QModelIndex)), this, SLOT(SlotSelectItem(QModelIndex)));
	connect(m_Delegate, SIGNAL(SigSelectCoverArt(int)), this, SLOT(SlotSelectItem(int)));
}
