#include <QMouseEvent>

#include "formplay.h"
#include "ui_formplay.h"

FormPlay::FormPlay(QWidget *parent) :
	QWidget(parent),
	m_TopMenu(new QMenu(this)),
	m_Favorite(0),
	m_Rating(0),
	ui(new Ui::FormPlay)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();

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

void FormPlay::ShowPlayFavorite()
{
	ui->labelFavorite->show();
}

void FormPlay::ShowPlayRating()
{
	ui->frameRating->show();
}

void FormPlay::ShowPlayTopMenu()
{
	ui->btnTopMenu->show();
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
		style = "play-btn28-rank-sel1@3x.png";
	}
	else if (m_Rating == 2)
	{
		style = "play-btn28-rank-sel2@3x.png";
	}
	else if (m_Rating == 3)
	{
		style = "play-btn28-rank-sel3@3x.png";
	}
	else if (m_Rating == 4)
	{
		style = "play-btn28-rank-sel4@3x.png";
	}
	else if (m_Rating == 5)
	{
		style = "play-btn28-rank-sel5@3x.png";
	}
	else
	{
		style = "play-btn28-rank-sel0@3x.png";
	}

	style = QString("QFrame	\
					{	\
					  border-image: url(\":/resource/%1\");	\
					}	\
					QFrame:hover	\
					{	\
					  border-image: url(\":/resource/play-btn28-rank-sel0@3x.png\");	\
					}").arg(style);

					ui->frameRating->setStyleSheet(style);
}

void FormPlay::ClearTopMenu()
{
	disconnect(m_TopMenu, SIGNAL(triggered(QAction*)));
	m_TopMenu->clear();
}

void FormPlay::SetTopMenu(QMap<int, QString> map)
{
	QMap<int, QString>::iterator i;
	for (i = map.begin(); i != map.end(); i++)
	{
		QAction *action = new QAction(i.value(), this);
		action->setData(i.key());
		m_TopMenu->addAction(action);
	}
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
			emit SigPlayFavorite(m_Favorite);
		}

	}

	return QObject::eventFilter(object, event);

}

void FormPlay::SlotBtnRating1()
{
	SetRating(1);
	emit SigPlayRating(m_Rating);
}

void FormPlay::SlotBtnRating2()
{
	SetRating(2);
	emit SigPlayRating(m_Rating);
}

void FormPlay::SlotBtnRating3()
{
	SetRating(3);
	emit SigPlayRating(m_Rating);
}

void FormPlay::SlotBtnRating4()
{
	SetRating(4);
	emit SigPlayRating(m_Rating);
}

void FormPlay::SlotBtnRating5()
{
	SetRating(5);
	emit SigPlayRating(m_Rating);
}

void FormPlay::SlotBtnTopMenu()
{
	emit SigPlayTopMenu();
}

void FormPlay::SlotTopMenuAction(QAction *action)
{
	emit SigTopMenuAction(action->data().toInt());
}

void FormPlay::ConnectSigToSlot()
{
	connect(ui->btnRating1, SIGNAL(clicked()), this, SLOT(SlotBtnRating1()));
	connect(ui->btnRating2, SIGNAL(clicked()), this, SLOT(SlotBtnRating2()));
	connect(ui->btnRating3, SIGNAL(clicked()), this, SLOT(SlotBtnRating3()));
	connect(ui->btnRating4, SIGNAL(clicked()), this, SLOT(SlotBtnRating4()));
	connect(ui->btnRating5, SIGNAL(clicked()), this, SLOT(SlotBtnRating5()));

	connect(ui->btnTopMenu, SIGNAL(pressed()), this, SLOT(SlotBtnTopMenu()));

}

void FormPlay::Initialize()
{
	ui->btnTopMenu->setMenu(m_TopMenu);

	ui->labelPlayAll->hide();
	ui->labelPlayRandom->hide();
	ui->labelFavorite->hide();
	ui->frameRating->hide();
	ui->btnTopMenu->hide();

	ui->labelPlayAll->installEventFilter(this);
	ui->labelPlayRandom->installEventFilter(this);
	ui->labelFavorite->installEventFilter(this);

	connect(m_TopMenu, SIGNAL(triggered(QAction*)), this, SLOT(SlotTopMenuAction(QAction*)));
}

