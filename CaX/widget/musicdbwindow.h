#ifndef MUSICDBWINDOW_H
#define MUSICDBWINDOW_H

#include <QWidget>
#include <QThread>

#include "util/CJsonNode.h"


class MusicDBManager;
class InfoHome;
class InfoTracks;
class IconTracks;
class ListTracks;
class Loading;


namespace Ui {
class MusicDBWindow;
}

class MusicDBWindow : public QWidget
{
	Q_OBJECT

public:
	explicit MusicDBWindow(QWidget *parent = nullptr, const QString &addr = "");
	~MusicDBWindow();

	void RequestMusicHome();
	void RequestCategoryHome(int nID, int nCategory);

signals:

//	void SigCategoryInfo(int nID, int nCategory);
	void SigAddWidget(QWidget *widget);

private slots:

	void SlotAddWidget(QWidget *widget);

	void SlotRespError(QString errMsg);
	void SlotRespMusicInfo(CJsonNode node);
	void SlotRespCategoryList(QList<CJsonNode> list);
	void SlotRespCategoryInfo(CJsonNode node);
	void SlotRespSongsOfCategory(QList<CJsonNode> list);
	void SlotCoverArtUpdate(QString fileName, int nIndex, int mode);

	void SlotMusicPlayAll();
	void SlotMusicPlayRandom();
	void SlotMusicAlbum();
	void SlotMusicArtist();
	void SlotMusicTrack();
	void SlotMusicComposer();
	void SlotMusicSubmenu();
	void SlotMusicSubmenu2();
	void SlotMusicDisplayMode();
	void SlotMusicSort();

	void SlotAlbumPlayAll();
	void SlotAlbumPlayRandom();
	void SlotAlbumSubmenu();
	void SlotAlbumSort();

	void SlotReqCategoryCover(int nID, int nIndex);
	void SlotSelectCategory(int nID);
	void SlotSelectCount(int nID);
	void SlotSelectFavorite(int nID, int nFavorite);
	void SlotSelectRating(int nID, int nRating);

	void SlotReqSongCover(int nID, int nIndex);

	void SlotSelectPlay(int nID);
	void SlotSelectMore(int nID);


private:

	void ConnectSigToSlot();

	MusicDBManager	*m_pMgr;
	InfoHome		*m_pInfoHome;
	InfoTracks		*m_pInfoTracks;
	IconTracks		*m_pIconTracks;
	ListTracks		*m_pListTracks;

	Loading			*m_pLoading;

//	QThread			*m_pCatThread;
//	QThread			*m_pSongThread;

	int				m_nCategory;


	Ui::MusicDBWindow *ui;
};

#endif // MUSICDBWINDOW_H
