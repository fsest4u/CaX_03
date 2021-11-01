#ifndef PLAYLISTWINDOW_H
#define PLAYLISTWINDOW_H

#include <QWidget>

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

	void AddWidgetPlaylistHome();
	void AddWidgetItemHome();
	void RequestPlaylist();
	void RequestPlaylistInfo(int id, QString coverArt);
	void RequestTrackList(int id);

signals:

	void SigAddWidget(QWidget *widget, QString title);

private slots:

	void SlotAddWidget(QWidget *widget, QString title);

	void SlotRespError(QString message);
	void SlotRespPlaylist(QList<CJsonNode> list);
	void SlotRespPlaylistInfo(CJsonNode node);
	void SlotRespTrackList(QList<CJsonNode> list);
	void SlotReqCoverArt(int id, int index, int mode);
	void SlotCoverArtUpdate(QString coverArt, int index, int mode);

	void SlotSelectTitle(int id, QString coverArt);
	void SlotSelectCount(int id);
	void SlotSelectPlay(int id, int playType);

	void SlotPlayAll();
	void SlotPlayRandom();
	void SlotTopMenu();
	void SlotTopMenuAction(int menuID);
	void SlotResize();

	void SlotItemPlayAll();
	void SlotItemPlayRandom();
	void SlotItemTopMenu();
	void SlotItemTopMenuAction(int menuID);



private:

	void ConnectSigToSlot();
	void Initialize();
	void SetSelectOffTopMenu(bool root = false);
	void SetSelectOnTopMenu(bool root = false);

	void DoTopMenuPlay(int nWhere);
	void DoTopMenuSelectAll();
	void DoTopMenuClearAll();
	void DoTopMenuAddItem();
	void DoTopMenuDeleteItem();

	void DoItemTopMenuPlay(int nWhere);
	void DoItemTopMenuSelectAll();
	void DoItemTopMenuClearAll();
	void DoItemTopMenuAddItem();
	void DoItemTopMenuDeleteItem();

	PlaylistManager	*m_pMgr;
	InfoService		*m_pInfoService;
	InfoTracks		*m_pInfoTracks;
	IconTracks		*m_pIconTracks;
	ListTracks		*m_pListTracks;

	QMap<int, QString> m_TopMenuMap;
	QMap<int, bool>	m_SelectMap;

	int				m_ID;


	Ui::PlaylistWindow *ui;
};

#endif // PLAYLISTWINDOW_H
