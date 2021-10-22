#include "browserwindow.h"
#include "ui_browserwindow.h"

#include "browser.h"

#include "manager/browsermanager.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

#include "widget/form/formplay.h"
#include "widget/form/formsort.h"

#include "widget/formTop/infoservice.h"
#include "widget/formBottom/iconservice.h"
#include "widget/formBottom/iconservicedelegate.h"
#include "widget/formBottom/listservice.h"
#include "widget/formBottom/listservicedelegate.h"

#include "dialog/logindialog.h"

#define BROWSER_TITLE		"Browser"

BrowserWindow::BrowserWindow(QWidget *parent, const QString &addr, const QString &root) :
	QWidget(parent),
	m_pMgr(new BrowserManager),
	m_pInfoService(new InfoService(this)),
	m_pIconService(new IconService(this)),
	m_pListService(new ListService(this)),
	ui(new Ui::BrowserWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);
	m_Root = root;
	m_Dirs.clear();
	m_Files.clear();

	ConnectSigToSlot();

	m_pInfoService->GetFormPlay()->ShowPlayTopMenu();
	m_pInfoService->GetFormSort()->ShowResize();

}

BrowserWindow::~BrowserWindow()
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

	if (m_pListService)
	{
		delete m_pListService;
		m_pListService = nullptr;
	}

}

void BrowserWindow::RequestRoot()
{
//	ui->gridLayoutTop->addWidget(m_pInfoService);
//	ui->gridLayoutBottom->addWidget(m_pIconService);

	m_pMgr->RequestRoot();

}

void BrowserWindow::RequestFolder(QString strPath)
{
//	ui->gridLayoutTop->addWidget(m_pInfoService);
//	ui->gridLayoutBottom->addWidget(m_pIconService);

	m_pMgr->RequestFolder(strPath);

}

void BrowserWindow::SlotAddWidget(QWidget *widget, QString title)
{
	emit SigAddWidget(widget, title);		// recursive
}

void BrowserWindow::SlotPlayAll()
{
	m_pMgr->RequestTrackPlay(m_Root, m_Dirs, m_Files);
}

void BrowserWindow::SlotPlayRandom()
{
	m_pMgr->RequestRandom();
}

void BrowserWindow::SlotPlayTopMenu()
{
	QMap<int, bool> map = m_pIconService->GetSelectMap();
	if (map.count() > 0)
	{
//		SetSelectOnTopMenu();
	}
	else
	{
//		SetSelectOffTopMenu();
	}
	// for debug
	QMap<int, bool>::iterator i;
	for (i = map.begin(); i!= map.end(); i++)
	{
		LogDebug("key [%d] value [%d]", i.key(), i.value());
	}

	QMap<int, bool> map2 = m_pListService->GetSelectMap();
	if (map2.count() > 0)
	{
//		SetSelectOnTopMenu();
	}
	else
	{
//		SetSelectOffTopMenu();
	}
	// for debug
	QMap<int, bool>::iterator i2;
	for (i2 = map2.begin(); i2!= map2.end(); i2++)
	{
		LogDebug("key2 [%d] value2 [%d]", i2.key(), i2.value());
	}
}

void BrowserWindow::SlotTopMenuAction(int menuID)
{
	LogDebug("click top menu [%d]", menuID);

}

void BrowserWindow::SlotResize()
{
	LogDebug("click resize");

}

void BrowserWindow::SlotSelectTitle(int nType, QString rawData)
{
	LogDebug("click title para 2 nType [%d]", nType);

	CJsonNode node;
	if (!node.SetContent(rawData))
	{
		LogCritical("invalid json");
		return;
	}

	if ( iFolderType_Mask_Dir & nType
		 && iFolderType_Mask_Sub & nType )
	{
		LogDebug("iFolderType_Mask_Sub");
		QString strPath = node.GetString(KEY_PATH);
		if (!m_Root.isEmpty())
			strPath = m_Root + "/" + strPath;

		BrowserWindow *widget = new BrowserWindow(this, m_pMgr->GetAddr(), strPath);
		emit SigAddWidget(widget, tr("Browser"));

		if (iFolderType_Mask_Play_Top & nType)
		{
			widget->ShowFormPlay();
		}
		widget->RequestFolder(strPath);
	}
	else if (iFolderType_Mask_Song & nType
			 && (iFolderType_Mask_Play_Top & nType
			 || iFolderType_Mask_Play_Option & nType
			 || iFolderType_Mask_Play_Check & nType
			 || iFolderType_Mask_Play_Select & nType ))
	{
		LogDebug("play...");

	}
}

void BrowserWindow::SlotSelectURL(QString rawData)
{
	CJsonNode node(JSON_OBJECT);
	if (!node.SetContent(rawData))
	{
		return;
	}

//	LogDebug("node [%s]", node.ToCompactByteArray().data());

//	QStringList dirs;
//	QStringList files;

//	files.append(node.GetString(KEY_PATH));
	m_pMgr->RequestTrackPlay(m_Root, m_Dirs, m_Files);
}

