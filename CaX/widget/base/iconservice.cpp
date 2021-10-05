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

QList<CJsonNode> IconService::GetNodeList() const
{
	return m_NodeList;
}

void IconService::SetNodeList(const QList<CJsonNode> &list, int nService)
{
	m_Model->clear();
	m_NodeList = list;
	m_Delegate->SetServiceType(nService);

	int index = 0;

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
	else if (ICON_SERVICE_GROUP_PLAY == nService)
	{
		foreach (CJsonNode node, m_NodeList)
		{
			LogDebug("node [%s]", node.ToCompactByteArray().data());
			QString subtitle = GetGroupPlayStatus(node.GetInt(KEY_GROUP_PLAY_STATUS))
					+ " / "
					+ GetActiveChannel(node.GetInt(KEY_ACTIVE_CHANNEL))
					+ " / "
					+ node.GetString(KEY_VOLUME);

			QStandardItem *item = new QStandardItem;
			item->setData(node.GetInt(KEY_MAC_ADDR), IconServiceDelegate::ICON_SERVICE_ID);
			item->setData(":/resource/Icon-playbar-volume-160.png", IconServiceDelegate::ICON_SERVICE_COVER);
			item->setData(node.GetString(KEY_CA_NAME), IconServiceDelegate::ICON_SERVICE_TITLE);
			item->setData(subtitle, IconServiceDelegate::ICON_SERVICE_SUBTITLE);
			item->setData(node.ToCompactString(), IconServiceDelegate::ICON_SERVICE_RAW);

			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);

			int coverID = GetCoverID(node.GetString(KEY_ART_URL));
			if (coverID >= 0)
			{
				emit SigReqCoverArt(coverID, index, QListView::IconMode);
			}
			index++;
		}
	}

	ui->gridLayout->addWidget(m_ListView);
}

void IconService::ClearNodeList()
{
	m_Model->clear();
	ui->gridLayout->removeWidget(m_ListView);
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

QString IconService::GetGroupPlayStatus(int type)
{
	QString value;
	switch (type)
	{
	case 0:
		value = "Idle";
		break;
	case 1:
		value = "Client";
		break;
	case 2:
		value = "Server";
		break;
	}

	return value;
}

QString IconService::GetActiveChannel(int type)
{
	QString value;
	switch (type)
	{
	case 0:
		value = "Stereo";
		break;
	case 1:
		value = "Left";
		break;
	case 2:
		value = "Right";
		break;
	}

	return value;
}

int IconService::GetCoverID(QString url)
{
	if (url.isEmpty())
		return -1;

	QStringList lsURL = url.split("/");
	int index = lsURL.count() - 1;
	if (index < 0)
		return -1;

	return lsURL.at(index).toInt();

}
