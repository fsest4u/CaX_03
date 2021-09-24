#include "iconservice.h"
#include "ui_iconservice.h"

#include "iconservicedelegate.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

IconService::IconService(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_Model(new QStandardItemModel),
	m_Delegate(new IconServiceDelegate),
	ui(new Ui::IconService)
{
	ui->setupUi(this);

	Initialize();
}

IconService::~IconService()
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

void IconService::ClearNodeList()
{
	m_Model->clear();
	ui->gridLayout->removeWidget(m_ListView);
}

QList<CJsonNode> IconService::GetNodeList() const
{
	return m_NodeList;
}

void IconService::SetNodeList(const QList<CJsonNode> &NodeList, int nService)
{
	m_NodeList = NodeList;
	if (ICON_SERVICE_BROWSER == nService)
	{
		foreach (CJsonNode node, m_NodeList)
		{
			QStandardItem *item = new QStandardItem;
			item->setData(node.GetString(KEY_PATH), IconServiceDelegate::ICON_SERVICE_ID);
			item->setData(node.GetInt(KEY_TYPE), IconServiceDelegate::ICON_SERVICE_TYPE);
			item->setData(node.GetString(KEY_COVER_ART), IconServiceDelegate::ICON_SERVICE_COVER);
			item->setData(node.GetString(KEY_PATH), IconServiceDelegate::ICON_SERVICE_TITLE);
			item->setData(node.ToCompactString(), IconServiceDelegate::ICON_SERVICE_RAW);

			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);
		}
	}
	else if (ICON_SERVICE_ISERVICE == nService)
	{
		foreach (CJsonNode node, m_NodeList)
		{
			QStandardItem *item = new QStandardItem;
			item->setData(node.GetInt(KEY_ID_UPPER), IconServiceDelegate::ICON_SERVICE_ID);
			item->setData(node.GetInt(KEY_TYPE), IconServiceDelegate::ICON_SERVICE_TYPE);
			item->setData(node.GetString(KEY_COVER_ART), IconServiceDelegate::ICON_SERVICE_COVER);
			item->setData(node.GetString(KEY_NAME), IconServiceDelegate::ICON_SERVICE_TITLE);
			item->setData(node.ToCompactString(), IconServiceDelegate::ICON_SERVICE_RAW);

			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);
		}
	}
	else if (ICON_SERVICE_INPUT == nService)
	{
		foreach (CJsonNode node, m_NodeList)
		{
			QStandardItem *item = new QStandardItem;
			item->setData(node.GetInt(KEY_ID_UPPER), IconServiceDelegate::ICON_SERVICE_ID);
			item->setData(node.GetInt(KEY_TYPE), IconServiceDelegate::ICON_SERVICE_TYPE);
			item->setData(node.GetString(KEY_COVER_ART), IconServiceDelegate::ICON_SERVICE_COVER);
			item->setData(node.GetString(KEY_RIGHT), IconServiceDelegate::ICON_SERVICE_TITLE);
			item->setData(node.ToCompactString(), IconServiceDelegate::ICON_SERVICE_RAW);

			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);
		}
	}
	else if (ICON_SERVICE_FM_RADIO == nService
			 || ICON_SERVICE_DAB_RADIO == nService)
	{
		LogDebug("todo-dylee dab radio");

		foreach (CJsonNode node, m_NodeList)
		{
			QStandardItem *item = new QStandardItem;
			item->setData(node.GetInt(KEY_ID_UPPER), IconServiceDelegate::ICON_SERVICE_ID);
			item->setData(node.GetInt(KEY_TYPE), IconServiceDelegate::ICON_SERVICE_TYPE);
			item->setData(node.GetString(KEY_COVER_ART), IconServiceDelegate::ICON_SERVICE_COVER);
			item->setData(node.GetString(KEY_RIGHT), IconServiceDelegate::ICON_SERVICE_TITLE);
			item->setData(node.ToCompactString(), IconServiceDelegate::ICON_SERVICE_RAW);

			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);
		}
	}

	ui->gridLayout->addWidget(m_ListView);
}

QListView::ViewMode IconService::GetViewMode()
{
	return m_ListView->viewMode();
}

void IconService::SetViewMode(QListView::ViewMode mode)
{
	m_ListView->setViewMode(mode);
	m_Delegate->SetViewMode(mode);

}

QStandardItemModel *IconService::GetModel()
{
	return m_Model;
}

IconServiceDelegate *IconService::GetDelegate()
{
	return m_Delegate;
}

void IconService::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	SetViewMode(QListView::IconMode);
}
