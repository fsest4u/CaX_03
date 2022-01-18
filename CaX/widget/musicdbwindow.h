#ifndef MUSICDBWINDOW_H
#define MUSICDBWINDOW_H

#include <QWidget>
#include <QStandardItemModel>

#include "manager/sqlmanager.h"

#include "util/caxconstants.h"
#include "util/CJsonNode.h"

class MusicDBManager;
class InfoHome;
class InfoTracks;
class IconTracks;
class TableTracks;

namespace Ui {
class MusicDBWindow;
}

class MusicDBWindow : public QWidget
{
	Q_OBJECT

public:
	explicit MusicDBWindow(QWidget *parent = nullptr, const QString &addr = "", const int &eventID = -1);
	~MusicDBWindow();

	void AddWidgetItem(int typeMode = TYPE_MODE_ITEM_TRACK, int category = SQLManager::CATEGORY_ALBUM);
	void AddWidgetTrack(int typeMode = TYPE_MODE_TRACK, int category = SQLManager::CATEGORY_ALBUM);

	void RequestCategoryList(int catID = -1, int catID2 = -1);
	void RequestTrackList(int nID, int catID = -1, int catID2 = -1);

//	int GetCategory() const;
//	void SetCategory(int nCategory);

	void SetCoverArt(QString coverArt);

signals:

	//	void SigCategoryInfo(int nID, int nCategory);
	void SigAddWidget(QWidget *widget, QString title);
	void SigRemoveWidget(QWidget* widget);
	void SigAddCategoryFromPlaylist(int category, QMap<int, bool> idMap);
	void SigAddTrackFromPlaylist(QMap<int, bool> idMap);


private slots:

	void SlotAddWidget(QWidget *widget, QString title);
	void SlotRemoveWidget(QWidget *widget);

	void SlotRespError(QString errMsg);
	void SlotRespMusicOverview(CJsonNode node);
	void SlotRespCategoryList(QList<CJsonNode> list);
	void SlotRespCategoryOverview(CJsonNode node);
	void SlotRespTrackList(QList<CJsonNode> list);
	void SlotRespTrackListForEditTag(QList<CJsonNode> list);
	void SlotRespClassifyArtist(QList<CJsonNode> list);
	void SlotRespClassifyGenre(QList<CJsonNode> list);
	void SlotRespClassifyComposer(QList<CJsonNode> list);
	void SlotRespCategoryInfoList(QList<CJsonNode> list);
	void SlotRespCategoryInfo(CJsonNode node);
	void SlotRespTrackInfo(CJsonNode node);
	void SlotRespSetCoverArt(int id, int category);
//	void SlotRespUpdateCategory(int updateId);
//	void SlotRespInsertReplaceCategoryAll();
	void SlotRespRefresh();
	void SlotCoverArtUpdate(QString fileName, int nIndex, int mode);

	void SlotPlayAll();
	void SlotPlayRandom();
	void SlotTopMenu();
	void SlotTopMenuAction(int menuID);

	void SlotSortMenu(int sort);
	void SlotIncDec(bool bIncrease);
	void SlotResize(int resize);

	void SlotAlbumList();
	void SlotAlbumArtistList();
	void SlotArtistList();
	void SlotGenreList();
	void SlotCategoryMenu();
	void SlotCategoryMenuAction(int nCategory, QString title);

	void SlotItemPlayAll();
	void SlotItemPlayRandom();
	void SlotItemFavorite(int nFavorite);
	void SlotItemRating(int nRating);
	void SlotItemTopMenu();
	void SlotItemTopMenuAction(int menuID);

	void SlotItemSortMenu(int sort);
	void SlotItemIncDec(bool bIncrease);

	void SlotSelectPlay(int nID, int where);
	void SlotSelectCategoryPlay(int nID, int where);
	void SlotSelectFavorite(int nID, int nFavorite);
	void SlotSelectRating(int nID, int nRating);
	void SlotSelectTitle(int nID, QString coverArt);

	void SlotReqCoverArt(int id, int index, int mode);
	void SlotAppendList();

	void SlotSelectTrackPlay(int nID, int where);
	void SlotSelectTrackFavorite(int nID, int nFavorite);

	void SlotClassifyFavorite(int nFavorite);
	void SlotClassifyRating(int nRating);
	void SlotClassifyArtist(bool bAdd, QString id);
	void SlotClassifyGenre(bool bAdd, QString id);
	void SlotClassifyComposer(bool bAdd, QString id);

	void SlotOptionMenuAction(int nID, int menuID);

	void SlotTopMenuAddToPlaylist(int id);
	void SlotOptionMenuAddToPlaylist(int id);

