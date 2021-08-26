#include "songcontscover.h"
#include "ui_songcontscover.h"

SongContsCover::SongContsCover(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SongContsCover)
{
	ui->setupUi(this);
}

SongContsCover::~SongContsCover()
{
	delete ui;
}
