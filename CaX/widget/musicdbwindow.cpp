#include <QStackedWidget>

#include "musicdbwindow.h"
#include "ui_musicdbwindow.h"

#include "dialog/commondialog.h"
#include "dialog/confirmcoverartdialog.h"
#include "dialog/edittagdialog.h"
#include "dialog/inputnamedialog.h"
#include "dialog/limitcountdialog.h"
#include "dialog/searchcoverartdialog.h"
#include "dialog/searchcoverartresultdialog.h"
#include "dialog/selectformatdialog.h"
#include "dialog/setcategorycolumndialog.h"
#include "dialog/settrackcolumndialog.h"
#include "dialog/trackinfodialog.h"
#include "dialog/trackinfo.h"

#include "manager/musicdbmanager.h"
#include "manager/sqlmanager.h"

#include "util/caxkeyvalue.h"
#include "util/caxtranslate.h"
#include "util/log.h"
#include "util/settingio.h"
#include "util/utilnovatron.h"

#include "widget/form/formplay.h"
#include "widget/form/formsort.h"
#include "widget/form/formclassify.h"

#include "widget/formTop/infohome.h"
#include "widget/formTop/infotracks.h"
#include "widget/formBottom/icontracks.h"
#include "widget/formBottom/icontracksdelegate.h"
#include "widget/formBottom/tabletracks.h"
#include "widget/formBottom/tabletracksdelegate.h"

#include "widget/playlistwindow.h"

const QString SETTINGS_GROUP = "MusicDB";

MusicDBWindow::MusicDBWindow(QWidget *parent, const QString &addr, const int &eventID) :
	QWidget(parent),
	m_pMgr(new MusicDBManager),
	m_pInfoHome(new InfoHome(this)),
	m_pInfoTracks(new InfoTracks(this)),
	m_pIconTracks(new IconTracks(this)),
	m_pTableTracks(new TableTracks(this)),
	m_EventID(eventID),
	m_nCategory(SQLManager::CATEGORY_ALBUM),
	m_nFavorite(-1),
	m_nRating(-1),
	m_ArtistID(""),
	m_GenreID(""),
	m_ComposerID(""),
	m_Format(""),
	m_MostPlayed(-1),
	m_CurPage(0),
	m_nID(-1),
	m_nCatID(-1),
	m_nCatID2(-1),
	m_nOptionID(-1),
	ui(new Ui::MusicDBWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
	ReadSettings();
	Initialize();
}

MusicDBWindow::~MusicDBWindow()
{
//	WriteSettings();

	if (m_pMgr)
	{
		delete m_pMgr;
		m_pMgr = nullptr;
	}

	if (m_pInfoHome)
	{
		delete m_pInfoHome;
		m_pInfoHome = nullptr;
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

	if (m_pTableTracks)
	{
		delete m_pTableTracks;
		m_pTableTracks = nullptr;
	}

	delete ui;

}

void MusicDBWindow::AddWidgetItem(int typeMode, int category)
{
	m_ListMode = GetListModeFromResize(m_ResizeItem);
	m_TypeMode = typeMode;
	m_nCategory = category;

	SetSortMenu(category);

	m_pIconTracks->GetDelegate()->SetTypeMode(m_TypeMode);

	ui->gridLayoutTop->addWidget(m_pInfoHome);
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pInfoHome->GetFormSort()->SetResize(m_ResizeItem);
		ui->gridLayoutBottom->addWidget(m_pIconTracks);
	}
	else
	{
		m_pInfoHome->GetFormSort()->SetResize(m_ResizeItem);
		ui->gridLayoutBottom->addWidget(m_pTableTracks);
	}

	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		m_pTableTracks->SetHeaderTitle(STR_NAME);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		m_pTableTracks->SetHeaderTitle(STR_TITLE);
	}

	if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		m_pInfoHome->GetFormPlay()->ShowPlayAll(false);
	}

	SetColumn(typeMode);
}

void MusicDBWindow::AddWidgetTrack(int typeMode, int category)
{
	m_ListMode = GetListModeFromResize(m_ResizeTrack);
	m_TypeMode = typeMode;
	m_nCategory = category;

	SetSortMenu(SQLManager::CATEGORY_TRACK);

	m_pIconTracks->GetDelegate()->SetTypeMode(m_TypeMode);

	ui->gridLayoutTop->addWidget(m_pInfoTracks);
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pInfoTracks->GetFormSort()->SetResize(m_ResizeTrack);
		ui->gridLayoutBottom->addWidget(m_pIconTracks);
	}
	else
	{
		m_pInfoTracks->GetFormSort()->SetResize(m_ResizeTrack);
		ui->gridLayoutBottom->addWidget(m_pTableTracks);
	}


	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		m_pTableTracks->SetHeaderTitle(STR_NAME);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		m_pTableTracks->SetHeaderTitle(STR_TITLE);
	}

	if (m_TypeMode == TYPE_MODE_TRACK_ADD)
	{
		m_pInfoTracks->GetFormPlay()->ShowPlayAll(false);
	}

	if (m_nCategory == SQLManager::CATEGORY_TRACK || m_nCategory == SQLManager::CATEGORY_YEAR)
	{
		m_pInfoTracks->GetFormPlay()->ShowFavorite(false);
		m_pInfoTracks->GetFormPlay()->ShowRating(false);
	}
	else
	{
		m_pInfoTracks->GetFormPlay()->ShowFavorite(true);
		m_pInfoTracks->GetFormPlay()->ShowRating(true);
	}

	SetColumn(typeMode);
}

void MusicDBWindow::RequestCategoryList(int catID, int catID2)
{
//	LogDebug("m_TypeMode [%d], category [%d], catID [%d], catID2 [%d]", m_TypeMode, m_nCategory, catID, catID2);
	m_pMgr->RequestMusicDBOverView();

	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		if (m_nCategory == SQLManager::CATEGORY_ARTIST
				|| m_nCategory == SQLManager::CATEGORY_COMPOSER
				|| m_nCategory == SQLManager::CATEGORY_MOOD
				|| m_nCategory == SQLManager::CATEGORY_GENRE)
		{
			if (SQLManager::DISP_MODE_TRACK == m_DispMode)
			{
				QString title = UtilNovatron::GetCategoryTitleName(m_nCategory) + " / " + KEY_TRACK;
				m_pInfoHome->SetTitle(title);
			}
			else if (SQLManager::DISP_MODE_ALBUM == m_DispMode)
			{
				QString title = UtilNovatron::GetCategoryTitleName(m_nCategory) + " / " + KEY_ALBUM;
				m_pInfoHome->SetTitle(title);
			}
			else if (SQLManager::DISP_MODE_ARTIST == m_DispMode)
			{
				QString title = UtilNovatron::GetCategoryTitleName(m_nCategory) + " / " + KEY_ARTIST;
				m_pInfoHome->SetTitle(title);
			}
			else
			{
				QString title = UtilNovatron::GetCategoryTitleName(m_nCategory);
				m_pInfoHome->SetTitle(title);
			}
		}
		else
		{
			QString title = UtilNovatron::GetCategoryTitleName(m_nCategory);
			m_pInfoHome->SetTitle(title);
		}

		if (SQLManager::CATEGORY_TRACK == m_nCategory)
		{
			m_pMgr->RequestTrackList(m_nID,
									 m_nCategory,
									 m_nSortTrack,
									 m_bIncreaseTrack,
									 m_ArtistID,
									 m_GenreID,
									 m_ComposerID,
									 m_Format,
									 m_MostPlayed,
									 m_nFavorite,
									 m_nRating,
									 m_LimitCount * m_CurPage,
									 m_LimitCount);
		}
		else
		{
			m_pMgr->RequestCategoryList(m_nCategory,
										m_nSortCategory,
										m_bIncreaseCategory,
										m_ArtistID,
										m_GenreID,
										m_ComposerID,
										m_Format,
										m_MostPlayed,
										m_nFavorite,
										m_nRating,
										m_LimitCount * m_CurPage,
										m_LimitCount);
		}
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ALBUM)
	{
		m_nCatID = catID;
		QString strCatID = (m_nCatID < 0) ? "" : QString::number(m_nCatID);

		m_pInfoHome->SetTitle(UtilNovatron::GetCategoryTitleName(m_nCategory) + " / " + KEY_ALBUM);
		m_pMgr->RequestAlbumOfCategoryList(m_nCategory,
										   m_nSortCategory,
										   m_bIncreaseCategory,
										   m_ArtistID,
										   m_GenreID,
										   m_ComposerID,
										   m_Format,
										   m_MostPlayed,
										   m_nFavorite,
										   m_nRating,
										   m_LimitCount * m_CurPage,
										   m_LimitCount,
										   strCatID);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ARTIST)
	{
		m_nCatID = catID;
		QString strCatID = (m_nCatID < 0) ? "" : QString::number(m_nCatID);

		m_pInfoHome->SetTitle(UtilNovatron::GetCategoryTitleName(m_nCategory) + " / " + KEY_ARTIST);
		m_pMgr->RequestArtistOfCategoryList(m_nCategory,
											m_nSortCategory,
											m_bIncreaseCategory,
											m_ArtistID,
											m_GenreID,
											m_ComposerID,
											m_Format,
											m_MostPlayed,
											m_nFavorite,
											m_nRating,
											m_LimitCount * m_CurPage,
											m_LimitCount,
											strCatID);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		m_nCatID = catID;
		m_nCatID2 = catID2;
		QString strCatID = (catID < 0) ? "" : QString::number(catID);
		QString strCatID2 = (catID2 < 0) ? "" : QString::number(catID2);

		m_pInfoHome->SetTitle(UtilNovatron::GetCategoryTitleName(m_nCategory) + " / " + KEY_ARTIST + " / " + KEY_ALBUM);
		m_pMgr->RequestAlbumOfArtistOfCategoryList(m_nCategory,
												   m_nSortCategory,
												   m_bIncreaseCategory,
												   m_ArtistID,
												   m_GenreID,
												   m_ComposerID,
												   m_Format,
												   m_MostPlayed,
												   m_nFavorite,
												   m_nRating,
												   m_LimitCount * m_CurPage,
												   m_LimitCount,
												   strCatID,
												   strCatID2);
	}

}

void MusicDBWindow::RequestTrackList(int nID, int catID, int catID2)
{
//	LogDebug("m_TypeMode [%d], category [%d], catID [%d], catID2 [%d]", m_TypeMode, m_nCategory, catID, catID2);

	m_nID = nID;

//	if (m_nCategory == SQLManager::CATEGORY_TRACK)
//	{
//		m_pInfoTracks->SetTitle(KEY_TRACK);
//		m_pMgr->RequestMusicDBOverView();
//	}
//	else
	{
		int overviewID = -1;
		if (catID2 >= 0)
		{
			overviewID = catID2;
		}
		else if (catID >= 0)
		{
			overviewID = catID;
		}
		else
		{
			overviewID = nID;
		}
		m_pMgr->RequestCategoryOverview(overviewID, m_nCategory);
	}

	if (m_TypeMode == TYPE_MODE_TRACK_ALBUM)
	{
		QString strCatID = (catID < 0) ? "" : QString::number(catID);

		m_pMgr->RequestTrackListOfAlbum(m_nID,
										m_nCategory,
										m_nSortTrack,
										m_bIncreaseTrack,
										m_LimitCount * m_CurPage,
										m_LimitCount,
										strCatID);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		QString strCatID = (catID < 0) ? "" : QString::number(catID);
		QString strCatID2 = (catID2 < 0) ? "" : QString::number(catID2);

		m_pMgr->RequestTrackListOfAlbumOfArtist(m_nID,
												m_nCategory,
												m_nSortTrack,
												m_bIncreaseTrack,
												m_LimitCount * m_CurPage,
												m_LimitCount,
												strCatID,
												strCatID2);
	}
	else
	{
		m_pMgr->RequestTrackList(m_nID,
								 m_nCategory,
								 m_nSortTrack,
								 m_bIncreaseTrack,
								 "",
								 "",
								 "",
								 "",
								 -1,
								 -1,
								 -1,
								 m_LimitCount * m_CurPage,
								 m_LimitCount);
	}
}

//int MusicDBWindow::GetCategory() const
//{
//	return m_nCategory;
//}

//void MusicDBWindow::SetCategory(int nCategory)
//{
//	m_nCategory = nCategory;
//}

void MusicDBWindow::SlotAddWidget(QWidget *widget, QString title)
{
	emit SigAddWidget(widget, title);	// recursive
}

void MusicDBWindow::SlotRemoveWidget(QWidget *widget)
{
	emit SigRemoveWidget(widget);
}

void MusicDBWindow::SlotUpdateFavoriteParent(int id, int favorite)
{
//	LogDebug("refresh id [%d] favorite [%d]", id, favorite);
	int index = -1;

	if (m_ListMode == VIEW_MODE_ICON)
	{
		int count = m_pIconTracks->GetModel()->rowCount();

		QModelIndex modelIndex;
		for (int i = 0; i < count; i++)
		{
			modelIndex = m_pIconTracks->GetModel()->index(i, 0);
			if (qvariant_cast<int>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_ID)) == id)
			{
				index = i;
				break;
			}
		}
	}
	else
	{
		int count = m_pTableTracks->GetModel()->rowCount();

		QModelIndex modelIndex;
		for (int i = 0; i < count; i++)
		{
			modelIndex = m_pTableTracks->GetModel()->index(i, 0);
			if (qvariant_cast<int>(m_pTableTracks->GetModel()->data(m_pTableTracks->GetModel()->index(i, TableTracks::TABLE_TRACKS_ID)) == id))
			{
				index = i;
				break;
			}
		}
	}

	if (index >= 0)
	{
		if (m_ListMode == VIEW_MODE_ICON)
		{
			QModelIndex modelIndex = m_pIconTracks->GetModel()->index(index, 0);
			m_pIconTracks->GetModel()->setData(modelIndex, favorite, IconTracksDelegate::ICON_TRACKS_FAVORITE);
		}
		else
		{
			QModelIndex modelIndex = m_pTableTracks->GetModel()->index(index, TableTracks::TABLE_TRACKS_FAVORITE);
			m_pTableTracks->GetModel()->setData(modelIndex, favorite);
		}
	}
	else
	{
		LogWarning("favorite item is not found");
	}

}

