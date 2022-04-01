#include "audiocdwindow.h"
#include "ui_audiocdwindow.h"

#include "dialog/cdripinfodialog.h"
#include "dialog/changemetainfodialog.h"
#include "dialog/commondialog.h"
#include "dialog/trackinfodialog.h"
#include "dialog/trackinfo.h"

#include "manager/audiocdmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/caxtranslate.h"
#include "util/log.h"
#include "util/settingio.h"
#include "util/utilnovatron.h"

#include "widget/form/formplay.h"
#include "widget/form/formsort.h"

#include "widget/formTop/infotracks.h"
#include "widget/formBottom/icontracks.h"
#include "widget/formBottom/icontracksdelegate.h"
#include "widget/formBottom/listtracks.h"
#include "widget/formBottom/listtracksdelegate.h"

const QString SETTINGS_GROUP = "AudioCD";

AudioCDWindow::AudioCDWindow(QWidget *parent, const QString &addr, const int &eventID) :
	QWidget(parent),
	m_pMgr(new AudioCDManager),
	m_pInfoTracks(new InfoTracks(this)),
	m_pIconTracks(new IconTracks(this)),
	m_pListTracks(new ListTracks(this)),
	m_Menu(new QMenu(this)),
	m_EventID(eventID),
	m_TotalCount(""),
	m_TotalTime(""),
	m_Format(""),
	m_Date(""),
	ui(new Ui::AudioCDWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
	ReadSettings();
	Initialize();
}

AudioCDWindow::~AudioCDWindow()
{
//	WriteSettings();

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

	disconnect(m_Menu, SIGNAL(triggered(QAction*)));
	if (m_Menu)
	{
		delete m_Menu;
		m_Menu = nullptr;
	}

	delete ui;

}

void AudioCDWindow::AddWidgetAudioCDHome()
{
	m_ListMode = GetListModeFromResize(m_ResizeTrack);

	ui->gridLayoutTop->addWidget(m_pInfoTracks);
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pInfoTracks->GetFormSort()->SetResize(m_ResizeTrack);
		ui->gridLayoutBottom->addWidget(m_pIconTracks);
	}
	else
	{
		m_pInfoTracks->GetFormSort()->SetResize(m_ResizeTrack);
		ui->gridLayoutBottom->addWidget(m_pListTracks);
	}
}

void AudioCDWindow::RequestTrackList()
{
	m_Loading = UtilNovatron::LoadingStart(parentWidget());

	m_pMgr->RequestTrackList();
}

void AudioCDWindow::SlotRespTrackList(CJsonNode node)
{
//	LogDebug("node : [%s]", node.ToCompactByteArray().data());
	CJsonNode result = node.GetArray(VAL_RESULT);
	if (result.ArraySize() <= 0)
	{
//		emit SigRespError(message.left(MSG_LIMIT_COUNT));
		return;
	}

	m_RespList.clear();
	for (int i = 0; i < result.ArraySize(); i++)
	{
		m_RespList.append(result.GetArrayAt(i));
//		LogDebug("node : [%s]", m_RespList[i].ToCompactByteArray().data());
	}

	CJsonNode track = m_RespList.at(0);
//	LogDebug("track [%s]", track.ToCompactByteArray().data());

	QString subtitle = track.GetString(KEY_BOT);
	QString art = track.GetString(KEY_ART);
	if (art.isEmpty())
	{
		m_pInfoTracks->SetCoverArt(art);
	}
	else
	{
		m_pMgr->RequestCoverArt(art);
	}
	int index = 0;
	if (node.GetInt(KEY_SOURCE_INDEX, index))
	{
		m_SourceList = node.GetStringList(KEY_SOURCE);
		QString source = m_SourceList[index];
		QStringList tempList = source.split(" : ");
		if (tempList.count() > 1)
		{
			m_pInfoTracks->SetTitle(tempList[1]);
		}
		else
		{
			m_pInfoTracks->SetTitle(source);
		}
	}
	else
	{
		m_pInfoTracks->SetTitle(node.GetString(KEY_SOURCE));
	}

	foreach(CJsonNode tempNode, m_RespList)
	{
		QString bot = tempNode.GetString(KEY_BOT);
		if (!bot.contains(subtitle))
		{
			subtitle = "Various Artists";
			break;
		}
	}
	m_pInfoTracks->SetSubtitle(subtitle);

//	m_pInfoTracks->SetInfo(MakeInfo());

	SetOptionMenu();

	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pIconTracks->ClearNodeList();
		m_pIconTracks->SetNodeList(m_RespList, SIDEMENU_AUDIO_CD);
	}
	else
	{
		m_pListTracks->ClearNodeList();
		m_pListTracks->SetNodeList(m_RespList, SIDEMENU_AUDIO_CD);
	}

