#include <QThread>

#include "listbrowser.h"
#include "ui_listbrowser.h"

#include "listbrowserdelegate.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/caxtranslate.h"
#include "util/loading.h"
#include "util/log.h"
#include "util/utilnovatron.h"

#include "widget/browser.h"

ListBrowser::ListBrowser(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_Model(new QStandardItemModel),
	m_Delegate(new ListBrowserDelegate),
//	m_pLoading(new Loading(this)),
	ui(new Ui::ListBrowser)
{
	ui->setupUi(this);

	Initialize();
}

ListBrowser::~ListBrowser()
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

void ListBrowser::ClearNodeList()
{
	m_Model->clear();
	ui->gridLayout->removeWidget(m_ListView);
}

QList<CJsonNode> ListBrowser::GetNodeList() const
{
	return m_NodeList;
}

int ListBrowser::SetNodeList(const QList<CJsonNode> &NodeList, int nService)
{
	int type = 0;
	int index = 0;

//	m_pLoading->Start();
	m_Model->clear();
	m_NodeList = NodeList;

	m_Delegate->SetService(nService);

	if (SIDEMENU_BROWSER == nService)
	{
		foreach (CJsonNode node, m_NodeList)
		{
//			LogDebug("node [%s]", node.ToCompactByteArray().data());
			QString title = node.GetString(KEY_NAME_CAP);
			if (title.isEmpty())
			{
				title = node.GetString(KEY_PATH);
			}
			int nodeType = node.GetInt(KEY_TYPE);
			type = type | nodeType;

			int seconds = node.GetInt(KEY_DURATION);
			QString hhmmss = UtilNovatron::CalcSecondToHMS(seconds);

			QStandardItem *item = new QStandardItem;
			item->setData(index, ListBrowserDelegate::LIST_BROWSER_ID);
			item->setData(nodeType, ListBrowserDelegate::LIST_BROWSER_TYPE);
			item->setData(UtilNovatron::GetCoverArtIcon(SIDEMENU_BROWSER), ListBrowserDelegate::LIST_BROWSER_COVER);
			item->setData(title, ListBrowserDelegate::LIST_BROWSER_TITLE);
			item->setData(node.GetString(KEY_BOT), ListBrowserDelegate::LIST_BROWSER_SUBTITLE);
			item->setData(hhmmss, ListBrowserDelegate::LIST_BROWSER_DURATION);
			item->setData(node.GetString(KEY_SIZE), ListBrowserDelegate::LIST_BROWSER_FILESIZE);
			item->setData(node.ToCompactString(), ListBrowserDelegate::LIST_BROWSER_RAW);
			item->setData(index, ListBrowserDelegate::LIST_BROWSER_INDEX);
			item->setData(false, ListBrowserDelegate::LIST_BROWSER_SELECT);

//			SetBrowserOptionMenu(nodeType);

			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);

//			if (nodeType & iFolderType_Mask_Play_Select)
//			{
//				emit SigReqInfoBot(title, index);
//				emit SigReqCoverArt(title, index);
//			}

			index++;
		}
	}
	else if (SIDEMENU_ISERVICE == nService)
	{
		foreach (CJsonNode node, m_NodeList)
		{
//			LogDebug("node [%s]", node.ToCompactByteArray().data());
			UtilNovatron::DebugTypeForAirable("SetNodeList", node.GetInt(KEY_TYPE));

			int nodeType = node.GetInt(KEY_TYPE);
			type = type | nodeType;

			int seconds = node.GetInt(KEY_TIME_CAP);
			QString hhmmss = UtilNovatron::CalcSecondToHMS(seconds);

			QStandardItem *item = new QStandardItem;
			item->setData(index, ListBrowserDelegate::LIST_BROWSER_ID);
			item->setData(nodeType, ListBrowserDelegate::LIST_BROWSER_TYPE);
			item->setData(UtilNovatron::GetCoverArtIcon(SIDEMENU_ISERVICE, node.GetString(KEY_COVER_ART)), ListBrowserDelegate::LIST_BROWSER_COVER);
			item->setData(node.GetString(KEY_TOP), ListBrowserDelegate::LIST_BROWSER_TITLE);
			item->setData(node.GetString(KEY_BOT1), ListBrowserDelegate::LIST_BROWSER_SUBTITLE);
			item->setData(hhmmss, ListBrowserDelegate::LIST_BROWSER_DURATION);
			item->setData(node.ToCompactString(), ListBrowserDelegate::LIST_BROWSER_RAW);
			item->setData(index, ListBrowserDelegate::LIST_BROWSER_INDEX);
			item->setData(false, ListBrowserDelegate::LIST_BROWSER_SELECT);

			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);

//			QString coverArt = node.GetString(KEY_ART);
//			if (!coverArt.isEmpty())
//			{
//				emit SigReqCoverArt(coverArt, index);
//			}
			index++;
		}
	}
	else // qobuz
	{
		foreach (CJsonNode node, m_NodeList)
		{
			LogDebug("node [%s]", node.ToCompactByteArray().data());
			UtilNovatron::DebugTypeForQobuz("SetNodeList", node.GetInt(KEY_TYPE));

			int nodeType = node.GetInt(KEY_TYPE);
			type = type | nodeType;

			int seconds = node.GetInt(KEY_TIME_CAP);
			QString hhmmss = UtilNovatron::CalcSecondToHMS(seconds);

			QStandardItem *item = new QStandardItem;
			item->setData(node.GetString(KEY_ID_UPPER), ListBrowserDelegate::LIST_BROWSER_ID);
			item->setData(nodeType, ListBrowserDelegate::LIST_BROWSER_TYPE);
			item->setData(UtilNovatron::GetCoverArtIcon(SIDEMENU_ISERVICE, node.GetString(KEY_COVER_ART)), ListBrowserDelegate::LIST_BROWSER_COVER);
			item->setData(node.GetString(KEY_TOP), ListBrowserDelegate::LIST_BROWSER_TITLE);
			item->setData(node.GetString(KEY_BOT1), ListBrowserDelegate::LIST_BROWSER_SUBTITLE);
			item->setData(hhmmss, ListBrowserDelegate::LIST_BROWSER_DURATION);
			item->setData(node.ToCompactString(), ListBrowserDelegate::LIST_BROWSER_RAW);
			item->setData(index, ListBrowserDelegate::LIST_BROWSER_INDEX);
			item->setData(false, ListBrowserDelegate::LIST_BROWSER_SELECT);

			m_Model->appendRow(item);
			QModelIndex modelIndex = m_Model->indexFromItem(item);
			m_ListView->openPersistentEditor(modelIndex);

			QString coverArt = node.GetString(KEY_ART);
			if (!coverArt.isEmpty())
			{
				emit SigReqCoverArt(coverArt, index);
			}
			index++;
		}
	}

	ui->gridLayoutList->addWidget(m_ListView);
