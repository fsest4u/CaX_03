#include "browserwindow.h"
#include "ui_browserwindow.h"

#include "browser.h"

#include "dialog/inputnamedialog.h"
#include "dialog/logindialog.h"
#include "dialog/selectformatdialog.h"
#include "dialog/searchcoverartdialog.h"
#include "dialog/searchcoverartdialog.h"
#include "dialog/searchcoverartresultdialog.h"
#include "dialog/trackinfodialog.h"
#include "dialog/trackinfo.h"

#include "manager/browsermanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/caxtranslate.h"
#include "util/log.h"
#include "util/settingio.h"
#include "util/utilnovatron.h"

#include "widget/form/formplay.h"
#include "widget/form/formsort.h"

#include "widget/formTop/infoservice.h"
#include "widget/formTop/infobrowser.h"
#include "widget/formBottom/iconservice.h"
#include "widget/formBottom/iconservicedelegate.h"
#include "widget/formBottom/listbrowser.h"
#include "widget/formBottom/listbrowserdelegate.h"


const QString SETTINGS_GROUP = "Browser";

#define BROWSER_TITLE		"Browser"

BrowserWindow::BrowserWindow(QWidget *parent, const QString &addr, const int &eventID, const QString &root) :
	QWidget(parent),
	m_pMgr(new BrowserManager),
	m_pInfoService(new InfoService(this)),
	m_pInfoBrowser(new InfoBrowser(this)),
	m_pIconService(new IconService(this)),
	m_pListBrowser(new ListBrowser(this)),
	m_Root(root),
	m_EventID(eventID),
	ui(new Ui::BrowserWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();

	Initialize();

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

	if (m_pInfoBrowser)
	{
		delete m_pInfoBrowser;
		m_pInfoBrowser = nullptr;
	}

	if (m_pIconService)
	{
		delete m_pIconService;
		m_pIconService = nullptr;
	}

	if (m_pListBrowser)
	{
		delete m_pListBrowser;
		m_pListBrowser = nullptr;
	}

}

void BrowserWindow::RequestRoot()
{
//	ui->gridLayoutTop->addWidget(m_pInfoService);
//	ui->gridLayoutBottom->addWidget(m_pIconService);

	if (BROWSER_MODE_COPY == m_BrowserMode || BROWSER_MODE_MOVE == m_BrowserMode)
	{
		m_pMgr->RequestRoot(true);
	}
	else
	{
		m_pMgr->RequestRoot();
	}

}

void BrowserWindow::RequestFolder(QString strPath)
{
//	ui->gridLayoutTop->addWidget(m_pInfoService);
//	ui->gridLayoutBottom->addWidget(m_pIconService);

	if (BROWSER_MODE_COPY == m_BrowserMode || BROWSER_MODE_MOVE == m_BrowserMode)
	{
		m_pMgr->RequestFolder(strPath, true);
	}
	else
	{
		m_pMgr->RequestFolder(strPath);
	}
}

int BrowserWindow::GetFolderType() const
{
	return m_FolderType;
}

void BrowserWindow::SetFolderType(int FolderType)
{
	m_FolderType = FolderType;
}

int BrowserWindow::GetBrowserMode() const
{
	return m_BrowserMode;
}

void BrowserWindow::SetBrowserMode(int BrowserMode)
{
	m_BrowserMode = BrowserMode;
}

void BrowserWindow::SlotAddWidget(QWidget *widget, QString title)
{
	emit SigAddWidget(widget, title);		// recursive
}

void BrowserWindow::SlotRemoveWidget(QWidget *widget)
{
	emit SigRemoveWidget(widget);
}

void BrowserWindow::SlotPlayAll(int where)
{
	SetDirFile();

	m_pMgr->RequestTrackPlay(m_Root, m_Dirs, m_Files, where);
}

void BrowserWindow::SlotPlayRandom()
{
	m_pMgr->RequestRandom();
}

void BrowserWindow::SlotTopMenu()
{

	m_SelectMap = m_pListBrowser->GetSelectMap();

	if (m_SelectMap.count() > 0)
	{
		SetSelectOnTopMenu();
	}
	else
	{
		SetSelectOffTopMenu();
	}
}

