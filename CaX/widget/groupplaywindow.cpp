#include "groupplaywindow.h"
#include "ui_groupplaywindow.h"

#include "dialog/commondialog.h"

#include "manager/groupplaymanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"

#include "widget/form/formsort.h"

#include "widget/formTop/infoservice.h"
#include "widget/formBottom/iconservice.h"
#include "widget/formBottom/iconservicedelegate.h"

GroupPlayWindow::GroupPlayWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new GroupPlayManager),
	m_pInfoService(new InfoService(this)),
	m_pIconService(new IconService(this)),
	ui(new Ui::GroupPlayWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();

//	m_pInfoService->GetFormSort()->ShowResize();

}

GroupPlayWindow::~GroupPlayWindow()
{
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

	delete ui;

}

void GroupPlayWindow::GroupPlayList(int eventID)
{
	m_EventID = eventID;
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pIconService);

	m_pInfoService->SetSubtitle(STR_GROUP_PLAY);

	m_pMgr->RequestGroupPlayList(m_EventID);
}

void GroupPlayWindow::SlotRespError(QString errMsg)
{
	CommonDialog dialog(this, STR_WARNING, errMsg);
	dialog.exec();
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

void GroupPlayWindow::SlotSelectPlay(int index, bool muted)
{
	LogDebug("good choice!~!~");
	muted = !muted;

	// server / client ??
	m_pMgr->RequestGroupPlayMute(muted);

	QStandardItem *item = m_pIconService->GetModel()->item(index);
	item->setData(muted, IconServiceDelegate::ICON_SERVICE_MUTE);
	m_pIconService->GetModel()->setItem(index, item);
}

void GroupPlayWindow::SlotSelectTitle(int type, QString rawData)
{
	Q_UNUSED(type)

	CJsonNode node;
	if (!node.SetContent(rawData))
	{
		SlotRespError(STR_INVALID_JSON);
		return;
	}

//	// todo-dylee
//	LogDebug("node [%s]", node.ToCompactByteArray().data());
//	bool enabled = node.GetBool(KEY_ENABLED);
//	m_pMgr->RequestGroupPlayEnable(!enabled);
}

void GroupPlayWindow::SlotEventGroupPlayUpdate()
{
	m_pMgr->RequestGroupPlayList(m_EventID);
}

//void GroupPlayWindow::SlotResize()
//{
//	LogDebug("click resize");
//}

void GroupPlayWindow::ConnectSigToSlot()
{
	connect(m_pIconService, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));

	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectPlay(int, bool)), this, SLOT(SlotSelectPlay(int, bool)));
//	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int)), this, SLOT(SlotSelectTitle(int)));
	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));

	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespGroupPlayList(QList<CJsonNode>)), this, SLOT(SlotRespGroupPlayList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));

//	connect(m_pInfoService->GetFormSort(), SIGNAL(SigResize()), this, SLOT(SlotResize()));


}
