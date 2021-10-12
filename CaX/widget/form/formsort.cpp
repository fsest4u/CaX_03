#include <QMouseEvent>

#include "formsort.h"
#include "ui_formsort.h"

FormSort::FormSort(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FormSort)
{
	ui->setupUi(this);

	ui->labelSort->installEventFilter(this);
	ui->labelIncDec->installEventFilter(this);
	ui->labelResize->installEventFilter(this);
}

FormSort::~FormSort()
{
	delete ui;
}

bool FormSort::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->labelSort)
		{
			emit SigSort();
		}
		else if (object == ui->labelIncDec)
		{
			emit SigIncDec();
		}
		else if (object == ui->labelResize)
		{
			emit SigResize();
		}
	}

	return QObject::eventFilter(object, event);
}
