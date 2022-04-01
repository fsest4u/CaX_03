#include <QThread>

#include "browserwindow.h"
#include "ui_browserwindow.h"

#include "browser.h"

#include "dialog/addsharedialog.h"
#include "dialog/browserdialog.h"
#include "dialog/commondialog.h"
#include "dialog/confirmcoverartdialog.h"
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

BrowserWindow::BrowserWindow(QWidget *parent, const QString &addr, const int &eventID, const QString &root) :
	QWidget(parent),
	m_pMgr(new BrowserManager),
	m_pInfoService(new InfoService(this)),
	m_pInfoBrowser(new InfoBrowser(this)),
	m_pIconService(new IconService(this)),
	m_pListBrowser(new ListBrowser(this)),
	m_Menu(new QMenu(this)),
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

	disconnect(m_Menu, SIGNAL(triggered(QAction*)));
	if (m_Menu)
	{
		delete m_Menu;
		m_Menu = nullptr;
	}

	delete ui;

}

void BrowserWindow::RequestRoot(QString ext)
{
//	ui->gridLayoutTop->addWidget(m_pInfoService);
//	ui->gridLayoutBottom->addWidget(m_pIconService);

	if (BROWSER_MODE_COPY == m_BrowserMode
			|| BROWSER_MODE_MOVE == m_BrowserMode
			|| BROWSER_MODE_COPY_OPTION == m_BrowserMode
			|| BROWSER_MODE_MOVE_OPTION == m_BrowserMode)
	{
		m_pMgr->RequestRoot(true);
	}
	else
	{
		if (ext.isEmpty())
		{
			m_pMgr->RequestRoot();
		}
		else
		{
			m_Ext = ext;
			m_pMgr->RequestRootSetup(ext);
		}
	}

}