void MusicDBWindow::SlotUpdateRatingParent(int id, int rating)
{
//	LogDebug("refresh id [%d] rating [%d]", id, rating);
	int index = -1;
	if (m_ListMode == VIEW_MODE_ICON)
	{
		int count = m_pIconTracks->GetModel()->rowCount();

		QModelIndex modelIndex;
		for (int i = 0; i < count; i++)
		{
			modelIndex = m_pIconTracks->GetModel()->index(i, 0);
			if (qvariant_cast<int>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_ID)) == id)
			{
				index = i;
				break;
			}
		}
	}
	else
	{
		int count = m_pTableTracks->GetModel()->rowCount();

		QModelIndex modelIndex;
		for (int i = 0; i < count; i++)
		{
			modelIndex = m_pTableTracks->GetModel()->index(i, 0);
			if (qvariant_cast<int>(m_pTableTracks->GetModel()->data(m_pTableTracks->GetModel()->index(i, TableTracks::TABLE_TRACKS_ID)) == id))
			{
				index = i;
				break;
			}
		}
	}

	if (index >= 0)
	{
		if (m_ListMode == VIEW_MODE_ICON)
		{
			QModelIndex modelIndex = m_pIconTracks->GetModel()->index(index, 0);
			m_pIconTracks->GetModel()->setData(modelIndex, rating, IconTracksDelegate::ICON_TRACKS_RATING);
		}
		else
		{
			QModelIndex modelIndex = m_pTableTracks->GetModel()->index(index, TableTracks::TABLE_TRACKS_RATING);
			m_pTableTracks->GetModel()->setData(modelIndex, rating);
		}
	}
	else
	{
		LogWarning("rating item is not found");
	}
}

void MusicDBWindow::SlotRespError(QString errMsg)
{
	if (errMsg.toLower().contains("not found"))
	{
		m_pIconTracks->ShowFrameEmpty(true);
		m_pTableTracks->ShowFrameEmpty(true);
	}
	else if (errMsg.toLower().contains("ok"))
	{
		// nothing
	}
	else
	{
		CommonDialog dialog(this, STR_WARNING, errMsg);
		dialog.exec();
	}
}

void MusicDBWindow::SlotRespMusicOverview(CJsonNode node)
{
//	LogDebug("node [%s]", node.ToCompactByteArray().data());

	QString strAlbumCnt = node.GetString(KEY_ALBUM);
	QString strAlbumArtistCnt = node.GetString(KEY_ALBUM_ARTIST);
	QString strArtistCnt = node.GetString(KEY_ARTIST);
	QString strGenreCnt = node.GetString(KEY_GENRE);

	QString strComposerCnt = node.GetString(KEY_COMPOSER);
	QString strMoodCnt = node.GetString(KEY_MOOD);
	QString strFolderCnt = node.GetString(KEY_FOLDER);
	QString strYearCnt = node.GetString(KEY_YEAR);
	QString strTrackCnt = node.GetString(KEY_SONG);

//	if (m_nCategory != SQLManager::CATEGORY_TRACK)
	{
		if (strAlbumCnt.toInt() > 0)
		{
			m_pInfoHome->SetAlbumCnt(strAlbumCnt);
		}
		if (strAlbumArtistCnt.toInt() > 0)
		{
			m_pInfoHome->SetAlbumArtistCnt(node.GetString(KEY_ALBUM_ARTIST));
		}
		if (strArtistCnt.toInt() > 0)
		{
			m_pInfoHome->SetArtistCnt(node.GetString(KEY_ARTIST));
			m_pMgr->RequestClassifyList(SQLManager::CLASSIFY_ARTIST);
		}
		if (strGenreCnt.toInt() > 0)
		{
			m_pInfoHome->SetGenreCnt(node.GetString(KEY_GENRE));
			m_pMgr->RequestClassifyList(SQLManager::CLASSIFY_GENRE);
		}
		if (strComposerCnt.toInt() > 0)
		{
			m_pMgr->RequestClassifyList(SQLManager::CLASSIFY_COMPOSER);
		}

		SetClassifyFormat();
		SetClassifyMostPlayed();
	}

	QString strCnt = "";
	switch (m_nCategory)
	{
	case SQLManager::CATEGORY_ALBUM:
		strCnt = strAlbumCnt;
		break;
	case SQLManager::CATEGORY_ALBUM_ARTIST:
		strCnt = strAlbumArtistCnt;
		break;
	case SQLManager::CATEGORY_ARTIST:
		strCnt = strArtistCnt;
		break;
	case SQLManager::CATEGORY_COMPOSER:
		strCnt = strComposerCnt;
		break;
	case SQLManager::CATEGORY_GENRE:
		strCnt = strGenreCnt;
		break;
	case SQLManager::CATEGORY_MOOD:
		strCnt = strMoodCnt;
		break;
	case SQLManager::CATEGORY_FOLDER:
		strCnt = strFolderCnt;
		break;
	case SQLManager::CATEGORY_YEAR:
		strCnt = strYearCnt;
		break;
	case SQLManager::CATEGORY_TRACK:
		strCnt = strTrackCnt;
		break;
	}

	m_pInfoHome->SetCategoryCnt(strCnt);
}

void MusicDBWindow::SlotRespCategoryList(QList<CJsonNode> list)
{
	m_CurPage++;

	m_RespList.append(list);
	SetOptionMenu();

	int service = SIDEMENU_MUSIC_DB;
	if (m_nCategory == SQLManager::CATEGORY_YEAR)
	{
		service = IconTracks::ICON_TRACKS_MUSIC_DB_YEAR;
	}
	else if (m_nCategory == SQLManager::CATEGORY_TRACK)
	{
		service = IconTracks::ICON_TRACKS_MUSIC_DB_TRACK;
	}

	if (m_ListMode == VIEW_MODE_ICON)
	{
//		m_pIconTracks->ClearNodeList();
		m_pIconTracks->SetNodeList(list, service);
	}
	else
	{
//		m_pTableTracks->ClearNodeList();
		m_pTableTracks->SetNodeList(list, service);
	}

	m_EnableAppend = true;
}

void MusicDBWindow::SlotRespCategoryOverview(CJsonNode node)
{
//	LogDebug("node [%s]", node.ToCompactByteArray().data());
	QString title = node.GetString(KEY_TITLE);
	QString artist = node.GetString(KEY_ARTIST);
	QString favorite = node.GetString(KEY_FAVORITE);
	QString rating = node.GetString(KEY_RATING);
	QString count = node.GetString(KEY_COUNT);
	QString total = node.GetString(KEY_TOTAL);
	QString extension = node.GetString(KEY_EXTENSION);
	QString samplerate = node.GetString(KEY_SAMPLERATE);
	QString bps = node.GetString(KEY_BPS);

//	QString info = count + " | " + total + " | " + extension + " | " + samplerate + " | " + bps;

	if (title.isEmpty())
	{
		title = STR_TRACK;
	}

	m_pInfoTracks->SetTitle(title);
	m_pInfoTracks->SetSubtitle(artist);
//	m_pInfoTracks->SetInfo(info);
	m_pInfoTracks->GetFormPlay()->SetFavorite(favorite.toInt());
	m_pInfoTracks->GetFormPlay()->SetRating(rating.toInt());

}

void MusicDBWindow::SlotRespTrackList(QList<CJsonNode> list)
{
	m_CurPage++;

	m_RespList.append(list);
	SetOptionMenu();

	int service = SIDEMENU_MUSIC_DB;
//	if (m_nCategory == SQLManager::CATEGORY_YEAR)
//	{
//		service = ListTracks::LIST_TRACKS_MUSIC_DB_YEAR;
//	}
//	else if (m_nCategory == SQLManager::CATEGORY_TRACK)
//	{
//		service = ListTracks::LIST_TRACKS_MUSIC_DB_TRACK;
//	}

	if (m_ListMode == VIEW_MODE_ICON)
	{
//		m_pIconTracks->ClearNodeList();
		m_pIconTracks->SetNodeList(list, service);
	}
	else
	{
//		m_pTableTracks->ClearNodeList();
		m_pTableTracks->SetNodeList(list, service);
	}

	m_EnableAppend = true;
}

void MusicDBWindow::SlotRespTrackListForEditTag(QList<CJsonNode> list)
{
	if (m_AlbumList.isEmpty())
	{
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_ALBUM);
	}
	if (m_AlbumArtistList.isEmpty())
	{
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_ALBUM_ARTIST);
	}
	if (m_ArtistList.isEmpty())
	{
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_ARTIST);
	}
	if (m_GenreList.isEmpty())
	{
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_GENRE);
	}
	if (m_ComposerList.isEmpty())
	{
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_COMPOSER);
	}
	if (m_MoodList.isEmpty())
	{
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_MOOD);
	}

	EditTagDialog dialog;
	dialog.SetAlbumList(m_AlbumList);
	dialog.SetAlbumArtistList(m_AlbumArtistList);
	dialog.SetArtistList(m_ArtistList);
	dialog.SetGenreList(m_GenreList);
	dialog.SetComposerList(m_ComposerList);
	dialog.SetMoodList(m_MoodList);
	dialog.SetNodeList(list);
	if (dialog.exec() == QDialog::Accepted)
	{
		m_UpdateModel = dialog.GetModel();
		m_UpdateMap = dialog.GetUpdateCell();

//		DoInsertIgnoreCategoryAll();
		m_pMgr->RequestSetTrackInfoAll(m_UpdateMap, m_UpdateModel, m_EventID);
	}
	else
	{
//		m_UpdateModel->clear();
//		m_UpdateMap.clear();
	}
}

void MusicDBWindow::SlotCoverArtUpdate(QString fileName, int nIndex, int mode)
{
	if (QListView::IconMode == mode)
	{
		QModelIndex modelIndex = m_pIconTracks->GetModel()->index(nIndex, 0);
		m_pIconTracks->GetModel()->setData(modelIndex, fileName, IconTracksDelegate::ICON_TRACKS_COVER);
	}
	else
	{
		QModelIndex modelIndex = m_pTableTracks->GetModel()->index(nIndex, TableTracks::TABLE_TRACKS_COVER);
		m_pTableTracks->GetModel()->setData(modelIndex, fileName);
	}
}

void MusicDBWindow::SlotPlayAll()
{
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_SelectMap = m_pIconTracks->GetSelectMap();
	}
	else
	{
		m_SelectMap = m_pTableTracks->GetSelectMap();
	}

	if (m_SelectMap.count() > 0)
	{
		m_pMgr->RequestManageCategory(VAL_PLAY, m_SelectMap, PLAY_CLEAR, m_nCategory);
	}
	else
	{
		QMap<int, bool> map;
		m_pMgr->RequestManageCategory(VAL_PLAY, map, PLAY_CLEAR, m_nCategory);
	}
}

void MusicDBWindow::SlotPlayRandom()
{
	m_pMgr->RequestRandom();
}

void MusicDBWindow::SlotTopMenu()
{
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_SelectMap = m_pIconTracks->GetSelectMap();
	}
	else
	{
		m_SelectMap = m_pTableTracks->GetSelectMap();
	}

	if (m_SelectMap.count() > 0)
	{
		SetSelectOnTopMenu();
	}
	else
	{
		SetSelectOffTopMenu();
	}
}

void MusicDBWindow::SlotTopMenuAction(int menuID)
{
	switch (menuID) {
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
	case TOP_MENU_RELOAD:
		DoTopMenuReload();
		break;
	case TOP_MENU_LOAD_COUNT:
		DoTopMenuSetLimitCount(m_LimitCount);
		break;
	case TOP_MENU_SELECT_ALL:
		DoTopMenuSelectAll();
		break;
	case TOP_MENU_CLEAR_ALL:
		DoTopMenuClearAll();
		break;
	case TOP_MENU_GAIN_SET:
		DoTopMenuGainSet();
		break;
	case TOP_MENU_GAIN_CLEAR:
		DoTopMenuGainClear();
		break;
	case TOP_MENU_ADD_TO_PLAYLIST:
		DoTopMenuAddToPlaylist();
		break;
	case TOP_MENU_SHOW_COLUMNS:
		DoTopMenuShowCategoryColumns();
		break;
	}

}

void MusicDBWindow::SlotSortMenu(int menuID)
{
	if (SQLManager::SORT_IMPORTED_DATE == menuID
			|| SQLManager::SORT_ALPHABET == menuID
			|| SQLManager::SORT_FAVORITE == menuID
			|| SQLManager::SORT_RATING == menuID)
	{
		m_DispMode = menuID;
		m_nSortCategory = menuID;
		WriteSettings();

//		RequestCategoryList(m_nCatID, m_nCatID2);
		DoTopMenuReload();

	}
	else if (SQLManager::DISP_MODE_TRACK == menuID)
	{
		m_DispMode = menuID;
		WriteSettings();

		QString title = UtilNovatron::GetCategoryTitleName(m_nCategory) + " / " + KEY_TRACK;
		m_pInfoHome->SetTitle(title);
	}
	else if (SQLManager::DISP_MODE_ALBUM == menuID)
	{
		m_DispMode = menuID;
		WriteSettings();

		QString title = UtilNovatron::GetCategoryTitleName(m_nCategory) + " / " + KEY_ALBUM;
		m_pInfoHome->SetTitle(title);
	}
	else if (SQLManager::DISP_MODE_ARTIST == menuID)
	{
		m_DispMode = menuID;
		WriteSettings();

		QString title = UtilNovatron::GetCategoryTitleName(m_nCategory) + " / " + KEY_ARTIST;
		m_pInfoHome->SetTitle(title);
	}
}

void MusicDBWindow::SlotIncDec(bool bIncrease)
{
	m_bIncreaseCategory = bIncrease;
	WriteSettings();

//	RequestCategoryList(m_nCatID, m_nCatID2);
	DoTopMenuReload();
}

