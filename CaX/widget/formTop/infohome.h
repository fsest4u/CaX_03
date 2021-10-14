#ifndef INFOHOME_H
#define INFOHOME_H

#include <QWidget>

#include "util/CJsonNode.h"

class SubmenuDialog;
class FormPlay;
class FormClassify;
class FormSort;

namespace Ui {
class InfoHome;
}

class InfoHome : public QWidget
{
	Q_OBJECT

public:
	explicit InfoHome(QWidget *parent = nullptr);
	~InfoHome();

	void SetTitle(const int nCategory);
	void SetAlbumCnt(const QString count);
	void SetArtistCnt(const QString count);
	void SetTrackCnt(const QString count);
	void SetGenreCnt(const QString count);
	void SetSortName(const QString name);

	FormPlay *GetFormPlay();
	FormClassify *GetFormClassify();
	FormSort *GetFormSort();




signals:

	void SigGenreList();
	void SigAlbumList();
	void SigArtistList();
	void SigTrackList();
	void SigSubmenu2();
	void SigDisplayMode();
//	void SigSort();



protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

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


	void ShowCategoryDialog();
	void ShowSortDialog();

	SubmenuDialog	*m_pCatDlg;
	SubmenuDialog	*m_pSortDlg;
	FormPlay		*m_pFormPlay;
	FormClassify	*m_pFormClassify;
	FormSort		*m_pFormSort;



	Ui::InfoHome *ui;
};

#endif // INFOHOME_H
