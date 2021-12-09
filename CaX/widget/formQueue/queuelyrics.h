#ifndef QUEUELYRICS_H
#define QUEUELYRICS_H

#include <QWidget>

namespace Ui {
class QueueLyrics;
}

class QueueLyrics : public QWidget
{
	Q_OBJECT

public:
	explicit QueueLyrics(QWidget *parent = nullptr);
	~QueueLyrics();

protected:
	void changeEvent(QEvent *e);

private:
	Ui::QueueLyrics *ui;
};

#endif // QUEUELYRICS_H
