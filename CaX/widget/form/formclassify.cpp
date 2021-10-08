#include <QMouseEvent>

#include "formclassify.h"
#include "ui_formclassify.h"

FormClassify::FormClassify(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FormClassify)
{
	ui->setupUi(this);
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

		}
		else if (object == ui->labelFavorite)
		{

		}
		else if (object == ui->labelRating)
		{

		}
		else if (object == ui->labelArtist)
		{

		}
		else if (object == ui->labelGenre)
		{

		}
		else if (object == ui->labelComposer)
		{

		}
		else if (object == ui->labelTemp)
		{

		}
		else if (object == ui->labelTemp_2)
		{

		}
	}

	return QObject::eventFilter(object, event);
}
