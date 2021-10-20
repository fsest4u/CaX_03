#include <QMouseEvent>

#include "formplay.h"
#include "ui_formplay.h"

FormPlay::FormPlay(QWidget *parent) :
	QWidget(parent),
	m_Favorite(0),
	m_Rating(0),
	ui(new Ui::FormPlay)
{
	ui->setupUi(this);

	ui->labelPlayAll->hide();
	ui->labelPlayRandom->hide();
	ui->labelFavorite->hide();
	ui->labelSubmenu->hide();
	ui->frameRating->hide();

	ui->labelPlayAll->installEventFilter(this);
	ui->labelPlayRandom->installEventFilter(this);
	ui->labelFavorite->installEventFilter(this);
	ui->labelSubmenu->installEventFilter(this);

	connect(ui->btnRating1, SIGNAL(clicked()), this, SLOT(SlotBtnRating1()));
	connect(ui->btnRating2, SIGNAL(clicked()), this, SLOT(SlotBtnRating2()));
	connect(ui->btnRating3, SIGNAL(clicked()), this, SLOT(SlotBtnRating3()));
	connect(ui->btnRating4, SIGNAL(clicked()), this, SLOT(SlotBtnRating4()));
	connect(ui->btnRating5, SIGNAL(clicked()), this, SLOT(SlotBtnRating5()));
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
	ui->frameRating->show();
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

	QString style;

	if (m_Rating == 1)
	{
		style = "mid-icon16-horizental-rankon-h1@3x.png";
	}
	else if (m_Rating == 2)
	{
		style = "mid-icon16-horizental-rankon-h2@3x.png";
	}
	else if (m_Rating == 3)
	{
		style = "mid-icon16-horizental-rankon-h3@3x.png";
	}
	else if (m_Rating == 4)
	{
		style = "mid-icon16-horizental-rankon-h4@3x.png";
	}
	else if (m_Rating == 5)
	{
		style = "mid-icon16-horizental-rankon-h5@3x.png";
	}
	else
	{
		style = "mid-icon16-horizental-rankon-h@3x.png";
	}

	style = QString("QFrame	\
					{	\
					  border-image: url(\":/resource/%1\");	\
					}	\
					QFrame:hover	\
					{	\
					  border-image: url(\":/resource/mid-icon16-horizental-rankon-h@3x.png\");	\
					}").arg(style);

	ui->frameRating->setStyleSheet(style);
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
		else if (object == ui->labelSubmenu)
		{
			emit SigSubmenu();
		}
	}

	return QObject::eventFilter(object, event);

}

void FormPlay::SlotBtnRating1()
{
	SetRating(1);
	SigRating(m_Rating);
}

void FormPlay::SlotBtnRating2()
{
	SetRating(2);
	SigRating(m_Rating);
}

void FormPlay::SlotBtnRating3()
{
	SetRating(3);
	SigRating(m_Rating);
}

void FormPlay::SlotBtnRating4()
{
	SetRating(4);
	SigRating(m_Rating);
}

void FormPlay::SlotBtnRating5()
{
	SetRating(5);
	SigRating(m_Rating);
}

