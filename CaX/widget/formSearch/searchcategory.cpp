#include <QThread>

#include "searchcategory.h"
#include "ui_searchcategory.h"

#include "searchcategorydelegate.h"

#include "manager/sqlmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"

SearchCategory::SearchCategory(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_Model(new QStandardItemModel),
	m_Delegate(new SearchCategoryDelegate),
	ui(new Ui::SearchCategory)
{
	ui->setupUi(this);

	Initialize();
}

SearchCategory::~SearchCategory()
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

void SearchCategory::ClearNodeList()
{
	m_Model->clear();
	ui->gridLayoutContent->removeWidget(m_ListView);
}

QList<CJsonNode> SearchCategory::GetNodeList() const
{
	return m_NodeList;
}

void SearchCategory::SetNodeList(const QList<CJsonNode> &NodeList, int category)
{
	m_Model->clear();
	m_NodeList = NodeList;

	int index = 0;

	foreach (CJsonNode node, m_NodeList)
	{
		LogDebug("node [%s]", node.ToCompactByteArray().data());

		int id = node.GetInt(KEY_ID_LOWER);

		QStandardItem *item = new QStandardItem;
		item->setData(id, SearchCategoryDelegate::SEARCH_CATEGORY_ID);
		item->setData(node.GetString(KEY_TITLE), SearchCategoryDelegate::SEARCH_CATEGORY_TITLE);
		item->setData(node.GetString(KEY_SUBTITLE), SearchCategoryDelegate::SEARCH_CATEGORY_SUBTITLE);

		m_Model->appendRow(item);
		QModelIndex modelIndex = m_Model->indexFromItem(item);
		m_ListView->openPersistentEditor(modelIndex);

		emit SigReqCoverArt(id, index, category);
		index++;
	}

	ui->gridLayoutContent->addWidget(m_ListView);
}

void SearchCategory::SetTitle(QString title)
{
	ui->labelTitle->setText(title);
}

QStandardItemModel *SearchCategory::GetModel()
{
	return m_Model;
}

QStyledItemDelegate *SearchCategory::GetDelegate()
{
	return m_Delegate;
}

void SearchCategory::SetBackgroundTask(QThread *thread)
{
	connect(thread, SIGNAL(started()), this, SLOT(SlotReqCoverArt()));
	connect(thread, SIGNAL(finished()), this, SLOT(SlotFinishThread()));
}

void SearchCategory::SlotReqCoverArt()
{
//	int index = 0;
//	foreach (CJsonNode node, m_NodeList)
//	{
//		QThread::msleep(5);
//		int id = node.GetInt(KEY_ID_LOWER);
//		emit SigReqCoverArt(id, index);
//		index++;
//	}
}

void SearchCategory::SlotFinishThread()
{
//	LogDebug("thread finish good");
}

void SearchCategory::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_ListView->setViewMode(QListView::IconMode);
}