void BrowserWindow::SlotTopMenuAction(int menuID)
{
	LogDebug("click top menu [%d]", menuID);
	switch (menuID) {
	case TOP_MENU_RELOAD:
		DoTopMenuReload();
		break;
	case TOP_MENU_SELECT_ALL:
		DoTopMenuSelectAll();
		break;
	case TOP_MENU_CLEAR_ALL:
		DoTopMenuClearAll();
		break;
	case TOP_MENU_OPTION_PLAY_SUBDIR:
		DoTopMenuOptionPlaySubDir();
		break;
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
	case TOP_MENU_CONVERT_FORMAT:
		DoTopMenuConverFormat();
		break;
	case TOP_MENU_GAIN_SET:
		DoTopMenuGain(VAL_SET);
		break;
	case TOP_MENU_GAIN_CLEAR:
		DoTopMenuGain(VAL_CLEAR);
		break;
	case TOP_MENU_SEARCH_COVER_ART:
		DoTopMenuSearchCoverArt();
		break;
	case TOP_MENU_ADD:
		DoTopMenuAdd();
		break;
	case TOP_MENU_DELETE:
		DoTopMenuDelete();
		break;
	case TOP_MENU_OPTION_OVERWRITE:
		DoTopMenuOptionOverwrite();
		break;
	case TOP_MENU_MOVE:
		DoTopMenuCopy(true);
		break;
	case TOP_MENU_COPY:
		DoTopMenuCopy(false);
		break;
	case TOP_MENU_MOVE_HERE:
		DoTopMenuCopyHere(true);
		break;
	case TOP_MENU_COPY_HERE:
		DoTopMenuCopyHere(false);
		break;
	}
}

void BrowserWindow::SlotOptionMenu(int index, int type)
{
	SetOptionMenu(type);

	m_pListBrowser->SetEditor(index);
}

void BrowserWindow::SlotOptionMenuAction(int id, int menuID)
{
	LogDebug("click option menu [%d] [%d]", id, menuID);
	switch (menuID) {
	case OPTION_MENU_OPTION_PLAY_SUBDIR:
		DoOptionMenuOptionPlaySubDir();
		break;
	case OPTION_MENU_PLAY_NOW:
		DoOptionMenuPlay(PLAY_NOW);
		break;
	case OPTION_MENU_PLAY_LAST:
		DoOptionMenuPlay(PLAY_LAST);
		break;
	case OPTION_MENU_PLAY_NEXT:
		DoOptionMenuPlay(PLAY_NEXT);
		break;
	case OPTION_MENU_PLAY_CLEAR:
		DoOptionMenuPlay(PLAY_CLEAR);
		break;
	case OPTION_MENU_GAIN_SET:
		DoOptionMenuGain(VAL_SET);
		break;
	case OPTION_MENU_GAIN_CLEAR:
		DoOptionMenuGain(VAL_CLEAR);
		break;
	case OPTION_MENU_SCAN_ON:
		DoOptionMenuScan(true);
		break;
	case OPTION_MENU_SCAN_OFF:
		DoOptionMenuScan(false);
		break;
	case OPTION_MENU_CONVERT_FORMAT:
		DoOptionMenuConverFormat();
		break;
	case OPTION_MENU_RENAME:
		DoOptionMenuRename();
		break;
	case OPTION_MENU_DELETE:
		DoOptionMenuDelete();
		break;
	case OPTION_MENU_OPTION_OVERWRITE:
		DoOptionMenuOptionOverwrite();
		break;
	case OPTION_MENU_MOVE:
		DoOptionMenuCopy(true);
		break;
	case OPTION_MENU_COPY:
		DoOptionMenuCopy(false);
		break;
	case OPTION_MENU_MOVE_HERE:
		DoOptionMenuCopyHere(true);
		break;
	case OPTION_MENU_COPY_HERE:
		DoOptionMenuCopyHere(false);
		break;
	case OPTION_MENU_EDIT_TAG:
		DoOptionMenuEditTag();
		break;
	case OPTION_MENU_SEARCH_COVER_ART:
		DoOptionMenuSearchCoverArt();
		break;
	}
}

//void BrowserWindow::SlotResize()
//{
//	LogDebug("click resize");
//}

