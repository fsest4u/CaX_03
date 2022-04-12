#include <QThread>

#include "browserwidget.h"
#include "ui_browserwidget.h"

#include "widget/browser.h"

#include "dialog/commondialog.h"

#include "manager/browsermanager.h"

#include "util/log.h"
#include "util/settingio.h"
#include "util/utilnovatron.h"

#include "widget/formBottom/iconservice.h"
#include "widget/formBottom/iconservicedelegate.h"
#include "widget/formBottom/listbrowser.h"
#include "widget/formBottom/listbrowserdelegate.h"

const QString SETTINGS_GROUP = "Browser";

BrowserWidget::BrowserWidget(QWidget *parent, const QString &addr, const int &eventID, const QString &root) :
	QWidget(parent),
	m_pMgr(new BrowserManager),
	m_pIconService(new IconService(this)),
	m_pListBrowser(new ListBrowser(this)),
	m_Menu(new QMenu(this)),
	m_Root(root),
	m_EventID(eventID),
	ui(new Ui::BrowserWidget)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();

	Initialize();
}

BrowserWidget::~BrowserWidget()
{
	if (m_pMgr)
	{
		delete m_pMgr;
		m_pMgr = nullptr;
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


void BrowserWidget::RequestRoot(QString ext)
{
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

void BrowserWidget::RequestFolder(QString strPath, QString ext, CJsonNode upnp)
{
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

int BrowserWidget::GetFolderType() const
{
	return m_FolderType;
}

void BrowserWidget::SetFolderType(int FolderType)
{
	m_FolderType = FolderType;
}

int BrowserWidget::GetBrowserMode() const
{
	return m_BrowserMode;
}

void BrowserWidget::SetBrowserMode(int BrowserMode, QString optionPath, int optionType)
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

void BrowserWidget::SlotAddWidget(QWidget *widget)
{
	emit SigAddWidget(widget);		// recursive
}

void BrowserWidget::SlotRemoveWidget(QWidget *widget)
{
	emit SigRemoveWidget(widget);
}

void BrowserWidget::SlotSelectCoverart(QString path)
{
	emit SigSelectCoverart(path);
}

void BrowserWidget::SlotIconSelectTitle(int nType, QString rawData)
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

		BrowserWidget *widget = new BrowserWidget(this, m_pMgr->GetAddr(), m_EventID, path);

//		if (iFolderType_Mask_Play_Top & nType
//				&& BROWSER_MODE_NORMAL == m_BrowserMode)
//		{
//			widget->ShowFormPlay(true);
//		}
		widget->SetBrowserMode(m_BrowserMode, m_OptionPath, m_OptionType);
		widget->SetFolderType(nType);
		widget->RequestFolder(path, m_Ext);
		emit widget->SigAddWidget(widget);
	}
}

void BrowserWidget::SlotSelectTitle(int nType, CJsonNode node)
{
	UtilNovatron::DebugTypeForBrowser("SlotSelectTitle", nType);
//	LogDebug("node [%s]", node.ToCompactByteArray().data());


	if (iFolderType_Mask_Song & nType)
	{
//		SlotSelectTrackPlay(nType, node);
	}
	else if (iFolderType_Mask_Net & nType
			 || iFolderType_Mask_Upnp & nType
			 || iFolderType_Mask_Sub & nType)
	{
		QString path = node.GetString(KEY_PATH);
		if (!m_Root.isEmpty())
			path = m_Root + "/" + path;

		BrowserWidget *widget = new BrowserWidget(this, m_pMgr->GetAddr(), m_EventID, path);

//		if (iFolderType_Mask_Play_Top & nType
//				&& BROWSER_MODE_NORMAL == m_BrowserMode)
//		{
//			widget->ShowFormPlay(true);
//		}
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
		widget->RequestFolder(path, m_Ext, m_NodeUpnp);
		emit widget->SigAddWidget(widget);
	}
	else if (iFolderType_Mask_File & nType && !m_Ext.isEmpty())
	{
//		QString path = node.GetString(KEY_PATH);
//		if (!m_Root.isEmpty())
//			path = m_Root + "/" + path;

//		emit SigBrowserPath(path);
//		emit SigRemoveWidget(this);
	}
}

void BrowserWidget::SlotSelectMenu(const QModelIndex &modelIndex, QPoint point)
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

void BrowserWidget::SlotMenuAction(QAction *action)
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

void BrowserWidget::SlotReqCoverArt(QString path, int index)
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

void BrowserWidget::SlotReqInfoBot(QString path, int nIndex)
{
	if (!m_Root.isEmpty())
		path = m_Root + "/" + path;

	m_pMgr->RequestInfoBot(path, nIndex);
}

void BrowserWidget::SlotRespError(QString errMsg)
{
	if (BROWSER_MODE_NORMAL == m_BrowserMode)
	{
		LogWarning("error [%s]", errMsg.toUtf8().data());
		if (errMsg.compare(STR_NO_RESULT))
		{
			CommonDialog dialog(this, STR_WARNING, errMsg);
			dialog.exec();
		}

//		ui->gridLayoutTop->addWidget(m_pInfoBrowser);

//		ShowFormPlay(false);
//		m_pInfoBrowser->GetFormPlay()->ShowMenu();
//		m_pInfoBrowser->SetCoverArt("");
//		m_pInfoBrowser->SetTitle(m_Root);
	}
	else
	{
//		ui->gridLayoutTop->addWidget(m_pInfoBrowser);

//		m_pInfoBrowser->GetFormPlay()->ShowMenu();
//		m_pInfoBrowser->SetCoverArt("");
//		m_pInfoBrowser->SetTitle(m_Root);
	}
}

void BrowserWidget::SlotRespList(QList<CJsonNode> list)
{
	int nType =  list[0].GetInt(KEY_TYPE);
	UtilNovatron::DebugTypeForBrowser("SlotRespList", nType);

	if (iFolderType_Mask_Hdd & nType)
	{
//		ui->gridLayoutTop->addWidget(m_pInfoService);
//		ui->gridLayoutBottom->addWidget(m_pIconService);

//		m_pInfoService->GetFormPlay()->ShowMenu();
//		m_pInfoService->SetTitle(STR_BROWSER);

		ui->gridLayoutFrame->addWidget(m_pIconService);

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
//		ui->gridLayoutTop->addWidget(m_pInfoBrowser);
//		ui->gridLayoutBottom->addWidget(m_pListBrowser);

//		m_pInfoBrowser->GetFormPlay()->ShowMenu();
////		m_pInfoBrowser->GetFormSort()->ShowResize();

//		m_pInfoBrowser->SetCoverArt(list[0].GetString(KEY_COVER_ART));
//		m_pInfoBrowser->SetTitle(m_Root);

		ui->gridLayout->addWidget(m_pListBrowser);

		m_pListBrowser->ClearNodeList();
		nType = m_pListBrowser->SetNodeList(list, SIDEMENU_BROWSER);
	}

//	ShowFormPlay(true);
	SetFolderType(nType);
}

void BrowserWidget::SlotCoverArtUpdate(QString filename, int index, int mode)
{
	Q_UNUSED(mode);

	QStandardItem *itemIcon = m_pListBrowser->GetModel()->item(index);
	itemIcon->setData(filename, ListBrowserDelegate::LIST_BROWSER_COVER);
	m_pListBrowser->GetModel()->setItem(index, itemIcon);
}

void BrowserWidget::SlotInfoBotUpdate(CJsonNode node, int index)
{
	if (index < 0)
		return;

	int seconds = node.GetInt(KEY_DURATION);
	QString hhmmss = UtilNovatron::CalcSecondToHMS(seconds);

	QModelIndex modelIndex = m_pListBrowser->GetModel()->index(index, 0);
	m_pListBrowser->GetModel()->setData(modelIndex, node.GetString(KEY_BOT), ListBrowserDelegate::LIST_BROWSER_SUBTITLE);
	m_pListBrowser->GetModel()->setData(modelIndex, hhmmss, ListBrowserDelegate::LIST_BROWSER_DURATION);
}

void BrowserWidget::SlotInfoTagUpdate(CJsonNode node)
{
	// nothing
}

void BrowserWidget::SlotRespCategoryList(QList<CJsonNode> list)
{
	SetCategoryList(list);
}

void BrowserWidget::SlotListUpdate()
{
	QThread::sleep(1);
	// refresh
	DoTopMenuReload();
}

void BrowserWidget::SlotRespSMBInfo(CJsonNode node)
{
	// nothing
}

void BrowserWidget::SlotOptionMenuAction(CJsonNode node, int type, int menuID)
{
	QString path = node.GetString(KEY_PATH);
	LogDebug("click option menu node [%s] [%d] [%d]", node.ToCompactByteArray().data(), type, menuID);
	switch (menuID) {
//	case OPTION_MENU_OPTION_PLAY_SUBDIR:
//		DoOptionMenuOptionPlaySubDir();
//		break;
//	case OPTION_MENU_PLAY_NOW:
//		DoOptionMenuPlay(node, type, PLAY_NOW);
//		break;
//	case OPTION_MENU_PLAY_LAST:
//		DoOptionMenuPlay(node, type, PLAY_LAST);
//		break;
//	case OPTION_MENU_PLAY_NEXT:
//		DoOptionMenuPlay(node, type, PLAY_NEXT);
//		break;
//	case OPTION_MENU_PLAY_CLEAR:
//		DoOptionMenuPlay(node, type, PLAY_CLEAR);
//		break;
//	case OPTION_MENU_GAIN_SET:
//		DoOptionMenuGain(path, type, VAL_SET);
//		break;
//	case OPTION_MENU_GAIN_CLEAR:
//		DoOptionMenuGain(path, type, VAL_CLEAR);
//		break;
//	case OPTION_MENU_SCAN_ON:
//		DoOptionMenuScan(path, true);
//		break;
//	case OPTION_MENU_SCAN_OFF:
//		DoOptionMenuScan(path, false);
//		break;
//	case OPTION_MENU_CONVERT_FORMAT:
//		DoOptionMenuConverFormat(path, type);
//		break;
//	case OPTION_MENU_RENAME:
//		DoOptionMenuRename(path);
//		break;
//	case OPTION_MENU_DELETE:
//		DoOptionMenuDelete(path, type);
//		break;
//	case OPTION_MENU_OPTION_OVERWRITE:
//		DoOptionMenuOptionOverwrite();
//		break;
//	case OPTION_MENU_MOVE:
//		DoOptionMenuCopy(path, type, true);
//		break;
//	case OPTION_MENU_COPY:
//		DoOptionMenuCopy(path, type, false);
//		break;
////	case OPTION_MENU_MOVE_HERE:
////		DoOptionMenuCopyHere(path, true);
////		break;
////	case OPTION_MENU_COPY_HERE:
////		DoOptionMenuCopyHere(path, false);
////		break;
//	case OPTION_MENU_EDIT_TAG:
//		DoOptionMenuEditTag(path, type);
//		break;
//	case OPTION_MENU_SEARCH_COVERART:
//		DoOptionMenuSearchCoverArt(path, type);
//		break;
//	case OPTION_MENU_MODIFY_SHARE:
//		DoOptionMenuModifyShare(path);
//		break;
//	case OPTION_MENU_DELETE_SHARE:
//		DoOptionMenuDeleteShare(path);
//		break;
	case OPTION_MENU_SELECT_COVERART:
		DoOptionMenuSelectCoverart(path);
		break;
	}
}

void BrowserWidget::ReadSettings()
{
	SettingIO settings;
	settings.beginGroup(SETTINGS_GROUP);

	m_pMgr->SetOptPlaySubDir(settings.value("option_play_sub_dir").toBool());
	m_pMgr->SetOptOverwrite(settings.value("option_overwrite").toBool());

	settings.endGroup();
}

void BrowserWidget::WriteSettings()
{
	SettingIO settings;
	settings.beginGroup(SETTINGS_GROUP);

	settings.setValue("option_play_sub_dir", m_pMgr->GetOptPlaySubDir());
	settings.setValue("option_overwrite", m_pMgr->GetOptOverwrite());

	settings.endGroup();
}

void BrowserWidget::ConnectSigToSlot()
{
	connect(this, SIGNAL(SigAddWidget(QWidget*)), parent(), SLOT(SlotAddWidget(QWidget*)));		// recursive
	connect(this, SIGNAL(SigRemoveWidget(QWidget*)), parent(), SLOT(SlotRemoveWidget(QWidget*)));
//////	connect(this, SIGNAL(SigCopyHere(bool, QString, QString, int)), parent(), SLOT(SlotCopyHere(bool, QString, QString, int)));
//	connect(this, SIGNAL(SigOptionCopyHere(bool, QString, QString, int)), parent(), SLOT(SlotOptionCopyHere(bool, QString, QString, int)));
//////	connect(this, SIGNAL(SigBrowserPath(QString)), parent(), SLOT(SlotBrowserPath(QString)));
	connect(this, SIGNAL(SigSelectCoverart(QString)), parent(), SLOT(SlotSelectCoverart(QString)));

	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespList(QList<CJsonNode>)), this, SLOT(SlotRespList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));
	connect(m_pMgr, SIGNAL(SigInfoBotUpdate(CJsonNode, int)), this, SLOT(SlotInfoBotUpdate(CJsonNode, int)));
	connect(m_pMgr, SIGNAL(SigInfoTagUpdate(CJsonNode)), this, SLOT(SlotInfoTagUpdate(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespCategoryList(QList<CJsonNode>)), this, SLOT(SlotRespCategoryList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigListUpdate()), this, SLOT(SlotListUpdate()));
	connect(m_pMgr, SIGNAL(SigRespSMBInfo(CJsonNode)), this, SLOT(SlotRespSMBInfo(CJsonNode)));

//	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigMenu()), this, SLOT(SlotTopMenu()));
//	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigMenuAction(int)), this, SLOT(SlotTopMenuAction(int)));

//	connect(m_pInfoBrowser->GetFormPlay(), SIGNAL(SigPlayAll()), this, SLOT(SlotPlayAll()));
//	connect(m_pInfoBrowser->GetFormPlay(), SIGNAL(SigPlayRandom()), this, SLOT(SlotPlayRandom()));
//	connect(m_pInfoBrowser->GetFormPlay(), SIGNAL(SigMenu()), this, SLOT(SlotTopMenu()));
//	connect(m_pInfoBrowser->GetFormPlay(), SIGNAL(SigMenuAction(int)), this, SLOT(SlotTopMenuAction(int)));
////	connect(m_pInfoBrowser->GetFormSort(), SIGNAL(SigResize()), this, SLOT(SlotResize()));

//	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectPlay(int)), this, SLOT(SlotSelectPlay(int)));
//	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int)), this, SLOT(SlotSelectTitle(int)));
	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotIconSelectTitle(int, QString)));

	connect(m_pListBrowser, SIGNAL(SigReqCoverArt(QString, int)), this, SLOT(SlotReqCoverArt(QString, int)));
	connect(m_pListBrowser, SIGNAL(SigReqInfoBot(QString, int)), this, SLOT(SlotReqInfoBot(QString, int)));
