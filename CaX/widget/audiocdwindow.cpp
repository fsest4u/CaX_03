#include "audiocdwindow.h"
#include "ui_audiocdwindow.h"

AudioCDWindow::AudioCDWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::AudioCDWindow)
{
	ui->setupUi(this);

}

AudioCDWindow::~AudioCDWindow()
{
	delete ui;
}