//	m_TotalCount = QString("%1 songs").arg(m_RespList.count());
//	m_pInfoTracks->SetInfo( MakeInfo() );

	if (m_Loading)
	{
		UtilNovatron::LoadingStop(m_Loading);
		delete m_Loading;
		m_Loading = nullptr;
	}
}

void AudioCDWindow::SlotRespTrackInfo(CJsonNode node)
{
	TrackInfoDialog dialog;
	dialog.SetWindowTitle(STR_TRACK_INFO);
	dialog.SetAddr(m_pMgr->GetAddr());
	dialog.SetMode(TrackInfo::TRACK_INFO_MODE_VIEW);
	dialog.SetInfoData(node);
	dialog.exec();

}

void AudioCDWindow::SlotRespCDRipInfo(CJsonNode node)
{
	if (m_Loading)
	{
		UtilNovatron::LoadingStop(m_Loading);
		delete m_Loading;
		m_Loading = nullptr;
	}

	CDRipInfoDialog *dialog = new CDRipInfoDialog(this);
	dialog->SetAddr(m_pMgr->GetAddr());
	dialog->SetEventID(m_EventID);
	dialog->SetAlbumList(m_AlbumList);
	dialog->SetAlbumArtistList(m_AlbumArtistList);
	dialog->SetArtistList(m_ArtistList);
	dialog->SetGenreList(m_GenreList);
	dialog->SetComposerList(m_ComposerList);
	dialog->SetMoodList(m_MoodList);
	dialog->SetInfoData(node);
	dialog->SetCoverArt(m_pInfoTracks->GetCoverArt());
	if (dialog->exec() == QDialog::Accepted)
	{
		node.Clear();
		node = dialog->GetInfoData();

		node.Del(VAL_SUCCESS);
		node.Del(VAL_MSG);
		node.Del(VAL_RESULT);
		node.Del(KEY_SOURCE);
		node.Del(KEY_FORMATS);

//		QString cdnum = node.GetString(KEY_CDNUMBER);
//		QString cdyear = node.GetString(KEY_CDYEAR);
//		QString cdtotal = node.GetString(KEY_CDTOTAL);

//		node.Add(KEY_CDNUMBER, cdnum);
//		node.Add(KEY_CDYEAR, cdyear);
//		node.Add(KEY_CDTOTAL, cdtotal);

		node.AddInt(KEY_EVENT_ID, m_EventID);
//		LogDebug("node [%s]", node.ToCompactByteArray().data());

		m_pMgr->RequestCDRip(node);
	}

	if (dialog)
	{
		delete dialog;
		dialog = nullptr;
	}
}

void AudioCDWindow::SlotRespCategoryList(CJsonNode node)
{
	CJsonNode result = node.GetArray(VAL_RESULT);
	if (result.ArraySize() <= 0)
	{
//		emit SigRespError(message.left(MSG_LIMIT_COUNT));
		return;
	}

	QList<CJsonNode> list;
	list.clear();
	for (int i = 0; i < result.ArraySize(); i++)
	{
		list.append(result.GetArrayAt(i));
//		LogDebug("node : [%s]", m_NodeList[i].ToCompactByteArray().data());
	}

	SetCategoryList(list);
}

void AudioCDWindow::SlotCoverArtUpdate(QString filename)
{
	m_pInfoTracks->SetCoverArt(filename);
}

void AudioCDWindow::SlotCoverArtUpdate(QString coverArt, int index, int mode)
{
	if (QListView::IconMode == mode)
	{
		QModelIndex modelIndex = m_pIconTracks->GetModel()->index(index, 0);
		m_pIconTracks->GetModel()->setData(modelIndex, coverArt, IconTracksDelegate::ICON_TRACKS_COVER);
	}
	else
	{
		QModelIndex modelIndex = m_pListTracks->GetModel()->index(index, 0);
		m_pListTracks->GetModel()->setData(modelIndex, coverArt, ListTracksDelegate::LIST_TRACKS_COVER);
	}
}

void AudioCDWindow::SlotRespError(QString errMsg)
{
	if (m_Loading)
	{
		UtilNovatron::LoadingStop(m_Loading);
		delete m_Loading;
		m_Loading = nullptr;
	}

	CommonDialog dialog(this, STR_WARNING, errMsg);
	dialog.exec();
}

void AudioCDWindow::SlotSelectTitle(int id, QString coverArt)
{
	Q_UNUSED(coverArt)

	m_pMgr->RequestTrackPlay(id);
}

