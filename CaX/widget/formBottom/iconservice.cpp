#include "iconservice.h"
#include "ui_iconservice.h"

#include "iconservicedelegate.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"
#include "util/utilnovatron.h"

IconService::IconService(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_Model(new QStandardItemModel),
	m_Delegate(new IconServiceDelegate),
//	m_pLoading(new Loading(this)),
	ui(new Ui::IconService)
{
	ui->setupUi(this);

	Initialize();
}

IconService::~IconService()
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

QList<CJsonNode> IconService::GetNodeList() const
{
	return m_NodeList;
}

int IconService::SetNodeList(const QList<CJsonNode> &list, int nService)
{
	int type = 0;

//	m_pLoading->Start();
	m_Model->clear();
	m_NodeList = list;
	m_Delegate->SetService(nService);

	int index = 0;

	if (ICON_SERVICE_BROWSER == nService)
	{
		foreach (CJsonNode node, m_NodeList)
		{
//			LogDebug("node [%s]", node.ToCompactByteArray().data());
			type = type | node.GetInt(KEY_TYPE);
			QString path = node.GetString(KEY_PATH);

			QStandardItem *item = new QStandardItem;
			item->setData(path, IconServiceDelegate::ICON_SERVICE_ID);
			item->setData(node.GetInt(KEY_TYPE), IconServiceDelegate::ICON_SERVICE_TYPE);
			item->setData(UtilNovatron::GetCoverArtIcon(SIDEMENU_BROWSER, path), IconServiceDelegate::ICON_SERVICE_COVER);
			item->setData(path, IconServiceDelegate::ICON_SERVICE_TITLE);
			if (!node.GetString(KEY_SIZE).isEmpty() && !node.GetString(KEY_FREE).isEmpty())
			{
				item->setData(node.GetString(KEY_SIZE) + " / " + node.GetString(KEY_FREE), IconServiceDelegate::ICON_SERVICE_SUBTITLE);
			}
			else
			{
				item->setData(" ", IconServiceDelegate::ICON_SERVICE_SUBTITLE);
			}
			item->setData(node.ToCompactString(), IconServiceDelegate::ICON_SERVICE_RAW);
			item->setData(index, IconServiceDelegate::ICON_SERVICE_INDEX);
			item->setData(false, IconServiceDelegate::ICON_SERVICE_SELECT);

			m_Model->appendRow(item);

			index++;
		}
	}
	else if (ICON_SERVICE_ISERVICE == nService)
	{
		foreach (CJsonNode node, m_NodeList)
		{
//			LogDebug("node [%s]", node.ToCompactByteArray().data());

			QStandardItem *item = new QStandardItem;
			item->setData(node.GetInt(KEY_ID_UPPER), IconServiceDelegate::ICON_SERVICE_ID);
			item->setData(node.GetInt(KEY_TYPE), IconServiceDelegate::ICON_SERVICE_TYPE);
			item->setData(node.GetString(KEY_COVER_ART), IconServiceDelegate::ICON_SERVICE_COVER);
			item->setData(node.GetString(KEY_NAME_CAP), IconServiceDelegate::ICON_SERVICE_TITLE);
			item->setData(node.ToCompactString(), IconServiceDelegate::ICON_SERVICE_RAW);
			item->setData(index, IconServiceDelegate::ICON_SERVICE_INDEX);
			item->setData(false, IconServiceDelegate::ICON_SERVICE_SELECT);

			m_Model->appendRow(item);

			index++;
		}
	}
	else if (ICON_SERVICE_INPUT == nService)
	{
		foreach (CJsonNode node, m_NodeList)
		{
			QStandardItem *item = new QStandardItem;
			item->setData(node.GetInt(KEY_ID_UPPER), IconServiceDelegate::ICON_SERVICE_ID);
			item->setData(node.GetInt(KEY_TYPE), IconServiceDelegate::ICON_SERVICE_TYPE);
			item->setData(UtilNovatron::GetCoverArtIcon(SIDEMENU_INPUT, node.GetString(KEY_RIGHT)), IconServiceDelegate::ICON_SERVICE_COVER);
			item->setData(node.GetString(KEY_RIGHT), IconServiceDelegate::ICON_SERVICE_TITLE);
			item->setData(node.ToCompactString(), IconServiceDelegate::ICON_SERVICE_RAW);
			item->setData(index, IconServiceDelegate::ICON_SERVICE_INDEX);
			item->setData(false, IconServiceDelegate::ICON_SERVICE_SELECT);

			m_Model->appendRow(item);

			index++;
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
			item->setData(index, IconServiceDelegate::ICON_SERVICE_INDEX);
			item->setData(false, IconServiceDelegate::ICON_SERVICE_SELECT);

			m_Model->appendRow(item);

			index++;
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
			item->setData(index, IconServiceDelegate::ICON_SERVICE_INDEX);
			item->setData(false, IconServiceDelegate::ICON_SERVICE_SELECT);

			m_Model->appendRow(item);

			index++;
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
			item->setData(index, IconServiceDelegate::ICON_SERVICE_ID);
			item->setData(node.GetInt(KEY_MAC_ADDR), IconServiceDelegate::ICON_SERVICE_TYPE);
			item->setData(UtilNovatron::GetCoverArtIcon(SIDEMENU_GROUP_PLAY), IconServiceDelegate::ICON_SERVICE_COVER);
			item->setData(node.GetBool(KEY_MUTED), IconServiceDelegate::ICON_SERVICE_MUTE);
			item->setData(node.GetString(KEY_CA_NAME), IconServiceDelegate::ICON_SERVICE_TITLE);
			item->setData(subtitle, IconServiceDelegate::ICON_SERVICE_SUBTITLE);
			item->setData(node.ToCompactString(), IconServiceDelegate::ICON_SERVICE_RAW);
			item->setData(index, IconServiceDelegate::ICON_SERVICE_INDEX);
			item->setData(false, IconServiceDelegate::ICON_SERVICE_SELECT);

			m_Model->appendRow(item);

			int coverID = GetCoverID(node.GetString(KEY_ART_URL));
			if (coverID >= 0)
			{
				emit SigReqCoverArt(coverID, index, QListView::IconMode);
			}
			index++;
		}
	}

//	m_pLoading->Stop();

	return type;

}

void IconService::ClearNodeList()
{
	m_Model->clear();
	m_NodeList.clear();
	m_SelectMap.clear();
}

void IconService::ClearSelectMap()
{
	int count = m_Model->rowCount();

	for (int i = 0; i < count; i++)
	{
		QModelIndex modelIndex = m_Model->index(i, 0);
		m_Model->setData(modelIndex, false, IconServiceDelegate::ICON_SERVICE_SELECT);
		int id = qvariant_cast<int>(modelIndex.data(IconServiceDelegate::ICON_SERVICE_ID));
		m_SelectMap.remove(id);
	}
}

void IconService::SetAllSelectMap()
{
	int count = m_Model->rowCount();

	for (int i = 0; i < count; i++)
	{
		QModelIndex modelIndex = m_Model->index(i, 0);
		m_Model->setData(modelIndex, true, IconServiceDelegate::ICON_SERVICE_SELECT);
		int id = qvariant_cast<int>(modelIndex.data(IconServiceDelegate::ICON_SERVICE_ID));
		m_SelectMap.insert(id, true);
	}
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

void IconService::SlotSelectCheck(const QModelIndex &modelIndex)
{
	if ((modelIndex.row() < 0)
			||  (m_Delegate->GetService() == ICON_SERVICE_BROWSER
				 || m_Delegate->GetService() == ICON_SERVICE_ISERVICE
				 || m_Delegate->GetService() == ICON_SERVICE_INPUT
				 ))
	{
		return;
	}

	int id = qvariant_cast<int>(modelIndex.data(IconServiceDelegate::ICON_SERVICE_ID));
	bool bSelect = !qvariant_cast<bool>(modelIndex.data(IconServiceDelegate::ICON_SERVICE_SELECT));
	m_Model->setData(modelIndex, bSelect, IconServiceDelegate::ICON_SERVICE_SELECT);

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
	m_ListView->setViewMode(QListView::IconMode);
	m_ListView->setEditTriggers(QAbstractItemView::EditTrigger::AllEditTriggers);
	m_ListView->setMouseTracking(true);

	connect(m_Delegate, SIGNAL(SigSelectCheck(const QModelIndex&)), this, SLOT(SlotSelectCheck(const QModelIndex&)));

	ui->gridLayout->addWidget(m_ListView);
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