void MusicDBWindow::SlotResize(int resize)
{
	if (m_TypeMode <= TYPE_MODE_ITEM_ADD)
	{
		m_ResizeItem = resize;
	}
	else
	{
		m_ResizeTrack = resize;
	}
	WriteSettings();

	int listMode = GetListModeFromResize(resize);
	if (listMode != m_ListMode)
	{
		m_ListMode = listMode;
		if (m_ListMode == VIEW_MODE_ICON)
		{
			ui->gridLayoutBottom->replaceWidget(m_pTableTracks, m_pIconTracks);

			int service = SIDEMENU_MUSIC_DB;
			if (m_nCategory == SQLManager::CATEGORY_YEAR)
			{
				service = IconTracks::ICON_TRACKS_MUSIC_DB_YEAR;
			}
			else if (m_nCategory == SQLManager::CATEGORY_TRACK)
			{
				service = IconTracks::ICON_TRACKS_MUSIC_DB_TRACK;
			}

//			LogDebug("icon~~~~~~~~[%d][%d]", m_pIconTracks->GetNodeList().count(), m_RespList.count());
			if (m_pIconTracks->GetNodeList().count() != m_RespList.count())
			{
				m_pIconTracks->ClearNodeList();
				m_pIconTracks->SetNodeList(m_RespList, service);
			}

			m_pTableTracks->hide();
			m_pIconTracks->show();
		}
		else
		{
			ui->gridLayoutBottom->replaceWidget(m_pIconTracks, m_pTableTracks);

			int service = SIDEMENU_MUSIC_DB;
//			if (m_nCategory == SQLManager::CATEGORY_YEAR)
//			{
//				service = ListTracks::LIST_TRACKS_MUSIC_DB_YEAR;
//			}
//			else if (m_nCategory == SQLManager::CATEGORY_TRACK)
//			{
//				service = ListTracks::LIST_TRACKS_MUSIC_DB_TRACK;
//			}

//			LogDebug("list~~~~~~~~[%d][%d]", m_pTableTracks->GetNodeList().count(), m_RespList.count());
			if (m_pTableTracks->GetNodeList().count() != m_RespList.count())
			{
				m_pTableTracks->ClearNodeList();
				m_pTableTracks->SetNodeList(m_RespList, service);
			}

			m_pIconTracks->hide();
			m_pTableTracks->show();
		}
	}

	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pIconTracks->SetResize(resize);
	}
	else
	{
		m_pTableTracks->SetColResize(0);
		m_pTableTracks->SetRowResize(resize);
	}
}

void MusicDBWindow::SlotAlbumList()
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetItem(TYPE_MODE_ITEM_TRACK, SQLManager::CATEGORY_ALBUM);
	emit widget->SigAddWidget(widget, STR_MUSIC_DB);
	widget->RequestCategoryList();
}

void MusicDBWindow::SlotAlbumArtistList()
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetItem(TYPE_MODE_ITEM_TRACK, SQLManager::CATEGORY_ALBUM_ARTIST);
	emit widget->SigAddWidget(widget, STR_MUSIC_DB);
	widget->RequestCategoryList();
}

void MusicDBWindow::SlotArtistList()
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetItem(TYPE_MODE_ITEM_TRACK, SQLManager::CATEGORY_ARTIST);
	emit widget->SigAddWidget(widget, STR_MUSIC_DB);
	widget->RequestCategoryList();
}

void MusicDBWindow::SlotGenreList()
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetItem(TYPE_MODE_ITEM_TRACK, SQLManager::CATEGORY_GENRE);
	emit widget->SigAddWidget(widget, STR_MUSIC_DB);
	widget->RequestCategoryList();
}

void MusicDBWindow::SlotCategoryMenu()
{
	m_CategoryMenuMap.clear();
	m_CategoryMenuMap.insert(SQLManager::CATEGORY_COMPOSER, STR_COMPOSER);
	m_CategoryMenuMap.insert(SQLManager::CATEGORY_MOOD, STR_MOOD);
	m_CategoryMenuMap.insert(SQLManager::CATEGORY_FOLDER, STR_FOLDER);
	m_CategoryMenuMap.insert(SQLManager::CATEGORY_TRACK, STR_TRACK);
	m_CategoryMenuMap.insert(SQLManager::CATEGORY_YEAR, STR_YEAR);

	m_pInfoHome->ClearMenu();
	m_pInfoHome->SetMenu(m_CategoryMenuMap);
}

void MusicDBWindow::SlotCategoryMenuAction(int nCategory, QString title)
{

	Q_UNUSED(title)

//	if (nCategory == SQLManager::CATEGORY_TRACK)
//	{
//		MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
//		widget->AddWidgetTrack(TYPE_MODE_TRACK, nCategory);
//		emit widget->SigAddWidget(widget, STR_MUSIC_DB);
//		widget->RequestTrackList(-1);
//		widget->SetCoverArt("");
//	}
//	else
	{
		MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
		widget->AddWidgetItem(TYPE_MODE_ITEM_TRACK, nCategory);
		emit widget->SigAddWidget(widget, STR_MUSIC_DB);
		widget->RequestCategoryList();
	}
}

void MusicDBWindow::SlotItemPlayAll()
{
//	m_pMgr->RequestPlayCategoryItem(m_nID, PLAY_CLEAR, m_nCategory);
	m_SelectMap = m_pTableTracks->GetSelectMap();
	if (m_SelectMap.count() > 0)
	{
		m_pMgr->RequestManageCategory(VAL_PLAY, m_SelectMap, PLAY_CLEAR, SQLManager::CATEGORY_TRACK);
	}
	else
	{
		QMap<int, bool> map;
		map.insert(m_nID, true);
		m_pMgr->RequestManageCategory(VAL_PLAY, map, PLAY_CLEAR, m_nCategory);
	}
}

void MusicDBWindow::SlotItemPlayRandom()
{
	m_pMgr->RequestRandom();
}

void MusicDBWindow::SlotItemTopMenu()
{
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_SelectMap = m_pIconTracks->GetSelectMap();
	}
	else
	{
		m_SelectMap = m_pTableTracks->GetSelectMap();
	}

	if (m_SelectMap.count() > 0)
	{
		SetSelectOnTopMenu();
	}
	else
	{
		SetSelectOffTopMenu();
	}
}

void MusicDBWindow::SlotItemTopMenuAction(int menuID)
{
	switch (menuID) {
	case TOP_MENU_PLAY_NOW:
		DoTopMenuItemPlay(PLAY_NOW);
		break;
	case TOP_MENU_PLAY_LAST:
		DoTopMenuItemPlay(PLAY_LAST);
		break;
	case TOP_MENU_PLAY_NEXT:
		DoTopMenuItemPlay(PLAY_NEXT);
		break;
	case TOP_MENU_PLAY_CLEAR:
		DoTopMenuItemPlay(PLAY_CLEAR);
		break;
	case TOP_MENU_RELOAD:
		DoTopMenuReload();
		break;
	case TOP_MENU_LOAD_COUNT:
		DoTopMenuSetLimitCount(m_LimitCount);
		break;
	case TOP_MENU_SELECT_ALL:
		DoTopMenuItemSelectAll();
		break;
	case TOP_MENU_CLEAR_ALL:
		DoTopMenuItemClearAll();
		break;
	case TOP_MENU_GAIN_SET:
		DoTopMenuItemGainSet();
		break;
	case TOP_MENU_GAIN_CLEAR:
		DoTopMenuItemGainClear();
		break;
	case TOP_MENU_ADD_TO_PLAYLIST:
		DoTopMenuItemAddToPlaylist();
		break;
	case TOP_MENU_SHOW_COLUMNS:
		DoTopMenuItemShowTrackColumns();
		break;
	}

}

void MusicDBWindow::SlotItemFavorite(int nFavorite)
{
	m_pMgr->RequestUpdateFavorite(m_nID, nFavorite, m_nCategory);
	emit SigUpdateFavoriteParent(m_nID, nFavorite);
}

void MusicDBWindow::SlotItemRating(int nRating)
{
	m_pMgr->RequestUpdateRating(m_nID, nRating, m_nCategory);
	emit SigUpdateRatingParent(m_nID, nRating);
}

void MusicDBWindow::SlotItemSortMenu(int sort)
{
	m_nSortTrack = sort;
	WriteSettings();

	DoTopMenuReload();
}

void MusicDBWindow::SlotItemIncDec(bool bIncrease)
{
	m_bIncreaseTrack = bIncrease;
	WriteSettings();

	DoTopMenuReload();
}

void MusicDBWindow::SlotSelectPlay(int nID, int where)
{
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		SlotSelectCategoryPlay(nID, where);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		SlotSelectTrackPlay(nID, where);
	}
}

void MusicDBWindow::SlotSelectTitle(const QModelIndex &index)
{
	int id;
	QString cover;
	QString title;
	QString count;

	if (m_ListMode == VIEW_MODE_ICON)
	{
		id = qvariant_cast<int>(index.data(IconTracksDelegate::ICON_TRACKS_ID));
		cover = qvariant_cast<QString>(index.data(IconTracksDelegate::ICON_TRACKS_COVER));
		title = qvariant_cast<QString>(index.data(IconTracksDelegate::ICON_TRACKS_TITLE));
		count = qvariant_cast<QString>(index.data(IconTracksDelegate::ICON_TRACKS_COUNT));
	}
	else
	{
		int row = index.row();
		QStandardItemModel *model = m_pTableTracks->GetModel();
		id = qvariant_cast<int>(model->data(model->index(row, TableTracks::TABLE_TRACKS_ID)));
		cover = qvariant_cast<QString>(model->data(model->index(row, TableTracks::TABLE_TRACKS_COVER)));
		title = qvariant_cast<QString>(model->data(model->index(row, TableTracks::TABLE_TRACKS_TITLE)));
		count = qvariant_cast<QString>(model->data(model->index(row, TableTracks::TABLE_TRACKS_TRACK_COUNT)));
	}

//	LogDebug("id [%d] cover art [%s]", nID, coverArt.toUtf8().data());
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK)
	{
		if (m_nCategory == SQLManager::CATEGORY_ARTIST
				|| m_nCategory == SQLManager::CATEGORY_COMPOSER
				|| m_nCategory == SQLManager::CATEGORY_MOOD
				|| m_nCategory == SQLManager::CATEGORY_GENRE)
		{
			if (m_DispMode == SQLManager::DISP_MODE_ALBUM)
			{
	//			LogDebug("### Album mode");
				MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
				widget->AddWidgetItem(TYPE_MODE_ITEM_ALBUM, m_nCategory);
				emit widget->SigAddWidget(widget, STR_MUSIC_DB);
				widget->RequestCategoryList(id);
				widget->SetInfoHome(title, count);
			}
			else if (m_DispMode == SQLManager::DISP_MODE_ARTIST)
			{
	//			LogDebug("### Artist mode");
				MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
				widget->AddWidgetItem(TYPE_MODE_ITEM_ARTIST, m_nCategory);
				emit widget->SigAddWidget(widget, STR_MUSIC_DB);
				widget->RequestCategoryList(id);
				widget->SetInfoHome(title, count);
			}
			else
			{
	//			LogDebug("### Track mode");
				MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
				widget->AddWidgetTrack(TYPE_MODE_TRACK, m_nCategory);
				emit widget->SigAddWidget(widget, STR_MUSIC_DB);
				widget->SetCoverArt(cover);
				widget->RequestTrackList(id);
			}
		}
		else
		{
//			LogDebug("### Track mode");
			MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
			widget->AddWidgetTrack(TYPE_MODE_TRACK, m_nCategory);
			emit widget->SigAddWidget(widget, STR_MUSIC_DB);
			widget->SetCoverArt(cover);
			widget->RequestTrackList(id);
		}

	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ALBUM)
	{
//		LogDebug("### Track mode");
		MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
		widget->AddWidgetTrack(TYPE_MODE_TRACK_ALBUM, m_nCategory);
		emit widget->SigAddWidget(widget, STR_MUSIC_DB);
		widget->SetCoverArt(cover);
		widget->RequestTrackList(id, m_nCatID);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ARTIST)
	{
//		LogDebug("### Album mode");
		MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
		widget->AddWidgetItem(TYPE_MODE_ITEM_ARTIST_ALBUM, m_nCategory);
		emit widget->SigAddWidget(widget, STR_MUSIC_DB);
		widget->RequestCategoryList(m_nCatID, id);
		widget->SetInfoHome(title, count);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
//		LogDebug("### Track mode");
		MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
		widget->AddWidgetTrack(TYPE_MODE_TRACK_ALBUM_ARTIST, m_nCategory);
		emit widget->SigAddWidget(widget, STR_MUSIC_DB);
		widget->SetCoverArt(cover);
		widget->RequestTrackList(id, m_nCatID, m_nCatID2);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
		widget->AddWidgetTrack(TYPE_MODE_TRACK_ADD, m_nCategory);
		emit widget->SigAddWidget(widget, STR_MUSIC_DB);
		widget->SetCoverArt(cover);
		widget->RequestTrackList(id);

		connect(widget, SIGNAL(SigAddTrackFromPlaylist(QMap<int, bool>)), this, SLOT(SlotAddTrackFromPlaylist(QMap<int, bool>)));

	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		SlotSelectTrackPlay(id, PLAY_CLEAR);
	}

}

void MusicDBWindow::SlotSelectCategoryPlay(int nID, int where)
{
	QMap<int, bool> map;
	map.insert(nID, true);
	m_pMgr->RequestManageCategory(VAL_PLAY, map, where, m_nCategory);
}

void MusicDBWindow::SlotSelectFavorite(int nID, int nFavorite)
{
	m_pMgr->RequestUpdateFavorite(nID, nFavorite, m_nCategory);
}

void MusicDBWindow::SlotSelectRating(int nID, int nRating)
{
	m_pMgr->RequestUpdateRating(nID, nRating, m_nCategory);
}

