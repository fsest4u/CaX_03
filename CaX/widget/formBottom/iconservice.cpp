#include "iconservice.h"
#include "ui_iconservice.h"

#include "iconservicedelegate.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"

IconService::IconService(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_Model(new QStandardItemModel),
	m_Delegate(new IconServiceDelegate),
	m_pLoading(new Loading(this)),
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

	if (m_pLoading)
	{
		delete m_pLoading;
		m_pLoading = nullptr;
	}
}

QList<CJsonNode> IconService::GetNodeList() const
{
	return m_NodeList;
}

void IconService::SetNodeList(const QList<CJsonNode> &list, int nService)
{
	m_pLoading->Start();
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
			item->setData(false, IconServiceDelegate::ICON_SERVICE_SELECT);

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
			item->setData(false, IconServiceDelegate::ICON_SERVICE_SELECT);

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
			item->setData(false, IconServiceDelegate::ICON_SERVICE_SELECT);

			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);
		}
	}
	else if (ICON_SERVICE_FM_RADIO == nService
			 || ICON_SERVICE_DAB_RADIO == nService)
	{
		foreach (CJsonNode node, m_NodeList)
		{
			QStandardItem *item = new QStandardItem;
			item->setData(node.GetInt(KEY_ID_UPPER), IconServiceDelegate::ICON_SERVICE_ID);
			item->setData(node.GetInt(KEY_TYPE), IconServiceDelegate::ICON_SERVICE_TYPE);
			item->setData(node.GetString(KEY_COVER_ART), IconServiceDelegate::ICON_SERVICE_COVER);
			item->setData(node.GetString(KEY_RIGHT), IconServiceDelegate::ICON_SERVICE_TITLE);
			item->setData(node.ToCompactString(), IconServiceDelegate::ICON_SERVICE_RAW);
			item->setData(false, IconServiceDelegate::ICON_SERVICE_SELECT);

			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);
		}
	}
	else if (ICON_SERVICE_FM_RADIO_RECORD == nService
			 || ICON_SERVICE_DAB_RADIO_RECORD == nService)
	{
		foreach (CJsonNode node, m_NodeList)
		{
			QStandardItem *item = new QStandardItem;
			item->setData(node.GetInt(KEY_ID_UPPER), IconServiceDelegate::ICON_SERVICE_ID);
			item->setData(node.GetInt(KEY_TYPE), IconServiceDelegate::ICON_SERVICE_TYPE);
			item->setData(node.GetString(KEY_COVER_ART), IconServiceDelegate::ICON_SERVICE_COVER);
			item->setData(node.GetString(KEY_TOP), IconServiceDelegate::ICON_SERVICE_TITLE);
			item->setData(node.ToCompactString(), IconServiceDelegate::ICON_SERVICE_RAW);
			item->setData(false, IconServiceDelegate::ICON_SERVICE_SELECT);

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
			item->setData(node.GetInt(KEY_MAC_ADDR), IconServiceDelegate::ICON_SERVICE_TYPE);
			item->setData(":/resource/groupp-img160-n@3x.png", IconServiceDelegate::ICON_SERVICE_COVER);
			item->setData(node.GetString(KEY_CA_NAME), IconServiceDelegate::ICON_SERVICE_TITLE);
			item->setData(subtitle, IconServiceDelegate::ICON_SERVICE_SUBTITLE);
			item->setData(node.ToCompactString(), IconServiceDelegate::ICON_SERVICE_RAW);
			item->setData(false, IconServiceDelegate::ICON_SERVICE_SELECT);

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
	m_pLoading->Stop();

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

void IconService::ClearSelectMap()
{
	m_pLoading->Start();
	int count = m_Model->rowCount();
	for (int i = 0; i < count; i++)
	{
		QModelIndex index = m_Model->index(i, 0);
		QStandardItem *item = m_Model->itemFromIndex(index);
		bool bSelect = qvariant_cast<bool>(item->data(IconServiceDelegate::ICON_SERVICE_SELECT));
		if (bSelect)
		{
			item->setData(false, IconServiceDelegate::ICON_SERVICE_SELECT);

//			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(index);

			int id = qvariant_cast<int>(item->data(IconServiceDelegate::ICON_SERVICE_ID));
			m_SelectMap.remove(id);
		}
	}
	m_pLoading->Stop();
}

void IconService::SetAllSelectMap()
{
	m_pLoading->Start();
	int count = m_Model->rowCount();
	for (int i = 0; i < count; i++)
	{
		QModelIndex index = m_Model->index(i, 0);
		QStandardItem *item = m_Model->itemFromIndex(index);
		bool bSelect = qvariant_cast<bool>(item->data(IconServiceDelegate::ICON_SERVICE_SELECT));
		if (!bSelect)
		{
			item->setData(true, IconServiceDelegate::ICON_SERVICE_SELECT);

//			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(index);

			int id = qvariant_cast<int>(item->data(IconServiceDelegate::ICON_SERVICE_ID));
			m_SelectMap.insert(id, true);
		}
	}
	m_pLoading->Stop();
}

QMap<int, bool> IconService::GetSelectMap() const
{
	return m_SelectMap;
}

void IconService::SetSelectMap(const QMap<int, bool> &SelectMap)
{
	m_SelectMap = SelectMap;
}

QStandardItemModel *IconService::GetModel()
{
	return m_Model;
}

IconServiceDelegate *IconService::GetDelegate()
{
	return m_Delegate;
}

void IconService::SlotDoubleClickItem(const QModelIndex &index)
{
	QStandardItem *item = m_Model->itemFromIndex(index);
	bool bSelect = !qvariant_cast<bool>(item->data(IconServiceDelegate::ICON_SERVICE_SELECT));
	item->setData(bSelect, IconServiceDelegate::ICON_SERVICE_SELECT);

//	QModelIndex modelIndex = m_Model->indexFromItem(item);
	m_ListView->openPersistentEditor(index);

	int id = qvariant_cast<int>(item->data(IconServiceDelegate::ICON_SERVICE_ID));
	if (bSelect)
	{
		m_SelectMap.insert(id, bSelect);
	}
	else
	{
		m_SelectMap.remove(id);
	}

}

void IconService::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_ListView->setGridSize(QSize(ICON_ITEM_WIDTH, ICON_ITEM_HEIGHT));
	SetViewMode(QListView::IconMode);

	connect(m_ListView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(SlotDoubleClickItem(const QModelIndex&)));
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