void BrowserWindow::RequestFolder(QString strPath, QString ext, CJsonNode upnp)
{
//	ui->gridLayoutTop->addWidget(m_pInfoService);
//	ui->gridLayoutBottom->addWidget(m_pIconService);

	if (BROWSER_MODE_COPY == m_BrowserMode
			|| BROWSER_MODE_MOVE == m_BrowserMode
			|| BROWSER_MODE_COPY_OPTION == m_BrowserMode
			|| BROWSER_MODE_MOVE_OPTION == m_BrowserMode)
	{
		m_pMgr->RequestFolder(strPath, true);
	}
	else
	{
		if (ext.isEmpty())
		{
			QString path = upnp.GetString(KEY_PATH);
			if (path.isEmpty())
			{
				m_pMgr->RequestFolder(strPath);
			}
			else
			{
//				LogDebug("request upnp [%s]", upnp.ToCompactByteArray().data());
				m_NodeUpnp = upnp;
				m_pMgr->RequestFolderUpnp(m_NodeUpnp);
			}
		}
		else
		{
			m_Ext = ext;
			m_pMgr->RequestFolderSetup(strPath, ext);
		}
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

void BrowserWindow::SetBrowserMode(int BrowserMode, QString optionPath, int optionType)
{
	m_BrowserMode = BrowserMode;
	if (!optionPath.isEmpty())
	{
		m_OptionPath = optionPath;
	}
	if (optionType > 0)
	{
		m_OptionType = optionType;
	}
}

void BrowserWindow::SlotAddWidget(QWidget *widget, QString title)
{
	emit SigAddWidget(widget, title);		// recursive
}

void BrowserWindow::SlotRemoveWidget(QWidget *widget)
{
	emit SigRemoveWidget(widget);
}

void BrowserWindow::SlotBrowserPath(QString path)
{
	emit SigBrowserPath(path);
//	emit SigRemoveWidget(this);
}

void BrowserWindow::SlotPlayAll(int where)
{
	if (m_FolderType & iFolderType_Mask_Pls && m_FolderType & iFolderType_Mask_Song)
	{
		m_Files.clear();
		m_pMgr->RequestPlaylistPlay(m_Root, m_Files, where);
	}
	else if (iFolderType_Mask_Upnp & m_FolderType)
	{
		QList<CJsonNode> nodeList = m_pListBrowser->GetNodeList();
//		LogDebug("play upnp");
		CJsonNode itemArr(JSON_ARRAY);
		foreach(CJsonNode node, nodeList)
		{
			itemArr.AppendArray(node);
//			LogDebug("node [%s]", node.ToCompactByteArray().data());
		}

		CJsonNode tempNode(JSON_OBJECT);
		tempNode.Add(KEY_ITEMS, itemArr);
		m_pMgr->RequestUpnpPlay(tempNode, m_EventID, where);
	}
	else
	{
		SetDirFile();

		m_pMgr->RequestTrackPlay(m_Root, m_Dirs, m_Files, where);
	}
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
//	LogDebug("click top menu [%d]", menuID);
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
	case TOP_MENU_SEARCH_COVERART:
		DoTopMenuSearchCoverArt();
		break;
	case TOP_MENU_MAKE_FOLDER:
		DoTopMenuMakeFolder();
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
	case TOP_MENU_ADD_SHARE:
		DoTopMenuAddShare();
		break;
	}
}

//void BrowserWindow::SlotOptionMenu(int index, int type)
//{
//	SetOptionMenu(type);

//	m_pListBrowser->SetEditor(index);
//}

void BrowserWindow::SlotOptionMenuAction(CJsonNode node, int type, int menuID)
{
	QString path = node.GetString(KEY_PATH);
	LogDebug("click option menu node [%s] [%d] [%d]", node.ToCompactByteArray().data(), type, menuID);
	switch (menuID) {
	case OPTION_MENU_OPTION_PLAY_SUBDIR:
		DoOptionMenuOptionPlaySubDir();
		break;
	case OPTION_MENU_PLAY_NOW:
		DoOptionMenuPlay(node, type, PLAY_NOW);
		break;
	case OPTION_MENU_PLAY_LAST:
		DoOptionMenuPlay(node, type, PLAY_LAST);
		break;
	case OPTION_MENU_PLAY_NEXT:
		DoOptionMenuPlay(node, type, PLAY_NEXT);
		break;
	case OPTION_MENU_PLAY_CLEAR:
		DoOptionMenuPlay(node, type, PLAY_CLEAR);
		break;
	case OPTION_MENU_GAIN_SET:
		DoOptionMenuGain(path, type, VAL_SET);
		break;
	case OPTION_MENU_GAIN_CLEAR:
		DoOptionMenuGain(path, type, VAL_CLEAR);
		break;
	case OPTION_MENU_SCAN_ON:
		DoOptionMenuScan(path, true);
		break;
	case OPTION_MENU_SCAN_OFF:
		DoOptionMenuScan(path, false);
		break;
	case OPTION_MENU_CONVERT_FORMAT:
		DoOptionMenuConverFormat(path, type);
		break;
	case OPTION_MENU_RENAME:
		DoOptionMenuRename(path);
		break;
	case OPTION_MENU_DELETE:
		DoOptionMenuDelete(path, type);
		break;
	case OPTION_MENU_OPTION_OVERWRITE:
		DoOptionMenuOptionOverwrite();
		break;
	case OPTION_MENU_MOVE:
		DoOptionMenuCopy(path, type, true);
		break;
	case OPTION_MENU_COPY:
		DoOptionMenuCopy(path, type, false);
		break;
//	case OPTION_MENU_MOVE_HERE:
//		DoOptionMenuCopyHere(path, true);
//		break;
//	case OPTION_MENU_COPY_HERE:
//		DoOptionMenuCopyHere(path, false);
//		break;
	case OPTION_MENU_EDIT_TAG:
		DoOptionMenuEditTag(path, type);
		break;
	case OPTION_MENU_SEARCH_COVERART:
		DoOptionMenuSearchCoverArt(path, type);
		break;
	case OPTION_MENU_MODIFY_SHARE:
		DoOptionMenuModifyShare(path);
		break;
	case OPTION_MENU_DELETE_SHARE:
		DoOptionMenuDeleteShare(path);
		break;
	}
}

//void BrowserWindow::SlotResize()
//{
//	LogDebug("click resize");
//}


void BrowserWindow::SlotIconSelectTitle(int nType, QString rawData)
{
	UtilNovatron::DebugTypeForBrowser("SlotIconSelectTitle", nType);

	CJsonNode node;
	if (!node.SetContent(rawData))
	{
		return;
	}

	if (iFolderType_Mask_Sub & nType && iFolderType_Mask_Dev & nType)
	{
		QString path = node.GetString(KEY_PATH);
		if (!m_Root.isEmpty())
			path = m_Root + "/" + path;

		BrowserWindow *widget = new BrowserWindow(this, m_pMgr->GetAddr(), m_EventID, path);

		if (iFolderType_Mask_Play_Top & nType
				&& BROWSER_MODE_NORMAL == m_BrowserMode)
		{
			widget->ShowFormPlay(true);
		}
		widget->SetBrowserMode(m_BrowserMode, m_OptionPath, m_OptionType);
		widget->SetFolderType(nType);
		emit widget->SigAddWidget(widget, STR_BROWSER);
		widget->RequestFolder(path, m_Ext);
	}

}

void BrowserWindow::SlotSelectTrackPlay(int nType, CJsonNode node)
{
	UtilNovatron::DebugTypeForBrowser("SlotSelectTrackPlay", nType);

	if  (iFolderType_Mask_Pls & nType)
	{
//		LogDebug("Pls node [%s]", node.ToCompactByteArray().data());
		m_Files.clear();
		m_Files.append(node.GetString(KEY_ID_LOWER));
		m_pMgr->RequestPlaylistPlay(m_Root, m_Files, PLAY_NOW);
	}
	else if (iFolderType_Mask_Upnp & nType)
	{
//		LogDebug("play upnp [%s]", node.ToCompactByteArray().data());
		CJsonNode itemArr(JSON_ARRAY);
		itemArr.AppendArray(node);

		CJsonNode tempNode(JSON_OBJECT);
		tempNode.Add(KEY_ITEMS, itemArr);
		m_pMgr->RequestUpnpPlay(tempNode, m_EventID, PLAY_NOW);
	}
	else if (iFolderType_Mask_Play_Select & nType)
	{
//		LogDebug("Play_Select node [%s]", node.ToCompactByteArray().data());
		m_Dirs.clear();
		m_Files.clear();
		m_Files.append(node.GetString(KEY_PATH));
		m_pMgr->RequestTrackPlay(m_Root, m_Dirs, m_Files);
	}
}

void BrowserWindow::SlotSelectTitle(int nType, CJsonNode node)
{
	UtilNovatron::DebugTypeForBrowser("SlotSelectTitle", nType);
//	LogDebug("node [%s]", node.ToCompactByteArray().data());


	if (iFolderType_Mask_Song & nType)
	{
		SlotSelectTrackPlay(nType, node);
	}
	else if (iFolderType_Mask_Net & nType
			 || iFolderType_Mask_Upnp & nType
			 || iFolderType_Mask_Sub & nType)
	{
		QString path = node.GetString(KEY_PATH);
		if (!m_Root.isEmpty())
			path = m_Root + "/" + path;

		BrowserWindow *widget = new BrowserWindow(this, m_pMgr->GetAddr(), m_EventID, path);

		if (iFolderType_Mask_Play_Top & nType
				&& BROWSER_MODE_NORMAL == m_BrowserMode)
		{
			widget->ShowFormPlay(true);
		}
		if (iFolderType_Mask_Upnp & nType)
		{
			if (m_NodeUpnp.GetString(KEY_PATH).isEmpty())
			{
				m_NodeUpnp = node;
				m_NodeUpnp.Add(KEY_PATH, path);
			}
			else
			{
				m_NodeUpnp.Add(KEY_ID_UPPER, node.GetString(KEY_ID_UPPER));
				m_NodeUpnp.Add(KEY_PATH, path);
			}
//			LogDebug("m_NodeUpnp [%s]", m_NodeUpnp.ToCompactByteArray().data());
		}
		widget->SetBrowserMode(m_BrowserMode, m_OptionPath, m_OptionType);
		widget->SetFolderType(nType);
		emit widget->SigAddWidget(widget, STR_BROWSER);
		widget->RequestFolder(path, m_Ext, m_NodeUpnp);
	}
	else if (iFolderType_Mask_File & nType && !m_Ext.isEmpty())
	{
		QString path = node.GetString(KEY_PATH);
		if (!m_Root.isEmpty())
			path = m_Root + "/" + path;

		emit SigBrowserPath(path);
		emit SigRemoveWidget(this);
	}
}

void BrowserWindow::SlotSelectMenu(const QModelIndex &modelIndex, QPoint point)
{
	m_ModelIndex = modelIndex;
	int type = qvariant_cast<int>(m_ModelIndex.data(ListBrowserDelegate::LIST_BROWSER_TYPE));
//	LogDebug("type [%d] x [%d] y [%d]", type, point.x(), point.y());

	SetOptionMenu(type);

	m_Menu->clear();

	QMap<int, QString>::iterator i;
	for (i = m_OptionMenuMap.begin(); i != m_OptionMenuMap.end(); i++)
	{
		QIcon icon = UtilNovatron::GetMenuIcon(i.value());
		QAction *action = new QAction(icon, i.value(), this);
		action->setData(i.key());
		m_Menu->addAction(action);
	}

	m_Menu->popup(m_pListBrowser->GetListView()->viewport()->mapToGlobal(point));
}

void BrowserWindow::SlotMenuAction(QAction *action)
{
	int id = qvariant_cast<int>(m_ModelIndex.data(ListBrowserDelegate::LIST_BROWSER_ID));
	int type = qvariant_cast<int>(m_ModelIndex.data(ListBrowserDelegate::LIST_BROWSER_TYPE));
	QString rawData = qvariant_cast<QString>(m_ModelIndex.data(ListBrowserDelegate::LIST_BROWSER_RAW));

	CJsonNode node;
	if (!node.SetContent(rawData))
	{
		return;
	}
	node.AddInt(KEY_ID_LOWER, id);

	SlotOptionMenuAction(node, type, action->data().toInt());
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
	UtilNovatron::DebugTypeForBrowser("SlotRespList", nType);

	if (iFolderType_Mask_Hdd & nType)
	{
		ui->gridLayoutTop->addWidget(m_pInfoService);
		ui->gridLayoutBottom->addWidget(m_pIconService);

		m_pInfoService->GetFormPlay()->ShowMenu();
		m_pInfoService->SetTitle(STR_BROWSER);

		m_pIconService->ClearNodeList();
		nType = m_pIconService->SetNodeList(list, IconService::ICON_SERVICE_BROWSER);

	}
//	else if (iFolderType_Mask_Usb & nType)
//	{

//	}
//	else if (iFolderType_Mask_Net & nType)
//	{

//	}
//	else if (iFolderType_Mask_Upnp & nType)
//	{

//	}
	else
	{
		ui->gridLayoutTop->addWidget(m_pInfoBrowser);
		ui->gridLayoutBottom->addWidget(m_pListBrowser);

		m_pInfoBrowser->GetFormPlay()->ShowMenu();
//		m_pInfoBrowser->GetFormSort()->ShowResize();

		m_pInfoBrowser->SetCoverArt(list[0].GetString(KEY_COVER_ART));
		m_pInfoBrowser->SetTitle(m_Root);

		m_pListBrowser->ClearNodeList();
		nType = m_pListBrowser->SetNodeList(list, SIDEMENU_BROWSER);
	}

	ShowFormPlay(true);
	SetFolderType(nType);
}

void BrowserWindow::SlotReqCoverArt(QString path, int index)
{
	if (m_FolderType & iFolderType_Mask_Upnp)
	{
		m_pMgr->RequestCoverArt(path, index, QListView::ListMode);
	}
	else
	{
		if (path.contains(UtilNovatron::GetTempDirectory()))
		{
			return;
		}

		if (!m_Root.isEmpty())
			path = m_Root + "/" + path;

		QStringList lsAddr = m_pMgr->GetAddr().split(":");
		QString fullpath = QString("%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(SRC_BROWSER).arg(path);

		m_pMgr->RequestCoverArt(fullpath, index, QListView::ListMode);

	}
}

void BrowserWindow::SlotReqInfoBot(QString path, int nIndex)
{
	if (!m_Root.isEmpty())
		path = m_Root + "/" + path;

	m_pMgr->RequestInfoBot(path, nIndex);
}

void BrowserWindow::SlotRespError(QString errMsg)
{
	if (BROWSER_MODE_NORMAL == m_BrowserMode)
	{
		LogWarning("error [%s]", errMsg.toUtf8().data());
		if (errMsg.compare(STR_NO_RESULT))
		{
			CommonDialog dialog(this, STR_WARNING, errMsg);
			dialog.exec();
		}

		ui->gridLayoutTop->addWidget(m_pInfoBrowser);

		ShowFormPlay(false);
		m_pInfoBrowser->GetFormPlay()->ShowMenu();
		m_pInfoBrowser->SetCoverArt("");
		m_pInfoBrowser->SetTitle(m_Root);
	}
	else
	{
		ui->gridLayoutTop->addWidget(m_pInfoBrowser);

		m_pInfoBrowser->GetFormPlay()->ShowMenu();
		m_pInfoBrowser->SetCoverArt("");
		m_pInfoBrowser->SetTitle(m_Root);
	}
}

void BrowserWindow::SlotCoverArtUpdate(QString filename, int index, int mode)
{
	Q_UNUSED(mode);

	QStandardItem *itemIcon = m_pListBrowser->GetModel()->item(index);
	itemIcon->setData(filename, ListBrowserDelegate::LIST_BROWSER_COVER);
	m_pListBrowser->GetModel()->setItem(index, itemIcon);
}

void BrowserWindow::SlotInfoBotUpdate(CJsonNode node, int nIndex)
{
	if (nIndex < 0)
		return;

	int seconds = node.GetInt(KEY_DURATION);
	QString hhmmss = UtilNovatron::CalcSecondToHMS(seconds);

	QModelIndex modelIndex = m_pListBrowser->GetModel()->index(nIndex, 0);
	m_pListBrowser->GetModel()->setData(modelIndex, node.GetString(KEY_BOT), ListBrowserDelegate::LIST_BROWSER_SUBTITLE);
	m_pListBrowser->GetModel()->setData(modelIndex, hhmmss, ListBrowserDelegate::LIST_BROWSER_DURATION);
}

void BrowserWindow::SlotInfoTagUpdate(CJsonNode node)
{
	TrackInfoDialog dialog;
	dialog.SetWindowTitle(STR_TAG_INFO);
	dialog.SetAddr(m_pMgr->GetAddr());
	dialog.SetAlbumList(m_AlbumList);
	dialog.SetAlbumArtistList(m_AlbumArtistList);
	dialog.SetArtistList(m_ArtistList);
	dialog.SetGenreList(m_GenreList);
	dialog.SetComposerList(m_ComposerList);
	dialog.SetMoodList(m_MoodList);
	dialog.SetMode(TrackInfo::TRACK_INFO_MODE_EDIT);
	dialog.SetInfoData(node);
	if (dialog.exec() == QDialog::Accepted)
	{
		// set tag
		node = dialog.GetInfoData();
		node.Del(VAL_SUCCESS);
		node.Del(VAL_MSG);

		m_pMgr->RequestSetTag(node, m_OptionPath);

	}
}

void BrowserWindow::SlotRespCategoryList(QList<CJsonNode> list)
{
	SetCategoryList(list);
}

void BrowserWindow::SlotListUpdate()
{
	QThread::sleep(1);
	// refresh
	DoTopMenuReload();
}

void BrowserWindow::SlotRespSMBInfo(CJsonNode node)
{
//	LogDebug("node [%s]", node.ToCompactByteArray().data());
	AddShareDialog dialog;
	dialog.SetInfo(m_pMgr->GetAddr(), m_EventID);
	dialog.SetNodeInfo(node);
	if (dialog.exec() == QDialog::Accepted)
	{
		CJsonNode node = dialog.GetNodeInfo();
		m_pMgr->RequestSMBSet(m_EventID, node);
	}
}

void BrowserWindow::SlotCopyHere(bool move, QString dstPath, QString path, int type)
{
//	LogDebug("click copy/move here [%d] [%d] [%s]", m_BrowserMode, move, dstPath.toUtf8().data());
	if (BROWSER_MODE_COPY == m_BrowserMode
			|| BROWSER_MODE_MOVE == m_BrowserMode)
	{
		emit SigCopyHere(move, dstPath);
		emit SigRemoveWidget(this);
	}
	else if (BROWSER_MODE_COPY_OPTION == m_BrowserMode
			 || BROWSER_MODE_MOVE_OPTION == m_BrowserMode)
	{
		emit SigCopyHere(move, dstPath, path, type);
		emit SigRemoveWidget(this);
	}
	else
	{
		if (path.isEmpty() && type == -1)
		{
			SetPaths();

			m_pMgr->RequestCopy(m_Root, m_Paths, dstPath, move, m_EventID);

		}
		else
		{
			QStringList paths;
			SetOptionPaths(path, type, paths);

			m_pMgr->RequestCopy(m_Root, paths, dstPath, move, m_EventID);

		}
	}
}

//void BrowserWindow::SlotOptionCopyHere(bool move, QString dstPath, QString path, int type)
//{
//	LogDebug("click option copy/move here [%d] [%d] [%s] [%s] [%d]", m_BrowserMode, move, dstPath.toUtf8().data(), path.toUtf8().data(), type);
//	if (BROWSER_MODE_COPY_OPTION == m_BrowserMode
//			|| BROWSER_MODE_MOVE_OPTION == m_BrowserMode)
//	{
//		emit SigOptionCopyHere(move, dstPath, path, type);
//		emit SigRemoveWidget(this);
//	}
//	else
//	{
//		QStringList paths;
//		SetOptionPaths(path, type, paths);

//		m_pMgr->RequestCopy(m_Root, paths, dstPath, move, m_EventID);
//	}
//}

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
	connect(this, SIGNAL(SigCopyHere(bool, QString, QString, int)), parent(), SLOT(SlotCopyHere(bool, QString, QString, int)));
//	connect(this, SIGNAL(SigOptionCopyHere(bool, QString, QString, int)), parent(), SLOT(SlotOptionCopyHere(bool, QString, QString, int)));
	connect(this, SIGNAL(SigBrowserPath(QString)), parent(), SLOT(SlotBrowserPath(QString)));

	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespList(QList<CJsonNode>)), this, SLOT(SlotRespList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));
	connect(m_pMgr, SIGNAL(SigInfoBotUpdate(CJsonNode, int)), this, SLOT(SlotInfoBotUpdate(CJsonNode, int)));
	connect(m_pMgr, SIGNAL(SigInfoTagUpdate(CJsonNode)), this, SLOT(SlotInfoTagUpdate(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespCategoryList(QList<CJsonNode>)), this, SLOT(SlotRespCategoryList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigListUpdate()), this, SLOT(SlotListUpdate()));
	connect(m_pMgr, SIGNAL(SigRespSMBInfo(CJsonNode)), this, SLOT(SlotRespSMBInfo(CJsonNode)));

	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigMenu()), this, SLOT(SlotTopMenu()));
	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigMenuAction(int)), this, SLOT(SlotTopMenuAction(int)));

	connect(m_pInfoBrowser->GetFormPlay(), SIGNAL(SigPlayAll()), this, SLOT(SlotPlayAll()));
	connect(m_pInfoBrowser->GetFormPlay(), SIGNAL(SigPlayRandom()), this, SLOT(SlotPlayRandom()));
	connect(m_pInfoBrowser->GetFormPlay(), SIGNAL(SigMenu()), this, SLOT(SlotTopMenu()));
	connect(m_pInfoBrowser->GetFormPlay(), SIGNAL(SigMenuAction(int)), this, SLOT(SlotTopMenuAction(int)));