//////	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigSelectPlay(int, CJsonNode)), this, SLOT(SlotSelectTrackPlay(int, CJsonNode)));
	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigSelectTitle(int, CJsonNode)), this, SLOT(SlotSelectTitle(int, CJsonNode)));
//	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigMenu(int, int)), this, SLOT(SlotOptionMenu(int, int)));
//	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigMenuAction(CJsonNode, int, int)), this, SLOT(SlotOptionMenuAction(CJsonNode, int, int)));
	connect(m_pListBrowser->GetDelegate(), SIGNAL(SigSelectMenu(const QModelIndex&, QPoint)), this, SLOT(SlotSelectMenu(const QModelIndex&, QPoint)));

	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuAction(QAction*)));
}

void BrowserWidget::Initialize()
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
								width: 300px;	\
								height: 40px;	\
								color: rgb(90, 91, 94);	\
								font-size: 12pt;	\
								padding: 0px 20px 0px 20px;	\
							}	\
							QMenu::item:selected {	\
								background: rgba(201,237,248,255);	\
							}");

	m_Menu->setStyleSheet(style);
}

void BrowserWidget::SetCategoryList(QList<CJsonNode> list)
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

void BrowserWidget::DoTopMenuReload()
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

void BrowserWidget::SetOptionMenu(int type)
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
	else if (BROWSER_MODE_COVER_ART_OPTION == m_BrowserMode)
	{
		if (type & iFolderType_Mask_Song)
		{
			m_OptionMenuMap.insert(OPTION_MENU_SELECT_COVERART, STR_SELECT_COVERART);
		}
	}
