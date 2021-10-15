#include <QMouseEvent>

#include "formsort.h"
#include "ui_formsort.h"

FormSort::FormSort(QWidget *parent) :
	QWidget(parent),
	m_bIncrease(false),
	ui(new Ui::FormSort)
{
	ui->setupUi(this);

	ui->labelSort->hide();
	ui->labelIncDec->hide();
	ui->labelResize->hide();

	ui->labelSort->installEventFilter(this);
	ui->labelIncDec->installEventFilter(this);
	ui->labelResize->installEventFilter(this);

	SetIncrease(m_bIncrease);
}

FormSort::~FormSort()
{
	delete ui;
}

void FormSort::ShowSort()
{
	ui->labelSort->show();

}

void FormSort::ShowIncDec()
{
	ui->labelIncDec->show();

}

void FormSort::ShowResize()
{
	ui->labelResize->show();

}

bool FormSort::GetIncrease() const
{
	return m_bIncrease;
}

void FormSort::SetIncrease(bool bIncrease)
{
	m_bIncrease = bIncrease;

	QString style;

	if (bIncrease)
	{
		style = QString("QLabel	\
								{	\
								  border-image: url(\":/resource/top-btn22-sortu-n@3x.png\");	\
								}	\
								QLabel:hover	\
								{	\
								  border-image: url(\":/resource/top-btn22-sortu-h@3x.png\");	\
								}");

	}
	else
	{
		style = QString("QLabel	\
								{	\
								  border-image: url(\":/resource/top-btn22-sortu-h@3x.png\");	\
								}	\
								QLabel:hover	\
								{	\
								  border-image: url(\":/resource/top-btn22-sortu-n@3x.png\");	\
								}");

	}
	ui->labelIncDec->setStyleSheet(style);
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
			bool bIncrease = !m_bIncrease;
			SetIncrease(bIncrease);
			emit SigIncDec(bIncrease);
		}
		else if (object == ui->labelResize)
		{
			emit SigResize();
		}
	}

	return QObject::eventFilter(object, event);
}
