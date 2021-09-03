#include <QMouseEvent>

#include "menuinfo.h"
#include "ui_menuinfo.h"

MenuInfo::MenuInfo(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MenuInfo)
{
	ui->setupUi(this);

	ui->framePlayAll->installEventFilter(this);
	ui->framePlayRandom->installEventFilter(this);
	ui->frameSubmenu->installEventFilter(this);
	ui->frameSort->installEventFilter(this);
}

MenuInfo::~MenuInfo()
{
	delete ui;
}

void MenuInfo::SetTitle(const QString title)
{
	ui->labelTitle->setText(title);
}

bool MenuInfo::eventFilter(QObject *object, QEvent *event)
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