//	else
//	{
//		if ((type & iFolderType_Mask_Play_Option
//				|| type & iFolderType_Mask_Play_Check) && !(type & iFolderType_Mask_Upnp))
//		{
//			QString option = m_pMgr->GetOptPlaySubDir() ? STR_ON : STR_OFF;
//			m_OptionMenuMap.insert(OPTION_MENU_OPTION_PLAY_SUBDIR, QString("%1 - %2").arg(STR_OPTION_PLAY_SUBDIR).arg(option));
//			m_OptionMenuMap.insert(OPTION_MENU_PLAY_NOW, STR_PLAY_NOW);
//			m_OptionMenuMap.insert(OPTION_MENU_PLAY_LAST, STR_PLAY_LAST);
//			m_OptionMenuMap.insert(OPTION_MENU_PLAY_NEXT, STR_PLAY_NEXT);
//			m_OptionMenuMap.insert(OPTION_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
//			m_OptionMenuMap.insert(OPTION_MENU_GAIN_SET, STR_GAIN_SET);
//			m_OptionMenuMap.insert(OPTION_MENU_GAIN_CLEAR, STR_GAIN_CLEAR);
//			m_OptionMenuMap.insert(OPTION_MENU_CONVERT_FORMAT, STR_CONVERT_FORMAT);
//		}