void BrowserWindow::SlotSelectTitle(int nType, QString rawData)
{
	LogDebug("========== select title ===========");
	UtilNovatron::DebugTypeForBrowser(nType);

	CJsonNode node;
	if (!node.SetContent(rawData))
	{
		LogCritical("invalid json");
		return;
	}

	if (iFolderType_Mask_Sub & nType)
	{
		QString path = node.GetString(KEY_PATH);
		if (!m_Root.isEmpty())
			path = m_Root + "/" + path;

		BrowserWindow *widget = new BrowserWindow(this, m_pMgr->GetAddr(), m_EventID, path);

		if (iFolderType_Mask_Play_Top & nType
				&& BROWSER_MODE_NORMAL == m_BrowserMode)
		{
			widget->ShowFormPlay();
		}
		widget->SetBrowserMode(m_BrowserMode);
		widget->RequestFolder(path);

		emit SigAddWidget(widget, STR_BROWSER);
	}
	else if (iFolderType_Mask_Play_Select & nType)
	{
		LogDebug("node [%s]", node.ToCompactByteArray().data());
		m_Dirs.clear();
		m_Files.clear();
		m_Files.append(node.GetString(KEY_PATH));
		m_pMgr->RequestTrackPlay(m_Root, m_Dirs, m_Files);
	}
}

//void BrowserWindow::SlotSelectURL(QString rawData)
//{
//	CJsonNode node(JSON_OBJECT);
//	if (!node.SetContent(rawData))
//	{
//		return;
//	}

////	LogDebug("node [%s]", node.ToCompactByteArray().data());

////	QStringList dirs;
////	QStringList files;

////	files.append(node.GetString(KEY_PATH));
//	m_pMgr->RequestTrackPlay(m_Root, m_Dirs, m_Files);
//}

void BrowserWindow::SlotRespList(QList<CJsonNode> list)
{
	int nType =  list[0].GetInt(KEY_TYPE);

	if (iFolderType_Mask_Root & nType)
	{
		ui->gridLayoutTop->addWidget(m_pInfoService);
		ui->gridLayoutBottom->addWidget(m_pIconService);

		m_pInfoService->GetFormPlay()->ShowMenu();

		m_pInfoService->SetSubtitle(STR_BROWSER);
		m_pIconService->ClearNodeList();
		nType = m_pIconService->SetNodeList(list, IconService::ICON_SERVICE_BROWSER);

	}
	else
	{
		ui->gridLayoutTop->addWidget(m_pInfoBrowser);
		ui->gridLayoutBottom->addWidget(m_pListBrowser);

		m_pInfoBrowser->GetFormPlay()->ShowMenu();
	//	m_pInfoBrowser->GetFormSort()->ShowResize();

		m_pInfoBrowser->SetCoverArt(list[0].GetString(KEY_COVER_ART));
		m_pInfoBrowser->SetSubtitle(m_Root);

		m_pListBrowser->ClearNodeList();
		nType = m_pListBrowser->SetNodeList(list, ListBrowser::LIST_BROWSER_BROWSER);
	}

	LogDebug("========== first info of response list  ===========");
	UtilNovatron::DebugTypeForBrowser(nType);
	SetFolderType(nType);

}

