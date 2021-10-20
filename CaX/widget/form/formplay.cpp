#include <QMouseEvent>

#include "formplay.h"
#include "ui_formplay.h"

FormPlay::FormPlay(QWidget *parent) :
	QWidget(parent),
	m_Favorite(0),
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

int FormPlay::GetRating() const
{
	return m_Rating;
}

void FormPlay::SetRating(int Rating)
{
	m_Rating = Rating;
}

int FormPlay::GetFavorite() const
{
	return m_Favorite;
}

void FormPlay::SetFavorite(int Favorite)
{
	m_Favorite = Favorite;

	QString style;

	if (m_Favorite == 0)
	{
		style = QString("QLabel	\
								{	\
								  border-image: url(\":/resource/play-btn28-like-n@3x.png\");	\
								}	\
								QLabel:hover	\
								{	\
								  border-image: url(\":/resource/play-btn28-like-h@3x.png\");	\
								}");

	}
	else
	{
		style = QString("QLabel	\
								{	\
								  border-image: url(\":/resource/play-btn28-like-h@3x.png\");	\
								}	\
								QLabel:hover	\
								{	\
								  border-image: url(\":/resource/play-btn28-like-n@3x.png\");	\
								}");

	}
	ui->labelFavorite->setStyleSheet(style);
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
			if (m_Favorite == 0)
			{
				SetFavorite(1);
			}
			else
			{
				SetFavorite(0);
			}
			emit SigFavorite(m_Favorite);
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

