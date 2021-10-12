#include <QMouseEvent>

#include "formclassify.h"
#include "ui_formclassify.h"

FormClassify::FormClassify(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FormClassify)
{
	ui->setupUi(this);

	ui->btnClassify->installEventFilter(this);
	ui->labelFavorite->installEventFilter(this);
	ui->labelRating->installEventFilter(this);
	ui->labelArtist->installEventFilter(this);
	ui->labelGenre->installEventFilter(this);
	ui->labelComposer->installEventFilter(this);
}

FormClassify::~FormClassify()
{
	delete ui;
}

bool FormClassify::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->btnClassify)
		{
			emit SigClassify();
		}
		else if (object == ui->labelFavorite)
		{
			emit SigFavorite();
		}
		else if (object == ui->labelRating)
		{
			emit SigRating();
		}
		else if (object == ui->labelArtist)
		{
			emit SigArtist();
		}
		else if (object == ui->labelGenre)
		{
			emit SigGenre();
		}
		else if (object == ui->labelComposer)
		{
			emit SigComposer();
		}

	}

	return QObject::eventFilter(object, event);
}
