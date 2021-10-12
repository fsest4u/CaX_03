#ifndef FORMCLASSIFY_H
#define FORMCLASSIFY_H

#include <QWidget>

namespace Ui {
class FormClassify;
}

class FormClassify : public QWidget
{
	Q_OBJECT

public:
	explicit FormClassify(QWidget *parent = nullptr);
	~FormClassify();

signals:

	void SigClassify();
	void SigFavorite();
	void SigRating();
	void SigArtist();
	void SigGenre();
	void SigComposer();

protected:

	bool eventFilter(QObject *object, QEvent *event);

private:
	Ui::FormClassify *ui;
};

#endif // FORMCLASSIFY_H