//	connect(m_pInfoBrowser->GetFormSort(), SIGNAL(SigResize()), this, SLOT(SlotResize()));

//	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectPlay(int)), this, SLOT(SlotSelectPlay(int)));
//	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int)), this, SLOT(SlotSelectTitle(int)));
	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotIconSelectTitle(int, QString)));

	connect(m_pListBrowser, SIGNAL(SigReqCoverArt(QString, int)), this, SLOT(SlotReqCoverArt(QString, int)));
	connect(m_pListBrowser, SIGNAL(SigReqInfoBot(QString, int)), this, SLOT(SlotReqInfoBot(QString, int)));
	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigSelectPlay(int, CJsonNode)), this, SLOT(SlotSelectTrackPlay(int, CJsonNode)));
	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigSelectTitle(int, CJsonNode)), this, SLOT(SlotSelectTitle(int, CJsonNode)));
//	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigMenu(int, int)), this, SLOT(SlotOptionMenu(int, int)));
//	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigMenuAction(CJsonNode, int, int)), this, SLOT(SlotOptionMenuAction(CJsonNode, int, int)));
	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigSelectMenu(const QModelIndex&, QPoint)), this, SLOT(SlotSelectMenu(const QModelIndex&, QPoint)));

	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuAction(QAction*)));

}