void MusicDBWindow::SlotReqCoverArt(int id, int index, int mode)
{
	QString strCat;
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		strCat = UtilNovatron::GetCategoryName(m_nCategory);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ALBUM
			 || m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		strCat = KEY_ALBUM;
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ARTIST)
	{
		strCat = KEY_ARTIST;
	}
	else
	{
		strCat = UtilNovatron::GetCategoryName(-1);
	}

	QStringList lsAddr = m_pMgr->GetAddr().split(":");
	QString fullpath = QString("%1:%2/%3/%4").arg(lsAddr[0]).arg(PORT_IMAGE_SERVER).arg(strCat).arg(id);

//	LogDebug("path [%s] index [%d] typeMode [%d]", fullpath.toUtf8().data(), index, m_TypeMode);
	m_pMgr->RequestCoverArt(fullpath, index, mode);
}

void MusicDBWindow::SlotAppendList()
{
	if (m_LimitCount <= 0 || !m_EnableAppend)
	{
		return;
	}

	m_EnableAppend = false;

	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		RequestCategoryList(m_nCatID, m_nCatID2);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		RequestTrackList(m_nID, m_nCatID, m_nCatID2);
	}
}

void MusicDBWindow::SlotSelectTrackPlay(int nID, int where)
{
//	m_pMgr->RequestPlaySong(nID);
	QMap<int, bool> map;
	map.insert(nID, true);
	m_pMgr->RequestManageCategory(VAL_PLAY, map, where, SQLManager::CATEGORY_TRACK);
}

void MusicDBWindow::SlotSelectTrackFavorite(int nID, int index, int nFavorite)
{
	if (m_TypeMode == TYPE_MODE_TRACK
			|| m_TypeMode == TYPE_MODE_TRACK_ALBUM
			|| m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		QModelIndex modelIndex = m_pTableTracks->GetModel()->index(index, TableTracks::TABLE_TRACKS_FAVORITE);
		nFavorite = nFavorite == 0 ? 1 : 0;
		m_pTableTracks->GetModel()->setData(modelIndex, nFavorite);

		m_pMgr->RequestUpdateTrackFavorite(nID, nFavorite);
	}
}

void MusicDBWindow::SlotRespClassifyArtist(QList<CJsonNode> list)
{
	m_pInfoHome->GetFormClassify()->ClearClassifyArtistMenu();
	m_pInfoHome->GetFormClassify()->SetClassifyArtistMenu(list);
}

void MusicDBWindow::SlotRespClassifyGenre(QList<CJsonNode> list)
{
	m_pInfoHome->GetFormClassify()->ClearClassifyGenreMenu();
	m_pInfoHome->GetFormClassify()->SetClassifyGenreMenu(list);
}

void MusicDBWindow::SlotRespClassifyComposer(QList<CJsonNode> list)
{
	m_pInfoHome->GetFormClassify()->ClearClassifyComposerMenu();
	m_pInfoHome->GetFormClassify()->SetClassifyComposerMenu(list);
}

void MusicDBWindow::SlotClassifyFavorite(int nFavorite)
{
	ClearList();

	m_nFavorite = nFavorite;
	RequestCategoryList(m_nCatID, m_nCatID2);
}

void MusicDBWindow::SlotClassifyRating(int nRating)
{
	ClearList();

	m_nRating = nRating;
	RequestCategoryList(m_nCatID, m_nCatID2);
}

void MusicDBWindow::SlotClassifyArtist(bool bAdd, QString id)
{
	ClearList();

	//	LogDebug("artist add [%d] id [%s]", bAdd, id.toUtf8().data());
	if (bAdd)
	{
		m_ArtistID = id;
	}
	else
	{
		m_ArtistID = "";
	}
	RequestCategoryList(m_nCatID, m_nCatID2);

}

void MusicDBWindow::SlotClassifyGenre(bool bAdd, QString id)
{
	ClearList();

	//	LogDebug("genre add [%d] id [%s]", bAdd, id.toUtf8().data());
	if (bAdd)
	{
		m_GenreID = id;
	}
	else
	{
		m_GenreID = "";
	}
	RequestCategoryList(m_nCatID, m_nCatID2);
}

void MusicDBWindow::SlotClassifyComposer(bool bAdd, QString id)
{
	ClearList();

	//	LogDebug("composer add [%d] id [%s]", bAdd, id.toUtf8().data());
	if (bAdd)
	{
		m_ComposerID = id;
	}
	else
	{
		m_ComposerID = "";
	}
	RequestCategoryList(m_nCatID, m_nCatID2);
}

void MusicDBWindow::SlotClassifyFormat(bool bAdd, QString id)
{
	ClearList();

//	LogDebug("format add [%d] id [%s]", bAdd, id.toUtf8().data());
	if (bAdd)
	{
		int nID = id.toInt();
		switch (nID)
		{
		case SelectFormatDialog::FORMAT_ID_CAF:
			m_Format = FORMAT_CAF;
			break;
		case SelectFormatDialog::FORMAT_ID_WAV:
			m_Format = FORMAT_WAV;
			break;
		case SelectFormatDialog::FORMAT_ID_AIFF:
			m_Format = FORMAT_AIFF;
			break;
		case SelectFormatDialog::FORMAT_ID_FLAC:
			m_Format = FORMAT_FLAC;
			break;
		case SelectFormatDialog::FORMAT_ID_M4A:
			m_Format = FORMAT_M4A;
			break;
		case SelectFormatDialog::FORMAT_ID_MP3:
			m_Format = FORMAT_MP3;
			break;
		case SelectFormatDialog::FORMAT_ID_OGG:
			m_Format = FORMAT_OGG;
			break;
		case SelectFormatDialog::FORMAT_ID_WMA:
			m_Format = FORMAT_WMA;
			break;
		}
	}
	else
	{
		m_Format = "";
	}
	RequestCategoryList(m_nCatID, m_nCatID2);
}

void MusicDBWindow::SlotClassifyMostPlayed(bool bAdd, QString id)
{
	ClearList();

//	LogDebug("most played add [%d] id [%s]", bAdd, id.toUtf8().data());
	if (bAdd)
	{
		int nID = id.toInt();
		switch (nID)
		{
		case 0:
			m_MostPlayed = 10;
			break;
		case 1:
			m_MostPlayed = 25;
			break;
		case 2:
			m_MostPlayed = 50;
			break;
		case 3:
			m_MostPlayed = 100;
			break;
		}
	}
	else
	{
		m_MostPlayed = -1;
	}
	RequestCategoryList(m_nCatID, m_nCatID2);
}

void MusicDBWindow::SlotRespCategoryInfo(CJsonNode node)
{
	TrackInfoDialog dialog;
	dialog.SetWindowTitle(STR_ALBUM_INFO);
	dialog.SetAddr(m_pMgr->GetAddr());
	dialog.SetAlbumList(m_AlbumList);
	dialog.SetAlbumArtistList(m_AlbumArtistList);
//	dialog.SetArtistList(m_ArtistList);
//	dialog.SetGenreList(m_GenreList);
//	dialog.SetComposerList(m_ComposerList);
//	dialog.SetMoodList(m_MoodList);
	dialog.SetBtnEdit(true);
	dialog.SetMode(TrackInfo::TRACK_INFO_MODE_VIEW);
	dialog.SetInfoData(node);
	if (dialog.exec() == QDialog::Accepted)
	{
		node = dialog.GetInfoData();
		m_pMgr->RequestSetCategoryInfo(m_nOptionID, m_EventID, node);
	}
}

void MusicDBWindow::SlotRespCategoryInfoList(QList<CJsonNode> list)
{
	SetCategoryList(list);
}

void MusicDBWindow::SlotRespTrackInfo(CJsonNode node)
{
	TrackInfoDialog dialog;
	dialog.SetWindowTitle(STR_TRACK_INFO);
	dialog.SetAddr(m_pMgr->GetAddr());
	dialog.SetAlbumList(m_AlbumList);
	dialog.SetAlbumArtistList(m_AlbumArtistList);
	dialog.SetArtistList(m_ArtistList);
	dialog.SetGenreList(m_GenreList);
	dialog.SetComposerList(m_ComposerList);
	dialog.SetMoodList(m_MoodList);
	dialog.SetBtnEdit(true);
	dialog.SetMode(TrackInfo::TRACK_INFO_MODE_VIEW);
	dialog.SetInfoData(node);
	if (dialog.exec() == QDialog::Accepted)
	{
		node = dialog.GetInfoData();
		m_pMgr->RequestSetTrackInfo(m_nOptionID, m_EventID, node);
	}
}

void MusicDBWindow::SlotRespSetCoverArt(int id, int category)
{
	// delete
	QString part = QString("%1_%2").arg(UtilNovatron::GetCategoryName(category)).arg(id);
	UtilNovatron::RemoveContainFilesInTempDirectory(part);
//	LogDebug("id [%d] category [%d] part [%s]", id, category, part.toUtf8().data());

	// refresh
	if (m_ListMode == VIEW_MODE_ICON)
	{
		int index = -1;
		int count = m_pIconTracks->GetModel()->rowCount();
		QModelIndex modelIndex;
		for (int i = 0; i < count; i++)
		{
			modelIndex = m_pIconTracks->GetModel()->index(i, 0);
			if (qvariant_cast<int>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_ID)) == id)
			{
				index = qvariant_cast<int>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_INDEX));
				break;
			}
		}

		if (index >= 0)
		{
			emit SlotReqCoverArt(id, index, QListView::IconMode);
		}

	}
	else
	{
		int index = -1;
		int count = m_pTableTracks->GetModel()->rowCount();
		for (int i = 0; i < count; i++)
		{
			if (qvariant_cast<int>(m_pTableTracks->GetModel()->data(m_pTableTracks->GetModel()->index(i, TableTracks::TABLE_TRACKS_ID))) == id)
			{
				index = qvariant_cast<int>(m_pTableTracks->GetModel()->data(m_pTableTracks->GetModel()->index(i, TableTracks::TABLE_TRACKS_INDEX)));
				break;
			}
		}

		if (index >= 0)
		{
			emit SlotReqCoverArt(id, index, QListView::ListMode);
		}
	}
}

//void MusicDBWindow::SlotRespUpdateCategory(int updateId)
//{
//	if (updateId >= 0)
//	{
//		m_nID = updateId;
//	}
//	DoTopMenuReload();
//}

//void MusicDBWindow::SlotRespInsertReplaceCategoryAll()
//{
//	m_pMgr->RequestUpdateCategoryAll(m_UpdateMap, m_UpdateModel);
//}

void MusicDBWindow::SlotRespRefresh()
{
	DoTopMenuReload();
}

void MusicDBWindow::SlotOptionMenuAction(const QModelIndex &index, int menuID)
{
	int row = index.row();
	QStandardItemModel *model = m_pTableTracks->GetModel();
	int id = qvariant_cast<int>(model->data(model->index(row, TableTracks::TABLE_TRACKS_ID)));

	switch (menuID) {
	case OPTION_MENU_PLAY_NOW:
		DoOptionMenuPlay(id, PLAY_NOW);
		break;
	case OPTION_MENU_PLAY_LAST:
		DoOptionMenuPlay(id, PLAY_LAST);
		break;
	case OPTION_MENU_PLAY_NEXT:
		DoOptionMenuPlay(id, PLAY_NEXT);
		break;
	case OPTION_MENU_PLAY_CLEAR:
		DoOptionMenuPlay(id, PLAY_CLEAR);
		break;
	case OPTION_MENU_ADD_TO_PLAYLIST:
		DoOptionMenuAddToPlaylist(id);
		break;
	case OPTION_MENU_EDIT_TAG:
		DoOptionMenuInfo(id);
		break;
	case OPTION_MENU_SEARCH_COVERART:
		DoOptionMenuSearchCoverArt(id);
		break;
	case OPTION_MENU_RENAME:
		DoOptionMenuRename(id);
		break;
	case OPTION_MENU_GAIN_SET:
		DoOptionMenuGain(id, VAL_GAIN_SET);
		break;
	case OPTION_MENU_GAIN_CLEAR:
		DoOptionMenuGain(id, VAL_GAIN_CLEAR);
		break;
	case OPTION_MENU_GO_TO_ALBUM:
	{
		int albumID = qvariant_cast<int>(model->data(model->index(row, TableTracks::TABLE_TRACKS_ALBUM_ID)));
		QString cover = qvariant_cast<QString>(model->data(model->index(row, TableTracks::TABLE_TRACKS_COVER)));
		DoOptionMenuGoToAlbum(albumID, cover);
	}
		break;
	case OPTION_MENU_GO_TO_ARTIST:
	{
		int artistID = qvariant_cast<int>(model->data(model->index(row, TableTracks::TABLE_TRACKS_ARTIST_ID)));
		DoOptionMenuGoToArtist(artistID);
	}
		break;
	}
}

void MusicDBWindow::SlotTopMenuAddToPlaylist(int id)
{
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		m_pMgr->RequestAddToPlaylist(id, m_SelectMap, m_nCategory);

	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		m_pMgr->RequestAddToPlaylist(id, m_SelectMap, -1);

	}
}

void MusicDBWindow::SlotOptionMenuAddToPlaylist(int id)
{
	QMap<int, bool> map;
	map.insert(m_nOptionID, true);

	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		m_pMgr->RequestAddToPlaylist(id, map, m_nCategory);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		m_pMgr->RequestAddToPlaylist(id, map, -1);
	}
}

void MusicDBWindow::SlotAddTrackFromPlaylist(QMap<int, bool> idMap)
{
	emit SigAddTrackFromPlaylist(idMap);	// recursive
	emit SigRemoveWidget(this);
}

void MusicDBWindow::SlotContextMenu(QPoint point)
{
	QModelIndex modelIndex = m_pIconTracks->GetListView()->indexAt(point);
	m_nID = qvariant_cast<int>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_ID));

