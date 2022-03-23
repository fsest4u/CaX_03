#include "queuelistwindow.h"
#include "ui_queuelistwindow.h"

#include "dialog/commondialog.h"
#include "dialog/inputnamedialog.h"

#include "manager/queuelistmanager.h"
#include "manager/playlistmanager.h"

#include "util/utilnovatron.h"

#include "widget/form/formcoverart.h"
#include "widget/form/formtitle.h"
#include "widget/formQueue/queuetrack.h"
#include "widget/formQueue/queuetrackdelegate.h"
#include "widget/formQueue/queuelyrics.h"
#include "widget/formQueue/queueartist.h"

#include "widget/musicdbwindow.h"
#include "widget/playlistwindow.h"

QueuelistWindow::QueuelistWindow(QWidget *parent, const QString &addr, const int &eventID) :
	QWidget(parent),
	m_pMgr(new QueuelistManager),
	m_pPlaylistMgr(new PlaylistManager),
	m_Lyrics(new QueueLyrics(this)),
	m_Artist(new QueueArtist(this)),
	m_Menu(new QMenu(this)),
	m_pFormCoverArt(new FormCoverArt(this)),
	m_pFormTitle(new FormTitle(this)),
	m_EventID(eventID),
	ui(new Ui::QueuelistWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);
	m_pPlaylistMgr->SetAddr(addr);

	m_Track = new QueueTrack(this, m_pMgr->GetAddr(), m_EventID);

	ConnectSigToSlot();
	Initialize();
}

QueuelistWindow::~QueuelistWindow()
{

	if (m_pMgr)
	{
		delete m_pMgr;
		m_pMgr = nullptr;
	}

	if (m_pPlaylistMgr)
	{
		delete m_pPlaylistMgr;
		m_pPlaylistMgr = nullptr;
	}

	if (m_Track)
	{
		delete m_Track;
		m_Track = nullptr;
	}

	if (m_Lyrics)
	{
		delete m_Lyrics;
		m_Lyrics = nullptr;
	}

	if (m_Artist)
	{
		delete m_Artist;
		m_Artist = nullptr;
	}

	disconnect(m_Menu, SIGNAL(triggered(QAction*)));
	if (m_Menu)
	{
		delete m_Menu;
		m_Menu = nullptr;
	}

	if (m_pFormCoverArt)
	{
		delete m_pFormCoverArt;
		m_pFormCoverArt = nullptr;
	}

	if (m_pFormTitle)
	{
		delete m_pFormTitle;
		m_pFormTitle = nullptr;
	}

	if (m_pMusicDBWin)
	{
//		emit SigRemoveWidget(m_pMusicDBWin);

		delete m_pMusicDBWin;
		m_pMusicDBWin = nullptr;
	}

	delete ui;
}

void QueuelistWindow::SetNodeInfo(CJsonNode node)
{
//	LogDebug("====================================");
//	LogDebug("SetNodeInfo [%s]", node.ToCompactByteArray().data());
//	LogDebug("====================================");

	m_Src = node.GetString(KEY_SRC);

	CJsonNode result;
	if (!node.GetArray(VAL_RESULT, result) || result.ArraySize() <= 0)
	{
		return;
	}

	QList<CJsonNode> list;
	list.clear();
	for (int i = 0; i < result.ArraySize(); i++)
	{
		list.append(result.GetArrayAt(i));
//		LogDebug("node : [%s]", list[i].ToCompactByteArray().data());
	}

//	CJsonNode track = list.at(0);
//	LogDebug("track [%s]", track.ToTabedByteArray().data());

	if (!m_Src.compare(SRC_MUSIC_DB))
	{
//		int id = track.GetString(KEY_SONG).toInt();
//		m_pMgr->RequestCategoryInfo(id);
		ui->btnMenu->show();
	}
//	else if (!m_Src.compare(SRC_AUDIO_CD))
//	{
//		m_AlbumName = track.GetString(KEY_TOP);
//		RequestCoverArtAudioCD(track.GetString(KEY_COVER_ART));
//	}
//	else if (!m_Src.compare(SRC_BROWSER))
//	{
//		m_AlbumName = track.GetString(KEY_TOP);
//		RequestCoverArtBrowser(track.GetString(KEY_FILE));
//	}

	m_Track->ClearNodeList();
	m_TotalTime = m_Track->SetNodeList(list, m_Src);
}

