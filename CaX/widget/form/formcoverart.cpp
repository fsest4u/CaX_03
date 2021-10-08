#include <QMouseEvent>

#include "formcoverart.h"
#include "ui_formcoverart.h"

FormCoverArt::FormCoverArt(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FormCoverArt)
{
	ui->setupUi(this);

	ui->labelCoverArt->installEventFilter(this);
}

FormCoverArt::~FormCoverArt()
{
	delete ui;
}

QString FormCoverArt::GetCoverArt()
{
	// there is no mean
	return ui->labelCoverArt->text();
}

void FormCoverArt::SetCoverArt(QString coverArt)
{
	if (coverArt.isEmpty())
	{
		coverArt = ":/resource/Icon-playbar-volume-160.png";
	}

	QImage image;
	if (image.load(coverArt))
	{
//			painter->drawImage(coverRect, image);
		QPixmap pixmap = QPixmap::fromImage(image);
		ui->labelCoverArt->setPixmap(pixmap.scaled(ui->labelCoverArt->width()
												   , ui->labelCoverArt->height()
												   , Qt::KeepAspectRatio));
	}
}

bool FormCoverArt::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->labelCoverArt)
		{
			emit SigCoverArt();
		}
	}

	return QObject::eventFilter(object, event);
}
