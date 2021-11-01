#include <QMessageBox>

#include "playlistwindow.h"
#include "ui_playlistwindow.h"

#include "dialog/addplaylistdialog.h"

#include "manager/playlistmanager.h"
#include "manager/sqlmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/caxtranslate.h"
#include "util/log.h"

#include "widget/form/formplay.h"
#include "widget/form/formsort.h"

#include "widget/formTop/infoservice.h"
#include "widget/formTop/infotracks.h"
#include "widget/formBottom/icontracks.h"
#include "widget/formBottom/icontracksdelegate.h"
#include "widget/formBottom/listtracks.h"
#include "widget/formBottom/listtracksdelegate.h"

PlaylistWindow::PlaylistWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new PlaylistManager),
	m_pInfoService(new InfoService(this)),
	m_pInfoTracks(new InfoTracks(this)),
	m_pIconTracks(new IconTracks(this)),
	m_pListTracks(new ListTracks(this)),
	m_ID(-1),
	ui(new Ui::PlaylistWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
	Initialize();

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

void PlaylistWindow::AddWidgetPlaylistHome()
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pIconTracks);

	m_pInfoService->SetSubtitle(STR_PLAYLIST);
}

void PlaylistWindow::AddWidgetItemHome()
{
	ui->gridLayoutTop->addWidget(m_pInfoTracks);
	ui->gridLayoutBottom->addWidget(m_pListTracks);

}



void PlaylistWindow::RequestPlaylist()
{
	m_pMgr->RequestPlaylist();
}

void PlaylistWindow::RequestPlaylistInfo(int id, QString coverArt)
{
	m_ID = id;
	m_pInfoTracks->SetCoverArt(coverArt);
	m_pMgr->RequestPlaylistInfo(id);
}

void PlaylistWindow::RequestTrackList(int id)
{
	m_pMgr->RequestTrackList(id);
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
	m_pIconTracks->ClearNodeList();
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
	m_pInfoTracks->SetSubtitle(QString("%1 : %2").arg(STR_ARTIST).arg(node.GetString(KEY_ARTIST)));
//	m_pInfoTracks->SetInfo(info);
}

void PlaylistWindow::SlotRespTrackList(QList<CJsonNode> list)
{
	m_pListTracks->ClearNodeList();
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
	widget->AddWidgetItemHome();
	emit SigAddWidget(widget, STR_PLAYLIST);

	widget->RequestPlaylistInfo(id, coverArt);
	widget->RequestTrackList(id);
}

void PlaylistWindow::SlotSelectCount(int id)
{
	LogDebug("click Count");
}

void PlaylistWindow::SlotSelectPlay(int id, int playType)
{
	QMap<int, bool> map;
	map.insert(id, true);
	m_pMgr->RequestTrackPlay(map, playType);
}

void PlaylistWindow::SlotPlayAll()
{
	m_pIconTracks->SetAllSelectMap();
	m_SelectMap = m_pIconTracks->GetSelectMap();
	m_pMgr->RequestPlaylistPlay(m_SelectMap, PLAY_CLEAR);
}

void PlaylistWindow::SlotPlayRandom()
{
	m_pMgr->RequestRandom();

}

void PlaylistWindow::SlotTopMenu()
{
	m_SelectMap = m_pIconTracks->GetSelectMap();
	if (m_SelectMap.count() > 0)
	{
		SetSelectOnTopMenu(true);
	}
	else
	{
		SetSelectOffTopMenu(true);
	}
}

void PlaylistWindow::SlotTopMenuAction(int menuID)
{
	switch (menuID) {
	case TOP_MENU_PLAY_NOW:
		DoTopMenuPlay(PLAY_NOW);
		break;
	case TOP_MENU_PLAY_LAST:
		DoTopMenuPlay(PLAY_LAST);
		break;
	case TOP_MENU_PLAY_NEXT:
		DoTopMenuPlay(PLAY_NEXT);
		break;
	case TOP_MENU_PLAY_CLEAR:
		DoTopMenuPlay(PLAY_CLEAR);
		break;
	case TOP_MENU_SELECT_ALL:
		DoTopMenuSelectAll();
		break;
	case TOP_MENU_CLEAR_ALL:
		DoTopMenuClearAll();
		break;
	case TOP_MENU_ADD_ITEM:
		DoTopMenuAddItem();
		break;
	case TOP_MENU_DELETE_ITEM:
		DoTopMenuDeleteItem();
		break;
	}

}

void PlaylistWindow::SlotResize()
{
	LogDebug("click resize");

}

void PlaylistWindow::SlotItemPlayAll()
{
	m_pListTracks->SetAllSelectMap();
	m_SelectMap = m_pListTracks->GetSelectMap();
	m_pMgr->RequestTrackPlay(m_SelectMap, PLAY_CLEAR);
}

void PlaylistWindow::SlotItemPlayRandom()
{
	m_pMgr->RequestRandom();
}

void PlaylistWindow::SlotItemTopMenu()
{
	m_SelectMap = m_pListTracks->GetSelectMap();
	if (m_SelectMap.count() > 0)
	{
		SetSelectOnTopMenu();
	}
	else
	{
		SetSelectOffTopMenu();
	}
}

