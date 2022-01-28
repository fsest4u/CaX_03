#include "groupplaywindow.h"
#include "ui_groupplaywindow.h"

#include "dialog/commondialog.h"
#include "dialog/groupplaydialog.h"
#include "dialog/poweroffdialog.h"

#include "manager/groupplaymanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"

#include "widget/form/formsort.h"

#include "widget/formTop/infoservice.h"
#include "widget/formBottom/iconservice.h"
#include "widget/formBottom/iconservicedelegate.h"

GroupPlayWindow::GroupPlayWindow(QWidget *parent, const QString &addr, const int &eventID) :
	QWidget(parent),
	m_pMgr(new GroupPlayManager),
	m_pInfoService(new InfoService(this)),
	m_pIconService(new IconService(this)),
	m_Dialog(new GroupPlayDialog(this)),
	m_EventID(eventID),
	ui(new Ui::GroupPlayWindow)
{
	ui->setupUi(this);

	m_Addr = addr;
	m_pMgr->SetAddr(m_Addr);

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

	if (m_Dialog)
	{
		delete m_Dialog;
		m_Dialog = nullptr;
	}

	delete ui;

}

void GroupPlayWindow::GroupPlayList()
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pIconService);

	m_pInfoService->SetTitle(STR_GROUP_PLAY);

	m_pMgr->SetAddr(m_Addr);
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
		QModelIndex modelIndex = m_pIconService->GetModel()->index(index, 0);
		m_pIconService->GetModel()->setData(modelIndex, coverArt, IconServiceDelegate::ICON_SERVICE_COVER);
	}
//	else
//	{
//		QModelIndex modelIndex = m_pListTracks->GetModel()->index(index, 0);
//		m_pListTracks->GetModel()->setData(modelIndex, coverArt, ListTracksDelegate::LIST_TRACKS_COVER);
//	}
}

void GroupPlayWindow::SlotSelectPlay(int index, bool muted)
{
	LogDebug("good choice!~!~");
	muted = !muted;

	CJsonNode node = m_pIconService->GetNodeList().at(index);
	QString addr = node.GetString(KEY_LOCATION);
	m_pMgr->SetAddr(addr);
	m_pMgr->RequestMute(muted, m_EventID);

	QModelIndex modelIndex = m_pIconService->GetModel()->index(index, 0);
	m_pIconService->GetModel()->setData(modelIndex, muted, IconServiceDelegate::ICON_SERVICE_MUTE);
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

	LogDebug("node [%s]", node.ToCompactByteArray().data());

	if (!node.GetString(KEY_LOCATION).compare(m_Addr))
	{
		m_Dialog->SetSelf(true);
	}
	else
	{
		m_Dialog->SetSelf(false);
	}
	m_Dialog->SetData(node);
	m_Dialog->exec();

}

void GroupPlayWindow::SlotEventGroupPlayUpdate()
{
	m_pMgr->SetAddr(m_Addr);
	m_pMgr->RequestGroupPlayList(m_EventID);
}

void GroupPlayWindow::SlotGroupPlay(QString addr, bool enabled)
{
	LogDebug("group play enable [%d]", enabled);
	m_pMgr->SetAddr(addr);
	m_pMgr->RequestGroupPlay(enabled, m_EventID);
}

void GroupPlayWindow::SlotAutoJoin(QString addr, bool enabled)
{
	LogDebug("auto join enable [%d]", enabled);
	m_pMgr->SetAddr(addr);
	m_pMgr->RequestAutoJoin(enabled, m_EventID);
}

void GroupPlayWindow::SlotMute(QString addr, bool enabled)
{
	LogDebug("mute enable [%d]", enabled);
	m_pMgr->SetAddr(addr);
	m_pMgr->RequestMute(enabled, m_EventID);
}

void GroupPlayWindow::SlotPlayStop(QString addr, bool enabled)
{
	m_pMgr->SetAddr(addr);
	m_pMgr->RequestPlayStop(m_EventID);
}

void GroupPlayWindow::SlotPowerOff(QString addr, bool self)
{
	LogDebug("power off self [%d]", self);
//	m_pMgr->SetAddr(addr);

	QList<CJsonNode> nodeList = m_pIconService->GetNodeList();

	if (self)	// self node
	{
		// remove self node
		nodeList.removeFirst();
		emit SigPowerOff(nodeList);
	}
	else	// dev node
	{
		CJsonNode devNode;
		foreach (CJsonNode node, nodeList)
		{
			QString tempAddr = node.GetString(KEY_LOCATION);
			if (!tempAddr.compare(addr))
			{
				devNode = node;
				break;
			}
		}
		nodeList.clear();
		nodeList.append(devNode);

		emit SigPowerOff(nodeList);
	}

}

void GroupPlayWindow::SlotVolumeSliderReleased(QString addr, int value)
{
	LogDebug("volume [%d]", value);
	m_pMgr->SetAddr(addr);
	m_pMgr->RequestVolume(value, m_EventID);
}

void GroupPlayWindow::SlotChangedChannel(QString addr, int value)
{
	LogDebug("channel [%d]", value);
	m_pMgr->SetAddr(addr);
	m_pMgr->RequestChannel(value, m_EventID);

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

	connect(m_Dialog, SIGNAL(SigGroupPlay(QString, bool)), this, SLOT(SlotGroupPlay(QString, bool)));
	connect(m_Dialog, SIGNAL(SigAutoJoin(QString, bool)), this, SLOT(SlotAutoJoin(QString, bool)));
	connect(m_Dialog, SIGNAL(SigMute(QString, bool)), this, SLOT(SlotMute(QString, bool)));
	connect(m_Dialog, SIGNAL(SigPlayStop(QString, bool)), this, SLOT(SlotPlayStop(QString, bool)));
	connect(m_Dialog, SIGNAL(SigPowerOff(QString, bool)), this, SLOT(SlotPowerOff(QString, bool)));

	connect(m_Dialog, SIGNAL(SigVolumeSliderReleased(QString, int)), this, SLOT(SlotVolumeSliderReleased(QString, int)));
	connect(m_Dialog, SIGNAL(SigChangedChannel(QString, int)), this, SLOT(SlotChangedChannel(QString, int)));

//	connect(m_pInfoService->GetFormSort(), SIGNAL(SigResize()), this, SLOT(SlotResize()));

}
