#include <QMessageBox>

#include "playlistwindow.h"
#include "ui_playlistwindow.h"

#include "widget/form/formplay.h"
#include "widget/form/formsort.h"

#include "widget/formTop/infoservice.h"
#include "widget/formTop/infotracks.h"
#include "widget/formBottom/icontracks.h"
#include "widget/formBottom/icontracksdelegate.h"
#include "widget/formBottom/listtracks.h"
#include "widget/formBottom/listtracksdelegate.h"

#include "manager/playlistmanager.h"
#include "manager/sqlmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"

PlaylistWindow::PlaylistWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new PlaylistManager),
	m_pInfoService(new InfoService(this)),
	m_pInfoTracks(new InfoTracks(this)),
	m_pIconTracks(new IconTracks(this)),
	m_pListTracks(new ListTracks(this)),
	ui(new Ui::PlaylistWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();

	m_pInfoService->GetFormPlay()->ShowPlayAll();
	m_pInfoService->GetFormPlay()->ShowPlayRandom();
	m_pInfoService->GetFormPlay()->ShowPlayTopMenu();
	m_pInfoService->GetFormSort()->ShowResize();
}

PlaylistWindow::~PlaylistWindow()
{
	delete ui;

	if (m_pMgr)
	{
		delete m_pMgr;
		m_pMgr = nullptr;
	}

	if (m_pInfoService)
	{
		delete m_pInfoService;
		m_pInfoService = nullptr;
	}

	if (m_pInfoTracks)
	{
		delete m_pInfoTracks;
		m_pInfoTracks = nullptr;
	}

	if (m_pIconTracks)
	{
		delete m_pIconTracks;
		m_pIconTracks = nullptr;
	}

	if (m_pListTracks)
	{
		delete m_pListTracks;
		m_pListTracks = nullptr;
	}

}

void PlaylistWindow::Playlist()
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pIconTracks);

	m_pInfoService->SetSubtitle(tr("Playlist"));

	m_pMgr->RequestPlaylist();
}

void PlaylistWindow::PlaylistInfo(int id, QString coverArt)
{
	ui->gridLayoutTop->addWidget(m_pInfoTracks);
	m_pInfoTracks->SetCoverArt(coverArt);
	m_pMgr->RequestPlaylistInfo(id);
}

void PlaylistWindow::TrackList(int id)
{
	ui->gridLayoutBottom->addWidget(m_pListTracks);
	m_pMgr->RequestTrackList(id);
}

void PlaylistWindow::TrackPlay(int id)
{
}

void PlaylistWindow::TracksPlay(QList<int> ids)
{
	m_pMgr->RequestTracksPlay(ids);
}

void PlaylistWindow::PlaylistPlay(int id)
{
	m_pMgr->RequestPlaylistPlay(id);
}

void PlaylistWindow::PlaylistsPlay(QList<int> ids)
{
}

void PlaylistWindow::SlotAddWidget(QWidget *widget, QString title)
{
	emit SigAddWidget(widget, title);		// recursive
}

void PlaylistWindow::SlotRespError(QString message)
{
	QMessageBox::warning(this, "Warning", message);
}

void PlaylistWindow::SlotRespPlaylist(QList<CJsonNode> list)
{
	m_IDs.clear();
	foreach (CJsonNode node, list)
	{
		m_IDs.append(node.GetString(KEY_ID_LOWER).toInt());
	}
	m_pIconTracks->SetNodeList(list, IconTracks::ICON_TRACKS_PLAYLIST);
}

void PlaylistWindow::SlotRespPlaylistInfo(CJsonNode node)
{
	LogDebug("playlist info [%s]", node.ToCompactByteArray().data());

	QString count = node.GetString(KEY_COUNT);
	QString total = node.GetString(KEY_TOTAL);
	QString extension = node.GetString(KEY_EXTENSION);
	QString samplerate = node.GetString(KEY_SAMPLERATE);
	QString bps = node.GetString(KEY_BPS);

	QString info = count + " | " + total + " | " + extension + " | " + samplerate + " | " + bps;

	m_pInfoTracks->SetTitle(node.GetString(KEY_TITLE));
	m_pInfoTracks->SetSubtitle("Artist : " + node.GetString(KEY_ARTIST));
//	m_pInfoTracks->SetInfo(info);
}

void PlaylistWindow::SlotRespTrackList(QList<CJsonNode> list)
{
	m_pListTracks->SetNodeList(list);
}