//	m_pLoading->Stop();

	return type;
}

void ListBrowser::SetNodeInfo(CJsonNode node)
{
	LogDebug("node [%s]", node.ToCompactByteArray().data());

	if (node.IsNull())
	{
		return;
	}

	ui->frameInfo->show();
	QString style;
	style = QString("QLabel	\
					{	\
					  border-image: url(\'%1\');	\
					}").arg(node.GetString(KEY_ART));
	ui->labelCoverArt->setStyleSheet(style);
	ui->labelTitle->setText(node.GetString(KEY_TOP));
	ui->labelSubtitle->setText(node.GetString(KEY_BOT1));
}

void ListBrowser::ClearSelectMap()
{
//	m_pLoading->Start();
	int count = m_Model->rowCount();

	int serviceType = m_Delegate->GetService();
	if (SIDEMENU_BROWSER == serviceType)
	{
		for (int i = 0; i < count; i++)
		{
			QModelIndex index = m_Model->index(i, 0);
			QStandardItem *item = m_Model->itemFromIndex(index);
	//		bool bSelect = qvariant_cast<bool>(item->data(ListBrowserDelegate::LIST_BROWSER_SELECT));
	//		if (bSelect)
			{
				item->setData(false, ListBrowserDelegate::LIST_BROWSER_SELECT);

	//			QModelIndex modelIndex = m_Model->indexFromItem(item);
				m_ListView->openPersistentEditor(index);

				QString path = qvariant_cast<QString>(item->data(ListBrowserDelegate::LIST_BROWSER_TITLE));
				m_SelectMap.remove(path);
			}
		}
	}
	else
	{
		for (int i = 0; i < count; i++)
		{
			QModelIndex index = m_Model->index(i, 0);
			QStandardItem *item = m_Model->itemFromIndex(index);
	//		bool bSelect = qvariant_cast<bool>(item->data(ListBrowserDelegate::LIST_BROWSER_SELECT));
	//		if (bSelect)
			{
				item->setData(false, ListBrowserDelegate::LIST_BROWSER_SELECT);

	//			QModelIndex modelIndex = m_Model->indexFromItem(item);
				m_ListView->openPersistentEditor(index);

				int index = qvariant_cast<int>(item->data(ListBrowserDelegate::LIST_BROWSER_INDEX));
				m_SelectMapIService.remove(index);
			}
		}
	}

//	m_pLoading->Stop();

}