void BrowserWindow::SlotReqCoverArt(QString path, int index)
{
	path = m_Root + "/" + path;
	QStringList lsAddr = m_pMgr->GetAddr().split(":");
	QString fullpath = QString("%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(SRC_BROWSER).arg(path);

	m_pMgr->RequestCoverArt(fullpath, index, QListView::ListMode);
}

void BrowserWindow::SlotReqInfoBot(QString path, int nIndex)
{
	path = m_Root + "/" + path;
	m_pMgr->RequestInfoBot(path, nIndex);
}

void BrowserWindow::SlotCoverArtUpdate(QString fileName, int nIndex, int mode)
{
	Q_UNUSED(mode);

	QStandardItem *itemIcon = m_pListBrowser->GetModel()->item(nIndex);
	itemIcon->setData(fileName, ListBrowserDelegate::LIST_BROWSER_COVER);
	m_pListBrowser->GetModel()->setItem(nIndex, itemIcon);
}

void BrowserWindow::SlotInfoBotUpdate(CJsonNode node, int nIndex)
{
	if (nIndex < 0)
		return;

	QStandardItem *item = m_pListBrowser->GetModel()->item(nIndex);
	item->setData(node.GetString(KEY_BOT), ListBrowserDelegate::LIST_BROWSER_SUBTITLE);
	item->setData(node.GetString(KEY_DURATION), ListBrowserDelegate::LIST_BROWSER_DURATION);
	m_pListBrowser->GetModel()->setItem(nIndex, item);
}

void BrowserWindow::SlotCopyHere(bool move, QString dstPath)
{
	LogDebug("SlotCopyHere [%d] [%d] [%s]", m_BrowserMode, move, dstPath.toUtf8().data());
	if (BROWSER_MODE_COPY == m_BrowserMode || BROWSER_MODE_MOVE == m_BrowserMode)
	{
		emit SigCopyHere(move, dstPath);
		emit SigRemoveWidget(this);
	}
	else
	{
		SetPaths();

		m_pMgr->RequestCopy(m_Root, m_Paths, dstPath, move, m_EventID);

		// refresh
	}
}

void BrowserWindow::ReadSettings()
{
	SettingIO settings;
	settings.beginGroup(SETTINGS_GROUP);

	m_pMgr->SetOptPlaySubDir(settings.value("option_play_sub_dir").toBool());
	m_pMgr->SetOptOverwrite(settings.value("option_overwrite").toBool());

	settings.endGroup();

}

void BrowserWindow::WriteSettings()
{
	SettingIO settings;
	settings.beginGroup(SETTINGS_GROUP);

	settings.setValue("option_play_sub_dir", m_pMgr->GetOptPlaySubDir());
	settings.setValue("option_overwrite", m_pMgr->GetOptOverwrite());

	settings.endGroup();

}

void BrowserWindow::ConnectSigToSlot()
{
	connect(this, SIGNAL(SigAddWidget(QWidget*, QString)), parent(), SLOT(SlotAddWidget(QWidget*, QString)));		// recursive
	connect(this, SIGNAL(SigRemoveWidget(QWidget*)), parent(), SLOT(SlotRemoveWidget(QWidget*)));

//	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectPlay(int)), this, SLOT(SlotSelectPlay(int)));
//	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int)), this, SLOT(SlotSelectTitle(int)));
	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));

//	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigSelectCoverArt(QString)), this, SLOT(SlotSelectURL(QString)));
	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));
	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigMenu(int, int)), this, SLOT(SlotOptionMenu(int, int)));
	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigMenuAction(int, int)), this, SLOT(SlotOptionMenuAction(int, int)));
	connect(m_pListBrowser, SIGNAL(SigReqCoverArt(QString, int)), this, SLOT(SlotReqCoverArt(QString, int)));
	connect(m_pListBrowser, SIGNAL(SigReqInfoBot(QString, int)), this, SLOT(SlotReqInfoBot(QString, int)));

	connect(m_pMgr, SIGNAL(SigRespList(QList<CJsonNode>)), this, SLOT(SlotRespList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigInfoBotUpdate(CJsonNode, int)), this, SLOT(SlotInfoBotUpdate(CJsonNode, int)));
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));

	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigMenu()), this, SLOT(SlotTopMenu()));
	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigMenuAction(int)), this, SLOT(SlotTopMenuAction(int)));

	connect(m_pInfoBrowser->GetFormPlay(), SIGNAL(SigPlayAll()), this, SLOT(SlotPlayAll()));
	connect(m_pInfoBrowser->GetFormPlay(), SIGNAL(SigPlayRandom()), this, SLOT(SlotPlayRandom()));
	connect(m_pInfoBrowser->GetFormPlay(), SIGNAL(SigMenu()), this, SLOT(SlotTopMenu()));
	connect(m_pInfoBrowser->GetFormPlay(), SIGNAL(SigMenuAction(int)), this, SLOT(SlotTopMenuAction(int)));
//	connect(m_pInfoBrowser->GetFormSort(), SIGNAL(SigResize()), this, SLOT(SlotResize()));


	connect(this, SIGNAL(SigCopyHere(bool, QString)), parent(), SLOT(SlotCopyHere(bool, QString)));

}

void BrowserWindow::Initialize()
{
	m_FolderType = 0;
	SetBrowserMode(BROWSER_MODE_NORMAL);

	m_Dirs.clear();
	m_Files.clear();

	m_TopMenuMap.clear();
	m_SelectMap.clear();
}

