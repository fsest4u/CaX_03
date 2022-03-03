#ifndef SETCATEGORYCOLUMNDIALOG_H
#define SETCATEGORYCOLUMNDIALOG_H

#include <QDialog>

namespace Ui {
class SetCategoryColumnDialog;
}

class SetCategoryColumnDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SetCategoryColumnDialog(QWidget *parent = nullptr);
	~SetCategoryColumnDialog();

	bool GetCBFavorite();
	void SetCBFavorite(bool bCheck);

	bool GetCBRating();
	void SetCBRating(bool bCheck);

	bool GetCBArtist();
	void SetCBArtist(bool bCheck);
	void ShowCBArtist(bool show);

	bool GetCBAlbum();
	void SetCBAlbum(bool bCheck);
	void ShowCBAlbum(bool show);

	bool GetCBGenre();
	void SetCBGenre(bool bCheck);
	void ShowCBGenre(bool show);

	bool GetCBAlbumArtist();
	void SetCBAlbumArtist(bool bCheck);
	void ShowCBAlbumArtist(bool show);

	bool GetCBTrackCount();
	void SetCBTrackCount(bool bCheck);
	void ShowCBTrackCount(bool show);

	bool GetCBAlbumGain();
	void SetCBAlbumGain(bool bCheck);

	void SetCategory(int category);


private:
	Ui::SetCategoryColumnDialog *ui;
};

#endif // SETCATEGORYCOLUMNDIALOG_H
