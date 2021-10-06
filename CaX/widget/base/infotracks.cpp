#include <QMouseEvent>

#include "infotracks.h"
#include "ui_infotracks.h"

InfoTracks::InfoTracks(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::InfoTracks)
{
	ui->setupUi(this);

	ui->framePlayAll->installEventFilter(this);
	ui->framePlayRandom->installEventFilter(this);
	ui->frameSubmenu->installEventFilter(this);
	ui->frameSort->installEventFilter(this);

}

InfoTracks::~InfoTracks()
{
	delete ui;
}

void InfoTracks::SetTitle(QString title)
{
	if (title.isEmpty())
	{
		title = tr("no title");
	}
	ui->labelTitle->setText(title);
}

void InfoTracks::SetSubtitle(QString subtitle)
{
	if (subtitle.isEmpty())
	{
		subtitle = tr("no title");
	}
	ui->labelSubtitle->setText(subtitle);
}

//void InfoTracks::SetInfo(QString info)
//{
//	if (info.isEmpty())
//	{
//		info = tr("-");
//	}
//	ui->labelInfo->setText(info);
//}

void InfoTracks::SetCoverArt(QString coverArt)
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

bool InfoTracks::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->framePlayAll)
		{
			emit SigPlayAll();
		}
		else if (object == ui->framePlayRandom)
		{
			emit SigPlayRandom();
		}
		else if (object == ui->frameSubmenu)
		{
			emit SigSubmenu();
		}
		else if (object == ui->frameSort)
		{
			emit SigSort();
		}
	}

	return QObject::eventFilter(object, event);
}
