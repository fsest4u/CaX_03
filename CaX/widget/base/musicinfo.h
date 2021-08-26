#ifndef MUSICINFO_H
#define MUSICINFO_H

#include <QWidget>

namespace Ui {
class MusicInfo;
}

class SortDialog;
class CategoryDialog;

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

	void ShowCategoryDialog();
	void ShowSortDialog();

	CategoryDialog	*m_pCatDlg;
	SortDialog		*m_pSortDlg;

	Ui::MusicInfo *ui;
};

#endif // MUSICINFO_H
