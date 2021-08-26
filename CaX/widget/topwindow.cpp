#include "topwindow.h"
#include "ui_topwindow.h"

TopWindow::TopWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TopWindow)
{
	ui->setupUi(this);

}

TopWindow::~TopWindow()
{
	delete ui;
}

void TopWindow::SetTitle(QString title)
{
	ui->labelDevice->setText(title);
}

QPushButton *TopWindow::GetBtnMenu()
{
	return ui->btnMenu;
}

QPushButton *TopWindow::GetBtnPrev()
{
	return ui->btnPrev;
}

QPushButton *TopWindow::GetBtnNext()
{
	return ui->btnNext;
}

QPushButton *TopWindow::GetBtnSearch()
{
	return ui->btnSearch;
}
