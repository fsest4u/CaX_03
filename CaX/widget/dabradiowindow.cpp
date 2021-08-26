#include "dabradiowindow.h"
#include "ui_dabradiowindow.h"

DABRadioWindow::DABRadioWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DABRadioWindow)
{
	ui->setupUi(this);
}

DABRadioWindow::~DABRadioWindow()
{
	delete ui;
}
