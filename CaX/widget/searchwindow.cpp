#include "searchwindow.h"
#include "ui_searchwindow.h"

#include "dialog/commondialog.h"

#include "manager/searchmanager.h"
#include "manager/sqlmanager.h"

#include "widget/musicdbwindow.h"
#include "widget/formSearch/searchtrack.h"
#include "widget/formSearch/searchtrackdelegate.h"
#include "widget/formSearch/searchcategory.h"
#include "widget/formSearch/searchcategorydelegate.h"

SearchWindow::SearchWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new SearchManager),
	m_Album(new SearchCategory(this)),
	m_Artist(new SearchCategory(this)),
	m_Track(new SearchTrack(this)),
	ui(new Ui::SearchWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
	Initialize();
}

SearchWindow::~SearchWindow()
{
	if (m_pMgr)
	{
		delete m_pMgr;
		m_pMgr = nullptr;
	}

	if (m_Album)
	{
		delete m_Album;
		m_Album = nullptr;
	}

	if (m_Artist)
	{
		delete m_Artist;
		m_Artist = nullptr;
	}

	if (m_Track)
	{
		delete m_Track;
		m_Track = nullptr;
	}

	delete ui;
}

void SearchWindow::SetKeyword(const QString keyword)
{
	QString title = QString("Search results for \"%1\"").arg(keyword);
	ui->labelTitle->setText(title);

	m_pMgr->RequestSearchAlbumList(keyword);
	m_pMgr->RequestSearchArtistList(keyword);
	m_pMgr->RequestSearchTrackList(keyword);
}

void SearchWindow::SlotAddWidget(QWidget *widget, QString title)
{
	emit SigAddWidget(widget, STR_SEARCH);	// recursive

}

void SearchWindow::SlotRemoveWidget(QWidget *widget)
{
	emit SigRemoveWidget(widget);

}

void SearchWindow::SlotRespError(QString errMsg)
{
//	CommonDialog dialog(this, STR_WARNING, errMsg);
//	dialog.exec();
	LogWarning("error [%s]", errMsg.toUtf8().data());
}

void SearchWindow::SlotRespAlbumList(QList<CJsonNode> list)
{
	ui->gridLayoutAlbum->addWidget(m_Album);

	m_Album->SetTitle(m_pMgr->GetSqlMgr()->GetCategoryName(SQLManager::CATEGORY_ALBUM));
	m_Album->ClearNodeList();
	m_Album->SetNodeList(list, SQLManager::CATEGORY_ALBUM);
}

void SearchWindow::SlotRespArtistList(QList<CJsonNode> list)
{
	ui->gridLayoutArtist->addWidget(m_Artist);

	m_Artist->SetTitle(m_pMgr->GetSqlMgr()->GetCategoryName(SQLManager::CATEGORY_ARTIST));
	m_Artist->ClearNodeList();
	m_Artist->SetNodeList(list, SQLManager::CATEGORY_ARTIST);
}

void SearchWindow::SlotRespTrackList(QList<CJsonNode> list)
{
	ui->gridLayoutTrack->addWidget(m_Track);

	SetOptionMenu();

	m_Track->SetTitle(STR_TRACK);
	m_Track->ClearNodeList();
	m_Track->SetNodeList(list, SQLManager::CATEGORY_TRACK);
}

