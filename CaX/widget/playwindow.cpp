#include "playwindow.h"
#include "ui_playwindow.h"

PlayWindow::PlayWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PlayWindow)
{
	ui->setupUi(this);
}

PlayWindow::~PlayWindow()
{
	delete ui;
}

QLabel *PlayWindow::GetLabelCoverArt()
{
	return ui->labelCoverArt;
}

QPushButton *PlayWindow::GetBtnInfo()
{
	return ui->btnInfo;
}

QPushButton *PlayWindow::GetBtnPlaylist()
{
	return ui->btnPlaylist;
}

QPushButton *PlayWindow::GetBtnPrev()
{
	return ui->btnPrev;
}

QPushButton *PlayWindow::GetBtnPlay()
{
	return ui->btnPlay;
}

QPushButton *PlayWindow::GetBtnNext()
{
	return ui->btnNext;
}

QPushButton *PlayWindow::GetBtnRandom()
{
	return ui->btnRandom;
}

QPushButton *PlayWindow::GetBtnRepeat()
{
	return ui->btnRepeat;
}

QPushButton *PlayWindow::GetBtnDevice()
{
	return ui->btnDevice;
}

QPushButton *PlayWindow::GetBtnVolume()
{
	return ui->btnVolume;
}
