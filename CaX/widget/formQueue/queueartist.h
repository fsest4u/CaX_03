#ifndef QUEUEARTIST_H
#define QUEUEARTIST_H

#include <QWidget>

namespace Ui {
class QueueArtist;
}

class QueueArtist : public QWidget
{
	Q_OBJECT

public:
	explicit QueueArtist(QWidget *parent = nullptr);
	~QueueArtist();

protected:
	void changeEvent(QEvent *e);

private:
	Ui::QueueArtist *ui;
};

#endif // QUEUEARTIST_H