void BrowserWindow::Initialize()
{
	m_FolderType = 0;
	SetBrowserMode(BROWSER_MODE_NORMAL);

	m_Dirs.clear();
	m_Files.clear();

	m_TopMenuMap.clear();
	m_OptionMenuMap.clear();
	m_SelectMap.clear();

	m_AlbumList.clear();
	m_AlbumArtistList.clear();
	m_ArtistList.clear();
	m_GenreList.clear();
	m_ComposerList.clear();
	m_MoodList.clear();

	m_Ext.clear();
	m_NodeUpnp.Clear();

	QString style = QString("QMenu::icon {	\
								padding: 0px 0px 0px 20px;	\
							}	\
							QMenu::item {	\
								width: 260px;	\
								height: 40px;	\
								color: rgb(90, 91, 94);	\
								font-size: 14pt;	\
								padding: 0px 20px 0px 20px;	\
							}	\
							QMenu::item:selected {	\
								background: rgba(201,237,248,255);	\
							}");

	m_Menu->setStyleSheet(style);
}

void BrowserWindow::SetCategoryList(QList<CJsonNode> list)
{
	CJsonNode temp = list.at(0);
	if (!temp.GetString(KEY_ALBUM).isEmpty())
	{
		m_AlbumList.clear();
		foreach (CJsonNode node, list)
		{
			m_AlbumList.append(node.GetString(KEY_ALBUM));
		}
	}
	else if (!temp.GetString(KEY_ALBUM_ARTIST).isEmpty())
	{
		m_AlbumArtistList.clear();
		foreach (CJsonNode node, list)
		{
			m_AlbumArtistList.append(node.GetString(KEY_ALBUM_ARTIST));
		}
	}
	else if (!temp.GetString(KEY_ARTIST).isEmpty())
	{
		m_ArtistList.clear();
		foreach (CJsonNode node, list)
		{
			m_ArtistList.append(node.GetString(KEY_ARTIST));
		}
	}
	else if (!temp.GetString(KEY_GENRE).isEmpty())
	{
		m_GenreList.clear();
		foreach (CJsonNode node, list)
		{
			m_GenreList.append(node.GetString(KEY_GENRE));
		}
	}
	else if (!temp.GetString(KEY_COMPOSER).isEmpty())
	{
		m_ComposerList.clear();
		foreach (CJsonNode node, list)
		{
			m_ComposerList.append(node.GetString(KEY_COMPOSER));
		}
	}
	else if (!temp.GetString(KEY_MOOD).isEmpty())
	{
		m_MoodList.clear();
		foreach (CJsonNode node, list)
		{
			m_MoodList.append(node.GetString(KEY_MOOD));
		}
	}
}