//	int index = modelIndex.row();
//	LogDebug("index [%d] id [%d]", index, m_nID);

	QAction *actionPlayNow = new QAction(STR_PLAY_NOW, this);
	QAction *actionPlayLast = new QAction(STR_PLAY_LAST, this);
	QAction *actionPlayNext = new QAction(STR_PLAY_NEXT, this);
	QAction *actionPlayClear = new QAction(STR_PLAY_CLEAR, this);
	QAction *actionSearchCoverArt = new QAction(STR_SEARCH_COVERART, this);
	QAction *actionTagEdit = new QAction(STR_TAG_EDIT, this);

	connect(actionPlayNow, SIGNAL(triggered()), this, SLOT(SlotContextMenuPlayNow()));
	connect(actionPlayLast, SIGNAL(triggered()), this, SLOT(SlotContextMenuPlayLast()));
	connect(actionPlayNext, SIGNAL(triggered()), this, SLOT(SlotContextMenuPlayNext()));
	connect(actionPlayClear, SIGNAL(triggered()), this, SLOT(SlotContextMenuPlayClear()));
	connect(actionSearchCoverArt, SIGNAL(triggered()), this, SLOT(SlotContextMenuSearchCoverArt()));
	connect(actionTagEdit, SIGNAL(triggered()), this, SLOT(SlotContextMenuTagEdit()));

	QMenu *menu=new QMenu(this);
	menu->addAction(actionPlayNow);
	menu->addAction(actionPlayLast);
	menu->addAction(actionPlayNext);
	menu->addAction(actionPlayClear);
	menu->addAction(actionSearchCoverArt);
	menu->addAction(actionTagEdit);

	menu->popup(m_pIconTracks->GetListView()->viewport()->mapToGlobal(point));

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

	menu->setStyleSheet(style);

}

void MusicDBWindow::SlotContextMenuPlayNow()
{
	DoOptionMenuPlay(m_nID, PLAY_NOW);
}

void MusicDBWindow::SlotContextMenuPlayLast()
{
	DoOptionMenuPlay(m_nID, PLAY_LAST);
}

void MusicDBWindow::SlotContextMenuPlayNext()
{
	DoOptionMenuPlay(m_nID, PLAY_NEXT);
}

void MusicDBWindow::SlotContextMenuPlayClear()
{
	DoOptionMenuPlay(m_nID, PLAY_CLEAR);
}

void MusicDBWindow::SlotContextMenuSearchCoverArt()
{
	DoOptionMenuSearchCoverArt(m_nID);
}

void MusicDBWindow::SlotContextMenuTagEdit()
{
	m_pMgr->RequestTrackListForEditTag(m_nID, m_nCategory);
}

void MusicDBWindow::ReadSettings()
{
	SettingIO settings;
	settings.beginGroup(SETTINGS_GROUP);

	m_ShowCategoryFavorite = settings.value("show_category_favorite").toBool();
	m_ShowCategoryRating = settings.value("show_category_rating").toBool();
	m_ShowCategoryArtist = settings.value("show_category_artist").toBool();
	m_ShowCategoryAlbum = settings.value("show_category_album").toBool();
	m_ShowCategoryGenre = settings.value("show_category_genre").toBool();
	m_ShowCategoryAlbumArtist = settings.value("show_category_album_artist").toBool();
	m_ShowCategoryTrackCount = settings.value("show_category_track_count").toBool();
	m_ShowCategoryAlbumGain = settings.value("show_category_album_gain").toBool();

	m_ShowTrackFavorite = settings.value("show_track_favorite").toBool();
	m_ShowTrackTime = settings.value("show_track_time").toBool();
	m_ShowTrackArtist = settings.value("show_track_artist").toBool();
	m_ShowTrackAlbum = settings.value("show_track_album").toBool();
	m_ShowTrackGenre = settings.value("show_track_genre").toBool();
	m_ShowTrackAlbumArtist = settings.value("show_track_album_artist").toBool();
	m_ShowTrackComposer = settings.value("show_track_composer").toBool();
	m_ShowTrackYear = settings.value("show_track_year").toBool();

	m_ShowTrackMood = settings.value("show_track_mood").toBool();
	m_ShowTrackTempo = settings.value("show_track_tempo").toBool();
	m_ShowTrackFormat = settings.value("show_track_format").toBool();
	m_ShowTrackSampleRate = settings.value("show_track_sample_rate").toBool();
	m_ShowTrackBitrate = settings.value("show_track_bitrate").toBool();
	m_ShowTrackRating = settings.value("show_track_rating").toBool();

	m_ResizeItem = settings.value("resize_item_value").toInt();
	m_ResizeTrack = settings.value("resize_track_value").toInt();

	m_nSortCategory = settings.value("sort_item_value").toInt();
	m_nSortTrack = settings.value("sort_track_value").toInt();
	m_DispMode = settings.value("display_mode").toInt();

	m_bIncreaseCategory = settings.value("increase_item").toBool();
	m_bIncreaseTrack = settings.value("increase_track").toBool();

	m_LimitCount = settings.value("limit_count").toInt();
	if (m_LimitCount == 0)
	{
		m_LimitCount = 1000;	// init value
	}

	settings.endGroup();

	if (m_ResizeItem <= 0)
	{
		m_ResizeItem = ICON_HEIGHT_MAX;
	}
	if (m_ResizeTrack <= 0)
	{
		m_ResizeTrack = TABLE_HEIGHT_MIN;
	}
}

void MusicDBWindow::WriteSettings()
{
	SettingIO settings;
	settings.beginGroup(SETTINGS_GROUP);

	settings.setValue("show_category_favorite", m_ShowCategoryFavorite);
	settings.setValue("show_category_rating", m_ShowCategoryRating);
	settings.setValue("show_category_artist", m_ShowCategoryArtist);
	settings.setValue("show_category_album", m_ShowCategoryAlbum);
	settings.setValue("show_category_genre", m_ShowCategoryGenre);
	settings.setValue("show_category_album_artist", m_ShowCategoryAlbumArtist);
	settings.setValue("show_category_track_count", m_ShowCategoryTrackCount);
	settings.setValue("show_category_album_gain", m_ShowCategoryAlbumGain);

	settings.setValue("show_track_favorite", m_ShowTrackFavorite);
	settings.setValue("show_track_time", m_ShowTrackTime);
	settings.setValue("show_track_artist", m_ShowTrackArtist);
	settings.setValue("show_track_album", m_ShowTrackAlbum);
	settings.setValue("show_track_genre", m_ShowTrackGenre);
	settings.setValue("show_track_album_artist", m_ShowTrackAlbumArtist);
	settings.setValue("show_track_composer", m_ShowTrackComposer);
	settings.setValue("show_track_year", m_ShowTrackYear);

	settings.setValue("show_track_mood", m_ShowTrackMood);
	settings.setValue("show_track_tempo", m_ShowTrackTempo);
	settings.setValue("show_track_format", m_ShowTrackFormat);
	settings.setValue("show_track_sample_rate", m_ShowTrackSampleRate);
	settings.setValue("show_track_bitrate", m_ShowTrackBitrate);
	settings.setValue("show_track_rating", m_ShowTrackRating);

	settings.setValue("resize_item_value", m_ResizeItem);
	settings.setValue("resize_track_value", m_ResizeTrack);

	settings.setValue("sort_item_value", m_nSortCategory);
	settings.setValue("sort_track_value", m_nSortTrack);
	settings.setValue("display_mode", m_DispMode);

	settings.setValue("increase_item", m_bIncreaseCategory);
	settings.setValue("increase_track", m_bIncreaseTrack);

	settings.setValue("limit_count", m_LimitCount);

	settings.endGroup();
}

//void MusicDBWindow::SlotEditAllArtist(QString value)
//{
//	LogDebug("edit artist [%s]", value.toUtf8().data());
//	m_pMgr->RequestCheckCategory(m_nID, m_nCategory, SQLManager::CATEGORY_ARTIST, value);
//}

//void MusicDBWindow::SlotEditAllAlbum(QString value)
//{
//	LogDebug("edit album [%s]", value.toUtf8().data());
//	m_pMgr->RequestCheckCategory(m_nID, m_nCategory, SQLManager::CATEGORY_ALBUM, value);
//}

//void MusicDBWindow::SlotEditAllGenre(QString value)
//{
//	LogDebug("edit genre [%s]", value.toUtf8().data());
//	m_pMgr->RequestCheckCategory(m_nID, m_nCategory, SQLManager::CATEGORY_GENRE, value);
//}

void MusicDBWindow::ConnectSigToSlot()
{
	// recursive
	connect(this, SIGNAL(SigAddWidget(QWidget*, QString)), parent(), SLOT(SlotAddWidget(QWidget*, QString)));
	connect(this, SIGNAL(SigRemoveWidget(QWidget*)), parent(), SLOT(SlotRemoveWidget(QWidget*)));

	connect(this, SIGNAL(SigUpdateFavoriteParent(int, int)), parent(), SLOT(SlotUpdateFavoriteParent(int, int)));
	connect(this, SIGNAL(SigUpdateRatingParent(int, int)), parent(), SLOT(SlotUpdateRatingParent(int, int)));

	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespMusicOverview(CJsonNode)), this, SLOT(SlotRespMusicOverview(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespCategoryList(QList<CJsonNode>)), this, SLOT(SlotRespCategoryList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespCategoryOverview(CJsonNode)), this, SLOT(SlotRespCategoryOverview(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespTrackList(QList<CJsonNode>)), this, SLOT(SlotRespTrackList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespTrackListForEditTag(QList<CJsonNode>)), this, SLOT(SlotRespTrackListForEditTag(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespClassifyArtist(QList<CJsonNode>)), this, SLOT(SlotRespClassifyArtist(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespClassifyGenre(QList<CJsonNode>)), this, SLOT(SlotRespClassifyGenre(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespClassifyComposer(QList<CJsonNode>)), this, SLOT(SlotRespClassifyComposer(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespCategoryInfo(CJsonNode)), this, SLOT(SlotRespCategoryInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespCategoryInfoList(QList<CJsonNode>)), this, SLOT(SlotRespCategoryInfoList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespTrackInfo(CJsonNode)), this, SLOT(SlotRespTrackInfo(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespSetCoverArt(int, int)), this, SLOT(SlotRespSetCoverArt(int, int)));
//	connect(m_pMgr, SIGNAL(SigRespUpdateCategory(int)), this, SLOT(SlotRespUpdateCategory(int)));
//	connect(m_pMgr, SIGNAL(SigRespInsertReplaceCategoryAll()), this, SLOT(SlotRespInsertReplaceCategoryAll()));
	connect(m_pMgr, SIGNAL(SigRespRefresh()), this, SLOT(SlotRespRefresh()));
	connect(m_pMgr, SIGNAL(SigCoverArtUpdate(QString, int, int)), this, SLOT(SlotCoverArtUpdate(QString, int, int)));

	connect(m_pInfoHome->GetFormPlay(), SIGNAL(SigPlayAll()), this, SLOT(SlotPlayAll()));
	connect(m_pInfoHome->GetFormPlay(), SIGNAL(SigPlayRandom()), this, SLOT(SlotPlayRandom()));
	connect(m_pInfoHome->GetFormPlay(), SIGNAL(SigMenu()), this, SLOT(SlotTopMenu()));
	connect(m_pInfoHome->GetFormPlay(), SIGNAL(SigMenuAction(int)), this, SLOT(SlotTopMenuAction(int)));
	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigClassifyFavorite(int)), this, SLOT(SlotClassifyFavorite(int)));
	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigClassifyRating(int)), this, SLOT(SlotClassifyRating(int)));
	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigClassifyArtist(bool, QString)), this, SLOT(SlotClassifyArtist(bool, QString)));
	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigClassifyGenre(bool, QString)), this, SLOT(SlotClassifyGenre(bool, QString)));
	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigClassifyComposer(bool, QString)), this, SLOT(SlotClassifyComposer(bool, QString)));
	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigClassifyFormat(bool, QString)), this, SLOT(SlotClassifyFormat(bool, QString)));
	connect(m_pInfoHome->GetFormClassify(), SIGNAL(SigClassifyMostPlayed(bool, QString)), this, SLOT(SlotClassifyMostPlayed(bool, QString)));
	connect(m_pInfoHome->GetFormSort(), SIGNAL(SigMenu(int)), this, SLOT(SlotSortMenu(int)));
	connect(m_pInfoHome->GetFormSort(), SIGNAL(SigIncDec(bool)), this, SLOT(SlotIncDec(bool)));
	connect(m_pInfoHome->GetFormSort(), SIGNAL(SigResize(int)), this, SLOT(SlotResize(int)));
	connect(m_pInfoHome, SIGNAL(SigAlbumList()), this, SLOT(SlotAlbumList()));
	connect(m_pInfoHome, SIGNAL(SigAlbumArtistList()), this, SLOT(SlotAlbumArtistList()));
	connect(m_pInfoHome, SIGNAL(SigArtistList()), this, SLOT(SlotArtistList()));
	connect(m_pInfoHome, SIGNAL(SigGenreList()), this, SLOT(SlotGenreList()));
	connect(m_pInfoHome, SIGNAL(SigMenu()), this, SLOT(SlotCategoryMenu()));
	connect(m_pInfoHome, SIGNAL(SigMenuAction(int, QString)), this, SLOT(SlotCategoryMenuAction(int, QString)));

	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigPlayAll()), this, SLOT(SlotItemPlayAll()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigPlayRandom()), this, SLOT(SlotItemPlayRandom()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigFavorite(int)), this, SLOT(SlotItemFavorite(int)));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigRating(int)), this, SLOT(SlotItemRating(int)));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigMenu()), this, SLOT(SlotItemTopMenu()));
	connect(m_pInfoTracks->GetFormPlay(), SIGNAL(SigMenuAction(int)), this, SLOT(SlotItemTopMenuAction(int)));
	connect(m_pInfoTracks->GetFormSort(), SIGNAL(SigMenu(int)), this, SLOT(SlotItemSortMenu(int)));
	connect(m_pInfoTracks->GetFormSort(), SIGNAL(SigIncDec(bool)), this, SLOT(SlotItemIncDec(bool)));
	connect(m_pInfoTracks->GetFormSort(), SIGNAL(SigResize(int)), this, SLOT(SlotResize(int)));

	connect(m_pIconTracks, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));
	connect(m_pIconTracks, SIGNAL(SigAppendList()), this, SLOT(SlotAppendList()));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectPlay(int, int)), this, SLOT(SlotSelectPlay(int, int)));
