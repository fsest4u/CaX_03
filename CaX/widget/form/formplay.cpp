#include <QMouseEvent>

#include "formplay.h"
#include "ui_formplay.h"

FormPlay::FormPlay(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FormPlay)
{
	ui->setupUi(this);

	ui->labelPlayAll->installEventFilter(this);
	ui->labelPlayRandom->installEventFilter(this);
	ui->labelFavorite->installEventFilter(this);
	ui->labelRating->installEventFilter(this);
	ui->labelSubmenu->installEventFilter(this);
}

FormPlay::~FormPlay()
{
	delete ui;
}

bool FormPlay::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->labelPlayAll)
		{
			emit SigPlayAll();
		}
		else if (object == ui->labelPlayRandom)
		{
			emit SigPlayRandom();
		}
		else if (object == ui->labelFavorite)
		{
			emit SigFavorite();
		}
		else if (object == ui->labelRating)
		{
			emit SigRating();
		}
		else if (object == ui->labelSubmenu)
		{
			emit SigSubmenu();
		}
	}

	return QObject::eventFilter(object, event);

}
