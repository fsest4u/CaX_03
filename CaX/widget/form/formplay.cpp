#include <QMouseEvent>

#include "formplay.h"
#include "ui_formplay.h"

FormPlay::FormPlay(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FormPlay)
{
	ui->setupUi(this);

	ui->labelPlayAll->hide();
	ui->labelPlayRandom->hide();
	ui->labelFavorite->hide();
	ui->labelRating->hide();
	ui->labelSubmenu->hide();

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

void FormPlay::ShowPlayAll()
{
	ui->labelPlayAll->show();
}

void FormPlay::ShowPlayRandom()
{
	ui->labelPlayRandom->show();
}

void FormPlay::ShowFavorite()
{
	ui->labelFavorite->show();
}

void FormPlay::ShowRating()
{
	ui->labelRating->show();
}

void FormPlay::ShowSubmenu()
{
	ui->labelSubmenu->show();
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
