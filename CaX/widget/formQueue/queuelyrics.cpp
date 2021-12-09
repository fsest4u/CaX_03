#include "queuelyrics.h"
#include "ui_queuelyrics.h"

QueueLyrics::QueueLyrics(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::QueueLyrics)
{
	ui->setupUi(this);
}

QueueLyrics::~QueueLyrics()
{
	delete ui;
}

void QueueLyrics::changeEvent(QEvent *e)
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
