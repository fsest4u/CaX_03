#include <QMenu>

#include "queuetrack.h"
#include "ui_queuetrack.h"

#include "queuetrackdelegate.h"

#include "dialog/commondialog.h"

#include "manager/queuelistmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/utilnovatron.h"

QueueTrack::QueueTrack(QWidget *parent, const QString &addr, const int &eventID) :
	QWidget(parent),
	m_pMgr(new QueuelistManager),
	m_ListView(new QListView),
	m_Model(new QStandardItemModel),
	m_Delegate(new QueueTrackDelegate),
	m_EventID(eventID),
	m_Menu(new QMenu(this)),
	ui(new Ui::QueueTrack)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
	Initialize();
}

QueueTrack::~QueueTrack()
{
	if (m_pMgr)
	{
		delete m_pMgr;
		m_pMgr = nullptr;
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

	disconnect(m_Menu, SIGNAL(triggered(QAction*)));
	if (m_Menu)
	{
		delete m_Menu;
		m_Menu = nullptr;
	}

	delete ui;
}

QList<CJsonNode> QueueTrack::GetNodeList() const
{
	return m_NodeList;
}

int QueueTrack::SetNodeList(QList<CJsonNode> list)
{
	Loading *loading = UtilNovatron::LoadingStart(parentWidget()->parentWidget());

	m_NodeList = list;

	int index = 0;
	int totalTime = 0;

	foreach (CJsonNode node, list)
	{
//		LogDebug("node [%s]", node.ToCompactByteArray().data());
		int seconds = node.GetInt(KEY_DURATION);
		QString hhmmss = UtilNovatron::CalcSecondToHMS(seconds);

		QStandardItem *item = new QStandardItem;
		int nID = node.GetString(KEY_SONG).toInt();
		item->setData(nID, QueueTrackDelegate::QUEUE_TRACKS_ID);
		item->setData(node.GetString(KEY_TOP), QueueTrackDelegate::QUEUE_TRACKS_TITLE);
		item->setData(hhmmss, QueueTrackDelegate::QUEUE_TRACKS_TIME);
		item->setData(node.GetString(KEY_BOT), QueueTrackDelegate::QUEUE_TRACKS_ARTIST);
		item->setData(index, QueueTrackDelegate::QUEUE_TRACKS_INDEX);
		item->setData(false, QueueTrackDelegate::QUEUE_TRACKS_PLAY_STATUS);

		m_Model->appendRow(item);

		totalTime += seconds;
		index++;
	}

	UtilNovatron::LoadingStop(loading);

	return totalTime;
}

void QueueTrack::ClearNodeList()
{
	m_Model->clear();
	m_NodeList.clear();
}

int QueueTrack::GetCurrentIndex()
{
	QModelIndex modelIndex = m_ListView->currentIndex();
	return qvariant_cast<int>(modelIndex.data(QueueTrackDelegate::QUEUE_TRACKS_INDEX));
}

void QueueTrack::SetCurrentIndex(int index)
{
	if (m_OldIndex >= 0)
	{
		QModelIndex oldModelIndex = m_Model->index(m_OldIndex, 0);
		m_Model->setData(oldModelIndex, false, QueueTrackDelegate::QUEUE_TRACKS_PLAY_STATUS);
	}

	m_OldIndex = index;

	QModelIndex modelIndex = m_Model->index(index, 0);
	m_Model->setData(modelIndex, true, QueueTrackDelegate::QUEUE_TRACKS_PLAY_STATUS);

	m_ListView->setCurrentIndex(modelIndex);
}

QStandardItemModel *QueueTrack::GetModel()
{
	return m_Model;
}

QueueTrackDelegate *QueueTrack::GetDelegate()
{
	return m_Delegate;
}

void QueueTrack::SlotRespError(QString errMsg)
{
	CommonDialog dialog(this, STR_WARNING, errMsg);
	dialog.exec();
}

void QueueTrack::SlotRespTrackInfo(CJsonNode node)
{
//	LogDebug("node [%s]", node.ToTabedByteArray().data());
	QMap<int, QString>	menuMap;

	m_TrackID = node.GetInt(KEY_ID_LOWER);
	m_TrackAlbumID = node.GetInt(KEY_ALBUM_ID);
	m_TrackArtistID = node.GetInt(KEY_ARTIST_ID);
	m_TrackFavorite = node.GetInt(KEY_FAVORITE);

	if (m_TrackFavorite == 1)
	{
		menuMap.insert(OPTION_MENU_FAVORITE, STR_DELETE_TO_FAVORITE);
	}
	else
	{
		menuMap.insert(OPTION_MENU_FAVORITE, STR_ADD_TO_FAVORITE);
	}

	menuMap.insert(OPTION_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
	menuMap.insert(OPTION_MENU_GO_TO_ALBUM, STR_GO_TO_ALBUM);
	menuMap.insert(OPTION_MENU_GO_TO_ARTIST, STR_GO_TO_ARTIST);
	menuMap.insert(OPTION_MENU_DELETE_FROM_PLAY_QUEUE, STR_DELETE_FROM_PLAY_QUEUE);

	ClearOptionMenu();
	SetOptionMenu(menuMap);
}

void QueueTrack::SlotRespQueueList(CJsonNode result)
{
	QList<CJsonNode> list;
	for (int i = 0; i < result.ArraySize(); i++)
	{
		list.append(result.GetArrayAt(i));
//		LogDebug("node : [%s]", m_NodeList[i].ToCompactByteArray().data());
	}

	ClearNodeList();
	int totTime = SetNodeList(list);

	emit SigUpdateTimeStamp(m_TimeStamp, list.count(), totTime);
}

void QueueTrack::SlotRespDeleteQueue(CJsonNode node)
{
	m_TimeStamp = node.GetInt64(KEY_TIME_STAMP);
	m_pMgr->RequestQueueList(m_TimeStamp);
}

void QueueTrack::SlotSelectMenu(const QModelIndex &modelIndex, QPoint point)
{
	int id = qvariant_cast<int>(modelIndex.data(QueueTrackDelegate::QUEUE_TRACKS_ID));
	m_SelIndex = qvariant_cast<int>(modelIndex.data(QueueTrackDelegate::QUEUE_TRACKS_INDEX));
	m_SelPoint = point;
//	LogDebug("id [%d] x [%d] y [%d]", id, m_SelPoint.x(), m_SelPoint.y());
	m_pMgr->RequestTrackInfo(id);
}

void QueueTrack::SlotMenuAction(QAction *action)
{
	int menuID = action->data().toInt();
	switch (menuID) {
	case OPTION_MENU_FAVORITE:
		DoOptionMenuFavorite();
		break;
	case OPTION_MENU_ADD_TO_PLAYLIST:
		DoOptionMenuAddToPlaylist();
		break;
	case OPTION_MENU_GO_TO_ALBUM:
		DoOptionMenuGoToAlbum();
		break;
	case OPTION_MENU_GO_TO_ARTIST:
		DoOptionMenuGoToArtist();
		break;
	case OPTION_MENU_DELETE_FROM_PLAY_QUEUE:
		DoOptionMenuDeleteFromPlayQueue();
		break;
	}
}

void QueueTrack::ConnectSigToSlot()
{
	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespTrackInfo(CJsonNode)), this, SLOT(SlotRespTrackInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespQueueList(CJsonNode)), this, SLOT(SlotRespQueueList(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespDeleteQueue(CJsonNode)), this, SLOT(SlotRespDeleteQueue(CJsonNode)));

	connect(m_Delegate, SIGNAL(SigSelectMenu(const QModelIndex&, QPoint)), this, SLOT(SlotSelectMenu(const QModelIndex&, QPoint)));

	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuAction(QAction*)));
}