//		if (type & iFolderType_Mask_Dir)
//		{
//			if (type & iFolderType_Mask_Scan)
//			{
//				m_OptionMenuMap.insert(OPTION_MENU_SCAN_OFF, STR_SCAN_OFF);
//			}
//			else
//			{
//				m_OptionMenuMap.insert(OPTION_MENU_SCAN_ON, STR_SCAN_ON);
//			}
//		}

//		if (type & iFolderType_Mask_FileMgr)
//		{
//			QString option = m_pMgr->GetOptOverwrite() ? STR_ON : STR_OFF;
//			m_OptionMenuMap.insert(OPTION_MENU_RENAME, STR_RENAME);
//			m_OptionMenuMap.insert(OPTION_MENU_DELETE, STR_DELETE);
//			m_OptionMenuMap.insert(OPTION_MENU_OPTION_OVERWRITE, QString("%1 - %2").arg(STR_OPTION_OVERWRITE).arg(option));
//			m_OptionMenuMap.insert(OPTION_MENU_MOVE, STR_MOVE);
//			m_OptionMenuMap.insert(OPTION_MENU_COPY, STR_COPY);
//		}

//		if ((type & iFolderType_Mask_Play_Select) && !(type & iFolderType_Mask_Upnp))
//		{
//			m_OptionMenuMap.insert(OPTION_MENU_EDIT_TAG, STR_EDIT_TAG);
//			m_OptionMenuMap.insert(OPTION_MENU_SEARCH_COVERART, STR_SEARCH_COVERART);
//		}

