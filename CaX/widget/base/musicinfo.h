#ifndef MUSICINFO_H
#define MUSICINFO_H

#include <QWidget>

#define CATEGORY_TYPE_NONE	200
#define SORT_TYPE_NONE	200

namespace Ui {
class MusicInfo;
}

class SubmenuDialog;

class MusicInfo : public QWidget
{
	Q_OBJECT

public:
	explicit MusicInfo(QWidget *parent = nullptr);
	~MusicInfo();

	void SetAlbumCnt(const QString count);
	void SetArtistCnt(const QString count);
	void SetTrackCnt(const QString count);
	void SetComposerCnt(const QString count);
	void SetSortName(const QString name);

protected:

	bool eventFilter(QObject *object, QEvent *event);

signals:

	void SigPlayAll();
	void SigPlayRandom();
	void SigAlbum();
	void SigArtist();
	void SigTrack();
	void SigComposer();
//	void SigSubmenu();
	void SigSubmenu2();
	void SigDisplayMode();
//	void SigSort();

private:

	enum categoryType {
		CAT_GENRE = Qt::UserRole + CATEGORY_TYPE_NONE,
		CAT_MOOD,
		CAT_FOLDER,
		CAT_YEAR,
		CAT_MAX
	};

	enum sortType {
		SORT_BY_GENRE = Qt::UserRole + SORT_TYPE_NONE,
		SORT_BY_MOOD,
		SORT_BY_FOLDER,
		SORT_BY_YEAR,
		SORT_BY_RATING,
		SORT_BY_SAMPLE_RATE,
		SORT_BY_MAX
	};

	void SetCategoryDialog();
	void SetSortDialog();

	void ShowCategoryDialog();
	void ShowSortDialog();

	SubmenuDialog	*m_pCatDlg;
	SubmenuDialog	*m_pSortDlg;

	Ui::MusicInfo *ui;
};

#endif // MUSICINFO_H