void BrowserWindow::SetSelectOffTopMenu()
{
	m_TopMenuMap.clear();

	if (BROWSER_MODE_COPY == m_BrowserMode || BROWSER_MODE_MOVE == m_BrowserMode)
	{
		if (m_FolderType & iFolderType_Mask_Root)
		{
			m_TopMenuMap.insert(TOP_MENU_RELOAD, STR_RELOAD);

			m_pInfoService->GetFormPlay()->ClearMenu();
			m_pInfoService->GetFormPlay()->SetMenu(m_TopMenuMap);
		}
		else
		{
			m_TopMenuMap.insert(TOP_MENU_RELOAD, STR_RELOAD);
			if (m_FolderType & iFolderType_Mask_FileMgr)
			{
				m_TopMenuMap.insert(TOP_MENU_ADD, STR_ADD);
			}
			if (BROWSER_MODE_COPY == m_BrowserMode)
			{
				m_TopMenuMap.insert(TOP_MENU_COPY_HERE, STR_COPY_HERE);
			}
			else if (BROWSER_MODE_MOVE == m_BrowserMode)
			{
				m_TopMenuMap.insert(TOP_MENU_MOVE_HERE, STR_MOVE_HERE);
			}

			m_pInfoBrowser->GetFormPlay()->ClearMenu();
			m_pInfoBrowser->GetFormPlay()->SetMenu(m_TopMenuMap);
		}
	}
	else
	{
		if (m_FolderType & iFolderType_Mask_Root)
		{
			m_TopMenuMap.insert(TOP_MENU_RELOAD, STR_RELOAD);

			m_pInfoService->GetFormPlay()->ClearMenu();
			m_pInfoService->GetFormPlay()->SetMenu(m_TopMenuMap);
		}
		else
		{
			m_TopMenuMap.insert(TOP_MENU_RELOAD, STR_RELOAD);
			m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);

			if (m_FolderType & iFolderType_Mask_Play_Top
					|| m_FolderType & iFolderType_Mask_Play_Option)
			{
				QString option = m_pMgr->GetOptPlaySubDir() ? STR_ON : STR_OFF;
				m_TopMenuMap.insert(TOP_MENU_OPTION_PLAY_SUBDIR, QString("%1 - %2").arg(STR_OPTION_PLAY_SUBDIR).arg(option));
				m_TopMenuMap.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
				m_TopMenuMap.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
				m_TopMenuMap.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
				m_TopMenuMap.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
				m_TopMenuMap.insert(TOP_MENU_GAIN_SET, STR_GAIN_SET);
				m_TopMenuMap.insert(TOP_MENU_GAIN_CLEAR, STR_GAIN_CLEAR);
				m_TopMenuMap.insert(TOP_MENU_CONVERT_FORMAT, STR_CONVERT_FORMAT);
			}
			if (m_FolderType & iFolderType_Mask_Play_Select)
			{
				m_TopMenuMap.insert(TOP_MENU_SEARCH_COVER_ART, STR_SEARCH_COVERART);
			}
			if (m_FolderType & iFolderType_Mask_FileMgr)
			{
				m_TopMenuMap.insert(TOP_MENU_ADD, STR_ADD);
			}

			m_pInfoBrowser->GetFormPlay()->ClearMenu();
			m_pInfoBrowser->GetFormPlay()->SetMenu(m_TopMenuMap);
		}
	}
}

