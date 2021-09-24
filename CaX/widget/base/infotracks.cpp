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

void InfoTracks::SetTitle(const QString title)
{
	ui->labelTitle->setText(title);
}

void InfoTracks::SetSubtitle(const QString subtitle)
{
	ui->labelSubtitle->setText(subtitle);
}

void InfoTracks::SetInfo(const QString info)
{
	ui->labelInfo->setText(info);
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
