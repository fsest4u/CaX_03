#include "setupwindow.h"
#include "ui_setupwindow.h"

SetupWindow::SetupWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SetupWindow)
{
	ui->setupUi(this);
}

SetupWindow::~SetupWindow()
{
	delete ui;
}