void ListBrowser::SetAllSelectMap()
{
//	m_pLoading->Start();
	int count = m_Model->rowCount();

	int serviceType = m_Delegate->GetService();
	if (SIDEMENU_BROWSER == serviceType)
	{
		for (int i = 0; i < count; i++)
		{
			QModelIndex index = m_Model->index(i, 0);
			QStandardItem *item = m_Model->itemFromIndex(index);
			bool bSelect = qvariant_cast<bool>(item->data(ListBrowserDelegate::LIST_BROWSER_SELECT));
			if (!bSelect)
			{
	//			QModelIndex modelIndex = m_Model->indexFromItem(item);
				m_ListView->openPersistentEditor(index);

				QString path = qvariant_cast<QString>(item->data(ListBrowserDelegate::LIST_BROWSER_TITLE));
				int type = qvariant_cast<int>(item->data(ListBrowserDelegate::LIST_BROWSER_TYPE));
				m_SelectMap.insert(path, type);
				item->setData(true, ListBrowserDelegate::LIST_BROWSER_SELECT);
			}
		}
	}
	else
	{
		for (int i = 0; i < count; i++)
		{
			QModelIndex index = m_Model->index(i, 0);
			QStandardItem *item = m_Model->itemFromIndex(index);
			bool bSelect = qvariant_cast<bool>(item->data(ListBrowserDelegate::LIST_BROWSER_SELECT));
			if (!bSelect)
			{
	//			QModelIndex modelIndex = m_Model->indexFromItem(item);
				m_ListView->openPersistentEditor(index);

				int index = qvariant_cast<int>(item->data(ListBrowserDelegate::LIST_BROWSER_INDEX));
				QString rawData = qvariant_cast<QString>(item->data(ListBrowserDelegate::LIST_BROWSER_RAW));

				CJsonNode node;
				if (!node.SetContent(rawData))
				{
					continue;
				}
//				QString icon = node.GetString(KEY_ICON);
//				if (icon.isEmpty() || icon.compare("feed"))
//				{
//					continue;;
//				}
				m_SelectMapIService.insert(index, node);
				item->setData(true, ListBrowserDelegate::LIST_BROWSER_SELECT);
			}
		}
	}

//	m_pLoading->Stop();

}

QMap<QString, int> ListBrowser::GetSelectMap() const
{
	return m_SelectMap;
}

void ListBrowser::SetSelectMap(const QMap<QString, int> &SelectMap)
{
	m_SelectMap = SelectMap;
}

QMap<int, CJsonNode> ListBrowser::GetSelectMapIService() const
{
	return m_SelectMapIService;
}

void ListBrowser::SetSelectMapIService(const QMap<int, CJsonNode> &SelectMap)
{
	m_SelectMapIService = SelectMap;
}

