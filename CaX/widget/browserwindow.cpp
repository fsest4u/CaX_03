#include "browserwindow.h"
#include "ui_browserwindow.h"

BrowserWindow::BrowserWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::BrowserWindow)
{
	ui->setupUi(this);

}

BrowserWindow::~BrowserWindow()
{
	delete ui;
}
