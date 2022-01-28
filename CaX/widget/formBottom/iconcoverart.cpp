#include <QScrollBar>

#include "iconcoverart.h"
#include "ui_iconcoverart.h"

#include "iconcoverartdelegate.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"

IconCoverArt::IconCoverArt(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_Model(new QStandardItemModel),
	m_Delegate(new IconCoverArtDelegate),
	m_ScrollBar(nullptr),
//	m_pLoading(new Loading(this)),
	ui(new Ui::IconCoverArt)
{
	ui->setupUi(this);

	Initialize();
}

IconCoverArt::~IconCoverArt()
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

QList<CJsonNode> IconCoverArt::GetNodeList() const
{
	return m_NodeList;
}

void IconCoverArt::SetNodeList(QList<CJsonNode> &list, int type)
{
//	m_pLoading->Start();
	int index = m_NodeList.count();
	m_NodeList.append(list);

	foreach (CJsonNode node, list)
	{
		QStandardItem *item = new QStandardItem;
		item->setData(index, IconCoverArtDelegate::ICON_COVER_ART_ID);
//		item->setData(node.GetString(KEY_IMAGE), IconCoverArtDelegate::ICON_COVER_ART_COVER);

		m_Model->appendRow(item);

//		LogDebug("url [%s]", node.GetString(KEY_IMAGE).toUtf8().data());
		emit SigSearchCoverArt(index, node.GetString(KEY_IMAGE));
		index++;
	}

//	m_pLoading->Stop();
}

void IconCoverArt::ClearNodeList()
{
	m_Model->clear();
	m_NodeList.clear();
}

QStandardItemModel *IconCoverArt::GetModel()
{
	return m_Model;
}

IconCoverArtDelegate *IconCoverArt::GetDelegate()
{
	return m_Delegate;
}

void IconCoverArt::SlotScrollValueChanged(int value)
{
	int min = m_ScrollBar->minimum();
	int max = m_ScrollBar->maximum();
//	LogDebug("value [%d] min [%d] max [%d]", value, min, max);
	if (value >= max)
	{
		emit SigAppendList();
	}
}

void IconCoverArt::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_ListView->setGridSize(QSize(ICON_ITEM_WIDTH, ICON_ITEM_HEIGHT));
	m_ListView->setViewMode(QListView::IconMode);
	m_ListView->setEditTriggers(QAbstractItemView::EditTrigger::AllEditTriggers);
	m_ListView->setMouseTracking(true);

	m_ScrollBar = m_ListView->verticalScrollBar();
	connect(m_ScrollBar, SIGNAL(valueChanged(int)), this, SLOT(SlotScrollValueChanged(int)));

	ui->gridLayout->addWidget(m_ListView);

}
