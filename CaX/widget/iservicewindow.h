#ifndef ISERVICEWINDOW_H
#define ISERVICEWINDOW_H

#include <QWidget>

namespace Ui {
class IServiceWindow;
}

class IServiceWindow : public QWidget
{
	Q_OBJECT

public:
	explicit IServiceWindow(QWidget *parent = nullptr);
	~IServiceWindow();



private:
	Ui::IServiceWindow *ui;
};

#endif // ISERVICEWINDOW_H