void BrowserWindow::SetSelectOnTopMenu()
{
	m_TopMenuMap.clear();

	if (BROWSER_MODE_COPY == m_BrowserMode || BROWSER_MODE_MOVE == m_BrowserMode)
	{
		if (m_FolderType & iFolderType_Mask_Root)
		{
			m_TopMenuMap.insert(TOP_MENU_RELOAD, STR_RELOAD);

			m_pInfoService->GetFormPlay()->ClearMenu();
			m_pInfoService->GetFormPlay()->SetMenu(m_TopMenuMap);
		}
		else
		{
			m_TopMenuMap.insert(TOP_MENU_RELOAD, STR_RELOAD);
			if (m_FolderType & iFolderType_Mask_FileMgr)
			{
				m_TopMenuMap.insert(TOP_MENU_ADD, STR_ADD);
			}
			if (BROWSER_MODE_COPY == m_BrowserMode)
			{
				m_TopMenuMap.insert(TOP_MENU_COPY_HERE, STR_COPY_HERE);
			}
			else if (BROWSER_MODE_MOVE == m_BrowserMode)
			{
				m_TopMenuMap.insert(TOP_MENU_MOVE_HERE, STR_MOVE_HERE);
			}

			m_pInfoBrowser->GetFormPlay()->ClearMenu();
			m_pInfoBrowser->GetFormPlay()->SetMenu(m_TopMenuMap);
		}
	}
	else
	{
		if (m_FolderType & iFolderType_Mask_Root)
		{
			m_TopMenuMap.insert(TOP_MENU_RELOAD, STR_RELOAD);

			m_pInfoService->GetFormPlay()->ClearMenu();
			m_pInfoService->GetFormPlay()->SetMenu(m_TopMenuMap);
		}
		else
		{
			m_TopMenuMap.insert(TOP_MENU_CLEAR_ALL, STR_CLEAR_ALL);

			if (m_FolderType & iFolderType_Mask_Play_Top
					|| m_FolderType & iFolderType_Mask_Play_Check)
			{
				QString option = m_pMgr->GetOptPlaySubDir() ? STR_ON : STR_OFF;
				m_TopMenuMap.insert(TOP_MENU_OPTION_PLAY_SUBDIR, QString("%1 - %2").arg(STR_OPTION_PLAY_SUBDIR).arg(option));
				m_TopMenuMap.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
				m_TopMenuMap.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
				m_TopMenuMap.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
				m_TopMenuMap.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
				m_TopMenuMap.insert(TOP_MENU_GAIN_SET, STR_GAIN_SET);
				m_TopMenuMap.insert(TOP_MENU_GAIN_CLEAR, STR_GAIN_CLEAR);
				m_TopMenuMap.insert(TOP_MENU_CONVERT_FORMAT, STR_CONVERT_FORMAT);
			}
			if (m_FolderType & iFolderType_Mask_FileMgr)
			{
				QString option = m_pMgr->GetOptOverwrite() ? STR_ON : STR_OFF;
				m_TopMenuMap.insert(TOP_MENU_OPTION_OVERWRITE, QString("%1 - %2").arg(STR_OPTION_OVERWRITE).arg(option));
				m_TopMenuMap.insert(TOP_MENU_DELETE, STR_DELETE);
				m_TopMenuMap.insert(TOP_MENU_MOVE, STR_MOVE);
				m_TopMenuMap.insert(TOP_MENU_COPY, STR_COPY);
			}

			m_pInfoBrowser->GetFormPlay()->ClearMenu();
			m_pInfoBrowser->GetFormPlay()->SetMenu(m_TopMenuMap);
		}
	}
}

void BrowserWindow::DoTopMenuPlay(int where)
{
	SlotPlayAll(where);
}

void BrowserWindow::DoTopMenuReload()
{
	if (m_FolderType & iFolderType_Mask_Root)
	{
		RequestRoot();
	}
	else
	{
		RequestFolder(m_Root);
	}
}

void BrowserWindow::DoTopMenuSelectAll()
{
	m_pListBrowser->SetAllSelectMap();
}

void BrowserWindow::DoTopMenuClearAll()
{
	m_pListBrowser->ClearSelectMap();
}

void BrowserWindow::DoTopMenuGain(QString gain)
{
	SetPaths();

	m_pMgr->RequestReplayGain(m_Root, m_Paths, gain, m_EventID);
}

void BrowserWindow::DoTopMenuOptionPlaySubDir()
{
	bool option = m_pMgr->GetOptPlaySubDir();
	m_pMgr->SetOptPlaySubDir(!option);
}

void BrowserWindow::DoTopMenuOptionOverwrite()
{
	bool option = m_pMgr->GetOptOverwrite();
	m_pMgr->SetOptOverwrite(!option);
}

void BrowserWindow::DoTopMenuConverFormat()
{
	SelectFormatDialog dialog;
	if (dialog.exec() == QDialog::Accepted)
	{
		QString format = dialog.GetFormat();

		SetPaths();

		m_pMgr->RequestConvertFormat(m_Root, m_Paths, format, m_EventID);

		// refresh
		DoTopMenuReload();

	}
}