	void SlotAddTrackFromPlaylist(QMap<int, bool> idMap);

	void SlotContextMenu(QPoint point);
	void SlotContextMenuPlayNow();
	void SlotContextMenuPlayLast();
	void SlotContextMenuPlayNext();
	void SlotContextMenuPlayClear();
	void SlotContextMenuAddCoverArt();
	void SlotContextMenuTagEdit();

//	void SlotEditAllArtist(QString value);
//	void SlotEditAllAlbum(QString value);
//	void SlotEditAllGenre(QString value);

private:

	void ReadSettings();
	void WriteSettings();

	void ConnectSigToSlot();
	void Initialize();
	void SetCategoryList(QList<CJsonNode> list);

	void SetSelectOffTopMenu();
	void SetSelectOnTopMenu();

	void DoTopMenuPlay(int nWhere);
	void DoTopMenuReload();
	void DoTopMenuSetLimitCount(int count);
	void DoTopMenuSelectAll();
	void DoTopMenuClearAll();
	void DoTopMenuGainSet();
	void DoTopMenuGainClear();
	void DoTopMenuAddToPlaylist();
	void DoTopMenuAddFromPlaylist();

	void DoTopMenuItemPlay(int nWhere);
	void DoTopMenuItemSelectAll();
	void DoTopMenuItemClearAll();
	void DoTopMenuItemGainSet();
	void DoTopMenuItemGainClear();
	void DoTopMenuItemAddToPlaylist();
	void DoTopMenuItemAddFromPlaylist();
	void DoTopMenuItemShowColumns();

	void SetOptionMenu();

	void DoOptionMenuPlay(int nID, int where);
	void DoOptionMenuAddToPlaylist(int nID);
	void DoOptionMenuInfo(int nID);
	void DoOptionMenuAddCoverArt(int nID);
	void DoOptionMenuRename(int nID);
	void DoOptionMenuGain(int nID, QString gainType);

//	void DoInsertIgnoreCategoryAll();

	void SetSortMenu(int category);

	void ClearClassifyMenu();

	void ThreadStartIcon();
	void ThreadStartList();

	void ThreadTerminateIcon();
	void ThreadTerminateList();

	void CalculatePage(int totalCount);

	void SetColumn(int typeMode);
	int GetListModeFromResize(int resize);
	QString GetTitleSortMenu(int sort);

	MusicDBManager		*m_pMgr;
	InfoHome			*m_pInfoHome;
	InfoTracks			*m_pInfoTracks;
	IconTracks			*m_pIconTracks;
	TableTracks			*m_pTableTracks;

	QThread				*m_pIconThread;
	QThread				*m_pListThread;
	QList<CJsonNode>	m_RespList;


	QMap<int, QString>	m_TopMenuMap;
	QMap<int, QString>	m_CategoryMenuMap;
	QMap<int, QString>	m_OptionMenuMap;
	QMap<int, bool>		m_SelectMap;

	QStandardItemModel	*m_UpdateModel;
	QMap<QStringList, QString> m_UpdateMap;

	QStringList			m_AlbumList;
	QStringList			m_AlbumArtistList;
	QStringList			m_ArtistList;
	QStringList			m_GenreList;
	QStringList			m_ComposerList;
	QStringList			m_MoodList;

	int					m_ListMode;
	int					m_TypeMode;
	int					m_DispMode;

	int					m_EventID;

	int					m_nCategory;
	int					m_nSortCategory;
	int					m_nSortTrack;
	bool				m_bIncreaseCategory;
	bool				m_bIncreaseTrack;

	int					m_ResizeItem;
	int					m_ResizeTrack;

	int					m_nFavorite;
	int					m_nRating;

	QString				m_ArtistID;
	QString				m_GenreID;
	QString				m_ComposerID;

	int					m_LimitCount;
	int					m_TotalPage;
	int					m_CurIndex;

	int					m_nID;
	int					m_nCatID;
	int					m_nCatID2;

	int					m_nOptionID;

	bool				m_ShowFavorite;
	bool				m_ShowTime;
	bool				m_ShowArtist;
	bool				m_ShowAlbum;
	bool				m_ShowGenre;
	bool				m_ShowAlbumArtist;
	bool				m_ShowComposer;
	bool				m_ShowYear;

	bool				m_ShowMood;
	bool				m_ShowTempo;
	bool				m_ShowFormat;
	bool				m_ShowSampleRate;
	bool				m_ShowBitDepth;
	bool				m_ShowRating;

	Ui::MusicDBWindow *ui;
};

#endif // MUSICDBWINDOW_H
