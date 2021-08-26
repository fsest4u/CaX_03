#ifndef GROUPPLAYWINDOW_H
#define GROUPPLAYWINDOW_H

#include <QWidget>

namespace Ui {
class GroupPlayWindow;
}

class GroupPlayWindow : public QWidget
{
	Q_OBJECT

public:
	explicit GroupPlayWindow(QWidget *parent = nullptr);
	~GroupPlayWindow();

private:
	Ui::GroupPlayWindow *ui;
};

#endif // GROUPPLAYWINDOW_H
