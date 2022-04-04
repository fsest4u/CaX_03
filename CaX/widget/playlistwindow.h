#ifndef PLAYLISTWINDOW_H
#define PLAYLISTWINDOW_H

#include <QWidget>
#include <QMenu>
#include <QModelIndex>

#include "util/caxconstants.h"
#include "util/CJsonNode.h"

class PlaylistManager;
class InfoService;
class InfoTracks;
class IconTracks;
class ListTracks;
class Loading;

namespace Ui {
class PlaylistWindow;
}

class PlaylistWindow : public QWidget
{
	Q_OBJECT

public:
	explicit PlaylistWindow(QWidget *parent = nullptr, const QString &addr = "");
	~PlaylistWindow();

	void AddWidgetItem(int typeMode = TYPE_MODE_ITEM_TRACK);
	void AddWidgetTrack();

	void RequestPlaylist();
	void RequestPlaylistInfo(int id, QString coverArt);
	void RequestTrackList(int id);

signals:

	void SigAddWidget(QWidget *widget, QString title);
	void SigRemoveWidget(QWidget *widget);
	void SigAddToPlaylist(int id);

private slots:

	void SlotAddWidget(QWidget *widget, QString title);
	void SlotRemoveWidget(QWidget *widget);

	void SlotRespError(QString errMsg);
	void SlotRefresh(CJsonNode node);
	void SlotRespPlaylist(QList<CJsonNode> list);
	void SlotRespPlaylistInfo(CJsonNode node);
	void SlotRespTrackList(QList<CJsonNode> list);
	void SlotSelectMenu(const QModelIndex &modelIndex, QPoint point);
	void SlotMenuAction(QAction *action);
	void SlotReqCoverArt(const QModelIndex &modelIndex, int mode);
	void SlotCoverArtUpdate(QString coverArt, int index, int mode);

	void SlotSelectTitle(const QModelIndex &modelIndex);
	void SlotSelectPlay(const QModelIndex &modelIndex, int playType);
	void SlotSelectTrackPlay(const QModelIndex &modelIndex, int playType);

	void SlotPlayAll();
	void SlotPlayRandom();
	void SlotTopMenu();
	void SlotTopMenuAction(int menuID);
	void SlotResize(int resize);

	void SlotItemPlayAll();
	void SlotItemPlayRandom();
	void SlotItemTopMenu();
	void SlotItemTopMenuAction(int menuID);

	void SlotOptionMenuAction(const QModelIndex &modelIndex, int menuID);
	void SlotAddCategoryFromPlaylist(int category, QMap<int, int> idMap);
	void SlotAddTrackFromPlaylist(QMap<int, int> idMap);

private:

	void ReadSettings();
	void WriteSettings();

	void ConnectSigToSlot();
	void Initialize();
	int GetListModeFromResize(int resize);

	void SetSelectOffTopMenu();
	void SetSelectOnTopMenu();

	void DoTopMenuPlay(int nWhere);
	void DoTopMenuSelectAll();
	void DoTopMenuClearAll();
	void DoTopMenuMakePlaylist();
	void DoTopMenuDelete();
	void DoTopMenuRename();
	void DoTopMenuAddToPlaylist();

	void DoTopMenuItemPlay(int nWhere);
	void DoTopMenuItemSelectAll();
	void DoTopMenuItemClearAll();
	void DoTopMenuItemAddToPlaylist();
	void DoTopMenuItemDeleteFromPlaylist();


	void SetOptionMenu();

	void DoOptionMenuPlay(const QModelIndex &modelIndex, int where);
	void DoOptionMenuRename(const QModelIndex &modelIndex);
	void DoOptionMenuDelete(const QModelIndex &modelIndex);
	void DoOptionMenuAddToPlaylist(const QModelIndex &modelIndex);
	void DoOptionMenuFavorite();
	void DoOptionMenuGoToAlbum();
	void DoOptionMenuGoToArtist();

	PlaylistManager		*m_pMgr;
	InfoService			*m_pInfoService;
	InfoTracks			*m_pInfoTracks;
	IconTracks			*m_pIconTracks;
	ListTracks			*m_pListTracks;

	Loading				*m_Loading;

	QList<CJsonNode>	m_RespList;

	QMap<int, QString>	m_TopMenuMap;
	QMap<int, QString>	m_OptionMenuMap;
	QMap<int, int>		m_SelectMap;

	QMenu				*m_Menu;

	int					m_ListMode;
	int					m_TypeMode;

	int					m_ID;
//	int					m_TrackID;
//	int					m_TrackAlbumID;
//	int					m_TrackArtistID;
//	int					m_TrackGenreID;
//	int					m_TrackFavorite;
//	int					m_TrackIndex;

	QModelIndex			m_ModelIndex;

	QString				m_TrackCover;

	int					m_ResizeItem;
	int					m_ResizeTrack;


	Ui::PlaylistWindow *ui;
};

#endif // PLAYLISTWINDOW_H