void QueuelistWindow::SetPlayInfo(CJsonNode node)
{
	if (node.IsNull())
	{
		LogWarning("node is null~");
		return;
	}
//	LogDebug("node [%s]", node.ToTabedByteArray().data());

	m_Src = node.GetString(KEY_SRC);

	if (!node.GetString(KEY_COVER_ART).isEmpty())
	{
		m_pMgr->RequestCoverArt(node.GetString(KEY_COVER_ART), -1, -1);
	}

	if (!m_Src.compare(SRC_MUSIC_DB))
	{
		m_AlbumName = node.GetString(KEY_ALBUM);
		m_pFormTitle->SetTitle(m_AlbumName);
		SetMqa(node.GetString(KEY_MQA));
		SetFormat(node.GetString(KEY_FORMAT));
		SetPlayIndex(node.GetInt(KEY_TOTAL_UPPER), node.GetInt(KEY_CURR_PLAY));
		SetTotalTime(m_TotalTime);

		m_TrackID = node.GetString(KEY_ID_UPPER).toInt();
		m_pMgr->RequestTrackInfo(m_TrackID);
	}
	else if (!m_Src.compare(SRC_AUDIO_CD)
			 || !m_Src.compare(SRC_BROWSER))
	{
		m_AlbumName = node.GetString(KEY_ALBUM);
		m_pFormTitle->SetTitle(m_AlbumName);
		SetMqa(node.GetString(KEY_MQA));
		SetFormat(node.GetString(KEY_FORMAT));
		SetPlayIndex(node.GetInt(KEY_TOTAL_UPPER), node.GetInt(KEY_CURR_PLAY));
		SetTotalTime(m_TotalTime);
	}
	else if (!m_Src.compare(SRC_FM_RADIO))
	{
		m_AlbumCoverArt = UtilNovatron::GetCoverArtIcon(SIDEMENU_FM_RADIO);
		m_pFormCoverArt->SetCoverArt(m_AlbumCoverArt);

		CJsonNode result;
		if (!node.GetArray(VAL_RESULT, result) || result.ArraySize() <= 0)
		{
			return;
		}

		QList<CJsonNode> list;
		list.clear();
		for (int i = 0; i < result.ArraySize(); i++)
		{
			list.append(result.GetArrayAt(i));
//			LogDebug("node : [%s]", list[i].ToCompactByteArray().data());
		}

		m_Track->ClearNodeList();
		m_TotalTime = m_Track->SetNodeList(list, m_Src);
		m_Track->SetRadioInfo(node.GetInt(KEY_FREQ_MIN), node.GetInt(KEY_FREQ_MAX), node.GetInt(KEY_FREQ_STEP));

		m_AlbumName = node.GetString(KEY_TOP);
		m_pFormTitle->SetTitle(m_AlbumName);
		SetFormat(node.GetString(KEY_FORMAT));
		SetPlayIndex(node.GetInt(KEY_TOTAL_UPPER), node.GetInt(KEY_CURR_PLAY));
		SetTotalTime(m_TotalTime);

	}
	else if (!m_Src.compare(SRC_I_RADIO)
			 || !m_Src.compare(SRC_PODCAST)
			 || !m_Src.compare(SRC_TIDAL)
			 || !m_Src.compare(SRC_DEEZER)
			 || !m_Src.compare(SRC_NAPSTER)
			 || !m_Src.compare(SRC_HIGH_RES_AUDIO)
			 || !m_Src.compare(SRC_AMAZON)
			 || !m_Src.compare(SRC_AIRABLE_UPNP)
			 || !m_Src.compare(SRC_QOBUZ))
	{
		m_AlbumName = node.GetString(KEY_ALBUM);
		m_pFormTitle->SetTitle(m_AlbumName);
		SetMqa(node.GetString(KEY_MQA));
		SetFormat(node.GetString(KEY_FORMAT));
		SetPlayIndex(node.GetInt(KEY_TOTAL_UPPER), node.GetInt(KEY_CURR_PLAY));
		SetTotalTime(m_TotalTime);
	}
}


void QueuelistWindow::SlotClickBtnTrack()
{
	m_pFormCoverArt->SetCoverArt(m_AlbumCoverArt);
	m_pFormTitle->SetTitle(m_AlbumName);

	ui->stackedQueue->setCurrentWidget(m_Track);
}

void QueuelistWindow::SlotClickBtnLyrics()
{
	m_pFormCoverArt->SetCoverArt(m_AlbumCoverArt);
	m_pFormTitle->SetTitle(m_AlbumName);

	ui->stackedQueue->setCurrentWidget(m_Lyrics);
}

