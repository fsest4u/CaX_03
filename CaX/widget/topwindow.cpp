#include "topwindow.h"
#include "ui_topwindow.h"

TopWindow::TopWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TopWindow)
{
	ui->setupUi(this);

	m_TitleList.clear();
}

TopWindow::~TopWindow()
{
	delete ui;
}

void TopWindow::AddTitle(QString title)
{
	m_TitleList.append(title);
}

void TopWindow::RemoveTitle()
{
	m_TitleList.removeLast();
}

void TopWindow::SetTitle(int index)
{
	int count = m_TitleList.count();
	if (count <= index)
	{
		return;
	}
	QString title = m_TitleList.at(index);
	ui->labelTitle->setText(title);
}

QPushButton *TopWindow::GetBtnMenu()
{
	return ui->btnMenu;
}

QPushButton *TopWindow::GetBtnHome()
{
	return ui->btnHome;
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
