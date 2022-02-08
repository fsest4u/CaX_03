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

	bool GetCBAlbum();
	void SetCBAlbum(bool bCheck);

	bool GetCBGenre();
	void SetCBGenre(bool bCheck);

	bool GetCBAlbumArtist();
	void SetCBAlbumArtist(bool bCheck);

	bool GetCBTrackCount();
	void SetCBTrackCount(bool bCheck);


private:
	Ui::SetCategoryColumnDialog *ui;
};

#endif // SETCATEGORYCOLUMNDIALOG_H
