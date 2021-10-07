#ifndef INFOHOME_H
#define INFOHOME_H

#include <QWidget>
#include <QMenu>

#include "util/CJsonNode.h"

namespace Ui {
class InfoHome;
}

class SubmenuDialog;

class InfoHome : public QWidget
{
	Q_OBJECT

public:
	explicit InfoHome(QWidget *parent = nullptr);
	~InfoHome();

	void SetAlbumCnt(const QString count);
	void SetArtistCnt(const QString count);
	void SetTrackCnt(const QString count);
	void SetGenreCnt(const QString count);
	void SetSortName(const QString name);

	void SetClassifyArtistMenu(QList<CJsonNode> list);
	void SetClassifyGenreMenu(QList<CJsonNode> list);
	void SetClassifyComposerMenu(QList<CJsonNode> list);

protected:

	bool eventFilter(QObject *object, QEvent *event);

signals:

	void SigPlayAll();
	void SigPlayRandom();
	void SigAlbum();
	void SigArtist();
	void SigTrack();
	void SigGenre();
//	void SigSubmenu();
	void SigSubmenu2();
	void SigDisplayMode();
//	void SigSort();

	void SigClassifyArtist(bool bAdd, QString id);
	void SigClassifyGenre(bool bAdd, QString id);
	void SigClassifyComposer(bool bAdd, QString id);


private slots:

	void SlotArtistMenu(QAction* action);
	void SlotGenreMenu(QAction* action);
	void SlotComposerMenu(QAction* action);

private:

	enum {
		CATEGORY_GENRE = 0,
		CATEGORY_MOOD,
		CATEGORY_FOLDER,
		CATEGORY_YEAR,
		CATEGORY_MAX
	};

	enum {
		SORT_GENRE = 0,
		SORT_MOOD,
		SORT_FOLDER,
		SORT_YEAR,
		SORT_RATING,
		SORT_SAMPLE_RATE,
		SORT_MAX
	};

	void SetCategoryDialog();
	void SetSortDialog();

	void SetClassifyMenu();

	void DoClickClassifyArtist();
	void DoClickClassifyGenre();
	void DoClickClassifyComposer();



	void ShowCategoryDialog();
	void ShowSortDialog();

	SubmenuDialog	*m_pCatDlg;
	SubmenuDialog	*m_pSortDlg;

	QMenu		*m_ClassifyMenu;
	QMenu		*m_GenreMenu;
	QMenu		*m_ArtistMenu;
	QMenu		*m_ComposerMenu;

	Ui::InfoHome *ui;
};

#endif // INFOHOME_H