void QueuelistWindow::SlotClickBtnArtist()
{
//	m_pFormCoverArt->SetCoverArt(m_ArtistCoverArt);
//	m_pFormTitle->SetTitle(m_ArtistName);

//	ui->stackedQueue->setCurrentWidget(m_Artist);
}

void QueuelistWindow::SlotClickBtnClose()
{
	if (m_pMusicDBWin)
	{
		emit SigRemoveWidget(m_pMusicDBWin);

		delete m_pMusicDBWin;
		m_pMusicDBWin = nullptr;
	}
	emit SigRemoveQueueWidget();
}

void QueuelistWindow::SlotRespError(QString errMsg)
{
	CommonDialog dialog(this, STR_WARNING, errMsg);
	dialog.exec();
}

void QueuelistWindow::SlotRespTrackInfo(CJsonNode node)
{
//	LogDebug("node [%s]", node.ToTabedByteArray().data());
	QMap<int, QString>	menuMap;

	m_TrackAlbumID = node.GetInt(KEY_ALBUM_ID);
	m_TrackArtistID = node.GetInt(KEY_ARTIST_ID);
	m_TrackFavorite = node.GetInt(KEY_FAVORITE);

//	if (m_TrackFavorite == 1)
//	{
//		menuMap.insert(OPTION_MENU_FAVORITE, STR_DELETE_TO_FAVORITE);
//	}
//	else
//	{
//		menuMap.insert(OPTION_MENU_FAVORITE, STR_ADD_TO_FAVORITE);
//	}

//	menuMap.insert(OPTION_MENU_GO_TO_ALBUM, STR_GO_TO_ALBUM);
//	menuMap.insert(OPTION_MENU_GO_TO_ARTIST, STR_GO_TO_ARTIST);
	menuMap.insert(OPTION_MENU_MAKE_PLAYLIST, STR_MAKE_PLAYLIST);

	ClearMenu();
	SetMenu(menuMap);

}

void QueuelistWindow::SlotRespRecordSet(CJsonNode node)
{
	QString desc = node.GetString(KEY_DESC);
	if (!desc.isEmpty())
	{
		CommonDialog dialog(this, STR_WARNING, desc);
		dialog.exec();
	}
}

void QueuelistWindow::SlotCoverArtUpdate(QString fileName, int nIndex, int mode)
{
	Q_UNUSED(mode)

//	LogDebug("filename [%s]", fileName.toUtf8().data());
	m_AlbumCoverArt = fileName;
	m_pFormCoverArt->SetCoverArt(m_AlbumCoverArt);

}

void QueuelistWindow::SlotSelectPlay(int index, int playType)
{
	Q_UNUSED(playType)

	if (!m_Src.compare(SRC_MUSIC_DB)
			|| !m_Src.compare(SRC_AUDIO_CD)
			|| !m_Src.compare(SRC_BROWSER)
			|| !m_Src.compare(SRC_I_RADIO)
			|| !m_Src.compare(SRC_PODCAST)
			|| !m_Src.compare(SRC_TIDAL)
			|| !m_Src.compare(SRC_DEEZER)
			|| !m_Src.compare(SRC_NAPSTER)
			|| !m_Src.compare(SRC_HIGH_RES_AUDIO)
			|| !m_Src.compare(SRC_AMAZON)
			|| !m_Src.compare(SRC_AIRABLE_UPNP)
			|| !m_Src.compare(SRC_QOBUZ))
	{
		m_pMgr->RequestTrackPlay(index);
	}
	else if (!m_Src.compare(SRC_FM_RADIO))
	{
		m_pMgr->RequestRadioPlay(index, m_EventID);
	}
}

void QueuelistWindow::SlotMenu()
{
//	emit SigMenu();
}

void QueuelistWindow::SlotMenuAction(QAction *action)
{
//	emit SigMenuAction(action->data().toInt());
	int menuID = action->data().toInt();
	switch (menuID)
	{
//	case OPTION_MENU_FAVORITE:
//		DoMenuFavorite(m_TrackID, m_TrackFavorite);
//		break;
//	case OPTION_MENU_GO_TO_ALBUM:
//		DoMenuGoToAlbum(m_TrackAlbumID, m_AlbumCoverArt);
//		break;
//	case OPTION_MENU_GO_TO_ARTIST:
//		DoMenuGoToArtist(m_TrackArtistID);
//		break;
	case OPTION_MENU_MAKE_PLAYLIST:
		DoMenuMakePlaylist();
		break;
	}
}