void BrowserWindow::DoTopMenuAdd()
{
	InputNameDialog dialog;
	if (dialog.exec() == QDialog::Accepted)
	{
		QString path = dialog.GetName();
		path = m_Root + "/" + path;
		m_pMgr->RequestCreate(path);

		// refresh
		DoTopMenuReload();
	}
}

void BrowserWindow::DoTopMenuDelete()
{
	SetPaths();

	m_pMgr->RequestDelete(m_Root, m_Paths, m_EventID);

	// refresh
	DoTopMenuReload();
}

void BrowserWindow::DoTopMenuCopy(bool move)
{
	int mode = BROWSER_MODE_MAX;
	QString title;
	if (move)
	{
		mode = BROWSER_MODE_MOVE;
		title = STR_MOVE;
	}
	else
	{
		mode = BROWSER_MODE_COPY;
		title = STR_COPY;
	}

	BrowserWindow *widget = new BrowserWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->SetBrowserMode(mode);
	widget->RequestRoot();
	emit SigAddWidget(widget, title);

}

void BrowserWindow::DoTopMenuCopyHere(bool move)
{
	emit SigCopyHere(move, m_Root);
	emit SigRemoveWidget(this);
}

void BrowserWindow::DoTopMenuSearchCoverArt()
{
	QString site;
	QString keyword;
	QString artist;

	SearchCoverArtDialog searchDialog;
	if (searchDialog.exec() == QDialog::Accepted)
	{
		site = searchDialog.GetSite();
		keyword = searchDialog.GetKeyword();
		artist = searchDialog.GetArtist();
	}
	else
	{
		return;
	}

	SearchCoverArtResultDialog resultDialog;
	resultDialog.SetAddr(m_pMgr->GetAddr());
	resultDialog.RequestCoverArtList(site, keyword, artist);
	if (resultDialog.exec() == QDialog::Accepted)
	{
		SetDirFile();

		QString image = resultDialog.GetImage();
		QString thumb = resultDialog.GetThumb();

		m_pMgr->RequestSetArt(m_Root, m_Files, image, thumb, m_EventID);

		// refresh
		DoTopMenuReload();

	}
}

