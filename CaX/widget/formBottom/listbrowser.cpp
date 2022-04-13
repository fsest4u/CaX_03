#include <QScrollBar>
#include <QListView>

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
	ShowFrameEmpty(false);

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
			UtilNovatron::DebugTypeForBrowser("SetNodeList", nodeType);
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

			if (nodeType & iFolderType_Mask_Upnp)
			{
				item->setData(node.GetString(KEY_ART), ListBrowserDelegate::LIST_BROWSER_COVER);
			}
			else if (nodeType & iFolderType_Mask_Play_Select)
			{
				if (!(nodeType & iFolderType_Mask_Pls))
				{
					item->setData(node.GetString(KEY_PATH), ListBrowserDelegate::LIST_BROWSER_SUBTITLE);
				}
				item->setData(node.GetString(KEY_PATH), ListBrowserDelegate::LIST_BROWSER_COVER);
			}

			m_Model->appendRow(item);
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

			QString coverArt = node.GetString(KEY_ART);
			if (coverArt.isEmpty())
			{
				coverArt = node.GetString(KEY_ICON);
			}

			QStandardItem *item = new QStandardItem;
			item->setData(index, ListBrowserDelegate::LIST_BROWSER_ID);
			item->setData(nodeType, ListBrowserDelegate::LIST_BROWSER_TYPE);
			item->setData(UtilNovatron::GetCoverArtIcon(SIDEMENU_ISERVICE, coverArt), ListBrowserDelegate::LIST_BROWSER_COVER);
			item->setData(node.GetString(KEY_TOP), ListBrowserDelegate::LIST_BROWSER_TITLE);
			item->setData(node.GetString(KEY_BOT1), ListBrowserDelegate::LIST_BROWSER_SUBTITLE);
			item->setData(hhmmss, ListBrowserDelegate::LIST_BROWSER_DURATION);
			item->setData(node.ToCompactString(), ListBrowserDelegate::LIST_BROWSER_RAW);
			item->setData(index, ListBrowserDelegate::LIST_BROWSER_INDEX);
			item->setData(false, ListBrowserDelegate::LIST_BROWSER_SELECT);

			if (coverArt.contains("http"))
			{
				item->setData(coverArt, ListBrowserDelegate::LIST_BROWSER_COVER);
			}

			m_Model->appendRow(item);
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

			QString coverArt = node.GetString(KEY_ART);
			if (!coverArt.isEmpty())
			{
				item->setData(coverArt, ListBrowserDelegate::LIST_BROWSER_COVER);
			}

			m_Model->appendRow(item);
			index++;
		}
	}

	SlotScrollReleased();

	UtilNovatron::LoadingStop(loading);
	delete loading;
	loading = nullptr;

	return type;
}

void ListBrowser::ClearNodeList()
{
	m_Model->clear();
	m_NodeList.clear();
	m_SelectMap.clear();
	m_SelectMapIService.clear();

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

void ListBrowser::ShowFrameEmpty(bool show)
{
	if (show)
	{
		ui->frameEmpty->show();
		m_ListView->hide();
	}
	else
	{
		ui->frameEmpty->hide();
		m_ListView->show();
	}
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
	m_PointCurrent = QPoint(0, min);
	SlotScrollReleased();
}

void ListBrowser::SlotScrollReleased()
{
	int service = m_Delegate->GetService();
//	LogDebug("=======================");
//	LogDebug("slider released...");
//	LogDebug("=======================");
	QModelIndex startModelIndex = m_ListView->indexAt(m_PointCurrent);
	if (!startModelIndex.isValid())
	{
		return;
	}

//	int id = qvariant_cast<int>(startModelIndex.data(ListBrowserDelegate::LIST_BROWSER_ID));
	int startIndex = qvariant_cast<int>(startModelIndex.data(ListBrowserDelegate::LIST_BROWSER_INDEX));
//	LogDebug("start id [%d] index [%d]", id, startIndex);

	QRect validRect = rect();
//	LogDebug("valid x [%d] y [%d] w [%d] h [%d]", validRect.x(), validRect.y(), validRect.width(), validRect.height());

	for (int i = startIndex; i < startIndex + 100; i++)
	{
		QModelIndex modelIndex = m_Model->index(i, 0);
		if (modelIndex.isValid())
		{
			QRect visualRect = m_ListView->visualRect(modelIndex);
//			LogDebug("visual x [%d] y [%d] w [%d] h [%d]", visualRect.x(), visualRect.y(), visualRect.width(), visualRect.height());
			if (visualRect.y() > validRect.height() || visualRect.y() < (ICON_HEIGHT_MID * -1))
			{
//				LogDebug("visual rect is invalid rect");
				break;
			}

			int id = qvariant_cast<int>(modelIndex.data(ListBrowserDelegate::LIST_BROWSER_ID));
			int index = qvariant_cast<int>(modelIndex.data(ListBrowserDelegate::LIST_BROWSER_INDEX));
			QString cover = qvariant_cast<QString>(modelIndex.data(ListBrowserDelegate::LIST_BROWSER_COVER));

//			QString title = qvariant_cast<QString>(modelIndex.data(ListBrowserDelegate::LIST_BROWSER_TITLE));
//			QString subtitle = qvariant_cast<QString>(modelIndex.data(ListBrowserDelegate::LIST_BROWSER_SUBTITLE));
//			LogDebug("id [%d] index [%d] cover [%s]", id, index, cover.toUtf8().data());
//			LogDebug("title [%s] subtitle [%s]", title.toUtf8().data(), subtitle.toUtf8().data());

			if (SIDEMENU_BROWSER == service)
			{
				int type = qvariant_cast<int>(modelIndex.data(ListBrowserDelegate::LIST_BROWSER_TYPE));

				if (type & iFolderType_Mask_Upnp)
				{
					if (!cover.contains(":/resource/"))
					{
						emit SigReqCoverArt(cover, index);
					}
				}
				else if (type & iFolderType_Mask_Play_Select)
				{
					if (!(type & iFolderType_Mask_Pls) && !(type & iFolderType_Dir))
					{
						QString path = qvariant_cast<QString>(modelIndex.data(ListBrowserDelegate::LIST_BROWSER_SUBTITLE));
						if (!path.isEmpty())
						{
							emit SigReqInfoBot(path, index);
						}
					}

					m_Model->setData(modelIndex, "", ListBrowserDelegate::LIST_BROWSER_SUBTITLE);
					if (!cover.contains(":/resource/"))
					{
						emit SigReqCoverArt(cover, index);
					}
				}
			}
			else if (SIDEMENU_ISERVICE == service)
			{
				if (cover.contains("http"))
				{
					emit SigReqCoverArt(cover, index);
				}
			}
			else // qobuz
			{
				if (cover.contains("http"))
				{
					emit SigReqCoverArt(cover, index);
				}
			}
		}
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
	connect(m_ScrollBar, SIGNAL(sliderReleased()), this, SLOT(SlotScrollReleased()));
//	connect(m_Delegate, SIGNAL(SigSelectCoverArt(int)), this, SLOT(SlotSelectCoverArt(int)));
	connect(m_Delegate, SIGNAL(SigSelectCheck(const QModelIndex&)), this, SLOT(SlotSelectCheck(const QModelIndex&)));

	m_SelectMap.clear();
	m_SelectMapIService.clear();

	ui->gridLayout->addWidget(m_ListView);
	ui->frameInfo->hide();

	ShowFrameEmpty(false);
}
