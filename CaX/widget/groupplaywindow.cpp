#include <QMessageBox>

#include "groupplaywindow.h"
#include "ui_groupplaywindow.h"

#include "manager/groupplaymanager.h"

#include "base/infoservice.h"
#include "base/iconservice.h"
#include "base/iconservicedelegate.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/loading.h"
#include "util/log.h"

GroupPlayWindow::GroupPlayWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new GroupPlayManager),
	m_pInfoService(new InfoService(this)),
	m_pIconService(new IconService(this)),
	m_pLoading(new Loading(this)),
	ui(new Ui::GroupPlayWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
}

GroupPlayWindow::~GroupPlayWindow()
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

	if (m_pIconService)
	{
		delete m_pIconService;
		m_pIconService = nullptr;
	}

	if (m_pLoading)
	{
		delete m_pLoading;
		m_pLoading = nullptr;
	}

}

void GroupPlayWindow::GroupPlayList(int eventID)
{
	m_nEventID = eventID;
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pIconService);

	m_pInfoService->SetTitle(tr("Group play"));

	m_pMgr->RequestGroupPlayList(m_nEventID);
}

void GroupPlayWindow::SlotRespError(QString message)
{
	QMessageBox::warning(this, "Warning", message);
}

void GroupPlayWindow::SlotRespGroupPlayList(QList<CJsonNode> list)
{
	m_pIconService->SetNodeList(list, IconService::ICON_SERVICE_GROUP_PLAY);
}

void GroupPlayWindow::SlotReqCoverArt(int id, int index, int mode)
{
	QString strCat;
	if (QListView::IconMode == mode)
	{
		strCat = KEY_SONG;
	}
	else
	{
		strCat = KEY_SONG;
	}

	QStringList lsAddr = m_pMgr->GetAddr().split(":");
	QString fullpath = QString("%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(strCat).arg(id);

	m_pMgr->RequestCoverArt(fullpath, index, mode);
}

void GroupPlayWindow::SlotCoverArtUpdate(QString coverArt, int index, int mode)
{
	if (QListView::IconMode == mode)
	{
		QStandardItem *item = m_pIconService->GetModel()->item(index);
		item->setData(coverArt, IconServiceDelegate::ICON_SERVICE_COVER);
		m_pIconService->GetModel()->setItem(index, item);
	}
//	else
//	{
//		QStandardItem *item = m_pListTracks->GetModel()->item(index);
//		item->setData(coverArt, ListTracksDelegate::LIST_TRACKS_COVER);
//		m_pListTracks->GetModel()->setItem(index, item);
	//	}
}

void GroupPlayWindow::SlotSelectCoverArt(int type, QString rawData)
{
	Q_UNUSED(type)

	CJsonNode node;
	if (!node.SetContent(rawData))
	{
		LogCritical("invalid json");
		return;
	}

	// temp_code, dylee
	LogDebug("node [%s]", node.ToCompactByteArray().data());
	bool enabled = node.GetBool(KEY_ENABLED);
	m_pMgr->RequestGroupPlayEnable(!enabled);
}

void GroupPlayWindow::SlotRespGroupPlayUpdate()
{
	m_pMgr->RequestGroupPlayList(m_nEventID);
}

void GroupPlayWindow::ConnectSigToSlot()
{
	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespGroupPlayList(QList<CJsonNode>)), this, SLOT(SlotRespGroupPlayList(QList<CJsonNode>)));

	connect(m_pIconService, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));

	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectCoverArt(int, QString)), this, SLOT(SlotSelectCoverArt(int, QString)));


}
