#include "groupplaywindow.h"
#include "ui_groupplaywindow.h"

GroupPlayWindow::GroupPlayWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::GroupPlayWindow)
{
	ui->setupUi(this);
}

GroupPlayWindow::~GroupPlayWindow()
{
	delete ui;
}