void QueuelistWindow::SlotRefresh(CJsonNode node)
{
	int id = node.GetInt(KEY_ID_UPPER);
	if (id <= 0)
	{
		return;
	}

	CommonDialog dialog(this, STR_WARNING, STR_NEW_PLAYLIST_MADE);
	dialog.exec();

	QMap<int, bool> idMap;

	QList<CJsonNode> list = m_Track->GetNodeList();
	foreach(CJsonNode tempNode, list)
	{
		int songID = tempNode.GetInt(KEY_SONG);
		idMap.insert(songID, true);
	}
	m_pPlaylistMgr->RequestAddTrackFromPlaylist(id, idMap);
}

void QueuelistWindow::SlotFavorite(int id, int favorite)
{
	DoMenuFavorite(id, favorite);
}

void QueuelistWindow::SlotAddToPlaylist(int id)
{
	PlaylistWindow *widget = new PlaylistWindow(this, m_pMgr->GetAddr());
	widget->AddWidgetItem(TYPE_MODE_ITEM_ADD);
	emit widget->SigAddWidget(widget, STR_PLAYLIST);
	widget->RequestPlaylist();

	connect(widget, SIGNAL(SigAddToPlaylist(int)), this, SLOT(SlotOptionMenuAddToPlaylist(int)));
}

void QueuelistWindow::SlotGoToAlbum(int albumID)
{
	DoMenuGoToAlbum(albumID, m_AlbumCoverArt);
}

void QueuelistWindow::SlotGoToArtist(int artistID)
{
	DoMenuGoToArtist(artistID);
}

void QueuelistWindow::SlotUpdateTimeStamp(uint timestamp, int count, int time)
{
	emit SigUpdateTimeStamp(timestamp);

	ui->labelTotalCount->setText(QString::number(count));
	SetTotalTime(time);
}

void QueuelistWindow::SlotOptionMenuAddToPlaylist(int id)
{
	QMap<int, bool> map;
	map.insert(m_TrackID, true);

	m_pMgr->RequestAddToPlaylist(id, map);
}


void QueuelistWindow::ConnectSigToSlot()
{
	connect(this, SIGNAL(SigAddWidget(QWidget*, QString)), parent(), SLOT(SlotAddWidget(QWidget*, QString)));
	connect(this, SIGNAL(SigRemoveWidget(QWidget*)), parent(), SLOT(SlotRemoveWidget(QWidget*)));
	connect(this, SIGNAL(SigRemoveQueueWidget()), parent(), SLOT(SlotRemoveQueueWidget()));

	connect(ui->btnTrack, SIGNAL(clicked()), this, SLOT(SlotClickBtnTrack()));
	connect(ui->btnLyrics, SIGNAL(clicked()), this, SLOT(SlotClickBtnLyrics()));
	connect(ui->btnArtist, SIGNAL(clicked()), this, SLOT(SlotClickBtnArtist()));
	connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(SlotClickBtnClose()));

	connect(ui->btnMenu, SIGNAL(pressed()), this, SLOT(SlotMenu()));

	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespTrackInfo(CJsonNode)), this, SLOT(SlotRespTrackInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespRecordSet(CJsonNode)), this, SLOT(SlotRespRecordSet(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));

	connect(m_pPlaylistMgr, SIGNAL(SigRefresh(CJsonNode)), this, SLOT(SlotRefresh(CJsonNode)));

	connect(m_Track, SIGNAL(SigFavorite(int, int)), this, SLOT(SlotFavorite(int, int)));
	connect(m_Track, SIGNAL(SigAddToPlaylist(int)), this, SLOT(SlotAddToPlaylist(int)));
	connect(m_Track, SIGNAL(SigGoToAlbum(int)), this, SLOT(SlotGoToAlbum(int)));
	connect(m_Track, SIGNAL(SigGoToArtist(int)), this, SLOT(SlotGoToArtist(int)));
	connect(m_Track, SIGNAL(SigUpdateTimeStamp(uint, int, int)), this, SLOT(SlotUpdateTimeStamp(uint, int, int)));

	connect(m_Track->GetDelegate(), SIGNAL(SigSelectPlay(int, int)), this, SLOT(SlotSelectPlay(int, int)));

}

