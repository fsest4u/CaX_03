#include <QMenu>

#include "queuetrack.h"
#include "ui_queuetrack.h"

#include "queuetrackdelegate.h"

#include "dialog/addradiodialog.h"
#include "dialog/commondialog.h"
#include "dialog/setupreservationrecordingdialog.h"

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

int QueueTrack::SetNodeList(QList<CJsonNode> list, QString src)
{
	Loading *loading = UtilNovatron::LoadingStart(parentWidget()->parentWidget());

	m_NodeList = list;
	m_Delegate->SetSrc(src);

	int index = 0;
	int totalTime = 0;

	if (!src.compare(SRC_MUSIC_DB)
			|| !src.compare(SRC_AUDIO_CD)
			|| !src.compare(SRC_BROWSER))
	{
		foreach (CJsonNode node, list)
		{
//			LogDebug("node [%s]", node.ToCompactByteArray().data());
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
	}
	else if (!src.compare(SRC_FM_RADIO))
	{
		foreach (CJsonNode node, list)
		{
//			LogDebug("node [%s]", node.ToCompactByteArray().data());
			int seconds = node.GetInt(KEY_DURATION);
			QString hhmmss = UtilNovatron::CalcSecondToHMS(seconds);

			QStandardItem *item = new QStandardItem;
			item->setData(index, QueueTrackDelegate::QUEUE_TRACKS_ID);
			item->setData(node.GetString(KEY_RIGHT), QueueTrackDelegate::QUEUE_TRACKS_TITLE);
			item->setData(node.GetString(KEY_LEFT), QueueTrackDelegate::QUEUE_TRACKS_ARTIST);
			item->setData(index, QueueTrackDelegate::QUEUE_TRACKS_INDEX);
			item->setData(false, QueueTrackDelegate::QUEUE_TRACKS_PLAY_STATUS);

			m_Model->appendRow(item);

			totalTime += seconds;
			index++;
		}
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

void QueueTrack::SetRadioInfo(int min, int max, int step)
{
	m_FreqMax = max;
	m_FreqMin = min;
	m_FreqStep = step;
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

	if (!m_Delegate->GetSrc().compare(SRC_MUSIC_DB))
	{
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
	}

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
	int totTime = SetNodeList(list, m_Delegate->GetSrc());

	emit SigUpdateTimeStamp(m_TimeStamp, list.count(), totTime);
}

void QueueTrack::SlotRespDeleteQueue(CJsonNode node)
{
	m_TimeStamp = node.GetInt64(KEY_TIME_STAMP);
	m_pMgr->RequestQueueList(m_TimeStamp);
}

void QueueTrack::SlotSelectMenu(const QModelIndex &modelIndex, QPoint point)
{
	m_SelPoint = point;
	m_SelIndex = qvariant_cast<int>(modelIndex.data(QueueTrackDelegate::QUEUE_TRACKS_INDEX));

	if (!m_Delegate->GetSrc().compare(SRC_MUSIC_DB))
	{
		int id = qvariant_cast<int>(modelIndex.data(QueueTrackDelegate::QUEUE_TRACKS_ID));
//		LogDebug("id [%d] x [%d] y [%d]", id, m_SelPoint.x(), m_SelPoint.y());
		m_pMgr->RequestTrackInfo(id);
	}
	else if (!m_Delegate->GetSrc().compare(SRC_FM_RADIO))
	{
		QMap<int, QString>	menuMap;
		menuMap.insert(OPTION_MENU_DELETE, STR_DELETE);
		menuMap.insert(OPTION_MENU_EDIT, STR_EDIT);
		menuMap.insert(OPTION_MENU_SETUP_RESERVED_RECORD, STR_SETUP_RESERVE_RECORD);

		ClearOptionMenu();
		SetOptionMenu(menuMap);
	}
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
	case OPTION_MENU_DELETE:
		DoOptionMenuDelete();
		break;
	case OPTION_MENU_EDIT:
		DoOptionMenuEdit();
		break;
	case OPTION_MENU_SETUP_RESERVED_RECORD:
		DoOptionMenuSetupReservedRecord();
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

	m_FreqMax = 0;
	m_FreqMin = 0;
	m_FreqStep = 0;

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

void QueueTrack::DoOptionMenuDelete()
{
	QModelIndex modelIndex = m_Model->index(m_SelIndex, 0);
	int index = qvariant_cast<int>(modelIndex.data(QueueTrackDelegate::QUEUE_TRACKS_INDEX));

	QMap<int, bool> map;
	map.insert(index, true);

	m_pMgr->RequestRadioDelete(map);
}

void QueueTrack::DoOptionMenuEdit()
{
	CJsonNode tempNode = m_NodeList.at(m_SelIndex);
//			LogDebug("node [%s]", node.ToCompactByteArray().data());

	AddRadioDialog dialog;
	dialog.SetTitle(STR_EDIT);
	dialog.SetName(tempNode.GetString(KEY_RIGHT));
	dialog.SetFrequency((tempNode.GetString(KEY_LEFT).toDouble() * 1000));
	dialog.SetRange(m_FreqMin/100.0, m_FreqMax/100.0, m_FreqStep/100.0);

	if (dialog.exec() == QDialog::Accepted)
	{
		QString name = dialog.GetName();
		double freq = dialog.GetFrequency();
		m_pMgr->RequestRadioSet(name, freq, m_SelIndex);
	}
}

void QueueTrack::DoOptionMenuSetupReservedRecord()
{
	CJsonNode tempNode = m_NodeList.at(m_SelIndex);

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_TOP, tempNode.GetString(KEY_RIGHT));
	node.Add(KEY_ACTIVE, false);
	node.AddInt(KEY_DURATION, 5);

	SetupReservationRecordingDialog dialog;
	dialog.SetNodeData(node);

	if (dialog.exec() == QDialog::Accepted)
	{
		node.Clear();
		node = dialog.GetNodeData();
		node.AddInt(KEY_FREQ, tempNode.GetString(KEY_LEFT).toDouble() * 100);
		node.AddInt(KEY_EVENT_ID, m_EventID);

		m_pMgr->RequestRadioRecordSet(node);
	}
}
