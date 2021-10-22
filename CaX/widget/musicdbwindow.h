#ifndef MUSICDBWINDOW_H
#define MUSICDBWINDOW_H

#include <QWidget>
#include <QThread>

#include "manager/sqlmanager.h"

#include "util/CJsonNode.h"

class MusicDBManager;
class InfoHome;
class InfoTracks;
class IconTracks;
class ListTracks;

namespace Ui {
class MusicDBWindow;
}

class MusicDBWindow : public QWidget
{
	Q_OBJECT

public:
	explicit MusicDBWindow(QWidget *parent = nullptr, const QString &addr = "");
	~MusicDBWindow();

	void RequestMusicDBHome();
	void RequestCategoryHome(int nID,
							 int nCategory,
							 int nSort = SQLManager::SORT_NAME,
							 bool bIncrease = true);

	void AddWidgetMusicDBHome();
	void AddWidgetCategoryHome();

	int GetCategory() const;
	void SetCategory(int nCategory);

signals:

	//	void SigCategoryInfo(int nID, int nCategory);
	void SigAddWidget(QWidget *widget, QString);

private slots:

	void SlotAddWidget(QWidget *widget, QString);

	void SlotRespError(QString errMsg);
	void SlotRespMusicInfo(CJsonNode node);
	void SlotRespCategoryList(QList<CJsonNode> list);
	void SlotRespCategoryInfo(CJsonNode node);
	void SlotRespSongsOfCategory(QList<CJsonNode> list);
	void SlotCoverArtUpdate(QString fileName, int nIndex, int mode);

	void SlotPlayAll();
	void SlotPlayRandom();
	void SlotPlayTopMenu();
	void SlotTopMenuAction(int menuID);

	void SlotSort(int sort);
	void SlotIncDec(bool bIncrease);
	void SlotResize();

	void SlotGenreList();
	void SlotAlbumList();
	void SlotArtistList();
	void SlotTrackList();
	void SlotSubmenu2();

	void SlotAlbumPlayAll();
	void SlotAlbumPlayRandom();
	void SlotAlbumPlayTopMenu();
	void SlotAlbumTopMenuAction(int menuID);
	void SlotAlbumFavorite(int nFavorite);
	void SlotAlbumRating(int nRating);
	void SlotAlbumSort(int sort);
	void SlotAlbumIncDec(bool bIncrease);
	void SlotAlbumResize();

	void SlotSelectPlay(int nID);
	void SlotSelectFavorite(int nID, int nFavorite);
	void SlotSelectRating(int nID, int nRating);
	void SlotSelectTitle(int nID, QString coverArt);

	void SlotReqCoverArt(int id, int index, int mode);
	void SlotAppendIconList();
	void SlotAppendList();

	void SlotSelectTrackPlay(int nID);
	void SlotSelectTrackMore(int nID);
	void SlotSelectTrackFavorite(int nID, int nFavorite);

	void SlotRespClassifyArtist(QList<CJsonNode> list);
	void SlotRespClassifyGenre(QList<CJsonNode> list);
	void SlotRespClassifyComposer(QList<CJsonNode> list);

	void SlotClassifyFavorite(int nFavorite);
	void SlotClassifyRating(int nRating);
	void SlotClassifyArtist(bool bAdd, QString id);
	void SlotClassifyGenre(bool bAdd, QString id);
	void SlotClassifyComposer(bool bAdd, QString id);



private:

	void ConnectSigToSlot();
	void Initialize();
	void SetSelectOffTopMenu();
	void SetSelectOnTopMenu();

	void SetCoverArt(QString coverArt);

	void AddSortMusicDBHome();
	void AddSortCategoryHome();

	void ClearClassifyMenu();

	int GetTotalCount(CJsonNode node);
	void CalculatePage(int totalCount);

	MusicDBManager	*m_pMgr;
	InfoHome		*m_pInfoHome;
	InfoTracks		*m_pInfoTracks;
	IconTracks		*m_pIconTracks;
	ListTracks		*m_pListTracks;

//	QThread			*m_pCatThread;
//	QThread			*m_pSongThread;

	QMap<int, QString> m_TopMenu;

	int				m_nCategory;
	int				m_nID;
	int				m_nSortCategory;
	int				m_nSortTrack;
	bool			m_bIncreaseCategory;
	bool			m_bIncreaseTrack;

	int				m_nFavorite;
	int				m_nRating;

	QString			m_ArtistID;
	QString			m_GenreID;
	QString			m_ComposerID;

	int				m_LimitCount;
	int				m_TotalPage;
	int				m_CurPage;

	Ui::MusicDBWindow *ui;
};

#endif // MUSICDBWINDOW_H
