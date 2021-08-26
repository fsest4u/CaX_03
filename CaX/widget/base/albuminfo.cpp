#include <QMouseEvent>

#include "albuminfo.h"
#include "ui_albuminfo.h"

AlbumInfo::AlbumInfo(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::AlbumInfo)
{
	ui->setupUi(this);

	ui->framePlayAll->installEventFilter(this);
	ui->framePlayRandom->installEventFilter(this);
	ui->frameSubmenu->installEventFilter(this);
	ui->frameSort->installEventFilter(this);

}

AlbumInfo::~AlbumInfo()
{
	delete ui;
}

void AlbumInfo::SetTitle(const QString title)
{
	ui->labelTitle->setText(title);
}

void AlbumInfo::SetSubtitle(const QString subtitle)
{
	ui->labelSubtitle->setText(subtitle);
}

void AlbumInfo::SetInfo(const QString info)
{
	ui->labelInfo->setText(info);
}

bool AlbumInfo::eventFilter(QObject *object, QEvent *event)
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