//	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectFavorite(int, int)), this, SLOT(SlotSelectFavorite(int, int)));
//	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectRating(int, int)), this, SLOT(SlotSelectRating(int, int)));
	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectTitle(const QModelIndex&)), this, SLOT(SlotSelectTitle(const QModelIndex&)));
//	connect(m_pIconTracks->GetDelegate(), SIGNAL(SigSelectSubtitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));
	connect(m_pIconTracks->GetListView(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(SlotContextMenu(QPoint)));

	connect(m_pTableTracks, SIGNAL(SigReqCoverArt(int, int, int)), this, SLOT(SlotReqCoverArt(int, int, int)));
	connect(m_pTableTracks, SIGNAL(SigAppendList()), this, SLOT(SlotAppendList()));
	connect(m_pTableTracks, SIGNAL(SigSelectPlay(int, int)), this, SLOT(SlotSelectPlay(int, int)));
	connect(m_pTableTracks, SIGNAL(SigSelectTitle(const QModelIndex&)), this, SLOT(SlotSelectTitle(const QModelIndex&)));
	connect(m_pTableTracks, SIGNAL(SigSelectFavorite(int, int, int)), this, SLOT(SlotSelectTrackFavorite(int, int, int)));
	connect(m_pTableTracks, SIGNAL(SigMenuAction(const QModelIndex&, int)), this, SLOT(SlotOptionMenuAction(const QModelIndex&, int)));

}

void MusicDBWindow::Initialize()
{
	m_pInfoHome->GetFormPlay()->ShowPlayAll();
	m_pInfoHome->GetFormPlay()->ShowMenu();

	m_pInfoHome->GetFormSort()->ShowMenu();
	m_pInfoHome->GetFormSort()->ShowResize();
	m_pInfoHome->GetFormSort()->ShowIncDec();
	m_pInfoHome->GetFormSort()->SetIncrease(m_bIncreaseCategory);
	m_pInfoHome->GetFormSort()->SetSliderMinimum(TABLE_HEIGHT_MIN);

	m_pInfoTracks->GetFormPlay()->ShowPlayAll();
	m_pInfoTracks->GetFormPlay()->ShowMenu();

	m_pInfoTracks->GetFormSort()->ShowMenu();
	m_pInfoTracks->GetFormSort()->ShowResize();
	m_pInfoTracks->GetFormSort()->ShowIncDec();
	m_pInfoTracks->GetFormSort()->SetIncrease(m_bIncreaseTrack);
	m_pInfoTracks->GetFormSort()->SetSliderMinimum(TABLE_HEIGHT_MIN);

	m_RespList.clear();

	m_TopMenuMap.clear();
	m_CategoryMenuMap.clear();
	m_OptionMenuMap.clear();
	m_SelectMap.clear();
	m_UpdateMap.clear();

	m_AlbumList.clear();
	m_AlbumArtistList.clear();
	m_ArtistList.clear();
	m_GenreList.clear();
	m_ComposerList.clear();
	m_MoodList.clear();

//	m_ListMode = VIEW_MODE_ICON;
	m_TypeMode = TYPE_MODE_ITEM_TRACK;
//	m_DispMode = SQLManager::DISP_MODE_TRACK;

	m_EnableAppend = true;
}

void MusicDBWindow::SetCategoryList(QList<CJsonNode> list)
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

void MusicDBWindow::SetSelectOffTopMenu()
{
	m_TopMenuMap.clear();
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		m_TopMenuMap.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_TopMenuMap.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_TopMenuMap.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_TopMenuMap.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_TopMenuMap.insert(TOP_MENU_RELOAD, STR_RELOAD);
		if (m_LimitCount <= 0)
		{
			m_TopMenuMap.insert(TOP_MENU_LOAD_COUNT, QString("%1 - %2").arg(STR_LOAD_COUNT).arg("All"));
		}
		else
		{
			m_TopMenuMap.insert(TOP_MENU_LOAD_COUNT, QString("%1 - %2").arg(STR_LOAD_COUNT).arg(m_LimitCount));
		}
		m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
		m_TopMenuMap.insert(TOP_MENU_SHOW_COLUMNS, STR_SHOW_COLUMNS);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		m_TopMenuMap.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_TopMenuMap.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_TopMenuMap.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_TopMenuMap.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_TopMenuMap.insert(TOP_MENU_RELOAD, STR_RELOAD);
		if (m_LimitCount <= 0)
		{
			m_TopMenuMap.insert(TOP_MENU_LOAD_COUNT, QString("%1 - %2").arg(STR_LOAD_COUNT).arg("All"));
		}
		else
		{
			m_TopMenuMap.insert(TOP_MENU_LOAD_COUNT, QString("%1 - %2").arg(STR_LOAD_COUNT).arg(m_LimitCount));
		}
		m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
		m_TopMenuMap.insert(TOP_MENU_SHOW_COLUMNS, STR_SHOW_COLUMNS);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK_ADD)
	{
		m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
	}

	m_pInfoHome->GetFormPlay()->ClearMenu();
	m_pInfoHome->GetFormPlay()->SetMenu(m_TopMenuMap);

	m_pInfoTracks->GetFormPlay()->ClearMenu();
	m_pInfoTracks->GetFormPlay()->SetMenu(m_TopMenuMap);
}

void MusicDBWindow::SetSelectOnTopMenu()
{
	m_TopMenuMap.clear();
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		m_TopMenuMap.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_TopMenuMap.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_TopMenuMap.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_TopMenuMap.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_TopMenuMap.insert(TOP_MENU_CLEAR_ALL, STR_CLEAR_ALL);
		m_TopMenuMap.insert(TOP_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
		m_TopMenuMap.insert(TOP_MENU_GAIN_SET, STR_GAIN_SET);
		m_TopMenuMap.insert(TOP_MENU_GAIN_CLEAR, STR_GAIN_CLEAR);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		m_TopMenuMap.insert(TOP_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_TopMenuMap.insert(TOP_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_TopMenuMap.insert(TOP_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_TopMenuMap.insert(TOP_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_TopMenuMap.insert(TOP_MENU_CLEAR_ALL, STR_CLEAR_ALL);
		m_TopMenuMap.insert(TOP_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
		m_TopMenuMap.insert(TOP_MENU_GAIN_SET, STR_GAIN_SET);
		m_TopMenuMap.insert(TOP_MENU_GAIN_CLEAR, STR_GAIN_CLEAR);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		m_TopMenuMap.insert(TOP_MENU_CLEAR_ALL, STR_CLEAR_ALL);
		m_TopMenuMap.insert(TOP_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK_ADD)
	{
		m_TopMenuMap.insert(TOP_MENU_CLEAR_ALL, STR_CLEAR_ALL);
		m_TopMenuMap.insert(TOP_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
	}

	m_pInfoHome->GetFormPlay()->ClearMenu();
	m_pInfoHome->GetFormPlay()->SetMenu(m_TopMenuMap);

	m_pInfoTracks->GetFormPlay()->ClearMenu();
	m_pInfoTracks->GetFormPlay()->SetMenu(m_TopMenuMap);
}

void MusicDBWindow::DoTopMenuPlay(int nWhere)
{
	m_pMgr->RequestManageCategory(VAL_PLAY, m_SelectMap, nWhere, m_nCategory);
}

void MusicDBWindow::DoTopMenuReload()
{
	QString strCat;
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		strCat = UtilNovatron::GetCategoryName(m_nCategory);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ALBUM
			 || m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		strCat = KEY_ALBUM;
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ARTIST)
	{
		strCat = KEY_ARTIST;
	}
	else
	{
		strCat = UtilNovatron::GetCategoryName(-1);
	}

	UtilNovatron::RemoveFilesInTempDirectory(strCat);

	ClearList();

//	if (m_nCategory == SQLManager::CATEGORY_TRACK)
//	{
//		RequestTrackList(m_nID, m_nCatID, m_nCatID2);
//	}
//	else
//	{
//		RequestCategoryList(m_nCatID, m_nCatID2);
//	}

	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		RequestCategoryList(m_nCatID, m_nCatID2);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		RequestTrackList(m_nID, m_nCatID, m_nCatID2);
	}
}

void MusicDBWindow::DoTopMenuSetLimitCount(int count)
{
	LimitCountDialog dialog;
	dialog.SetLimitCount(count);
	if (dialog.exec() == QDialog::Accepted)
	{
		m_LimitCount = dialog.GetLimitCount();
		if (m_LimitCount == 0)
		{
			m_LimitCount = -1;
		}
		WriteSettings();

		DoTopMenuReload();
	}
}

void MusicDBWindow::DoTopMenuSelectAll()
{
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pIconTracks->SetAllSelectMap();
	}
	else
	{
		m_pTableTracks->SetAllSelectMap();
	}
}

void MusicDBWindow::DoTopMenuClearAll()
{
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pIconTracks->ClearSelectMap();
	}
	else
	{
		m_pTableTracks->ClearSelectMap();
	}
}

void MusicDBWindow::DoTopMenuGainSet()
{
	m_pMgr->RequestManageCategory(VAL_GAIN_SET,
								  m_SelectMap,
								  PLAY_NONE,
								  m_nCategory,
								  m_EventID);
}

void MusicDBWindow::DoTopMenuGainClear()
{
	m_pMgr->RequestManageCategory(VAL_GAIN_CLEAR,
								  m_SelectMap,
								  PLAY_NONE,
								  m_nCategory,
								  m_EventID);

}

void MusicDBWindow::DoTopMenuAddToPlaylist()
{
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM
			|| m_TypeMode == TYPE_MODE_TRACK
			|| m_TypeMode == TYPE_MODE_TRACK_ALBUM
			|| m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		PlaylistWindow *widget = new PlaylistWindow(this, m_pMgr->GetAddr());
		widget->AddWidgetItem(TYPE_MODE_ITEM_ADD);
		emit widget->SigAddWidget(widget, STR_PLAYLIST);
		widget->RequestPlaylist();

		connect(widget, SIGNAL(SigAddToPlaylist(int)), this, SLOT(SlotTopMenuAddToPlaylist(int)));
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		emit SigAddCategoryFromPlaylist(m_nCategory, m_SelectMap);
		emit SigRemoveWidget(this);
	}

//	m_pMgr->RequestManageCategory(VAL_ADD,
//								  m_SelectMap,
//								  PLAY_NONE,
//								  m_nCategory);
}

void MusicDBWindow::DoTopMenuShowCategoryColumns()
{
	SetCategoryColumnDialog dialog;
	dialog.SetCategory(m_nCategory);
	dialog.SetCBFavorite(m_pTableTracks->GetColumnShow(TableTracks::TABLE_TRACKS_FAVORITE));
	dialog.SetCBRating(m_pTableTracks->GetColumnShow(TableTracks::TABLE_TRACKS_RATING));
	dialog.SetCBArtist(m_pTableTracks->GetColumnShow(TableTracks::TABLE_TRACKS_ARTIST));
	dialog.SetCBAlbum(m_pTableTracks->GetColumnShow(TableTracks::TABLE_TRACKS_ALBUM));
	dialog.SetCBGenre(m_pTableTracks->GetColumnShow(TableTracks::TABLE_TRACKS_GENRE));
	dialog.SetCBAlbumArtist(m_pTableTracks->GetColumnShow(TableTracks::TABLE_TRACKS_ALBUM_ARTIST));
	dialog.SetCBTrackCount(m_pTableTracks->GetColumnShow(TableTracks::TABLE_TRACKS_TRACK_COUNT));
	dialog.SetCBAlbumGain(m_pTableTracks->GetColumnShow(TableTracks::TABLE_TRACKS_ALBUM_GAIN));

	if (dialog.exec() == QDialog::Accepted)
	{
		m_ShowCategoryFavorite = dialog.GetCBFavorite();
		m_ShowCategoryRating = dialog.GetCBRating();
		m_ShowCategoryArtist = dialog.GetCBArtist();
		m_ShowCategoryAlbum = dialog.GetCBAlbum();
		m_ShowCategoryGenre = dialog.GetCBGenre();
		m_ShowCategoryAlbumArtist = dialog.GetCBAlbumArtist();
		m_ShowCategoryTrackCount = dialog.GetCBTrackCount();
		m_ShowCategoryAlbumGain = dialog.GetCBAlbumGain();

		WriteSettings();

		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_FAVORITE, m_ShowCategoryFavorite);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_RATING, m_ShowCategoryRating);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_ARTIST, m_ShowCategoryArtist);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_ALBUM, m_ShowCategoryAlbum);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_GENRE, m_ShowCategoryGenre);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_ALBUM_ARTIST, m_ShowCategoryAlbumArtist);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_TRACK_COUNT, m_ShowCategoryTrackCount);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_ALBUM_GAIN, m_ShowCategoryAlbumGain);

		m_pTableTracks->SetColResize(0);

	}
}


void MusicDBWindow::DoTopMenuItemPlay(int nWhere)
{
	if (m_SelectMap.count() > 0)
	{
		m_pMgr->RequestManageCategory(VAL_PLAY,
									  m_SelectMap,
									  nWhere,
									  SQLManager::CATEGORY_TRACK);
	}
	else
	{
		QMap<int, bool> map;
		map.insert(m_nID, true);
		m_pMgr->RequestManageCategory(VAL_PLAY,
									  map,
									  nWhere,
									  m_nCategory);
	}

}

void MusicDBWindow::DoTopMenuItemSelectAll()
{
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pIconTracks->SetAllSelectMap();
	}
	else
	{
		m_pTableTracks->SetAllSelectMap();
	}
}

void MusicDBWindow::DoTopMenuItemClearAll()
{
	if (m_ListMode == VIEW_MODE_ICON)
	{
		m_pIconTracks->ClearSelectMap();
	}
	else
	{
		m_pTableTracks->ClearSelectMap();
	}
}

