#ifndef DABRADIOWINDOW_H
#define DABRADIOWINDOW_H

#include <QWidget>

namespace Ui {
class DABRadioWindow;
}

class DABRadioWindow : public QWidget
{
	Q_OBJECT

public:
	explicit DABRadioWindow(QWidget *parent = nullptr);
	~DABRadioWindow();


private:
	Ui::DABRadioWindow *ui;
};

#endif // DABRADIOWINDOW_H
