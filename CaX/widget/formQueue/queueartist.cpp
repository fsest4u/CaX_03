#include "queueartist.h"
#include "ui_queueartist.h"

QueueArtist::QueueArtist(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::QueueArtist)
{
	ui->setupUi(this);
}

QueueArtist::~QueueArtist()
{
	delete ui;
}

void QueueArtist::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}
