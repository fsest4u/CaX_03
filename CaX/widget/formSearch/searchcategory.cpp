#include "searchcategory.h"
#include "ui_searchcategory.h"

#include "searchcategorydelegate.h"

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

		emit SigReqCoverArt(id, index, category);
		index++;
	}

}

void SearchCategory::ClearNodeList()
{
	m_Model->clear();
	m_NodeList.clear();
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

void SearchCategory::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_ListView->setViewMode(QListView::IconMode);
	m_ListView->setGridSize(QSize(SEARCH_ITEM_WIDTH, SEARCH_ITEM_HEIGHT));
	m_ListView->setEditTriggers(QAbstractItemView::EditTrigger::AllEditTriggers);
	m_ListView->setMouseTracking(true);

	ui->gridLayout->addWidget(m_ListView);

}