//		if (type & iFolderType_Mask_Pls || type & iFolderType_Mask_Upnp)
//		{
//			if (type & iFolderType_Mask_Song)
//			{
//				m_OptionMenuMap.insert(OPTION_MENU_PLAY_NOW, STR_PLAY_NOW);
//				m_OptionMenuMap.insert(OPTION_MENU_PLAY_LAST, STR_PLAY_LAST);
//				m_OptionMenuMap.insert(OPTION_MENU_PLAY_NEXT, STR_PLAY_NEXT);
//				m_OptionMenuMap.insert(OPTION_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
//			}
//		}

//		if (type & iFolderType_Mask_Net)
//		{
//			m_OptionMenuMap.insert(OPTION_MENU_MODIFY_SHARE, STR_MODIFY_SHARE);
//			m_OptionMenuMap.insert(OPTION_MENU_DELETE_SHARE, STR_DELETE_SHARE);
//		}

	//	}
}

void BrowserWidget::DoOptionMenuSelectCoverart(QString path)
{
	LogDebug("root [%s] path [%s]", m_Root.toUtf8().data(), path.toUtf8().data());
	if (!m_Root.isEmpty())
		path = m_Root + "/" + path;

	emit SigSelectCoverart(path);
}

void BrowserWidget::AnalyzeNode(CJsonNode node)
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

void BrowserWidget::SetDirFile()
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

void BrowserWidget::SetPaths()
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

void BrowserWidget::SetOptionDirFile(QString path, int type, QStringList &dirs, QStringList &files)
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

void BrowserWidget::SetOptionPaths(QString path, int type, QStringList &paths)
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
