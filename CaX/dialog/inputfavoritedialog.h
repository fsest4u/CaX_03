#ifndef INPUTFAVORITEDIALOG_H
#define INPUTFAVORITEDIALOG_H

#include <QDialog>

namespace Ui {
class InputFavoriteDialog;
}

class InputFavoriteDialog : public QDialog
{
	Q_OBJECT

public:
	explicit InputFavoriteDialog(QWidget *parent = nullptr);
	~InputFavoriteDialog();

	bool GetFavorite() const;
	void SetFavorite(bool Favorite);

private slots:

	void SlotClickFavorite();

private:

	bool m_Favorite;

	Ui::InputFavoriteDialog *ui;
};

#endif // INPUTFAVORITEDIALOG_H
