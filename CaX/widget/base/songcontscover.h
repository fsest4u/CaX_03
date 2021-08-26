#ifndef SONGCONTSCOVER_H
#define SONGCONTSCOVER_H

#include <QWidget>

namespace Ui {
class SongContsCover;
}

class SongContsCover : public QWidget
{
	Q_OBJECT

public:
	explicit SongContsCover(QWidget *parent = nullptr);
	~SongContsCover();

private:
	Ui::SongContsCover *ui;
};

#endif // SONGCONTSCOVER_H
