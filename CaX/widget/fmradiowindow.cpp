#include "fmradiowindow.h"
#include "ui_fmradiowindow.h"

FMRadioWindow::FMRadioWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FMRadioWindow)
{
	ui->setupUi(this);
}

FMRadioWindow::~FMRadioWindow()
{
	delete ui;
}