void BrowserWindow::SetOptionMenu(int type)
{
	UtilNovatron::DebugTypeForBrowser(type);

	m_OptionMenuMap.clear();

	if (BROWSER_MODE_COPY == m_BrowserMode || BROWSER_MODE_MOVE == m_BrowserMode)
	{
		if (BROWSER_MODE_COPY == m_BrowserMode)
		{
			m_OptionMenuMap.insert(OPTION_MENU_COPY_HERE, STR_COPY_HERE);
		}
		else if (BROWSER_MODE_MOVE == m_BrowserMode)
		{
			m_OptionMenuMap.insert(OPTION_MENU_MOVE_HERE, STR_MOVE_HERE);
		}
	}
	else
	{
		if (type & iFolderType_Mask_Play_Option
				|| type & iFolderType_Mask_Play_Check)
		{
			QString option = m_pMgr->GetOptPlaySubDir() ? STR_ON : STR_OFF;
			m_OptionMenuMap.insert(OPTION_MENU_OPTION_PLAY_SUBDIR, QString("%1 - %2").arg(STR_OPTION_PLAY_SUBDIR).arg(option));
			m_OptionMenuMap.insert(OPTION_MENU_PLAY_NOW, STR_PLAY_NOW);
			m_OptionMenuMap.insert(OPTION_MENU_PLAY_LAST, STR_PLAY_LAST);
			m_OptionMenuMap.insert(OPTION_MENU_PLAY_NEXT, STR_PLAY_NEXT);
			m_OptionMenuMap.insert(OPTION_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
			m_OptionMenuMap.insert(OPTION_MENU_GAIN_SET, STR_GAIN_SET);
			m_OptionMenuMap.insert(OPTION_MENU_GAIN_CLEAR, STR_GAIN_CLEAR);
			m_OptionMenuMap.insert(OPTION_MENU_CONVERT_FORMAT, STR_CONVERT_FORMAT);
		}
		if (type & iFolderType_Mask_Scan)
		{
			m_OptionMenuMap.insert(OPTION_MENU_SCAN_OFF, STR_SCAN_OFF);
		}
		else
		{
			m_OptionMenuMap.insert(OPTION_MENU_SCAN_ON, STR_SCAN_ON);
		}
		if (type & iFolderType_Mask_FileMgr)
		{
			QString option = m_pMgr->GetOptOverwrite() ? STR_ON : STR_OFF;
			m_OptionMenuMap.insert(OPTION_MENU_RENAME, STR_RENAME);
			m_OptionMenuMap.insert(OPTION_MENU_DELETE, STR_DELETE);
			m_OptionMenuMap.insert(OPTION_MENU_OPTION_OVERWRITE, QString("%1 - %2").arg(STR_OPTION_OVERWRITE).arg(option));
			m_OptionMenuMap.insert(OPTION_MENU_MOVE, STR_MOVE);
			m_OptionMenuMap.insert(OPTION_MENU_COPY, STR_COPY);
		}
		if (type & iFolderType_Mask_Play_Select)
		{
			m_OptionMenuMap.insert(OPTION_MENU_EDIT_TAG, STR_EDIT_TAG);
			m_OptionMenuMap.insert(OPTION_MENU_SEARCH_COVER_ART, STR_SEARCH_COVERART);
		}
	}

	m_pListBrowser->GetDelegate()->SetOptionMenuMap(m_OptionMenuMap);

}

void BrowserWindow::DoOptionMenuPlay(int where)
{

}

void BrowserWindow::DoOptionMenuGain(QString gain)
{

}

void BrowserWindow::DoOptionMenuOptionPlaySubDir()
{
	bool option = m_pMgr->GetOptPlaySubDir();
	m_pMgr->SetOptPlaySubDir(!option);
}

void BrowserWindow::DoOptionMenuOptionOverwrite()
{
	bool option = m_pMgr->GetOptOverwrite();
	m_pMgr->SetOptOverwrite(!option);
}

void BrowserWindow::DoOptionMenuConverFormat()
{

}

void BrowserWindow::DoOptionMenuScan(bool on)
{

}

void BrowserWindow::DoOptionMenuRename()
{

}

void BrowserWindow::DoOptionMenuDelete()
{

}

void BrowserWindow::DoOptionMenuCopy(bool move)
{

}

void BrowserWindow::DoOptionMenuCopyHere(bool move)
{

}

void BrowserWindow::DoOptionMenuEditTag()
{

}

void BrowserWindow::DoOptionMenuSearchCoverArt()
{

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

void BrowserWindow::ShowFormPlay()
{
	m_pInfoBrowser->GetFormPlay()->ShowPlayAll();
	m_pInfoBrowser->GetFormPlay()->ShowPlayRandom();
}

void BrowserWindow::SetDirFile()
{
	m_SelectMap = m_pListBrowser->GetSelectMap();

	m_Dirs.clear();
	m_Files.clear();

	if (m_SelectMap.count() > 0)
	{
		QMap<QString, int>::iterator i;
		for (i = m_SelectMap.begin(); i!= m_SelectMap.end(); i++)
		{
			const QString path = i.key();
			const int type = i.value();
			if (iFolderType_Mask_Dir & type)
			{
				m_Dirs.append(path);
			}
			else if (iFolderType_Mask_Song & type)
			{
				m_Files.append(path);
			}
		}
	}
	else
	{
		QList<CJsonNode> nodeList = m_pListBrowser->GetNodeList();
		foreach (CJsonNode node, nodeList)
		{
			const QString path = node.GetString(KEY_PATH);
			const int type = node.GetInt(KEY_TYPE);
			if (iFolderType_Mask_Dir & type)
			{
				m_Dirs.append(path);
			}
			else if (iFolderType_Mask_Song & type)
			{
				m_Files.append(path);
			}
		}
	}
}

void BrowserWindow::SetPaths()
{
	SetDirFile();

	m_Paths.clear();
	for (int i = 0; i < m_Dirs.count(); i++)
	{
		m_Paths.append(m_Dirs.at(i));
	}
	for (int i = 0; i < m_Files.count(); i++)
	{
		m_Paths.append(m_Files.at(i));
	}
}

