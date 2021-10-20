#ifndef FORMPLAY_H
#define FORMPLAY_H

#include <QWidget>

namespace Ui {
class FormPlay;
}

class FormPlay : public QWidget
{
	Q_OBJECT

public:
	explicit FormPlay(QWidget *parent = nullptr);
	~FormPlay();

	void ShowPlayAll();
	void ShowPlayRandom();
	void ShowFavorite();
	void ShowRating();
	void ShowSubmenu();

	int GetFavorite() const;
	void SetFavorite(int Favorite);

	int GetRating() const;
	void SetRating(int Rating);

signals:

	void SigPlayAll();
	void SigPlayRandom();
	void SigFavorite(int Favorite);
	void SigRating();
	void SigSubmenu();

protected:

	bool eventFilter(QObject *object, QEvent *event);

private:

	int m_Favorite;
	int m_Rating;

	Ui::FormPlay *ui;
};

#endif // FORMPLAY_H