void PlaylistWindow::SlotItemTopMenuAction(int menuID)
{
	switch (menuID) {
	case TOP_MENU_PLAY_NOW:
		DoItemTopMenuPlay(PLAY_NOW);
		break;
	case TOP_MENU_PLAY_LAST:
		DoItemTopMenuPlay(PLAY_LAST);
		break;
	case TOP_MENU_PLAY_NEXT:
		DoItemTopMenuPlay(PLAY_NEXT);
		break;
	case TOP_MENU_PLAY_CLEAR:
		DoItemTopMenuPlay(PLAY_CLEAR);
		break;
	case TOP_MENU_SELECT_ALL:
		DoItemTopMenuSelectAll();
		break;
	case TOP_MENU_CLEAR_ALL:
		DoItemTopMenuClearAll();
		break;
	case TOP_MENU_ADD_ITEM:
		DoItemTopMenuAddItem();
		break;
	case TOP_MENU_DELETE_ITEM:
		DoItemTopMenuDeleteItem();
		break;
	}
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
	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectPlay(int, int)), this, SLOT(SlotSelectPlay(int, int)));

	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigPlayAll()), this, SLOT(SlotPlayAll()));
	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigPlayRandom()), this, SLOT(SlotPlayRandom()));
	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigMenu()), this, SLOT(SlotTopMenu()));
	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigMenuAction(int)), this, SLOT(SlotTopMenuAction(int)));
	connect(m_pInfoService->GetFormSort(), SIGNAL(SigResize()), this, SLOT(SlotResize()));

	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigPlayAll()), this, SLOT(SlotItemPlayAll()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigPlayRandom()), this, SLOT(SlotItemPlayRandom()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigMenu()), this, SLOT(SlotItemTopMenu()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigMenuAction(int)), this, SLOT(SlotItemTopMenuAction(int)));

}

void PlaylistWindow::Initialize()
{

	m_pInfoService->GetFormPlay()->ShowPlayAll();
	m_pInfoService->GetFormPlay()->ShowPlayRandom();
	m_pInfoService->GetFormPlay()->ShowMenu();
	m_pInfoService->GetFormSort()->ShowResize();

	m_pInfoTracks->GetFormPlay()->ShowPlayAll();
	m_pInfoTracks->GetFormPlay()->ShowPlayRandom();
	m_pInfoTracks->GetFormPlay()->ShowMenu();

	m_TopMenuMap.clear();
	m_SelectMap.clear();
}

void PlaylistWindow::SetSelectOffTopMenu(bool root)
{
	m_TopMenuMap.clear();

	if (root)
	{
		m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
		m_TopMenuMap.insert(TOP_MENU_NEW_PLAYLIST, STR_NEW_PLAYLIST);

		m_pInfoService->GetFormPlay()->ClearMenu();
		m_pInfoService->GetFormPlay()->SetMenu(m_TopMenuMap);
	}
	else
	{
		m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
//		m_TopMenuMap.insert(TOP_MENU_ADD_ITEM, STR_ADD_ITEM);	// todo-dylee

		m_pInfoTracks->GetFormPlay()->ClearMenu();
		m_pInfoTracks->GetFormPlay()->SetMenu(m_TopMenuMap);
	}

}

void PlaylistWindow::SetSelectOnTopMenu(bool root)
{
	m_TopMenuMap.clear();

	if (root)
	{
		m_TopMenuMap.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_TopMenuMap.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_TopMenuMap.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_TopMenuMap.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_TopMenuMap.insert(TOP_MENU_CLEAR_ALL, STR_SELECT_ALL);
		m_TopMenuMap.insert(TOP_MENU_DELETE_ITEM, STR_DELETE_ITEM);
//		m_TopMenuMap.insert(TOP_MENU_EXPORT_TRACK, STR_EXPORT_TRACK);

		m_pInfoService->GetFormPlay()->ClearMenu();
		m_pInfoService->GetFormPlay()->SetMenu(m_TopMenuMap);
	}
	else
	{
		m_TopMenuMap.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_TopMenuMap.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_TopMenuMap.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_TopMenuMap.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_TopMenuMap.insert(TOP_MENU_CLEAR_ALL, STR_SELECT_ALL);
		m_TopMenuMap.insert(TOP_MENU_DELETE_ITEM, STR_DELETE_ITEM);

		m_pInfoTracks->GetFormPlay()->ClearMenu();
		m_pInfoTracks->GetFormPlay()->SetMenu(m_TopMenuMap);
	}

}

void PlaylistWindow::DoTopMenuPlay(int nWhere)
{
	if (m_SelectMap.count() > 0)
	{
		m_pMgr->RequestPlaylistPlay(m_SelectMap, nWhere);
	}
}

void PlaylistWindow::DoTopMenuSelectAll()
{
	m_pIconTracks->SetAllSelectMap();
}

void PlaylistWindow::DoTopMenuClearAll()
{
	m_pIconTracks->ClearSelectMap();
}

void PlaylistWindow::DoTopMenuAddItem()
{
	AddPlaylistDialog dialog;
	if (dialog.exec() == QDialog::Accepted)
	{
		QString name = dialog.GetName();
		m_pMgr->RequestAddPlaylist(name);

		//refresh
		RequestPlaylist();
	}
}

void PlaylistWindow::DoTopMenuDeleteItem()
{
	if (m_SelectMap.count() > 0)
	{
		m_pMgr->RequestDeletePlaylist(m_SelectMap);

		//refresh
		RequestPlaylist();
	}
}

void PlaylistWindow::DoItemTopMenuPlay(int nWhere)
{
	if (m_SelectMap.count() > 0)
	{
		m_pMgr->RequestTrackPlay(m_SelectMap, nWhere);
	}

}

void PlaylistWindow::DoItemTopMenuSelectAll()
{
	m_pListTracks->SetAllSelectMap();

}

void PlaylistWindow::DoItemTopMenuClearAll()
{
	m_pListTracks->ClearSelectMap();

}

void PlaylistWindow::DoItemTopMenuAddItem()
{
	LogDebug("do item add ");

}

void PlaylistWindow::DoItemTopMenuDeleteItem()
{
	if (m_SelectMap.count() > 0)
	{
		m_pMgr->RequestDelTrack(m_ID, m_SelectMap);

		//refresh
		m_pMgr->RequestTrackList(m_ID);
	}
}