void AudioCDWindow::SlotCalcTotalTime(int time)
{
	m_TotalTime = QString::number(time);
	//	m_pInfoTracks->SetInfo( MakeInfo() );
}

void AudioCDWindow::SlotSelectPlay(int id, int playType)
{
	Q_UNUSED(playType)

	m_pMgr->RequestTrackPlay(id);

}

void AudioCDWindow::SlotSelectMenu(const QModelIndex &modelIndex, QPoint point)
{
	m_ID = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_ID));
//	LogDebug("id [%d] x [%d] y [%d]", m_ID, point.x(), point.y());

	m_Menu->clear();

	QMap<int, QString>::iterator i;
	for (i = m_OptionMenuMap.begin(); i != m_OptionMenuMap.end(); i++)
	{
		QIcon icon = UtilNovatron::GetMenuIcon(i.value());
		QAction *action = new QAction(icon, i.value(), this);
		action->setData(i.key());
		m_Menu->addAction(action);
	}

	m_Menu->popup(m_pListTracks->GetListView()->viewport()->mapToGlobal(point));
}

void AudioCDWindow::SlotMenuAction(QAction *action)
{
	SlotOptionMenuAction(m_ID, action->data().toInt());
}

void AudioCDWindow::SlotReqCoverArt(int id, int index, int mode)
{
	QStringList lsAddr = m_pMgr->GetAddr().split(":");
	QString fullpath = QString("%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(KEY_CD).arg(id);

	m_pMgr->RequestCoverArt(fullpath, index, mode);
}

void AudioCDWindow::SlotPlayAll()
{
//	if (m_ListMode == VIEW_MODE_ICON)
//	{
//		m_pIconTracks->SetAllSelectMap();
//		m_SelectMap = m_pIconTracks->GetSelectMap();
//	}
//	else
//	{
//		m_pListTracks->SetAllSelectMap();
//		m_SelectMap = m_pListTracks->GetSelectMap();
//	}

	m_pMgr->RequestTrackPlay();
}

void AudioCDWindow::SlotPlayRandom()
{
	m_pMgr->RequestRandom();
}

void AudioCDWindow::SlotTopMenu()
{
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_SelectMap = m_pIconTracks->GetSelectMap();
	}
	else
	{
		m_SelectMap = m_pListTracks->GetSelectMap();
	}

	ResetSelectMap();

	if (m_SelectMap.count() > 0)
	{
		SetSelectOnTopMenu();
	}
	else
	{
		SetSelectOffTopMenu();
	}

}

void AudioCDWindow::SlotTopMenuAction(int menuID)
{
	switch (menuID) {
	case TOP_MENU_SELECT_ALL:
		DoTopMenuSelectAll();
		break;
	case TOP_MENU_CLEAR_ALL:
		DoTopMenuClearAll();
		break;
	case TOP_MENU_CHANGE_META_INFO:
		DoTopMenuChangeMetaInfo();
		break;
	case TOP_MENU_CD_RIPPING:
		DoTopMenuCDRipping();
		break;
	case TOP_MENU_EJECT_CD:
		DoTopMenuEjectCD();
		break;
	}
}

void AudioCDWindow::SlotResize(int resize)
{
	m_ResizeTrack = resize;
	WriteSettings();

	int listMode = GetListModeFromResize(resize);
	if (listMode != m_ListMode)
	{
		m_ListMode = listMode;
		if (m_ListMode == VIEW_MODE_ICON)
		{
//			LogDebug("icon~~~~~~~~");
			if (m_pIconTracks->GetNodeList().count() != m_RespList.count())
			{
				m_pIconTracks->ClearNodeList();
				m_pIconTracks->SetNodeList(m_RespList, SIDEMENU_AUDIO_CD);
			}

			m_pListTracks->hide();
			m_pIconTracks->show();
			ui->gridLayoutBottom->replaceWidget(m_pListTracks, m_pIconTracks);
		}
		else
		{
//			LogDebug("list~~~~~~~~");
			if (m_pListTracks->GetNodeList().count() != m_RespList.count())
			{
				m_pListTracks->ClearNodeList();
				m_pListTracks->SetNodeList(m_RespList, SIDEMENU_AUDIO_CD);
			}

			m_pIconTracks->hide();
			m_pListTracks->show();
			ui->gridLayoutBottom->replaceWidget(m_pIconTracks, m_pListTracks);
		}
	}

	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pIconTracks->SetResize(resize);
	}
	else
	{
		m_pListTracks->SetResize(resize);
	}
}

