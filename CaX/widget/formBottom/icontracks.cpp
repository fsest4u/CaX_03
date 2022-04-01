#include <QScrollBar>

#include "icontracks.h"
#include "ui_icontracks.h"

#include "icontracksdelegate.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/utilnovatron.h"

IconTracks::IconTracks(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_Model(new QStandardItemModel),
	m_Delegate(new IconTracksDelegate),
	m_ScrollBar(nullptr),
	ui(new Ui::IconTracks)
{
	ui->setupUi(this);

	Initialize();
}

IconTracks::~IconTracks()
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

QList<CJsonNode> IconTracks::GetNodeList() const
{
	return m_NodeList;
}

void IconTracks::SetNodeList(QList<CJsonNode> &list, int service)
{
	ShowFrameEmpty(false);

//	Loading *loading = UtilNovatron::LoadingStart(parentWidget()->parentWidget());

	int index = m_NodeList.count();
	m_NodeList.append(list);
	m_Service = service;

	if (SIDEMENU_AUDIO_CD == m_Service)
	{
//		int totalTime = 0;
		foreach (CJsonNode node, list)
		{
//			LogDebug("node [%s]", node.ToCompactByteArray().data());
//			int seconds = node.GetInt(KEY_TIME_CAP);
//			QString hhmmss = UtilNovatron::CalcSecondToHMS(seconds);

			QStandardItem *item = new QStandardItem;
			int nID = node.GetString(KEY_TRACK).toInt();
			item->setData(nID, IconTracksDelegate::ICON_TRACKS_ID);
			item->setData(UtilNovatron::GetCoverArtIcon(SIDEMENU_AUDIO_CD), IconTracksDelegate::ICON_TRACKS_COVER);
			item->setData(node.GetString(KEY_TOP), IconTracksDelegate::ICON_TRACKS_TITLE);
			item->setData(node.GetString(KEY_BOT), IconTracksDelegate::ICON_TRACKS_SUBTITLE);
			item->setData(node.GetString(KEY_COUNT), IconTracksDelegate::ICON_TRACKS_COUNT);
			item->setData(false, IconTracksDelegate::ICON_TRACKS_SELECT);
			item->setData(index, IconTracksDelegate::ICON_TRACKS_INDEX);
			item->setData(-1, IconTracksDelegate::ICON_TRACKS_FAVORITE);
			item->setData(-1, IconTracksDelegate::ICON_TRACKS_RATING);

			m_Model->appendRow(item);

//			totalTime += seconds;
//			emit SigReqCoverArt(nID, index, QListView::IconMode);
			index++;
		}
//		emit SigCalcTotalTime(totalTime);
	}
	else
	{
		foreach (CJsonNode node, list)
		{
//			LogDebug("node [%s]", node.ToCompactByteArray().data());

			QStandardItem *item = new QStandardItem;
			int nID = node.GetString(KEY_ID_LOWER).toInt();
			item->setData(nID, IconTracksDelegate::ICON_TRACKS_ID);
			item->setData(node.GetString(KEY_TITLE), IconTracksDelegate::ICON_TRACKS_TITLE);
			item->setData(node.GetString(KEY_SUBTITLE), IconTracksDelegate::ICON_TRACKS_SUBTITLE);
			item->setData(node.GetString(KEY_COUNT), IconTracksDelegate::ICON_TRACKS_COUNT);
			item->setData(false, IconTracksDelegate::ICON_TRACKS_SELECT);
			item->setData(index, IconTracksDelegate::ICON_TRACKS_INDEX);
			if (SIDEMENU_MUSIC_DB == service)
			{
				item->setData(node.GetString(KEY_FAVORITE), IconTracksDelegate::ICON_TRACKS_FAVORITE);
				item->setData(node.GetString(KEY_RATING), IconTracksDelegate::ICON_TRACKS_RATING);
			}
			else if (ICON_TRACKS_MUSIC_DB_TRACK == service)
			{
				item->setData(node.GetString(KEY_FAVORITE), IconTracksDelegate::ICON_TRACKS_FAVORITE);
				item->setData(-1, IconTracksDelegate::ICON_TRACKS_RATING);
			}
			else
			{
				item->setData(-1, IconTracksDelegate::ICON_TRACKS_FAVORITE);
				item->setData(-1, IconTracksDelegate::ICON_TRACKS_RATING);
			}

			m_Model->appendRow(item);
//			LogDebug("set data [%d] - [%s]", index, node.GetString(KEY_TITLE).toUtf8().data());
//			emit SigReqCoverArt(nID, index, QListView::IconMode);
			index++;
		}
	}

	SlotScrollReleased();

//	UtilNovatron::LoadingStop(loading);
}

void IconTracks::ClearNodeList()
{
	m_Model->clear();
	m_NodeList.clear();
	m_SelectMap.clear();
}

void IconTracks::ClearSelectMap()
{
	int count = m_Model->rowCount();

	for (int i = 0; i < count; i++)
	{
		QModelIndex modelIndex = m_Model->index(i, 0);
		m_Model->setData(modelIndex, false, IconTracksDelegate::ICON_TRACKS_SELECT);
		int id = qvariant_cast<int>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_ID));
		m_SelectMap.remove(id);
	}
}

