#include <QMessageBox>

#include "audiocdwindow.h"
#include "ui_audiocdwindow.h"

#include "manager/audiocdmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/caxtranslate.h"
#include "util/log.h"

#include "widget/form/formplay.h"
#include "widget/form/formsort.h"

#include "widget/formTop/infotracks.h"
#include "widget/formBottom/icontracks.h"
#include "widget/formBottom/icontracksdelegate.h"
#include "widget/formBottom/listtracks.h"
#include "widget/formBottom/listtracksdelegate.h"

#include "dialog/cdripinfodialog.h"
#include "dialog/trackinfodialog.h"
#include "dialog/trackinfo.h"

AudioCDWindow::AudioCDWindow(QWidget *parent, const QString &addr, const int &eventID) :
	QWidget(parent),
	m_pMgr(new AudioCDManager),
	m_pInfoTracks(new InfoTracks(this)),
	m_pIconTracks(new IconTracks(this)),
	m_pListTracks(new ListTracks(this)),
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
	Initialize();
}

AudioCDWindow::~AudioCDWindow()
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

}

void AudioCDWindow::AddWidgetAudioCDHome()
{
	ui->gridLayoutTop->addWidget(m_pInfoTracks);
//	if (m_ListMode == VIEW_MODE_ICON)
//	{
//		m_pInfoTracks->GetFormSort()->SetResize(ICON_HEIGHT_MAX);
//		ui->gridLayoutBottom->addWidget(m_pIconTracks);
//	}
//	else
	{
		m_pInfoTracks->GetFormSort()->SetResize(LIST_HEIGHT_MIN);
		ui->gridLayoutBottom->addWidget(m_pListTracks);
	}
}

void AudioCDWindow::RequestTrackList()
{
	m_pMgr->RequestTrackList();
}

void AudioCDWindow::SlotRespTrackList(QList<CJsonNode> list)
{
	CJsonNode track = list.at(0);
	LogDebug("track [%s]", track.ToCompactByteArray().data());

	m_pInfoTracks->SetCoverArt(track.GetString(KEY_COVER_ART));
	m_pInfoTracks->SetTitle(track.GetString(KEY_ALBUM));
	m_pInfoTracks->SetSubtitle(track.GetString(KEY_ALBUM_ARTIST));
//	m_pInfoTracks->SetInfo(MakeInfo());

	SetOptionMenu();

	m_pIconTracks->ClearNodeList();
	m_pIconTracks->SetNodeList(list, IconTracks::ICON_TRACKS_AUDIO_CD);

	m_pListTracks->ClearNodeList();
	m_pListTracks->SetNodeList(list, ListTracks::LIST_TRACKS_AUDIO_CD);

	m_TotalCount = QString("%1 songs").arg(list.count());
//	m_pInfoTracks->SetInfo( MakeInfo() );

}

void AudioCDWindow::SlotRespTrackInfo(CJsonNode node)
{
	TrackInfoDialog dialog;
	dialog.SetAddr(m_pMgr->GetAddr());
	dialog.SetMode(TrackInfo::TRACK_INFO_MODE_VIEW);
	dialog.SetInfoData(node);
	dialog.exec();

}

void AudioCDWindow::SlotRespCDRipInfo(CJsonNode node)
{
	CDRipInfoDialog dialog;
	dialog.SetAddr(m_pMgr->GetAddr());
	dialog.SetAlbumList(m_AlbumList);
	dialog.SetAlbumArtistList(m_AlbumArtistList);
	dialog.SetArtistList(m_ArtistList);
	dialog.SetGenreList(m_GenreList);
	dialog.SetComposerList(m_ComposerList);
	dialog.SetMoodList(m_MoodList);
	dialog.SetInfoData(node);
	if (dialog.exec() == QDialog::Accepted)
	{
		node.Clear();
		node = dialog.GetInfoData();

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
		LogDebug("node [%s]", node.ToCompactByteArray().data());

		m_pMgr->RequestCDRip(node);
	}
}

void AudioCDWindow::SlotRespCategoryList(QList<CJsonNode> list)
{
	SetCategoryList(list);
}

void AudioCDWindow::SlotRespError(QString errMsg)
{
	QMessageBox::warning(this, "Warning", errMsg);
}

void AudioCDWindow::SlotSelectTitle(int id, QString coverArt)
{
	Q_UNUSED(coverArt)

	m_pMgr->RequestTrackPlay(id);
}

void AudioCDWindow::SlotCalcTotalTime(int time)
{
	m_TotalTime = QString("%1").arg(time);
	//	m_pInfoTracks->SetInfo( MakeInfo() );
}

