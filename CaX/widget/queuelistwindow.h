#ifndef QUEUELISTWINDOW_H
#define QUEUELISTWINDOW_H

#include <QWidget>
#include <QMenu>

#include "util/CJsonNode.h"

class QueuelistManager;
class PlaylistManager;
class QueueTrack;
class QueueLyrics;
class QueueArtist;
class FormCoverArt;
class FormTitle;
class MusicDBWindow;

namespace Ui {
class QueuelistWindow;
}

class QueuelistWindow : public QWidget
{
	Q_OBJECT

public:
	explicit QueuelistWindow(QWidget *parent = nullptr, const QString &addr = "", const int &eventID = -1);
	~QueuelistWindow();

	void SetNodeInfo(CJsonNode node);
	void SetPlayInfo(CJsonNode node);

	int GetFavorite() const;
	void SetFavorite(int Favorite);

	int GetRating() const;
	void SetRating(int Rating);

	void ClearMenu();
	void SetMenu(QMap<int, QString> map);

signals:

	void SigAddWidget(QWidget *widget, QString);
	void SigRemoveWidget(QWidget* widget);

	void SigRemoveQueueWidget();

	void SigUpdateTimeStamp(uint timestamp);

	void SigMenu();
	void SigMenuAction(int menuID);

private slots:

	void SlotAddWidget(QWidget *widget, QString);
	void SlotRemoveWidget(QWidget *widget);

	void SlotClickBtnTrack();
	void SlotClickBtnLyrics();
	void SlotClickBtnArtist();
	void SlotClickBtnClose();

	void SlotRespError(QString errMsg);
	void SlotRespTrackInfo(CJsonNode node);
	void SlotRespRecordSet(CJsonNode node);
	void SlotCoverArtUpdate(QString fileName, int nIndex, int mode);
	void SlotSelectPlay(int index, int playType);

	void SlotMenu();
	void SlotMenuAction(QAction *action);

	void SlotRefresh(CJsonNode node);

	void SlotFavorite(int id, int favorite);
	void SlotAddToPlaylist(int id);
	void SlotGoToAlbum(int albumID);
	void SlotGoToArtist(int artistID);

	void SlotUpdateTimeStamp(uint timestamp, int count, int time);
	void SlotOptionMenuAddToPlaylist(int id);

private:

	void ConnectSigToSlot();
	void Initialize();

	void RequestCoverArtMusicDB(int id, int index);
	void RequestCoverArtBrowser(QString path);
	void RequestCoverArtAudioCD(QString fullpath);

	void SetMqa(QString value);
	void SetFormat(QString value);
	void SetPlayIndex(int total, int currPlay);
	void SetTotalTime(int time);

	void DoMenuFavorite(int trackID, int favorite);
	void DoMenuGoToAlbum(int albumID, QString cover);
	void DoMenuGoToArtist(int artistID);
	void DoMenuMakePlaylist();

	QueuelistManager	*m_pMgr;
	PlaylistManager		*m_pPlaylistMgr;

	QueueTrack			*m_Track;
	QueueLyrics			*m_Lyrics;
	QueueArtist			*m_Artist;

	QMenu				*m_Menu;

	FormCoverArt		*m_pFormCoverArt;
	FormTitle			*m_pFormTitle;

	MusicDBWindow		*m_pMusicDBWin;

	int					m_EventID;

	QString				m_AlbumName;
	QString				m_AlbumCoverArt;
//	QString				m_ArtistName;
//	QString				m_ArtistCoverArt;

	QString				m_Src;
	int					m_TotalTime;
	int					m_Favorite;
	int					m_Rating;

	int					m_TrackID;
	int					m_TrackAlbumID;
	int					m_TrackArtistID;
	int					m_TrackFavorite;


	Ui::QueuelistWindow *ui;
};

#endif // QUEUELISTWINDOW_H