void BrowserWindow::SlotRespList(QList<CJsonNode> list)
{
	CJsonNode node = list[0];
	int nType = node.GetInt(KEY_TYPE);

	if ( iFolderType_Mask_Root & nType
		 || iFolderType_Mask_Dir & nType)
	{
		ui->gridLayoutTop->addWidget(m_pInfoService);
		ui->gridLayoutBottom->addWidget(m_pIconService);

		SetList(list);

		if (m_Root.isEmpty())
		{
			m_pInfoService->SetSubtitle(tr("Browser"));
		}
		else
		{
			m_pInfoService->SetSubtitle(m_Root);
		}
		m_pIconService->SetNodeList(list, IconService::ICON_SERVICE_BROWSER);
	}
	// song
	else
	{
		ui->gridLayoutTop->addWidget(m_pInfoService);
		ui->gridLayoutBottom->addWidget(m_pListService);

		m_pInfoService->SetSubtitle(m_Root);
		m_pListService->SetNodeList(list, ListService::LIST_SERVICE_BROWSER);

		// get play time
	}



}

void BrowserWindow::SlotReqInfoBot(QString strPath, int nIndex)
{
	strPath = m_Root + "/" + strPath;
	m_pMgr->RequestInfoBot(strPath, nIndex);
}

void BrowserWindow::SlotRespNodeUpdate(CJsonNode node, int nIndex)
{
	if (nIndex < 0)
		return;

	QStandardItem *item = m_pListService->GetModel()->item(nIndex);
	item->setData(node.GetString(KEY_BOT), ListServiceDelegate::LIST_SERVICE_SUBTITLE);
	item->setData(node.GetString(KEY_DURATION), ListServiceDelegate::LIST_SERVICE_DURATION);
	m_pListService->GetModel()->setItem(nIndex, item);
}

void BrowserWindow::ConnectSigToSlot()
{
	connect(this, SIGNAL(SigAddWidget(QWidget*, QString)), parent(), SLOT(SlotAddWidget(QWidget*, QString)));		// recursive

//	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectPlay(int)), this, SLOT(SlotSelectPlay(int)));
//	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int)), this, SLOT(SlotSelectTitle(int)));
	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));

	connect(m_pListService->GetDelegate(), SIGNAL(SigSelectCoverArt(QString)), this, SLOT(SlotSelectURL(QString)));
	connect(m_pListService->GetDelegate(), SIGNAL(SigSelectTitle(QString)), this, SLOT(SlotSelectURL(QString)));
//	connect(m_pListService, SIGNAL(SigReqArt(QString, int)), this, SLOT(SlotReqArt(QString, int)));
	connect(m_pListService, SIGNAL(SigReqInfoBot(QString, int)), this, SLOT(SlotReqInfoBot(QString, int)));

	connect(m_pMgr, SIGNAL(SigRespList(QList<CJsonNode>)), this, SLOT(SlotRespList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespNodeUpdate(CJsonNode, int)), this, SLOT(SlotRespNodeUpdate(CJsonNode, int)));

	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigPlayAll()), this, SLOT(SlotPlayAll()));
	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigPlayRandom()), this, SLOT(SlotPlayRandom()));
	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigPlayTopMenu()), this, SLOT(SlotPlayTopMenu()));
	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigTopMenuAction(int)), this, SLOT(SlotTopMenuAction(int)));
	connect(m_pInfoService->GetFormSort(), SIGNAL(SigResize()), this, SLOT(SlotResize()));

}

void BrowserWindow::SetList(QList<CJsonNode> &list)
{
	QList<CJsonNode> tempList;
	int index = 0;
	QString strCover = "";

	foreach (CJsonNode node, list)
	{
		LogDebug("node [%s]", node.ToCompactByteArray().data());
		AnalyzeNode(node);
		strCover = GetCoverArtIcon(node);
		node.Add(KEY_COVER_ART, strCover);
//		node.AddInt(KEY_ID_UPPER, index);
//		node.AddInt(KEY_TYPE, index);

		tempList.append(node);
		index++;
	}
	list = tempList;
}

void BrowserWindow::AnalyzeNode(CJsonNode node)
{
	int type = node.GetInt(KEY_TYPE);
	QString path = node.GetString(KEY_PATH);

	if (type & iFolderType_Mask_Dir)
	{
		m_Dirs.append(path);
	}
	else if (type & iFolderType_Mask_Song)
	{
		m_Files.append(path);
	}
}

QString BrowserWindow::GetCoverArtIcon(CJsonNode node)
{
	QString path = node.GetString(KEY_PATH);
	QString strCover;

	if (!path.compare("HDD1"))
	{
		strCover = ":/resource/browser-img160-hdd-n@3x.png";
	}
	else if (!path.compare("NET"))
	{
		strCover = ":/resource/browser-img160-net-n@3x.png";
	}
	else if (!path.compare("UPnP"))
	{
		strCover = ":/resource/browser-img160-upnp-n@3x.png";
	}
	else
	{
		strCover = ":/resource/browser-img160-brank-n@3x.png";
	}

	return strCover;
}

void BrowserWindow::ShowFormPlay()
{
	m_pInfoService->GetFormPlay()->ShowPlayAll();
	m_pInfoService->GetFormPlay()->ShowPlayRandom();
}
