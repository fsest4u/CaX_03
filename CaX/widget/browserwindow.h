#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QWidget>

namespace Ui {
class BrowserWindow;
}

class BrowserWindow : public QWidget
{
	Q_OBJECT

public:
	explicit BrowserWindow(QWidget *parent = nullptr);
	~BrowserWindow();



private:
	Ui::BrowserWindow *ui;
};

#endif // BROWSERWINDOW_H
