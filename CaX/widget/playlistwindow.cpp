#include "playlistwindow.h"
#include "ui_playlistwindow.h"

#include "base/infotracks.h"
#include "base/icontracks.h"
#include "base/icontracksdelegate.h"
#include "base/listtracks.h"
#include "base/listtracksdelegate.h"

#include "manager/playlistmanager.h"
#include "manager/sqlmanager.h"

#include "util/caxconstants.h"
#include "util/loading.h"
#include "util/log.h"

PlaylistWindow::PlaylistWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new PlaylistManager),
	m_pInfoTracks(new InfoTracks(this)),
	m_pIconTracks(new IconTracks(this)),
	m_pListTracks(new ListTracks(this)),
	m_pLoading(new Loading(this)),
	ui(new Ui::PlaylistWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
}

PlaylistWindow::~PlaylistWindow()
{
	delete ui;

	if (m_pMgr)
	{
		delete m_pMgr;
		m_pMgr = nullptr;
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

	if (m_pLoading)
	{
		delete m_pLoading;
		m_pLoading = nullptr;
	}
}

void PlaylistWindow::PlayList()
{
	ui->gridLayoutTop->addWidget(m_pInfoTracks);
	ui->gridLayoutBottom->addWidget(m_pIconTracks);

	m_pMgr->RequestPlaylist();
}

void PlaylistWindow::SlotRespPlaylist(QList<CJsonNode> list)
{
	m_pIconTracks->SetNodeList(list, IconTracks::ICON_TRACKS_PLAYLIST);

}

void PlaylistWindow::SlotReqCoverArt(int nID, int nIndex)
{
	QString strCat = "Pls";
	QStringList lsAddr = m_pMgr->GetAddr().split(":");
	QString fullpath = QString("%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(strCat).arg(nID);

	m_pMgr->RequestCoverArt(fullpath, nIndex, QListView::IconMode);
}

void PlaylistWindow::SlotCoverArtUpdate(QString fileName, int nIndex, int mode)
{
	if (QListView::IconMode == mode)
	{
		QStandardItem *item = m_pIconTracks->GetModel()->item(nIndex);
		item->setData(fileName, IconTracksDelegate::ICON_TRACKS_COVER);
		m_pIconTracks->GetModel()->setItem(nIndex, item);
	}
//	else
//	{
//		QStandardItem *item = m_pListTracks->GetModel()->item(nIndex);
//		item->setData(fileName, ListTracksDelegate::LIST_TRACKS_COVER);
//		m_pListTracks->GetModel()->setItem(nIndex, item);
//	}
}

//void PlaylistWindow::SlotRespError(QString errMsg)
//{
//	QMessageBox::warning(this, "Warning", errMsg);
//}

void PlaylistWindow::ConnectSigToSlot()
{
	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespPlaylist(QList<CJsonNode>)), this, SLOT(SlotRespPlaylist(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));

	connect(m_pIconTracks, SIGNAL(SigReqCoverArt(int, int)), this, SLOT(SlotReqCoverArt(int, int)));

}