void BrowserWindow::SetSelectOffTopMenu()
{
	m_TopMenuMap.clear();

	if (BROWSER_MODE_COPY == m_BrowserMode
			|| BROWSER_MODE_MOVE == m_BrowserMode
			|| BROWSER_MODE_COPY_OPTION == m_BrowserMode
			|| BROWSER_MODE_MOVE_OPTION == m_BrowserMode)
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
				m_TopMenuMap.insert(TOP_MENU_MAKE_FOLDER, STR_MAKE_FOLDER);
			}

			if (BROWSER_MODE_COPY == m_BrowserMode
					|| BROWSER_MODE_COPY_OPTION == m_BrowserMode)
			{
				m_TopMenuMap.insert(TOP_MENU_COPY_HERE, STR_COPY_HERE);
			}
			else if (BROWSER_MODE_MOVE == m_BrowserMode
					 || BROWSER_MODE_MOVE_OPTION == m_BrowserMode)
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
		else if (m_FolderType & iFolderType_Mask_Net)
		{
//			LogDebug("select Net");
			m_TopMenuMap.insert(TOP_MENU_RELOAD, STR_RELOAD);
			m_TopMenuMap.insert(TOP_MENU_ADD_SHARE, STR_ADD_SHARE);

			m_pInfoBrowser->GetFormPlay()->ClearMenu();
			m_pInfoBrowser->GetFormPlay()->SetMenu(m_TopMenuMap);
		}
		else if (m_FolderType & iFolderType_Mask_Upnp)
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

			}

			m_pInfoBrowser->GetFormPlay()->ClearMenu();
			m_pInfoBrowser->GetFormPlay()->SetMenu(m_TopMenuMap);
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
				m_TopMenuMap.insert(TOP_MENU_SEARCH_COVERART, STR_SEARCH_COVERART);
			}
			if (m_FolderType & iFolderType_Mask_FileMgr)
			{
				m_TopMenuMap.insert(TOP_MENU_MAKE_FOLDER, STR_MAKE_FOLDER);
			}

			if (m_FolderType & iFolderType_Mask_Pls)
			{
				m_TopMenuMap.clear();
				if (m_FolderType & iFolderType_Mask_Song)
				{
					m_TopMenuMap.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
					m_TopMenuMap.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
					m_TopMenuMap.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
					m_TopMenuMap.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
				}
			}

			m_pInfoBrowser->GetFormPlay()->ClearMenu();
			m_pInfoBrowser->GetFormPlay()->SetMenu(m_TopMenuMap);
		}
	}
}