void IconTracks::SetAllSelectMap()
{
	int count = m_Model->rowCount();

	for (int i = 0; i < count; i++)
	{
		QModelIndex modelIndex = m_Model->index(i, 0);
		m_Model->setData(modelIndex, true, IconTracksDelegate::ICON_TRACKS_SELECT);
		int id = qvariant_cast<int>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_ID));
		m_SelectMap.insert(id, true);
	}
}

QMap<int, bool> IconTracks::GetSelectMap() const
{
	return m_SelectMap;
}

void IconTracks::SetSelectMap(const QMap<int, bool> &SelectMap)
{
	m_SelectMap = SelectMap;
}

void IconTracks::SetResize(int resize)
{
//	m_Delegate->SetResize(resize);
	m_ListView->setGridSize(QSize(resize, resize + 30));
}

void IconTracks::ShowFrameEmpty(bool show)
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

QListView *IconTracks::GetListView()
{
	return m_ListView;
}

QStandardItemModel *IconTracks::GetModel()
{
	return m_Model;
}

IconTracksDelegate *IconTracks::GetDelegate()
{
	return m_Delegate;
}

void IconTracks::SlotScrollValueChanged(int value)
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

void IconTracks::SlotScrollReleased()
{
//	LogDebug("=======================");
//	LogDebug("slider released...");
//	LogDebug("=======================");
	QModelIndex startModelIndex = m_ListView->indexAt(m_PointCurrent);
	if (!startModelIndex.isValid())
	{
		return;
	}

//	int id = qvariant_cast<int>(startModelIndex.data(IconTracksDelegate::ICON_TRACKS_ID));
	int startIndex = qvariant_cast<int>(startModelIndex.data(IconTracksDelegate::ICON_TRACKS_INDEX));
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
			if (visualRect.y() > validRect.height() || visualRect.y() < (ICON_ITEM_HEIGHT * -1))
			{
//				LogDebug("visual rect is invalid rect");
				break;
			}
			int id = qvariant_cast<int>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_ID));
			int index = qvariant_cast<int>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_INDEX));
//			QString title = qvariant_cast<QString>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_TITLE));
//			QString subtitle = qvariant_cast<QString>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_SUBTITLE));
//			LogDebug("id [%d] index [%d]", id, index);
//			LogDebug("title [%s] subtitle [%s]", title.toUtf8().data(), subtitle.toUtf8().data());
			emit SigReqCoverArt(id, index, QListView::IconMode);

			if (SIDEMENU_MUSIC_DB == m_Service)
			{
				emit SigReqCount(modelIndex);
			}
		}
	}
}

void IconTracks::SlotSelectCheck(const QModelIndex &modelIndex)
{
	int id = qvariant_cast<int>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_ID));
	bool bSelect = !qvariant_cast<bool>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_SELECT));
	m_Model->setData(modelIndex, bSelect, IconTracksDelegate::ICON_TRACKS_SELECT);

	if (bSelect)
	{
		m_SelectMap.insert(id, bSelect);
	}
	else
	{
		m_SelectMap.remove(id);
	}
}

void IconTracks::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_ListView->setViewMode(QListView::IconMode);
	m_ListView->setContextMenuPolicy(Qt::CustomContextMenu);
	m_ListView->setEditTriggers(QAbstractItemView::EditTrigger::AllEditTriggers);
	m_ListView->setMouseTracking(true);
	SetResize(ICON_HEIGHT_INIT);

	m_ScrollBar = m_ListView->verticalScrollBar();
	connect(m_ScrollBar, SIGNAL(valueChanged(int)), this, SLOT(SlotScrollValueChanged(int)));
	connect(m_ScrollBar, SIGNAL(sliderReleased()), this, SLOT(SlotScrollReleased()));
	connect(m_Delegate, SIGNAL(SigSelectCheck(const QModelIndex&)), this, SLOT(SlotSelectCheck(const QModelIndex&)));

	m_SelectMap.clear();

	ui->gridLayout->addWidget(m_ListView);

	ShowFrameEmpty(false);

	m_Service = -1;
}