void AudioCDWindow::SlotSelectPlay(int id, int playType)
{
	Q_UNUSED(playType)

	m_pMgr->RequestTrackPlay(id);

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
	int listMode = VIEW_MODE_ICON;
	if (resize > 130)
	{
		listMode = VIEW_MODE_ICON;
	}
	else
	{
		listMode = VIEW_MODE_LIST;
	}

	if (listMode != m_ListMode)
	{
		m_ListMode = listMode;
		if (m_ListMode == VIEW_MODE_ICON)
		{
			LogDebug("icon~~~~~~~~");
			m_pListTracks->hide();
			m_pIconTracks->show();
			ui->gridLayoutBottom->replaceWidget(m_pListTracks, m_pIconTracks);
		}
		else
		{
			LogDebug("list~~~~~~~~");
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
	case OPTION_MENU_INFO:
		DoOptionMenuTrackInfo(id);
		break;
	}
}

void AudioCDWindow::ConnectSigToSlot()
{
	connect(this, SIGNAL(SigRemoveWidget(QWidget*)), parent(), SLOT(SlotRemoveWidget(QWidget*)));

	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespTrackList(QList<CJsonNode>)), this, SLOT(SlotRespTrackList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespTrackInfo(CJsonNode)), this, SLOT(SlotRespTrackInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespCDRipInfo(CJsonNode)), this, SLOT(SlotRespCDRipInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespCategoryList(QList<CJsonNode>)), this, SLOT(SlotRespCategoryList(QList<CJsonNode>)));

	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigPlayAll()), this, SLOT(SlotPlayAll()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigPlayRandom()), this, SLOT(SlotPlayRandom()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigMenu()), this, SLOT(SlotTopMenu()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigMenuAction(int)), this, SLOT(SlotTopMenuAction(int)));
	connect(m_pInfoTracks->GetFormSort(), SIGNAL(SigResize(int)), this, SLOT(SlotResize(int)));

	connect(m_pIconTracks, SIGNAL(SigCalcTotalTime(int)), this, SLOT(SlotCalcTotalTime(int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectPlay(int, int)), this, SLOT(SlotSelectPlay(int, int)));

	connect(m_pListTracks->GetDelegate(), SIGNAL(SigSelectPlay(int, int)), this, SLOT(SlotSelectPlay(int, int)));
	connect(m_pListTracks->GetDelegate(), SIGNAL(SigMenuAction(int, int)), this, SLOT(SlotOptionMenuAction(int, int)));

}

void AudioCDWindow::Initialize()
{

	m_pInfoTracks->GetFormPlay()->ShowPlayAll();
	m_pInfoTracks->GetFormPlay()->ShowPlayRandom();
	m_pInfoTracks->GetFormPlay()->ShowMenu();
	m_pInfoTracks->GetFormSort()->ShowResize();
//	m_pInfoTracks->GetFormSort()->SetResize(ICON_HEIGHT_MAX);

	m_TopMenuMap.clear();
	m_SelectMap.clear();

	m_ListMode = VIEW_MODE_LIST;

	m_AlbumList.clear();
	m_AlbumArtistList.clear();
	m_ArtistList.clear();
	m_GenreList.clear();
	m_ComposerList.clear();
	m_MoodList.clear();

}

void AudioCDWindow::ResetSelectMap()
{
	QMap<int, bool> map;
	QMap<int, bool>::iterator i;
	int index = 0;
	for (i = m_SelectMap.begin(); i!= m_SelectMap.end(); i++)
	{
		LogDebug("key [%d] value [%d]", i.key(), i.value());
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

void AudioCDWindow::SetSelectOffTopMenu()
{
	m_TopMenuMap.clear();

	m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
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

void AudioCDWindow::DoTopMenuCDRipping()
{
	m_pMgr->RequestCategoryList(SQLManager::CATEGORY_ALBUM);
	m_pMgr->RequestCategoryList(SQLManager::CATEGORY_ALBUMARTIST);
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
	m_OptionMenuMap.insert(OPTION_MENU_INFO, STR_INFO);

	m_pListTracks->GetDelegate()->SetOptionMenuMap(m_OptionMenuMap);
}

void AudioCDWindow::DoOptionMenuCDRipping(int id)
{
	m_pMgr->RequestCategoryList(SQLManager::CATEGORY_ALBUM);
	m_pMgr->RequestCategoryList(SQLManager::CATEGORY_ALBUMARTIST);
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


