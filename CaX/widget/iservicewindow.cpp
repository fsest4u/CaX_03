#include "iservicewindow.h"
#include "ui_iservicewindow.h"

IServiceWindow::IServiceWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::IServiceWindow)
{
	ui->setupUi(this);
}

IServiceWindow::~IServiceWindow()
{
	delete ui;
}