void MusicDBWindow::DoTopMenuItemGainSet()
{
	if (m_SelectMap.count() > 0)
	{
		m_pMgr->RequestManageCategory(VAL_GAIN_SET,
									  m_SelectMap,
									  PLAY_NONE,
									  SQLManager::CATEGORY_TRACK,
									  m_EventID);
	}
	else
	{
		QMap<int, bool> map;
		map.insert(m_nID, true);
		m_pMgr->RequestManageCategory(VAL_GAIN_SET,
									  map,
									  PLAY_NONE,
									  m_nCategory,
									  m_EventID);
	}
}

void MusicDBWindow::DoTopMenuItemGainClear()
{
	if (m_SelectMap.count() > 0)
	{
		m_pMgr->RequestManageCategory(VAL_GAIN_CLEAR,
									  m_SelectMap,
									  PLAY_NONE,
									  SQLManager::CATEGORY_TRACK,
									  m_EventID);
	}
	else
	{
		QMap<int, bool> map;
		map.insert(m_nID, true);
		m_pMgr->RequestManageCategory(VAL_GAIN_CLEAR,
									  map,
									  PLAY_NONE,
									  m_nCategory,
									  m_EventID);
	}
}

void MusicDBWindow::DoTopMenuItemAddToPlaylist()
{
	if (m_TypeMode == TYPE_MODE_TRACK)
	{
		PlaylistWindow *widget = new PlaylistWindow(this, m_pMgr->GetAddr());
		widget->AddWidgetItem(TYPE_MODE_ITEM_ADD);
		emit widget->SigAddWidget(widget, STR_PLAYLIST);
		widget->RequestPlaylist();

		connect(widget, SIGNAL(SigAddToPlaylist(int)), this, SLOT(SlotTopMenuAddToPlaylist(int)));
	}
	else if (m_TypeMode == TYPE_MODE_TRACK_ADD)
	{
		emit SigAddTrackFromPlaylist(m_SelectMap);
		emit SigRemoveWidget(this);
	}

}

void MusicDBWindow::DoTopMenuItemShowTrackColumns()
{
	SetTrackColumnDialog dialog;
	dialog.SetCBFavorite(m_pTableTracks->GetColumnShow(TableTracks::TABLE_TRACKS_FAVORITE));
	dialog.SetCBTime(m_pTableTracks->GetColumnShow(TableTracks::TABLE_TRACKS_TIME));
	dialog.SetCBArtist(m_pTableTracks->GetColumnShow(TableTracks::TABLE_TRACKS_ARTIST));
	dialog.SetCBAlbum(m_pTableTracks->GetColumnShow(TableTracks::TABLE_TRACKS_ALBUM));
	dialog.SetCBGenre(m_pTableTracks->GetColumnShow(TableTracks::TABLE_TRACKS_GENRE));
	dialog.SetCBAlbumArtist(m_pTableTracks->GetColumnShow(TableTracks::TABLE_TRACKS_ALBUM_ARTIST));
	dialog.SetCBComposer(m_pTableTracks->GetColumnShow(TableTracks::TABLE_TRACKS_COMPOSER));
	dialog.SetCBYear(m_pTableTracks->GetColumnShow(TableTracks::TABLE_TRACKS_YEAR));

	dialog.SetCBMood(m_pTableTracks->GetColumnShow(TableTracks::TABLE_TRACKS_MOOD));
	dialog.SetCBTempo(m_pTableTracks->GetColumnShow(TableTracks::TABLE_TRACKS_TEMPO));
	dialog.SetCBFormat(m_pTableTracks->GetColumnShow(TableTracks::TABLE_TRACKS_FORMAT));
	dialog.SetCBSampleRate(m_pTableTracks->GetColumnShow(TableTracks::TABLE_TRACKS_SAMPLE_RATE));
	dialog.SetCBBitrate(m_pTableTracks->GetColumnShow(TableTracks::TABLE_TRACKS_BITRATE));
//	dialog.SetCBRating(m_pTableTracks->GetColumnShow(TableTracks::TABLE_TRACKS_RATING));

	if (dialog.exec() == QDialog::Accepted)
	{
		m_ShowTrackFavorite = dialog.GetCBFavorite();
		m_ShowTrackTime = dialog.GetCBTime();
		m_ShowTrackArtist = dialog.GetCBArtist();
		m_ShowTrackAlbum = dialog.GetCBAlbum();
		m_ShowTrackGenre = dialog.GetCBGenre();
		m_ShowTrackAlbumArtist = dialog.GetCBAlbumArtist();
		m_ShowTrackComposer = dialog.GetCBComposer();
		m_ShowTrackYear = dialog.GetCBYear();

		m_ShowTrackMood = dialog.GetCBMood();
		m_ShowTrackTempo = dialog.GetCBTempo();
		m_ShowTrackFormat = dialog.GetCBFormat();
		m_ShowTrackSampleRate = dialog.GetCBSampleRate();
		m_ShowTrackBitrate = dialog.GetCBBitrate();
//		m_ShowTrackRating = dialog.GetCBRating();

		WriteSettings();

		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_FAVORITE, m_ShowTrackFavorite);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_TIME, m_ShowTrackTime);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_ARTIST, m_ShowTrackArtist);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_ALBUM, m_ShowTrackAlbum);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_GENRE, m_ShowTrackGenre);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_ALBUM_ARTIST, m_ShowTrackAlbumArtist);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_COMPOSER, m_ShowTrackComposer);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_YEAR, m_ShowTrackYear);

		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_MOOD, m_ShowTrackMood);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_TEMPO, m_ShowTrackTempo);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_FORMAT, m_ShowTrackFormat);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_SAMPLE_RATE, m_ShowTrackSampleRate);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_BITRATE, m_ShowTrackBitrate);
//		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_RATING, m_ShowTrackRating);

		// change value
//		{
//			int count = m_pListTracks->GetModel()->rowCount();
//			for (int i = 0; i < count; i++)
//			{
//				m_pListTracks->SetEditor(i);
//			}
//		}

		m_pTableTracks->SetColResize(0);

	}
}


void MusicDBWindow::SetOptionMenu()
{
	m_OptionMenuMap.clear();
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_OptionMenuMap.insert(OPTION_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
		m_OptionMenuMap.insert(OPTION_MENU_EDIT_TAG, STR_TAG_EDIT);
		m_OptionMenuMap.insert(OPTION_MENU_SEARCH_COVERART, STR_SEARCH_COVERART);
		m_OptionMenuMap.insert(OPTION_MENU_RENAME, STR_RENAME);
		m_OptionMenuMap.insert(OPTION_MENU_GAIN_SET, STR_GAIN_SET);
		m_OptionMenuMap.insert(OPTION_MENU_GAIN_CLEAR, STR_GAIN_CLEAR);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_NOW, STR_PLAY_NOW);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_LAST, STR_PLAY_LAST);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_NEXT, STR_PLAY_NEXT);
		m_OptionMenuMap.insert(OPTION_MENU_PLAY_CLEAR, STR_PLAY_CLEAR);
		m_OptionMenuMap.insert(OPTION_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
		m_OptionMenuMap.insert(OPTION_MENU_EDIT_TAG, STR_TAG_EDIT);
		m_OptionMenuMap.insert(OPTION_MENU_SEARCH_COVERART, STR_SEARCH_COVERART);
		m_OptionMenuMap.insert(OPTION_MENU_RENAME, STR_RENAME);
		m_OptionMenuMap.insert(OPTION_MENU_GAIN_SET, STR_GAIN_SET);
		m_OptionMenuMap.insert(OPTION_MENU_GAIN_CLEAR, STR_GAIN_CLEAR);
		m_OptionMenuMap.insert(OPTION_MENU_GO_TO_ALBUM, STR_GO_TO_ALBUM);
		m_OptionMenuMap.insert(OPTION_MENU_GO_TO_ARTIST, STR_GO_TO_ARTIST);
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		m_OptionMenuMap.insert(OPTION_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK_ADD)
	{
		m_OptionMenuMap.insert(OPTION_MENU_ADD_TO_PLAYLIST, STR_ADD_TO_PLAYLIST);
	}

	m_pTableTracks->SetOptionMenuMap(m_OptionMenuMap);

}

void MusicDBWindow::DoOptionMenuPlay(int nID, int where)
{
	SlotSelectPlay(nID, where);
}

void MusicDBWindow::DoOptionMenuAddToPlaylist(int nID)
{
	m_nOptionID = nID;

	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		PlaylistWindow *widget = new PlaylistWindow(this, m_pMgr->GetAddr());
		widget->AddWidgetItem(TYPE_MODE_ITEM_ADD);
		emit widget->SigAddWidget(widget, STR_PLAYLIST);
		widget->RequestPlaylist();

		connect(widget, SIGNAL(SigAddToPlaylist(int)), this, SLOT(SlotOptionMenuAddToPlaylist(int)));
	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		PlaylistWindow *widget = new PlaylistWindow(this, m_pMgr->GetAddr());
		widget->AddWidgetItem(TYPE_MODE_ITEM_ADD);
		emit widget->SigAddWidget(widget, STR_PLAYLIST);
		widget->RequestPlaylist();

		connect(widget, SIGNAL(SigAddToPlaylist(int)), this, SLOT(SlotOptionMenuAddToPlaylist(int)));
	}
	else if (m_TypeMode == TYPE_MODE_ITEM_ADD)
	{
		QMap<int, bool> map;
		map.insert(nID, true);
		emit SigAddCategoryFromPlaylist(m_nCategory, map);
		emit SigRemoveWidget(this);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK_ADD)
	{
		QMap<int, bool> map;
		map.insert(nID, true);
		emit SigAddTrackFromPlaylist(map);
		emit SigRemoveWidget(this);
//		emit SigRemoveWidget(this);
	}

}

void MusicDBWindow::DoOptionMenuInfo(int nID)
{
	m_nOptionID = nID;

	if (m_AlbumList.isEmpty())
	{
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_ALBUM);
	}
	if (m_AlbumArtistList.isEmpty())
	{
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_ALBUM_ARTIST);
	}
	if (m_ArtistList.isEmpty())
	{
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_ARTIST);
	}
	if (m_GenreList.isEmpty())
	{
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_GENRE);
	}
	if (m_ComposerList.isEmpty())
	{
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_COMPOSER);
	}
	if (m_MoodList.isEmpty())
	{
		m_pMgr->RequestCategoryInfoList(SQLManager::CATEGORY_MOOD);
	}

	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		m_pMgr->RequestCategoryInfo(nID);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		m_pMgr->RequestTrackInfo(nID);
	}
}