void BrowserWindow::SetSelectOnTopMenu()
{
	m_TopMenuMap.clear();

	if (BROWSER_MODE_COPY == m_BrowserMode
			|| BROWSER_MODE_MOVE == m_BrowserMode
			|| BROWSER_MODE_COPY_OPTION == m_BrowserMode
			|| BROWSER_MODE_MOVE_OPTION == m_BrowserMode)
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
				m_TopMenuMap.insert(TOP_MENU_MAKE_FOLDER, STR_MAKE_FOLDER);
			}
			if (BROWSER_MODE_COPY == m_BrowserMode
					|| BROWSER_MODE_COPY_OPTION == m_BrowserMode)
			{
				m_TopMenuMap.insert(TOP_MENU_COPY_HERE, STR_COPY_HERE);
			}
			else if (BROWSER_MODE_MOVE == m_BrowserMode
					 || BROWSER_MODE_MOVE_OPTION == m_BrowserMode)
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
		QString prefix = UtilNovatron::ConvertURLToFilename(m_Root);
		UtilNovatron::RemoveContainFilesInTempDirectory(prefix);

		RequestFolder(m_Root, m_Ext, m_NodeUpnp);
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
	}
}

void BrowserWindow::DoTopMenuMakeFolder()
{
	InputNameDialog dialog;
	if (dialog.exec() == QDialog::Accepted)
	{
		QString path = dialog.GetName();
		if (!m_Root.isEmpty())
			path = m_Root + "/" + path;

		m_pMgr->RequestCreate(path);

	}
}

void BrowserWindow::DoTopMenuDelete()
{
	SetPaths();

	m_pMgr->RequestDelete(m_Root, m_Paths, m_EventID);

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
	emit widget->SigAddWidget(widget, STR_BROWSER);
	widget->RequestRoot();
}

void BrowserWindow::DoTopMenuCopyHere(bool move)
{
	if (BROWSER_MODE_COPY == m_BrowserMode
			|| BROWSER_MODE_MOVE == m_BrowserMode)
	{
		emit SigCopyHere(move, m_Root);
	}
	else if (BROWSER_MODE_COPY_OPTION == m_BrowserMode
			 || BROWSER_MODE_MOVE_OPTION == m_BrowserMode)

	{
		emit SigCopyHere(move, m_Root, m_OptionPath, m_OptionType);
	}
	emit SigRemoveWidget(this);
}

void BrowserWindow::DoTopMenuSearchCoverArt()
{
	QString site;
	QString keyword;
	QString artist;

	SearchCoverArtDialog searchDialog;
	searchDialog.SetKeyWord(keyword);
	searchDialog.SetArtist(artist);
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

	if (site.contains(SEARCH_BROWSER))
	{
		BrowserDialog *dialog = new BrowserDialog(this, m_pMgr->GetAddr(), m_EventID);
		dialog->SetBrowserMode(BROWSER_MODE_COVER_ART_OPTION);
		dialog->DoBrowserHome();
		if (dialog->exec() == QDialog::Accepted)
		{
			DoBrowserSelectCoverart(dialog->GetPath());
		}

		if (dialog)
		{
			delete dialog;
			dialog = nullptr;
		}
	}
	else
	{
		SearchCoverArtResultDialog *resultDialog = new SearchCoverArtResultDialog(this);
		resultDialog->SetAddr(m_pMgr->GetAddr());
		resultDialog->RequestCoverArtList(site, keyword, artist);
		if (resultDialog->exec() == QDialog::Accepted)
		{
			ConfirmCoverArtDialog dialog;
			dialog.SetImagePath(resultDialog->GetImagePath());
			if (dialog.exec() == QDialog::Accepted)
			{
				QString image = resultDialog->GetImage();
				QString thumb = resultDialog->GetThumb();

				SetDirFile();
				m_pMgr->RequestSetArt(m_Root, m_Files, image, thumb, m_EventID);
			}
		}

		if (resultDialog)
		{
			delete resultDialog;
			resultDialog = nullptr;
		}
	}
}

void BrowserWindow::DoTopMenuAddShare()
{
//	LogDebug("do add share");
	AddShareDialog dialog;
	dialog.SetInfo(m_pMgr->GetAddr(), m_EventID);
	if (dialog.exec() == QDialog::Accepted)
	{
		CJsonNode node = dialog.GetNodeInfo();
		m_pMgr->RequestSMBSet(m_EventID, node);
	}
}