void PlaylistWindow::SlotReqCoverArt(int id, int index, int mode)
{
	QString strCat;
	if (QListView::IconMode == mode)
	{
		strCat = KEY_PLS;
	}
	else
	{
		strCat = KEY_SONG;
	}

	QStringList lsAddr = m_pMgr->GetAddr().split(":");
	QString fullpath = QString("%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(strCat).arg(id);

	m_pMgr->RequestCoverArt(fullpath, index, mode);
}

void PlaylistWindow::SlotCoverArtUpdate(QString coverArt, int index, int mode)
{
	if (QListView::IconMode == mode)
	{
		QStandardItem *item = m_pIconTracks->GetModel()->item(index);
		item->setData(coverArt, IconTracksDelegate::ICON_TRACKS_COVER);
		m_pIconTracks->GetModel()->setItem(index, item);
	}
	else
	{
		QStandardItem *item = m_pListTracks->GetModel()->item(index);
		item->setData(coverArt, ListTracksDelegate::LIST_TRACKS_COVER);
		m_pListTracks->GetModel()->setItem(index, item);
	}
}

void PlaylistWindow::SlotSelectTitle(int id, QString coverArt)
{
	PlaylistWindow *widget = new PlaylistWindow(this, m_pMgr->GetAddr());
	emit SigAddWidget(widget, tr("Playlist"));
	widget->PlaylistInfo(id, coverArt);
	widget->TrackList(id);
}

void PlaylistWindow::SlotSelectCount(int id)
{
	LogDebug("click Count");
}

void PlaylistWindow::SlotSelectPlay(int id)
{
	m_pMgr->RequestTrackPlay(id);
}

void PlaylistWindow::SlotPlayAll()
{

	m_pMgr->RequestPlaylistsPlay(m_IDs);

}

void PlaylistWindow::SlotPlayRandom()
{
	m_pMgr->RequestRandom();

}

void PlaylistWindow::SlotPlayTopMenu()
{
	QMap<int, bool> map = m_pIconTracks->GetSelectMap();
	if (map.count() > 0)
	{
		SetSelectOnTopMenu();
	}
	else
	{
		SetSelectOffTopMenu();
	}
	// for debug
	QMap<int, bool>::iterator i;
	for (i = map.begin(); i!= map.end(); i++)
	{
		LogDebug("key [%d] value [%d]", i.key(), i.value());
	}
}

void PlaylistWindow::SlotTopMenuAction(int menuID)
{
	LogDebug("click top menu [%d]", menuID);


}

void PlaylistWindow::SlotResize()
{
	LogDebug("click resize");

}

void PlaylistWindow::ConnectSigToSlot()
{
	connect(this, SIGNAL(SigAddWidget(QWidget*, QString)), parent(), SLOT(SlotAddWidget(QWidget*, QString)));		// recursive

	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));

	connect(m_pMgr, SIGNAL(SigRespPlaylist(QList<CJsonNode>)), this, SLOT(SlotRespPlaylist(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespPlaylistInfo(CJsonNode)), this, SLOT(SlotRespPlaylistInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespTrackList(QList<CJsonNode>)), this, SLOT(SlotRespTrackList(QList<CJsonNode>)));
	connect(m_pIconTracks, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));
	connect(m_pListTracks, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));

	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectCount(int)), this, SLOT(SlotSelectCoverArt(int, QString)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectSubtitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));
	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectPlay(int)), this, SLOT(SlotSelectPlay(int)));

	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigPlayAll()), this, SLOT(SlotPlayAll()));
	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigPlayRandom()), this, SLOT(SlotPlayRandom()));
	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigPlayTopMenu()), this, SLOT(SlotPlayTopMenu()));
	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigTopMenuAction(int)), this, SLOT(SlotTopMenuAction(int)));
	connect(m_pInfoService->GetFormSort(), SIGNAL(SigResize()), this, SLOT(SlotResize()));

}

void PlaylistWindow::SetSelectOffTopMenu(bool root)
{
	m_TopMenu.clear();

	if (root)
	{
		m_TopMenu.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
		m_TopMenu.insert(TOP_MENU_NEW_PLAYLIST, STR_NEW_PLAYLIST);
	}
	else
	{
		m_TopMenu.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_TopMenu.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_TopMenu.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_TopMenu.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_TopMenu.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
		m_TopMenu.insert(TOP_MENU_ADD_ITEM, STR_ADD_ITEM);
	}

	m_pInfoService->GetFormPlay()->ClearTopMenu();
	m_pInfoService->GetFormPlay()->SetTopMenu(m_TopMenu);
}

void PlaylistWindow::SetSelectOnTopMenu(bool root)
{
	m_TopMenu.clear();

	if (root)
	{
		m_TopMenu.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_TopMenu.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_TopMenu.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_TopMenu.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_TopMenu.insert(TOP_MENU_UNSELECT, STR_UNSELECT);
		m_TopMenu.insert(TOP_MENU_DELETE_ITEM, STR_DELETE_ITEM);
//		m_TopMenu.insert(TOP_MENU_EXPORT_TRACK, STR_EXPORT_TRACK);
	}
	else
	{
		m_TopMenu.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_TopMenu.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_TopMenu.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_TopMenu.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_TopMenu.insert(TOP_MENU_UNSELECT, STR_UNSELECT);
		m_TopMenu.insert(TOP_MENU_DELETE_ITEM, STR_DELETE_ITEM);
	}

	m_pInfoService->GetFormPlay()->ClearTopMenu();
	m_pInfoService->GetFormPlay()->SetTopMenu(m_TopMenu);
}
