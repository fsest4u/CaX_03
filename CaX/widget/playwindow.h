#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

namespace Ui {
class PlayWindow;
}

class PlayWindow : public QWidget
{
	Q_OBJECT

public:
	explicit PlayWindow(QWidget *parent = nullptr);
	~PlayWindow();

	QLabel		*GetLabelCoverArt();
	QPushButton *GetBtnInfo();
	QPushButton *GetBtnPlaylist();
	QPushButton *GetBtnPrev();
	QPushButton *GetBtnPlay();
	QPushButton *GetBtnNext();
	QPushButton *GetBtnRandom();
	QPushButton *GetBtnRepeat();
	QPushButton *GetBtnDevice();
	QPushButton *GetBtnVolume();

private:
	Ui::PlayWindow *ui;
};

#endif // PLAYWINDOW_H