void ListBrowser::SetEditor(int index)
{
	QModelIndex modelIndex = m_Model->index(index, 0);

	QStandardItem *item = m_Model->itemFromIndex(modelIndex);
	QString nothing = qvariant_cast<QString>(item->data(ListBrowserDelegate::LIST_BROWSER_MAX));
	item->setData(nothing + " ", ListBrowserDelegate::LIST_BROWSER_MAX);

	m_ListView->openPersistentEditor(modelIndex);
}

QStandardItemModel *ListBrowser::GetModel()
{
	return m_Model;
}

ListBrowserDelegate *ListBrowser::GetDelegate()
{
	return m_Delegate;
}

void ListBrowser::SetBackgroundTask(QThread *thread)
{
	connect(thread, SIGNAL(started()), this, SLOT(SlotReqCoverArt()));
	connect(thread, SIGNAL(finished()), this, SLOT(SlotFinishThread()));
}

void ListBrowser::SlotReqCoverArt()
{
	int index = 0;
	if (SIDEMENU_BROWSER == m_Delegate->GetService())
	{
		foreach (CJsonNode node, m_NodeList)
		{
			QThread::msleep(5);
			QString path = node.GetString(KEY_PATH);
			int nodeType = node.GetInt(KEY_TYPE);
			if (nodeType & iFolderType_Mask_Play_Select)
			{
				if (!(nodeType & iFolderType_Mask_Pls))
				{
					emit SigReqInfoBot(path, index);
				}
				emit SigReqCoverArt(path, index);
			}

			index++;
		}
	}
	else
	{
		foreach (CJsonNode node, m_NodeList)
		{
			QThread::msleep(5);
			QString coverArt = node.GetString(KEY_ART);
			if (!coverArt.isEmpty())
			{
				emit SigReqCoverArt(coverArt, index);
			}
			index++;
		}
	}

}

void ListBrowser::SlotFinishThread()
{
//	LogDebug("thread finish good");
}

void ListBrowser::SlotSelectCoverArt(int index)
{
	int serviceType = m_Delegate->GetService();
	if (SIDEMENU_BROWSER == serviceType)
	{
		QStandardItem *item = m_Model->item(index);
		bool bSelect = !qvariant_cast<bool>(item->data(ListBrowserDelegate::LIST_BROWSER_SELECT));

		QModelIndex modelIndex = m_Model->indexFromItem(item);
		m_ListView->openPersistentEditor(modelIndex);

		QString path = qvariant_cast<QString>(item->data(ListBrowserDelegate::LIST_BROWSER_TITLE));
		int type = qvariant_cast<int>(item->data(ListBrowserDelegate::LIST_BROWSER_TYPE));
		if (bSelect)
		{
			m_SelectMap.insert(path, type);
		}
		else
		{
			m_SelectMap.remove(path);
		}
		item->setData(bSelect, ListBrowserDelegate::LIST_BROWSER_SELECT);

	}
	else
	{
		QStandardItem *item = m_Model->item(index);
		bool bSelect = !qvariant_cast<bool>(item->data(ListBrowserDelegate::LIST_BROWSER_SELECT));

		QModelIndex modelIndex = m_Model->indexFromItem(item);
		m_ListView->openPersistentEditor(modelIndex);

		int index = qvariant_cast<int>(item->data(ListBrowserDelegate::LIST_BROWSER_INDEX));
		QString rawData = qvariant_cast<QString>(item->data(ListBrowserDelegate::LIST_BROWSER_RAW));
		if (bSelect)
		{
			CJsonNode node;
			if (!node.SetContent(rawData))
			{
				return;
			}
//			QString icon = node.GetString(KEY_ICON);
//			if (icon.isEmpty() || icon.compare("feed"))
//			{
//				return;
//			}

			m_SelectMapIService.insert(index, node);
		}
		else
		{
			m_SelectMapIService.remove(index);
		}
		item->setData(bSelect, ListBrowserDelegate::LIST_BROWSER_SELECT);

	}
}

void ListBrowser::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_ListView->setViewMode(QListView::ListMode);

	connect(m_Delegate, SIGNAL(SigSelectCoverArt(int)), this, SLOT(SlotSelectCoverArt(int)));

	ui->frameInfo->hide();

	m_SelectMap.clear();
	m_SelectMapIService.clear();

}
