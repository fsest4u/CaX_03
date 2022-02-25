#include <QScrollBar>

#include "listbrowser.h"
#include "ui_listbrowser.h"

#include "listbrowserdelegate.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/caxtranslate.h"
#include "util/log.h"
#include "util/utilnovatron.h"

#include "widget/browser.h"

ListBrowser::ListBrowser(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_Model(new QStandardItemModel),
	m_Delegate(new ListBrowserDelegate),
	m_ScrollBar(nullptr),
	ui(new Ui::ListBrowser)
{
	ui->setupUi(this);

	Initialize();
}

ListBrowser::~ListBrowser()
{
	if (m_ScrollBar)
	{
		delete m_ScrollBar;
		m_ScrollBar = nullptr;
	}

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

QList<CJsonNode> ListBrowser::GetNodeList() const
{
	return m_NodeList;
}

int ListBrowser::SetNodeList(const QList<CJsonNode> list, int service)
{
	Loading *loading = UtilNovatron::LoadingStart(parentWidget()->parentWidget());

	int type = 0;
	int index = m_NodeList.count();
	m_NodeList.append(list);

	m_Delegate->SetService(service);

	if (SIDEMENU_BROWSER == service)
	{
		foreach (CJsonNode node, list)
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

			QString path = node.GetString(KEY_PATH);
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
	else if (SIDEMENU_ISERVICE == service)
	{
		foreach (CJsonNode node, list)
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

			QString coverArt = node.GetString(KEY_ART);
			if (!coverArt.isEmpty())
			{
				emit SigReqCoverArt(coverArt, index);
			}
			index++;
		}
	}
	else // qobuz
	{
		foreach (CJsonNode node, list)
		{
//			LogDebug("node [%s]", node.ToCompactByteArray().data());
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

			QString coverArt = node.GetString(KEY_ART);
			if (!coverArt.isEmpty())
			{
				emit SigReqCoverArt(coverArt, index);
			}
			index++;
		}
	}

	UtilNovatron::LoadingStop(loading);

	return type;
}

void ListBrowser::ClearNodeList()
{
	m_Model->clear();
	m_NodeList.clear();
	m_SelectMap.clear();
	m_SelectMapIService.clear();

}

void ListBrowser::SetNodeInfo(CJsonNode node)
{
//	LogDebug("node [%s]", node.ToCompactByteArray().data());

//	if (node.IsNull())
//	{
//		return;
//	}

//	ui->frameInfo->show();
//	QString style;
//	style = QString("QLabel	\
//					{	\
//					  border-image: url(\'%1\');	\
//					}").arg(node.GetString(KEY_ART));
//	ui->labelCoverArt->setStyleSheet(style);
//	ui->labelTitle->setText(node.GetString(KEY_TOP));
//	ui->labelSubtitle->setText(node.GetString(KEY_BOT1));
}

void ListBrowser::ClearSelectMap()
{
	int count = m_Model->rowCount();
	int serviceType = m_Delegate->GetService();
	if (SIDEMENU_BROWSER == serviceType)
	{
		for (int i = 0; i < count; i++)
		{
			QModelIndex modelIndex = m_Model->index(i, 0);
			m_Model->setData(modelIndex, false, ListBrowserDelegate::LIST_BROWSER_SELECT);
			QString path = qvariant_cast<QString>(modelIndex.data(ListBrowserDelegate::LIST_BROWSER_TITLE));
			m_SelectMap.remove(path);
		}
	}
	else
	{
		for (int i = 0; i < count; i++)
		{
			QModelIndex modelIndex = m_Model->index(i, 0);
			m_Model->setData(modelIndex, false, ListBrowserDelegate::LIST_BROWSER_SELECT);
			int index = qvariant_cast<int>(modelIndex.data(ListBrowserDelegate::LIST_BROWSER_INDEX));
			m_SelectMapIService.remove(index);
		}
	}
}

void ListBrowser::SetAllSelectMap()
{
	int count = m_Model->rowCount();
	int serviceType = m_Delegate->GetService();
	if (SIDEMENU_BROWSER == serviceType)
	{
		for (int i = 0; i < count; i++)
		{
			QModelIndex modelIndex = m_Model->index(i, 0);
			m_Model->setData(modelIndex, true, ListBrowserDelegate::LIST_BROWSER_SELECT);
			QString path = qvariant_cast<QString>(modelIndex.data(ListBrowserDelegate::LIST_BROWSER_TITLE));
			int type = qvariant_cast<int>(modelIndex.data(ListBrowserDelegate::LIST_BROWSER_TYPE));
			m_SelectMap.insert(path, type);
		}
	}
	else
	{
		for (int i = 0; i < count; i++)
		{
			QModelIndex modelIndex = m_Model->index(i, 0);
			m_Model->setData(modelIndex, true, ListBrowserDelegate::LIST_BROWSER_SELECT);
			int index = qvariant_cast<int>(modelIndex.data(ListBrowserDelegate::LIST_BROWSER_INDEX));
			QString rawData = qvariant_cast<QString>(modelIndex.data(ListBrowserDelegate::LIST_BROWSER_RAW));
			CJsonNode node;
			if (!node.SetContent(rawData))
			{
				continue;
			}
			m_SelectMapIService.insert(index, node);
		}
	}
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

//void ListBrowser::SetEditor(int index)
//{
//	QModelIndex modelIndex = m_Model->index(index, 0);

//	QStandardItem *item = m_Model->itemFromIndex(modelIndex);
//	QString nothing = qvariant_cast<QString>(item->data(ListBrowserDelegate::LIST_BROWSER_MAX));
//	item->setData(nothing + " ", ListBrowserDelegate::LIST_BROWSER_MAX);

//	m_ListView->openPersistentEditor(modelIndex);
//}

QListView *ListBrowser::GetListView()
{
	return m_ListView;
}

QStandardItemModel *ListBrowser::GetModel()
{
	return m_Model;
}

ListBrowserDelegate *ListBrowser::GetDelegate()
{
	return m_Delegate;
}

void ListBrowser::SlotScrollValueChanged(int value)
{
	int min = m_ScrollBar->minimum();
	int max = m_ScrollBar->maximum();
//	LogDebug("value [%d] min [%d] max [%d]", value, min, max);
	if (value >= max && max != 0)
	{
		emit SigAppendList();
	}
}

void ListBrowser::SlotSelectCheck(const QModelIndex &modelIndex)
{
	int serviceType = m_Delegate->GetService();
	if (SIDEMENU_BROWSER == serviceType)
	{
		QString path = qvariant_cast<QString>(modelIndex.data(ListBrowserDelegate::LIST_BROWSER_TITLE));
		int type = qvariant_cast<int>(modelIndex.data(ListBrowserDelegate::LIST_BROWSER_TYPE));
		bool bSelect = !qvariant_cast<bool>(modelIndex.data(ListBrowserDelegate::LIST_BROWSER_SELECT));
		m_Model->setData(modelIndex, bSelect, ListBrowserDelegate::LIST_BROWSER_SELECT);

		if (bSelect)
		{
			m_SelectMap.insert(path, type);
		}
		else
		{
			m_SelectMap.remove(path);
		}

	}
	else
	{
		int index = qvariant_cast<int>(modelIndex.data(ListBrowserDelegate::LIST_BROWSER_INDEX));
		QString rawData = qvariant_cast<QString>(modelIndex.data(ListBrowserDelegate::LIST_BROWSER_RAW));
		bool bSelect = !qvariant_cast<bool>(modelIndex.data(ListBrowserDelegate::LIST_BROWSER_SELECT));
		m_Model->setData(modelIndex, bSelect, ListBrowserDelegate::LIST_BROWSER_SELECT);

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

	}
}

void ListBrowser::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_ListView->setViewMode(QListView::ListMode);
	m_ListView->setGridSize(QSize(ICON_ITEM_WIDTH, LIST_HEIGHT_MIN));
	m_ListView->setEditTriggers(QAbstractItemView::EditTrigger::AllEditTriggers);
	m_ListView->setMouseTracking(true);

	m_ScrollBar = m_ListView->verticalScrollBar();
	connect(m_ScrollBar, SIGNAL(valueChanged(int)), this, SLOT(SlotScrollValueChanged(int)));
//	connect(m_Delegate, SIGNAL(SigSelectCoverArt(int)), this, SLOT(SlotSelectCoverArt(int)));
	connect(m_Delegate, SIGNAL(SigSelectCheck(const QModelIndex&)), this, SLOT(SlotSelectCheck(const QModelIndex&)));

	m_SelectMap.clear();
	m_SelectMapIService.clear();

	ui->gridLayout->addWidget(m_ListView);
	ui->frameInfo->hide();
}
