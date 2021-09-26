#ifndef PLAYLISTWINDOW_H
#define PLAYLISTWINDOW_H

#include <QWidget>

#include "util/CJsonNode.h"

class PlaylistManager;
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

	void PlayList();

private slots:

	void SlotRespPlaylist(QList<CJsonNode> list);

	void SlotReqCoverArt(int nID, int nIndex);
	void SlotCoverArtUpdate(QString fileName, int nIndex, int mode);


private:

	void ConnectSigToSlot();

	PlaylistManager	*m_pMgr;
	InfoTracks		*m_pInfoTracks;
	IconTracks		*m_pIconTracks;
	ListTracks		*m_pListTracks;

	Loading			*m_pLoading;

	Ui::PlaylistWindow *ui;
};

#endif // PLAYLISTWINDOW_H