void QueueTrack::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_ListView->setViewMode(QListView::ListMode);
	m_ListView->setGridSize(QSize(ICON_ITEM_WIDTH, LIST_HEIGHT_MIN));
	m_ListView->setEditTriggers(QAbstractItemView::EditTrigger::AllEditTriggers);
	m_ListView->setMouseTracking(true);

	ui->gridLayout->addWidget(m_ListView);

	m_OldIndex = -1;
	m_SelIndex = -1;
	m_TimeStamp = 0;

	QString style = QString("QMenu::icon {	\
								padding: 0px 0px 0px 20px;	\
							}	\
							QMenu::item {	\
								width: 260px;	\
								height: 40px;	\
								color: rgb(255, 255, 255);	\
								font-size: 14pt;	\
								padding: 0px 20px 0px 20px;	\
							}	\
							QMenu::item:selected {	\
								background: rgba(84,84,84,255);	\
							}");

	m_Menu->setStyleSheet(style);

	m_TrackID = -1;
	m_TrackAlbumID = -1;
	m_TrackArtistID = -1;
	m_TrackFavorite = -1;

}

void QueueTrack::ClearOptionMenu()
{
	m_Menu->clear();
}

void QueueTrack::SetOptionMenu(QMap<int, QString> map)
{
	QMap<int, QString>::iterator i;
	for (i = map.begin(); i != map.end(); i++)
	{
		QIcon icon = UtilNovatron::GetMenuIcon(i.value());
		QAction *action = new QAction(icon, i.value(), this);
		action->setData(i.key());
		m_Menu->addAction(action);
	}

	m_Menu->popup(m_ListView->viewport()->mapToGlobal(m_SelPoint));

}

void QueueTrack::DoOptionMenuFavorite()
{
	emit SigFavorite(m_TrackID, m_TrackFavorite);
}

void QueueTrack::DoOptionMenuAddToPlaylist()
{
	emit SigAddToPlaylist(m_TrackID);
}

void QueueTrack::DoOptionMenuGoToAlbum()
{
	emit SigGoToAlbum(m_TrackAlbumID);
}

void QueueTrack::DoOptionMenuGoToArtist()
{
	emit SigGoToArtist(m_TrackArtistID);
}

void QueueTrack::DoOptionMenuDeleteFromPlayQueue()
{
	m_pMgr->RequestDeletePlayQueue(m_SelIndex, m_EventID);
}