void QueuelistWindow::Initialize()
{

	QString style = QString("QMenu::icon {	\
								padding: 0px 0px 0px 20px;	\
							}	\
							QMenu::item {	\
								width: 260px;	\
								height: 40px;	\
								color: rgb(255, 255, 255);	\
								font-size: 14pt;	\
								padding: 0px 20px 0px 20px;	\
							}	\
							QMenu::item:selected {	\
								background: rgba(84,84,84,255);	\
							}");

	m_Menu->setStyleSheet(style);
	ui->btnMenu->setMenu(m_Menu);
	ui->btnMenu->hide();

	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuAction(QAction*)));

	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);
	ui->gridLayoutFormTitle->addWidget(m_pFormTitle);

	ui->stackedQueue->addWidget(m_Track);
	ui->stackedQueue->addWidget(m_Lyrics);
	ui->stackedQueue->addWidget(m_Artist);

	// temp_code, dylee
	ui->btnLyrics->hide();
	ui->btnArtist->hide();

	ui->labelFavorite->hide();
	ui->frameRating->hide();

	m_pFormTitle->SetTitleFont(FONT_SIZE_QUEUE_TITLE, FONT_COLOR_WHITE, FONT_WEIGHT_NORMAL);

	m_pMusicDBWin = nullptr;

	m_Src = "";
	m_TotalTime = 0;
	m_Favorite = -1;
	m_Rating = -1;

	m_TrackID = -1;
	m_TrackAlbumID = -1;
	m_TrackArtistID = -1;
	m_TrackFavorite = -1;
}

