#ifndef FMRADIOWINDOW_H
#define FMRADIOWINDOW_H

#include <QWidget>

namespace Ui {
class FMRadioWindow;
}

class FMRadioWindow : public QWidget
{
	Q_OBJECT

public:
	explicit FMRadioWindow(QWidget *parent = nullptr);
	~FMRadioWindow();



private:
	Ui::FMRadioWindow *ui;
};

#endif // FMRADIOWINDOW_H
