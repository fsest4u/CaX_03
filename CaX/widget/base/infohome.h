#ifndef INFOHOME_H
#define INFOHOME_H

#include <QWidget>

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

	Ui::InfoHome *ui;
};

#endif // INFOHOME_H
