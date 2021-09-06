#ifndef MYMUSICWINDOW_H
#define MYMUSICWINDOW_H

#include <QWidget>
#include <QThread>

#include "util/CJsonNode.h"

namespace Ui {
class MyMusicWindow;
}

class MyMusicManager;
class MusicInfo;
class AlbumInfo;
class CategoryConts;
class SongConts;
class Loading;

class MyMusicWindow : public QWidget
{
	Q_OBJECT

public:
	explicit MyMusicWindow(QWidget *parent = nullptr, const QString &addr = "");
	~MyMusicWindow();

	void RequestMusicHome();
	void RequestCategoryHome(int nID, int nCategory);

signals:

//	void SigCategoryInfo(int nID, int nCategory);
	void SigAddWidget(QWidget *widget);

private slots:

	void SlotAddWidget(QWidget *widget);

	void SlotRespError(QString errMsg);
	void SlotRespMusicInfo(CJsonNode node);
	void SlotRespCategoryList(QList<CJsonNode> nodeList);
	void SlotRespCategoryInfo(CJsonNode node);
	void SlotRespSongsOfCategory(QList<CJsonNode> nodeList);
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

	MyMusicManager	*m_pMgr;
	MusicInfo		*m_pMusicInfo;
	AlbumInfo		*m_pAlbumInfo;
	CategoryConts	*m_pCatConts;
	SongConts		*m_pSongConts;

	Loading			*m_pLoading;

//	QThread			*m_pCatThread;
//	QThread			*m_pSongThread;

	int				m_nCategory;


	Ui::MyMusicWindow *ui;
};

#endif // MYMUSICWINDOW_H