void BrowserWindow::SetOptionMenu(int type)
{
	UtilNovatron::DebugTypeForBrowser("SetOptionMenu", type);
	m_OptionMenuMap.clear();

	if (BROWSER_MODE_COPY_OPTION == m_BrowserMode
			|| BROWSER_MODE_MOVE_OPTION == m_BrowserMode)
	{
//		if (BROWSER_MODE_COPY_OPTION == m_BrowserMode)
//		{
//			m_OptionMenuMap.insert(OPTION_MENU_COPY_HERE, STR_COPY_HERE);
//		}
//		else if (BROWSER_MODE_MOVE_OPTION == m_BrowserMode)
//		{
//			m_OptionMenuMap.insert(OPTION_MENU_MOVE_HERE, STR_MOVE_HERE);
//		}
	}
	else
	{
		if ((type & iFolderType_Mask_Play_Option
				|| type & iFolderType_Mask_Play_Check) && !(type & iFolderType_Mask_Upnp))
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

		if (type & iFolderType_Mask_Dir)
		{
			if (type & iFolderType_Mask_Scan)
			{
				m_OptionMenuMap.insert(OPTION_MENU_SCAN_OFF, STR_SCAN_OFF);
			}
			else
			{
				m_OptionMenuMap.insert(OPTION_MENU_SCAN_ON, STR_SCAN_ON);
			}
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

		if ((type & iFolderType_Mask_Play_Select) && !(type & iFolderType_Mask_Upnp))
		{
			m_OptionMenuMap.insert(OPTION_MENU_EDIT_TAG, STR_EDIT_TAG);
			m_OptionMenuMap.insert(OPTION_MENU_SEARCH_COVERART, STR_SEARCH_COVERART);
		}

		if (type & iFolderType_Mask_Pls || type & iFolderType_Mask_Upnp)
		{
			if (type & iFolderType_Mask_Song)
			{
				m_OptionMenuMap.insert(OPTION_MENU_PLAY_NOW, STR_PLAY_NOW);
				m_OptionMenuMap.insert(OPTION_MENU_PLAY_LAST, STR_PLAY_LAST);
				m_OptionMenuMap.insert(OPTION_MENU_PLAY_NEXT, STR_PLAY_NEXT);
				m_OptionMenuMap.insert(OPTION_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
			}
		}

		if (type & iFolderType_Mask_Net)
		{
			m_OptionMenuMap.insert(OPTION_MENU_MODIFY_SHARE, STR_MODIFY_SHARE);
			m_OptionMenuMap.insert(OPTION_MENU_DELETE_SHARE, STR_DELETE_SHARE);
		}

	}

//	m_pListBrowser->GetDelegate()->SetOptionMenuMap(m_OptionMenuMap);

}

void BrowserWindow::DoOptionMenuPlay(CJsonNode node, int type, int where)
{
	if (iFolderType_Mask_Pls & type)
	{
//		LogDebug("Pls node [%s]", node.ToCompactByteArray().data());
		m_Files.clear();
		m_Files.append(node.GetString(KEY_ID_LOWER));
		m_pMgr->RequestPlaylistPlay(m_Root, m_Files, where);
	}
	else if (iFolderType_Mask_Upnp & type)
	{
//		LogDebug("play upnp [%s]", node.ToCompactByteArray().data());
		CJsonNode itemArr(JSON_ARRAY);
		itemArr.AppendArray(node);

		CJsonNode tempNode(JSON_OBJECT);
		tempNode.Add(KEY_ITEMS, itemArr);
		m_pMgr->RequestUpnpPlay(tempNode, m_EventID, where);
	}
	else
	{
//		LogDebug("Play_Select node [%s]", node.ToCompactByteArray().data());
		QString path = node.GetString(KEY_PATH);
		QStringList dirs;
		QStringList files;
		SetOptionDirFile(path, type, dirs, files);

		m_pMgr->RequestTrackPlay(m_Root, dirs, files, where);
	}
}

void BrowserWindow::DoOptionMenuGain(QString path, int type, QString gain)
{
	QStringList paths;
	SetOptionPaths(path, type, paths);

	m_pMgr->RequestReplayGain(m_Root, paths, gain, m_EventID);
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

void BrowserWindow::DoOptionMenuConverFormat(QString path, int type)
{
	SelectFormatDialog dialog;
	if (dialog.exec() == QDialog::Accepted)
	{
		QString format = dialog.GetFormat();

		QStringList paths;
		SetOptionPaths(path, type, paths);

		m_pMgr->RequestConvertFormat(m_Root, paths, format, m_EventID);
	}
}

void BrowserWindow::DoOptionMenuScan(QString path, bool scanOn)
{
	if (!m_Root.isEmpty())
		path = m_Root + "/" + path;

	if (scanOn)
	{
		m_pMgr->RequestScanDB(path, m_EventID);
	}
	else
	{
		m_pMgr->RequestRemoveDB(path, m_EventID);
	}
}

void BrowserWindow::DoOptionMenuRename(QString path)
{
	InputNameDialog dialog;
	if (dialog.exec() == QDialog::Accepted)
	{
		if (!m_Root.isEmpty())
			path = m_Root + "/" + path;

		QString dst = dialog.GetName();
		if (dst.isEmpty())
		{
			LogWarning("please input name");
		}
		else
		{
			dst = "/" + dst;
		}

		m_pMgr->RequestRename(path, dst);

	}
}

void BrowserWindow::DoOptionMenuDelete(QString path, int type)
{
	QStringList paths;
	SetOptionPaths(path, type, paths);

	m_pMgr->RequestDelete(m_Root, paths, m_EventID);
}

void BrowserWindow::DoOptionMenuCopy(QString path, int type, bool move)
{
	int mode = BROWSER_MODE_MAX;
	QString title;
	if (move)
	{
		mode = BROWSER_MODE_MOVE_OPTION;
		title = STR_MOVE;
	}
	else
	{
		mode = BROWSER_MODE_COPY_OPTION;
		title = STR_COPY;
	}

	BrowserWindow *widget = new BrowserWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->SetBrowserMode(mode, path, type);
	emit widget->SigAddWidget(widget, STR_BROWSER);
	widget->RequestRoot();
}

//void BrowserWindow::DoOptionMenuCopyHere(QString path, bool move)
//{
//	if (!m_Root.isEmpty())
//		path = m_Root + "/" + path;

//	emit SigOptionCopyHere(move, path, m_OptionPath, m_OptionType);
//	emit SigRemoveWidget(this);
//}

void BrowserWindow::DoOptionMenuEditTag(QString path, int type)
{
	if (!m_Root.isEmpty())
		path = m_Root + "/" + path;

	m_OptionPath = path;

	m_pMgr->RequestCategoryList(SQLManager::CATEGORY_ALBUM);
	m_pMgr->RequestCategoryList(SQLManager::CATEGORY_ALBUM_ARTIST);
	m_pMgr->RequestCategoryList(SQLManager::CATEGORY_ARTIST);
	m_pMgr->RequestCategoryList(SQLManager::CATEGORY_GENRE);
	m_pMgr->RequestCategoryList(SQLManager::CATEGORY_COMPOSER);
	m_pMgr->RequestCategoryList(SQLManager::CATEGORY_MOOD);

	m_pMgr->RequestInfoTag(path);
}

void BrowserWindow::DoOptionMenuSearchCoverArt(QString path, int type)
{
	QString site;
	QString keyword;
	QString artist;

	int count = m_pListBrowser->GetModel()->rowCount();
	for (int i = 0; i < count; i++)
	{
		QModelIndex modelIndex = m_pListBrowser->GetModel()->index(i, 0);
		QString title = qvariant_cast<QString>(modelIndex.data(ListBrowserDelegate::LIST_BROWSER_TITLE));
		if (!path.compare(title))
		{
			keyword = title;
			artist = qvariant_cast<QString>(modelIndex.data(ListBrowserDelegate::LIST_BROWSER_SUBTITLE));
			break;
		}
	}

	SearchCoverArtDialog searchDialog;
	searchDialog.SetKeyWord(keyword);
	searchDialog.SetArtist(artist);
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

	if (site.contains(SEARCH_BROWSER))
	{
		BrowserDialog *dialog = new BrowserDialog(this, m_pMgr->GetAddr(), m_EventID);
		dialog->SetBrowserMode(BROWSER_MODE_COVER_ART_OPTION);
		dialog->DoBrowserHome();
		if (dialog->exec() == QDialog::Accepted)
		{
			DoBrowserSelectCoverart(dialog->GetPath());
		}

		if (dialog)
		{
			delete dialog;
			dialog = nullptr;
		}
	}
	else
	{
		SearchCoverArtResultDialog *resultDialog = new SearchCoverArtResultDialog(this);
		resultDialog->SetAddr(m_pMgr->GetAddr());
		resultDialog->RequestCoverArtList(site, keyword, artist);
		if (resultDialog->exec() == QDialog::Accepted)
		{
			ConfirmCoverArtDialog dialog;
			dialog.SetImagePath(resultDialog->GetImagePath());
			if (dialog.exec() == QDialog::Accepted)
			{
				QString image = resultDialog->GetImage();
				QString thumb = resultDialog->GetThumb();

				QStringList dirs;
				QStringList files;
				SetOptionDirFile(path, type, dirs, files);

				m_pMgr->RequestSetArt(m_Root, files, image, thumb, m_EventID);
			}
		}

		if (resultDialog)
		{
			delete resultDialog;
			resultDialog = nullptr;
		}
	}
}

void BrowserWindow::DoOptionMenuModifyShare(QString path)
{
	m_pMgr->RequestSMBInfo(m_EventID, path);

}

void BrowserWindow::DoOptionMenuDeleteShare(QString path)
{
	m_pMgr->RequestSMBDelete(m_EventID, path);
}

void BrowserWindow::ShowFormPlay(bool show)
{
	m_pInfoBrowser->GetFormPlay()->ShowPlayAll(show);
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

void BrowserWindow::SetOptionDirFile(QString path, int type, QStringList &dirs, QStringList &files)
{
	if (iFolderType_Mask_Dir & type)
	{
		dirs.append(path);
	}
	else if (iFolderType_Mask_Song & type)
	{
		files.append(path);
	}
}

void BrowserWindow::SetOptionPaths(QString path, int type, QStringList &paths)
{
	QStringList dirs;
	QStringList files;
	SetOptionDirFile(path, type, dirs, files);

	paths.clear();
	for (int i = 0; i < dirs.count(); i++)
	{
		paths.append(dirs.at(i));
	}
	for (int i = 0; i < files.count(); i++)
	{
		paths.append(files.at(i));
	}
}

void BrowserWindow::DoBrowserSelectCoverart(QString path)
{
	QString image = "";
	QStringList lsAddr = m_pMgr->GetAddr().split(":");
	QString thumb = QString("http://%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(SRC_BROWSER).arg(path);
	QString tempThumb = UtilNovatron::ConvertURLToFilenameWithExtension(thumb);
	tempThumb = UtilNovatron::GetTempDirectory() + "/" + tempThumb;

	ConfirmCoverArtDialog dialog;
	dialog.SetImagePath(tempThumb);
	if (dialog.exec() == QDialog::Accepted)
	{
		SetDirFile();
		m_pMgr->RequestSetArt(m_Root, m_Files, image, thumb, m_EventID);
	}
}

void BrowserWindow::DoBrowserOptionSelectCoverart(QString path)
{
	QString image = "";
	QStringList lsAddr = m_pMgr->GetAddr().split(":");
	QString thumb = QString("http://%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(SRC_BROWSER).arg(path);
	QString tempThumb = UtilNovatron::ConvertURLToFilenameWithExtension(thumb);
	tempThumb = UtilNovatron::GetTempDirectory() + "/" + tempThumb;

	ConfirmCoverArtDialog dialog;
	dialog.SetImagePath(tempThumb);
	if (dialog.exec() == QDialog::Accepted)
	{
		QStringList dirs;
		QStringList files;
		SetOptionDirFile(path, iFolderType_Mask_Song, dirs, files);

		m_pMgr->RequestSetArt(m_Root, files, image, thumb, m_EventID);
	}
}