void MusicDBWindow::DoOptionMenuSearchCoverArt(int nID)
{
	QString site;
	QString keyword;
	QString artist;

	if (m_ListMode == VIEW_MODE_ICON)
	{
		int count = m_pIconTracks->GetModel()->rowCount();
		QModelIndex modelIndex;
		for (int i = 0; i < count; i++)
		{
			modelIndex = m_pIconTracks->GetModel()->index(i, 0);
			if (qvariant_cast<int>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_ID)) == nID)
			{
				keyword = qvariant_cast<QString>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_TITLE));
				artist = qvariant_cast<QString>(modelIndex.data(IconTracksDelegate::ICON_TRACKS_SUBTITLE));
				break;
			}
		}
	}
	else
	{
		int count = m_pTableTracks->GetModel()->rowCount();
		for (int i = 0; i < count; i++)
		{
			if (qvariant_cast<int>(m_pTableTracks->GetModel()->data(m_pTableTracks->GetModel()->index(i, TableTracks::TABLE_TRACKS_ID))) == nID)
			{
				keyword = qvariant_cast<QString>(m_pTableTracks->GetModel()->data(m_pTableTracks->GetModel()->index(i, TableTracks::TABLE_TRACKS_TITLE)));
				artist = qvariant_cast<QString>(m_pTableTracks->GetModel()->data(m_pTableTracks->GetModel()->index(i, TableTracks::TABLE_TRACKS_ARTIST)));
				break;
			}
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

	SearchCoverArtResultDialog resultDialog;
	resultDialog.SetAddr(m_pMgr->GetAddr());
	resultDialog.RequestCoverArtList(site, keyword, artist);
	if (resultDialog.exec() == QDialog::Accepted)
	{
		ConfirmCoverArtDialog dialog;
		dialog.SetImagePath(resultDialog.GetImagePath());
		if (dialog.exec() == QDialog::Accepted)
		{
			QString image = resultDialog.GetImage();
			QString thumb = resultDialog.GetThumb();
			if (m_TypeMode == TYPE_MODE_ITEM_TRACK
					|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
					|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
					|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
			{
				m_pMgr->RequestSetCategoryCoverArt(nID,
										   m_nCategory,
										   m_EventID,
										   image,
										   thumb);
			}
			else if (m_TypeMode == TYPE_MODE_TRACK
					 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
					 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
			{
				m_pMgr->RequestSetTrackCoverArt(nID,
										   SQLManager::CATEGORY_TRACK,
										   m_EventID,
										   image,
										   thumb);
			}
	//		DoTopMenuReload();
		}
	}
}

void MusicDBWindow::DoOptionMenuRename(int nID)
{
	InputNameDialog dialog;
	if (dialog.exec() == QDialog::Accepted)
	{
		QString name = dialog.GetName();
		if (m_TypeMode == TYPE_MODE_ITEM_TRACK
				|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
				|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
				|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
		{
			m_pMgr->RequestRenameCategory(nID, name, m_nCategory, m_EventID);
		}
		else if (m_TypeMode == TYPE_MODE_TRACK
				 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
				 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
		{
			m_pMgr->RequestRenameTrack(nID, name, m_EventID);
		}
//		DoTopMenuReload();
	}

}

void MusicDBWindow::DoOptionMenuGain(int nID, QString gainType)
{
	if (m_TypeMode == TYPE_MODE_ITEM_TRACK
			|| m_TypeMode == TYPE_MODE_ITEM_ALBUM
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST
			|| m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
	{
		QMap<int, bool> map;
		map.insert(nID, true);
		m_pMgr->RequestManageCategory(gainType,
									  map,
									  PLAY_NONE,
									  m_nCategory,
									  m_EventID);
	}
	else if (m_TypeMode == TYPE_MODE_TRACK
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM
			 || m_TypeMode == TYPE_MODE_TRACK_ALBUM_ARTIST)
	{
		QMap<int, bool> map;
		map.insert(nID, true);
		m_pMgr->RequestManageCategory(gainType,
									  map,
									  PLAY_NONE,
									  SQLManager::CATEGORY_TRACK,
									  m_EventID);
	}
}

void MusicDBWindow::DoOptionMenuGoToAlbum(int albumID, QString cover)
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetTrack(TYPE_MODE_TRACK, SQLManager::CATEGORY_ALBUM);
	emit widget->SigAddWidget(widget, STR_MUSIC_DB);
	widget->SetCoverArt(cover);
	widget->RequestTrackList(albumID);
}

void MusicDBWindow::DoOptionMenuGoToArtist(int artistID)
{
	MusicDBWindow *widget = new MusicDBWindow(this, m_pMgr->GetAddr(), m_EventID);
	widget->AddWidgetItem(TYPE_MODE_ITEM_ALBUM, SQLManager::CATEGORY_ARTIST);
	emit widget->SigAddWidget(widget, STR_MUSIC_DB);
	widget->RequestCategoryList(artistID);
}

//void MusicDBWindow::DoInsertIgnoreCategoryAll()
//{
//	m_pMgr->RequestInsertIgnoreCategoryAll(m_UpdateMap);
//}

void MusicDBWindow::SetCoverArt(QString coverArt)
{
	m_pInfoTracks->SetCoverArt(coverArt);
}

void MusicDBWindow::SetInfoHome(QString title, QString count)
{
	m_pInfoHome->SetTitle(title);
	m_pInfoHome->SetCategoryCnt(count);
}

void MusicDBWindow::SetSortMenu(int category)
{
	QString title;
	QMap<int, QString> list;
	if (category == SQLManager::CATEGORY_TRACK)
	{
		title = GetTitleSortMenu(m_nSortTrack);

		list.insert(SQLManager::SORT_IMPORTED_DATE, STR_SORT_IMPORTED_DATE);
		list.insert(SQLManager::SORT_ALPHABET, STR_SORT_ALPHABET);
		list.insert(SQLManager::SORT_FAVORITE, STR_SORT_FAVORITE);

		m_pInfoHome->GetFormSort()->ClearMenu();
		m_pInfoHome->GetFormSort()->SetMenu(list);
		m_pInfoHome->GetFormSort()->SetMenuTitle(title);

		m_pInfoTracks->GetFormSort()->ClearMenu();
		m_pInfoTracks->GetFormSort()->SetMenu(list);
		m_pInfoTracks->GetFormSort()->SetMenuTitle(title);
	}
	else if (category == SQLManager::CATEGORY_ARTIST
			 || category == SQLManager::CATEGORY_COMPOSER
			 || category == SQLManager::CATEGORY_MOOD
			 || category == SQLManager::CATEGORY_GENRE)
	{
		title = GetTitleSortMenu(m_nSortCategory);

		list.insert(SQLManager::SORT_IMPORTED_DATE, STR_SORT_IMPORTED_DATE);
		list.insert(SQLManager::SORT_ALPHABET, STR_SORT_ALPHABET);
		list.insert(SQLManager::SORT_FAVORITE, STR_SORT_FAVORITE);
		list.insert(SQLManager::SORT_RATING, STR_SORT_RATING);

		if (m_TypeMode == TYPE_MODE_ITEM_TRACK)
		{
			title = GetTitleSortMenu(m_DispMode);

			list.insert(SQLManager::DISP_MODE_TRACK, STR_DISP_MODE_TRACK);
			list.insert(SQLManager::DISP_MODE_ALBUM, STR_DISP_MODE_ALBUM);

			if (category == SQLManager::CATEGORY_GENRE)
			{
				list.insert(SQLManager::DISP_MODE_ARTIST, STR_DISP_MODE_ARTIST);
			}

		}
		else if (m_TypeMode == TYPE_MODE_ITEM_ALBUM
				 || m_TypeMode == TYPE_MODE_ITEM_ARTIST_ALBUM)
		{

		}
		else if (m_TypeMode == TYPE_MODE_ITEM_ARTIST)
		{
			title = GetTitleSortMenu(m_DispMode);

			list.insert(SQLManager::DISP_MODE_ALBUM, STR_DISP_MODE_ALBUM);
		}

		m_pInfoHome->GetFormSort()->ClearMenu();
		m_pInfoHome->GetFormSort()->SetMenu(list);
		m_pInfoHome->GetFormSort()->SetMenuTitle(title);
	}
	else
	{
		title = GetTitleSortMenu(m_nSortCategory);

		if (category == SQLManager::CATEGORY_YEAR)
		{
			title = STR_SORT_ALPHABET;
			list.insert(SQLManager::SORT_ALPHABET, STR_SORT_ALPHABET);

		}
		else
		{
			list.insert(SQLManager::SORT_IMPORTED_DATE, STR_SORT_IMPORTED_DATE);
			list.insert(SQLManager::SORT_ALPHABET, STR_SORT_ALPHABET);
			list.insert(SQLManager::SORT_FAVORITE, STR_SORT_FAVORITE);
			list.insert(SQLManager::SORT_RATING, STR_SORT_RATING);
		}

		m_pInfoHome->GetFormSort()->ClearMenu();
		m_pInfoHome->GetFormSort()->SetMenu(list);
		m_pInfoHome->GetFormSort()->SetMenuTitle(title);
	}
}

void MusicDBWindow::ClearList()
{
	m_RespList.clear();
	m_pIconTracks->ClearNodeList();
	m_pTableTracks->ClearNodeList();
	m_CurPage = 0;
}

void MusicDBWindow::ClearClassifyMenu()
{
	m_pInfoHome->GetFormClassify()->ClearClassifyArtistMenu();
	m_pInfoHome->GetFormClassify()->ClearClassifyGenreMenu();
	m_pInfoHome->GetFormClassify()->ClearClassifyComposerMenu();
}

void MusicDBWindow::SetClassifyFormat()
{
	QList<CJsonNode> list;

	CJsonNode nodeCaf(JSON_OBJECT);
	nodeCaf.AddInt(KEY_ID_LOWER, SelectFormatDialog::FORMAT_ID_CAF);
	nodeCaf.Add(KEY_NAME, FORMAT_CAF);
	CJsonNode nodeWav(JSON_OBJECT);
	nodeWav.AddInt(KEY_ID_LOWER, SelectFormatDialog::FORMAT_ID_WAV);
	nodeWav.Add(KEY_NAME, FORMAT_WAV);
	CJsonNode nodeAiff(JSON_OBJECT);
	nodeAiff.AddInt(KEY_ID_LOWER, SelectFormatDialog::FORMAT_ID_AIFF);
	nodeAiff.Add(KEY_NAME, FORMAT_AIFF);
	CJsonNode nodeFlac(JSON_OBJECT);
	nodeFlac.AddInt(KEY_ID_LOWER, SelectFormatDialog::FORMAT_ID_FLAC);
	nodeFlac.Add(KEY_NAME, FORMAT_FLAC);
	CJsonNode nodeM4a(JSON_OBJECT);
	nodeM4a.AddInt(KEY_ID_LOWER, SelectFormatDialog::FORMAT_ID_M4A);
	nodeM4a.Add(KEY_NAME, FORMAT_M4A);
	CJsonNode nodeMp3(JSON_OBJECT);
	nodeMp3.AddInt(KEY_ID_LOWER, SelectFormatDialog::FORMAT_ID_MP3);
	nodeMp3.Add(KEY_NAME, FORMAT_MP3);
	CJsonNode nodeOgg(JSON_OBJECT);
	nodeOgg.AddInt(KEY_ID_LOWER, SelectFormatDialog::FORMAT_ID_OGG);
	nodeOgg.Add(KEY_NAME, FORMAT_OGG);
	CJsonNode nodeWma(JSON_OBJECT);
	nodeWma.AddInt(KEY_ID_LOWER, SelectFormatDialog::FORMAT_ID_WMA);
	nodeWma.Add(KEY_NAME, FORMAT_WMA);

	list.append(nodeCaf);
	list.append(nodeWav);
	list.append(nodeAiff);
	list.append(nodeFlac);
	list.append(nodeM4a);
	list.append(nodeMp3);
	list.append(nodeOgg);
	list.append(nodeWma);

	m_pInfoHome->GetFormClassify()->ClearClassifyFormatMenu();
	m_pInfoHome->GetFormClassify()->SetClassifyFormatMenu(list);

}

void MusicDBWindow::SetClassifyMostPlayed()
{
	QList<CJsonNode> list;

	CJsonNode nodeTop10(JSON_OBJECT);
	nodeTop10.AddInt(KEY_ID_LOWER, 0);
	nodeTop10.Add(KEY_NAME, STR_TOP_10);
	CJsonNode nodeTop25(JSON_OBJECT);
	nodeTop25.AddInt(KEY_ID_LOWER, 1);
	nodeTop25.Add(KEY_NAME, STR_TOP_25);
	CJsonNode nodeTop50(JSON_OBJECT);
	nodeTop50.AddInt(KEY_ID_LOWER, 2);
	nodeTop50.Add(KEY_NAME, STR_TOP_50);
	CJsonNode nodeTop100(JSON_OBJECT);
	nodeTop100.AddInt(KEY_ID_LOWER, 3);
	nodeTop100.Add(KEY_NAME, STR_TOP_100);

	list.append(nodeTop10);
	list.append(nodeTop25);
	list.append(nodeTop50);
	list.append(nodeTop100);

	m_pInfoHome->GetFormClassify()->ClearClassifyMostPlayedMenu();
	m_pInfoHome->GetFormClassify()->SetClassifyMostPlayedMenu(list);
}

void MusicDBWindow::SetColumn(int typeMode)
{
	if (typeMode <= TYPE_MODE_ITEM_ADD)
	{
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_FAVORITE, m_ShowCategoryFavorite);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_RATING, m_ShowCategoryRating);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_ARTIST, m_ShowCategoryArtist);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_ALBUM, m_ShowCategoryAlbum);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_GENRE, m_ShowCategoryGenre);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_ALBUM_ARTIST, m_ShowCategoryAlbumArtist);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_TRACK_COUNT, m_ShowCategoryTrackCount);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_ALBUM_GAIN, m_ShowCategoryAlbumGain);

		if (SQLManager::CATEGORY_ALBUM == m_nCategory)
		{
			m_ShowCategoryAlbum = false;
			m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_ALBUM, m_ShowCategoryAlbum);
		}
		else if (SQLManager::CATEGORY_ALBUM_ARTIST == m_nCategory)
		{
			m_ShowCategoryAlbumArtist = false;
			m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_ALBUM_ARTIST, m_ShowCategoryAlbumArtist);
		}
		else if (SQLManager::CATEGORY_ARTIST == m_nCategory)
		{
			m_ShowCategoryArtist = false;
			m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_ARTIST, m_ShowCategoryArtist);
		}
		else if (SQLManager::CATEGORY_GENRE == m_nCategory)
		{
			m_ShowCategoryGenre = false;
			m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_GENRE, m_ShowCategoryGenre);
		}
		else if (SQLManager::CATEGORY_YEAR == m_nCategory)
		{
			m_ShowCategoryRating = false;
			m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_RATING, m_ShowCategoryRating);
		}
		else if (SQLManager::CATEGORY_TRACK == m_nCategory)
		{
			m_ShowCategoryRating = false;
			m_ShowCategoryTrackCount = false;

			m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_RATING, m_ShowCategoryRating);
			m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_TRACK_COUNT, m_ShowCategoryTrackCount);
		}
	}
	else
	{
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_FAVORITE, m_ShowTrackFavorite);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_TIME, m_ShowTrackTime);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_ARTIST, m_ShowTrackArtist);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_ALBUM, m_ShowTrackAlbum);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_GENRE, m_ShowTrackGenre);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_ALBUM_ARTIST, m_ShowTrackAlbumArtist);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_COMPOSER, m_ShowTrackComposer);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_YEAR, m_ShowTrackYear);

		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_MOOD, m_ShowTrackMood);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_TEMPO, m_ShowTrackTempo);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_FORMAT, m_ShowTrackFormat);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_SAMPLE_RATE, m_ShowTrackSampleRate);
		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_BITRATE, m_ShowTrackBitrate);
//		m_pTableTracks->SetColumnShow(TableTracks::TABLE_TRACKS_RATING, m_ShowTrackRating);
	}

	m_pTableTracks->SetColResize(0);
}

int MusicDBWindow::GetListModeFromResize(int resize)
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

QString MusicDBWindow::GetTitleSortMenu(int sort)
{

	if (m_nCategory == SQLManager::CATEGORY_ARTIST
			|| m_nCategory == SQLManager::CATEGORY_COMPOSER
			|| m_nCategory == SQLManager::CATEGORY_MOOD)
	{
		if (sort == SQLManager::DISP_MODE_ARTIST)
		{
			sort = SQLManager::SORT_IMPORTED_DATE;
			m_DispMode = SQLManager::SORT_IMPORTED_DATE;
			m_nSortCategory = SQLManager::SORT_IMPORTED_DATE;
		}
	}

	QString title;

	switch (sort)
	{
	case SQLManager::SORT_IMPORTED_DATE:
		title = STR_SORT_IMPORTED_DATE;
		break;
	case SQLManager::SORT_ALPHABET:
		title = STR_SORT_ALPHABET;
		break;
	case SQLManager::SORT_FAVORITE:
		title = STR_SORT_FAVORITE;
		break;
	case SQLManager::SORT_RATING:
		title = STR_SORT_RATING;
		break;
	case SQLManager::DISP_MODE_TRACK:
		title = STR_DISP_MODE_TRACK;
		break;
	case SQLManager::DISP_MODE_ALBUM:
		title = STR_DISP_MODE_ALBUM;
		break;
	case SQLManager::DISP_MODE_ARTIST:
		title = STR_DISP_MODE_ARTIST;
		break;
//	case SQLManager::DISP_MODE_ARTIST_ALBUM:
//		title = STR_DISP_MODE_ALBUM;
//		break;
	}



	return title;
}




