#ifndef PLAYLISTWINDOW_H
#define PLAYLISTWINDOW_H

#include <QWidget>

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

	void Playlist();
	void PlaylistInfo(int id, QString coverArt);
	void TrackList(int id);
	void TrackPlay(int id);
	void TracksPlay(QList<int> ids);

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
	void SlotSelectPlay(int id);

private:

	void ConnectSigToSlot();

	PlaylistManager	*m_pMgr;
	InfoService		*m_pInfoService;
	InfoTracks		*m_pInfoTracks;
	IconTracks		*m_pIconTracks;
	ListTracks		*m_pListTracks;

	Loading			*m_pLoading;

	Ui::PlaylistWindow *ui;
};

#endif // PLAYLISTWINDOW_H