void QueuelistWindow::RequestCoverArtMusicDB(int id, int index)
{
	QString strCat;
	if (index == 0)
	{
		strCat = UtilNovatron::GetCategoryName(SQLManager::CATEGORY_ALBUM);
	}
	else
	{
		strCat = UtilNovatron::GetCategoryName(SQLManager::CATEGORY_ARTIST);
	}

	QStringList lsAddr = m_pMgr->GetAddr().split(":");
	QString fullpath = QString("%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(strCat).arg(id);

//	LogDebug("path [%s] index [%d]", fullpath.toUtf8().data(), index);
	m_pMgr->RequestCoverArt(fullpath, index, -1);
}

void QueuelistWindow::RequestCoverArtBrowser(QString path)
{
	QStringList lsAddr = m_pMgr->GetAddr().split(":");
	QString fullpath = QString("%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(SRC_BROWSER).arg(path);

	m_pMgr->RequestCoverArt(fullpath, -1, -1);
}

void QueuelistWindow::RequestCoverArtAudioCD(QString fullpath)
{
	m_pMgr->RequestCoverArt(fullpath, -1, -1);

}

void QueuelistWindow::SetMqa(QString value)
{
	QString style;
	if (value.contains(MQA_BLUE))
	{
		style = QString("QLabel	\
						{	\
							border-image: url(\":/resource/mqa_blue.png\");	\
						}");
	}
	else if (value.contains(MQA_GREEN))
	{
		style = QString("QLabel	\
						{	\
							border-image: url(\":/resource/mqa_green.png\");	\
						}");
	}
	else
	{
		style = QString("QLabel	\
						{	\
							border-image: url(\"\");	\
						}");
	}
	ui->labelMqa->setStyleSheet(style);
}

void QueuelistWindow::SetFormat(QString value)
{
	ui->labelFormat->setText(value);
}

void QueuelistWindow::SetPlayIndex(int total, int currPlay)
{
	if (total >= 0 && currPlay >= 0)
	{
		ui->label->show();
		ui->labelCurPlay->show();
		ui->labelTotalCount->show();

		ui->labelCurPlay->setText(QString::number(currPlay + 1));
		ui->labelTotalCount->setText(QString::number(total));

		m_Track->SetCurrentIndex(currPlay);
	}
	else
	{
		ui->label->hide();
		ui->labelCurPlay->hide();
		ui->labelTotalCount->hide();

		ui->labelPlayQueue->hide();
	}
}

void QueuelistWindow::SetTotalTime(int time)
{
	if (time > 0)
	{
		QString hhmmss = UtilNovatron::CalcSecondToHMS(time);
		ui->labelTotalTime->show();
		ui->labelTotalTime->setText(hhmmss);
	}
	else
	{
		ui->labelTotalTime->hide();
	}
}

void QueuelistWindow::DoMenuFavorite(int trackID, int favorite)
{
	favorite = favorite == 0 ? 1 : 0;
//	LogDebug("DoMenuFavorite favorite [%d]", m_TrackFavorite);

	m_pMgr->RequestUpdateTrackFavorite(trackID, favorite);
}

void QueuelistWindow::DoMenuGoToAlbum(int albumID, QString cover)
{
	if (m_pMusicDBWin)
	{
		emit SigRemoveWidget(m_pMusicDBWin);

		delete m_pMusicDBWin;
		m_pMusicDBWin = nullptr;
	}

//	LogDebug("DoMenuGoToAlbum id [%d]", m_TrackAlbumID);
	m_pMusicDBWin = new MusicDBWindow(this, m_pMgr->GetAddr(), -1);
	m_pMusicDBWin->AddWidgetTrack(TYPE_MODE_TRACK, SQLManager::CATEGORY_ALBUM);
	emit m_pMusicDBWin->SigAddWidget(m_pMusicDBWin, STR_MUSIC_DB);
	m_pMusicDBWin->RequestTrackList(albumID);
	m_pMusicDBWin->SetCoverArt(cover);
}

void QueuelistWindow::DoMenuGoToArtist(int artistID)
{
	if (m_pMusicDBWin)
	{
		emit SigRemoveWidget(m_pMusicDBWin);

		delete m_pMusicDBWin;
		m_pMusicDBWin = nullptr;
	}

//	LogDebug("DoMenuGoToArtist id [%d]", m_TrackArtistID);
	m_pMusicDBWin = new MusicDBWindow(this, m_pMgr->GetAddr(), -1);
	m_pMusicDBWin->AddWidgetItem(TYPE_MODE_ITEM_ALBUM, SQLManager::CATEGORY_ARTIST);
	emit m_pMusicDBWin->SigAddWidget(m_pMusicDBWin, STR_MUSIC_DB);
	m_pMusicDBWin->RequestCategoryList(artistID);
}

void QueuelistWindow::DoMenuMakePlaylist()
{
	InputNameDialog dialog;
	if (dialog.exec() == QDialog::Accepted)
	{
		QString name = dialog.GetName();
		m_pPlaylistMgr->RequestAddPlaylist(name);
	}
}

int QueuelistWindow::GetRating() const
{
	return m_Rating;
}

void QueuelistWindow::SetRating(int Rating)
{
	if (Rating < 0)
	{
		ui->frameRating->hide();
		return;
	}

	ui->frameRating->show();
	m_Rating = Rating;

	QString style;

	if (m_Rating == 1)
	{
		style = "play-btn52-rank-sel1.png";
	}
	else if (m_Rating == 2)
	{
		style = "play-btn52-rank-sel2.png";
	}
	else if (m_Rating == 3)
	{
		style = "play-btn52-rank-sel3.png";
	}
	else if (m_Rating == 4)
	{
		style = "play-btn52-rank-sel4.png";
	}
	else if (m_Rating == 5)
	{
		style = "play-btn52-rank-sel5.png";
	}
	else
	{
		style = "play-btn52-rank-sel0.png";
	}

	style = QString("QFrame	\
					{	\
					  border-image: url(\":/resource/%1\");	\
					}").arg(style);

					ui->frameRating->setStyleSheet(style);
}

void QueuelistWindow::ClearMenu()
{
	m_Menu->clear();
}

void QueuelistWindow::SetMenu(QMap<int, QString> map)
{
	QMap<int, QString>::iterator i;
	for (i = map.begin(); i != map.end(); i++)
	{
		QIcon icon = UtilNovatron::GetMenuIcon(i.value());
		QAction *action = new QAction(icon, i.value(), this);
		action->setData(i.key());
		m_Menu->addAction(action);
	}
}

void QueuelistWindow::SlotAddWidget(QWidget *widget, QString title)
{
	emit SigAddWidget(widget, title);	// recursive
}

void QueuelistWindow::SlotRemoveWidget(QWidget *widget)
{
	emit SigRemoveWidget(widget);
}

int QueuelistWindow::GetFavorite() const
{
	return m_Favorite;
}

void QueuelistWindow::SetFavorite(int Favorite)
{
	if (Favorite < 0)
	{
		ui->labelFavorite->hide();
		return;
	}

	ui->labelFavorite->show();
	m_Favorite = Favorite;

	QString style;

	if (m_Favorite == 0)
	{
		style = QString("QLabel	\
						{	\
						  border-image: url(\":/resource/play-btn52-like-n.png\");	\
						}");

	}
	else
	{
		style = QString("QLabel	\
						{	\
						  border-image: url(\":/resource/play-btn52-like-h.png\");	\
						}");

	}
	ui->labelFavorite->setStyleSheet(style);
}