void SearchWindow::SlotReqCoverArt(int id, int index, int category)
{
	QString strCat = m_pMgr->GetSqlMgr()->GetCategoryName(category);

	QStringList lsAddr = m_pMgr->GetAddr().split(":");
	QString fullpath = QString("%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(strCat).arg(id);

	LogDebug("path [%s] index [%d]", fullpath.toUtf8().data(), index);
	m_pMgr->RequestCoverArt(fullpath, index, category);
}

void SearchWindow::SlotCoverArtUpdate(QString fileName, int nIndex, int category)
{
	if (category == SQLManager::CATEGORY_ALBUM)
	{
		QStandardItem *itemIcon = m_Album->GetModel()->item(nIndex);
		itemIcon->setData(fileName, SearchCategoryDelegate::SEARCH_CATEGORY_COVER);
		m_Album->GetModel()->setItem(nIndex, itemIcon);
	}
	else if (category == SQLManager::CATEGORY_ARTIST)
	{
		QStandardItem *itemIcon = m_Artist->GetModel()->item(nIndex);
		itemIcon->setData(fileName, SearchCategoryDelegate::SEARCH_CATEGORY_COVER);
		m_Artist->GetModel()->setItem(nIndex, itemIcon);
	}
	else if (category == SQLManager::CATEGORY_TRACK)
	{
		QStandardItem *itemList = m_Track->GetModel()->item(nIndex);
		itemList->setData(fileName, SearchTrackDelegate::SEARCH_TRACKS_COVER);
		m_Track->GetModel()->setItem(nIndex, itemList);
	}
}

void SearchWindow::SlotSelectAlbum(int id, QString coverArt)
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), -1);
	widget->AddWidgetTrack();
	emit widget->SigAddWidget(widget, STR_MUSIC_DB);

	widget->RequestTrackList(id, SQLManager::CATEGORY_ALBUM);
	widget->SetCoverArt(coverArt);
}

void SearchWindow::SlotSelectArtist(int id, QString coverArt)
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), -1);
	widget->AddWidgetTrack();
	emit widget->SigAddWidget(widget, STR_MUSIC_DB);

	widget->RequestTrackList(id, SQLManager::CATEGORY_ARTIST);
	widget->SetCoverArt(coverArt);
}

void SearchWindow::SlotSelectPlay(int id, int playType)
{
	QMap<int, bool> map;
	map.insert(id, true);
	m_pMgr->RequestPlayTrack(map, playType);
}

void SearchWindow::SlotOptionMenuAction(int id, int menuID, QString coverArt)
{
	switch (menuID) {
	case OPTION_MENU_GO_TO_ALBUM:
		DoOptionMenuGoToAlbum(id, coverArt);
		break;
	}
}

void SearchWindow::ConnectSigToSlot()
{
	connect(this, SIGNAL(SigAddWidget(QWidget*, QString)), parent(), SLOT(SlotAddWidget(QWidget*, QString)));
	connect(this, SIGNAL(SigRemoveWidget(QWidget*)), parent(), SLOT(SlotRemoveWidget(QWidget*)));

	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespAlbumList(QList<CJsonNode>)), this, SLOT(SlotRespAlbumList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespArtistList(QList<CJsonNode>)), this, SLOT(SlotRespArtistList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespTrackList(QList<CJsonNode>)), this, SLOT(SlotRespTrackList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));

	connect(m_Album, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));
	connect(m_Album->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectAlbum(int, QString)));

	connect(m_Artist, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));
	connect(m_Artist->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectArtist(int, QString)));

	connect(m_Track, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));
	connect(m_Track->GetDelegate(), SIGNAL(SigSelectPlay(int, int)), this, SLOT(SlotSelectPlay(int, int)));
//	connect(m_Track->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));
	connect(m_Track->GetDelegate(), SIGNAL(SigMenuAction(int, int, QString)), this, SLOT(SlotOptionMenuAction(int, int, QString)));

}

void SearchWindow::Initialize()
{

}

void SearchWindow::SetOptionMenu()
{
	m_OptionMenuMap.clear();
	m_OptionMenuMap.insert(OPTION_MENU_GO_TO_ALBUM, STR_GO_TO_ALBUM);

	m_Track->GetDelegate()->SetOptionMenuMap(m_OptionMenuMap);
}

void SearchWindow::DoOptionMenuGoToAlbum(int id, QString coverArt)
{
	LogDebug("id [%d]", id);
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), -1);
	widget->AddWidgetTrack();
	emit widget->SigAddWidget(widget, STR_MUSIC_DB);

	widget->RequestTrackList(id, SQLManager::CATEGORY_ALBUM);
	widget->SetCoverArt(coverArt);
}
