#ifndef AUDIOCDWINDOW_H
#define AUDIOCDWINDOW_H

#include <QWidget>

namespace Ui {
class AudioCDWindow;
}

class AudioCDWindow : public QWidget
{
	Q_OBJECT

public:
	explicit AudioCDWindow(QWidget *parent = nullptr);
	~AudioCDWindow();


private:
	Ui::AudioCDWindow *ui;
};

#endif // AUDIOCDWINDOW_H
