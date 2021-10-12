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

signals:

	void SigPlayAll();
	void SigPlayRandom();
	void SigFavorite();
	void SigRating();
	void SigSubmenu();

protected:

	bool eventFilter(QObject *object, QEvent *event);

private:
	Ui::FormPlay *ui;
};

#endif // FORMPLAY_H
