#ifndef PLAYLISTWINDOW_H
#define PLAYLISTWINDOW_H

#include <QWidget>
#include <QMenu>

#include "util/caxconstants.h"
#include "util/CJsonNode.h"

class PlaylistManager;
class InfoService;
class InfoTracks;
class IconTracks;
class ListTracks;

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
	void SlotRespPlaylist(QList<CJsonNode> list);
	void SlotRespPlaylistInfo(CJsonNode node);
	void SlotRespTrackList(QList<CJsonNode> list);
	void SlotSelectMenu(const QModelIndex &modelIndex, QPoint point);
	void SlotMenuAction(QAction *action);
	void SlotReqCoverArt(int id, int index, int mode);
	void SlotCoverArtUpdate(QString coverArt, int index, int mode);

	void SlotSelectTitle(int id, QString coverArt);
	void SlotSelectPlay(int id, int playType);
	void SlotSelectTrackPlay(int id, int playType);

	void SlotPlayAll();
	void SlotPlayRandom();
	void SlotTopMenu();
	void SlotTopMenuAction(int menuID);
	void SlotResize(int resize);

	void SlotItemPlayAll();
	void SlotItemPlayRandom();
	void SlotItemTopMenu();
	void SlotItemTopMenuAction(int menuID);

	void SlotOptionMenuAction(int nID, int menuID);
	void SlotAddCategoryFromPlaylist(int category, QMap<int, bool> idMap);
	void SlotAddTrackFromPlaylist(QMap<int, bool> idMap);

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
	void DoTopMenuAdd();
	void DoTopMenuDelete();
	void DoTopMenuAddToPlaylist();

	void DoTopMenuItemPlay(int nWhere);
	void DoTopMenuItemSelectAll();
	void DoTopMenuItemClearAll();
	void DoTopMenuItemAddToPlaylist();
	void DoTopMenuItemDeleteToPlaylist();


	void SetOptionMenu();

	void DoOptionMenuPlay(int nID, int where);
	void DoOptionMenuRename(int nID);
	void DoOptionMenuDelete(int nID);
	void DoOptionMenuAddToPlaylist(int nID);
	void DoOptionMenuFavorite();
	void DoOptionMenuGoToAlbum();
	void DoOptionMenuGoToArtist();

	PlaylistManager		*m_pMgr;
	InfoService			*m_pInfoService;
	InfoTracks			*m_pInfoTracks;
	IconTracks			*m_pIconTracks;
	ListTracks			*m_pListTracks;

	QList<CJsonNode>	m_RespList;

	QMap<int, QString>	m_TopMenuMap;
	QMap<int, QString>	m_OptionMenuMap;
	QMap<int, bool>		m_SelectMap;

	QMenu				*m_Menu;

	int					m_ListMode;
	int					m_TypeMode;

	int					m_ID;
	int					m_TrackID;
	int					m_TrackAlbumID;
	int					m_TrackArtistID;
	int					m_TrackGenreID;
	int					m_TrackFavorite;
	int					m_TrackIndex;

	QString				m_TrackCover;

	int					m_ResizeItem;
	int					m_ResizeTrack;


	Ui::PlaylistWindow *ui;
};

#endif // PLAYLISTWINDOW_H