void AudioCDWindow::SlotOptionMenuAction(int id, int menuID)
{
	switch (menuID) {
	case OPTION_MENU_CD_RIPPING:
		DoOptionMenuCDRipping(id);
		break;
	case OPTION_MENU_EDIT_TAG:
		DoOptionMenuTrackInfo(id);
		break;
	}
}

void AudioCDWindow::SlotEjectCD()
{
	DoTopMenuEjectCD();
}

void AudioCDWindow::ReadSettings()
{
	SettingIO settings;
	settings.beginGroup(SETTINGS_GROUP);

	m_ResizeTrack = settings.value("resize_track_value").toInt();

	settings.endGroup();

	if (m_ResizeTrack <= 0)
	{
		m_ResizeTrack = LIST_HEIGHT_MIN;
	}
}

void AudioCDWindow::WriteSettings()
{
	SettingIO settings;
	settings.beginGroup(SETTINGS_GROUP);

	settings.setValue("resize_track_value", m_ResizeTrack);

	settings.endGroup();
}

void AudioCDWindow::ConnectSigToSlot()
{
	connect(this, SIGNAL(SigRemoveWidget(QWidget*)), parent(), SLOT(SlotRemoveWidget(QWidget*)));

	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespTrackList(CJsonNode)), this, SLOT(SlotRespTrackList(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespTrackInfo(CJsonNode)), this, SLOT(SlotRespTrackInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespCDRipInfo(CJsonNode)), this, SLOT(SlotRespCDRipInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespCategoryList(CJsonNode)), this, SLOT(SlotRespCategoryList(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString)), this, SLOT(SlotCoverArtUpdate(QString)));

	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigPlayAll()), this, SLOT(SlotPlayAll()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigPlayRandom()), this, SLOT(SlotPlayRandom()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigMenu()), this, SLOT(SlotTopMenu()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigMenuAction(int)), this, SLOT(SlotTopMenuAction(int)));
	connect(m_pInfoTracks->GetFormSort(), SIGNAL(SigResize(int)), this, SLOT(SlotResize(int)));

//	connect(m_pIconTracks, SIGNAL(SigCalcTotalTime(int)), this, SLOT(SlotCalcTotalTime(int)));
	connect(m_pIconTracks, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectPlay(int, int)), this, SLOT(SlotSelectPlay(int, int)));

	connect(m_pListTracks, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));
	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectPlay(int, int)), this, SLOT(SlotSelectPlay(int, int)));
	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectMenu(const QModelIndex&, QPoint)), this, SLOT(SlotSelectMenu(const QModelIndex&, QPoint)));

	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuAction(QAction*)));

}

void AudioCDWindow::Initialize()
{
	m_pInfoTracks->GetFormPlay()->ShowPlayAll();
	m_pInfoTracks->GetFormPlay()->ShowMenu();
	m_pInfoTracks->GetFormSort()->ShowResize();
	m_pInfoTracks->GetFormSort()->SetSliderMinimum(LIST_HEIGHT_MIN);

	m_TopMenuMap.clear();
	m_SelectMap.clear();

//	m_ListMode = GetListModeFromResize(m_ResizeTrack);

	m_Loading = nullptr;

	m_AlbumList.clear();
	m_AlbumArtistList.clear();
	m_ArtistList.clear();
	m_GenreList.clear();
	m_ComposerList.clear();
	m_MoodList.clear();
	m_SourceList.clear();

	m_ID = -1;

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

	UtilNovatron::RemoveFilesInTempDirectory(KEY_CD);
}

void AudioCDWindow::ResetSelectMap()
{
	QMap<int, bool> map;
	QMap<int, bool>::iterator i;
	int index = 0;
	for (i = m_SelectMap.begin(); i!= m_SelectMap.end(); i++)
	{
//		LogDebug("key [%d] value [%d]", i.key(), i.value());
		index = i.key() - 1;
		if (index < 0)
			index = 0;
		map.insert(index, i.value());
	}
	m_SelectMap.clear();
	m_SelectMap = map;
}

void AudioCDWindow::SetCategoryList(QList<CJsonNode> list)
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

int AudioCDWindow::GetListModeFromResize(int resize)
{
	if (resize > ICON_HEIGHT_MID)
	{
		return VIEW_MODE_ICON;
	}
	else
	{
		return VIEW_MODE_LIST;
	}
}

