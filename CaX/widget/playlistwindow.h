#ifndef PLAYLISTWINDOW_H
#define PLAYLISTWINDOW_H

#include <QWidget>

namespace Ui {
class PlaylistWindow;
}

class PlaylistWindow : public QWidget
{
	Q_OBJECT

public:
	explicit PlaylistWindow(QWidget *parent = nullptr);
	~PlaylistWindow();



private:
	Ui::PlaylistWindow *ui;
};

#endif // PLAYLISTWINDOW_H