void AudioCDWindow::SetSelectOffTopMenu()
{
	m_TopMenuMap.clear();

	m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
	if (m_SourceList.count() > 1)
	{
		m_TopMenuMap.insert(TOP_MENU_CHANGE_META_INFO, STR_CHANGE_META_INFO);
	}
	m_TopMenuMap.insert(TOP_MENU_CD_RIPPING, STR_CD_RIPPING);
	m_TopMenuMap.insert(TOP_MENU_EJECT_CD, STR_EJECT_CD);

	m_pInfoTracks->GetFormPlay()->ClearMenu();
	m_pInfoTracks->GetFormPlay()->SetMenu(m_TopMenuMap);
}

void AudioCDWindow::SetSelectOnTopMenu()
{
	m_TopMenuMap.clear();

	m_TopMenuMap.insert(TOP_MENU_CLEAR_ALL, STR_CLEAR_ALL);
	m_TopMenuMap.insert(TOP_MENU_CD_RIPPING, STR_CD_RIPPING);

	m_pInfoTracks->GetFormPlay()->ClearMenu();
	m_pInfoTracks->GetFormPlay()->SetMenu(m_TopMenuMap);
}

void AudioCDWindow::DoTopMenuSelectAll()
{
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pIconTracks->SetAllSelectMap();
	}
	else
	{
		m_pListTracks->SetAllSelectMap();
	}
}

void AudioCDWindow::DoTopMenuClearAll()
{
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pIconTracks->ClearSelectMap();
	}
	else
	{
		m_pListTracks->ClearSelectMap();
	}
}

void AudioCDWindow::DoTopMenuChangeMetaInfo()
{
	ChangeMetaInfoDialog dialog;
	dialog.SetIndex(m_SourceList);
	if (dialog.exec() == QDialog::Accepted)
	{
		int index = dialog.GetIndex();
		m_pMgr->RequestTrackList(index);
	}
}

void AudioCDWindow::DoTopMenuCDRipping()
{
	m_Loading = UtilNovatron::LoadingStart(parentWidget());

	m_pMgr->RequestCategoryList(SQLManager::CATEGORY_ALBUM);
	m_pMgr->RequestCategoryList(SQLManager::CATEGORY_ALBUM_ARTIST);
	m_pMgr->RequestCategoryList(SQLManager::CATEGORY_ARTIST);
	m_pMgr->RequestCategoryList(SQLManager::CATEGORY_GENRE);
	m_pMgr->RequestCategoryList(SQLManager::CATEGORY_COMPOSER);
	m_pMgr->RequestCategoryList(SQLManager::CATEGORY_MOOD);

	if (m_SelectMap.count() > 0)
	{
		m_pMgr->RequestCDRipInfo(-1, m_SelectMap);
	}
	else
	{
		QMap<int, bool> map;
		m_pMgr->RequestCDRipInfo(-1, map);
	}

}

void AudioCDWindow::DoTopMenuEjectCD()
{
	m_pMgr->RequestEject();

	emit SigRemoveWidget(this);
}

void AudioCDWindow::SetOptionMenu()
{
	m_OptionMenuMap.clear();
	m_OptionMenuMap.insert(OPTION_MENU_CD_RIPPING, STR_CD_RIPPING);
//	m_OptionMenuMap.insert(OPTION_MENU_EDIT_TAG, STR_TAG_EDIT);

}

void AudioCDWindow::DoOptionMenuCDRipping(int id)
{
	m_Loading = UtilNovatron::LoadingStart(parentWidget());

	m_pMgr->RequestCategoryList(SQLManager::CATEGORY_ALBUM);
	m_pMgr->RequestCategoryList(SQLManager::CATEGORY_ALBUM_ARTIST);
	m_pMgr->RequestCategoryList(SQLManager::CATEGORY_ARTIST);
	m_pMgr->RequestCategoryList(SQLManager::CATEGORY_GENRE);
	m_pMgr->RequestCategoryList(SQLManager::CATEGORY_COMPOSER);
	m_pMgr->RequestCategoryList(SQLManager::CATEGORY_MOOD);

	int index = id - 1;
	if (index < 0)
		index = 0;

	QMap<int, bool> map;
	map.insert(index, true);
	m_pMgr->RequestCDRipInfo(-1, map);
}

void AudioCDWindow::DoOptionMenuTrackInfo(int id)
{
	m_pMgr->RequestTrackInfo(id);
}

QString AudioCDWindow::MakeInfo()
{
	QString info = m_TotalCount;

	if (!m_TotalTime.isEmpty())
	{
		info += " | " + m_TotalTime;
	}
	if (!m_Format.isEmpty())
	{
		info += " | " + m_Format;
	}
	if (!m_Date.isEmpty())
	{
		info += " | " + m_Date;
	}

	return info;
}

