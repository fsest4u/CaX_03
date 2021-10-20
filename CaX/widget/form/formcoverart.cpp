#include <QMouseEvent>

#include "formcoverart.h"
#include "ui_formcoverart.h"

#include "util/log.h"

FormCoverArt::FormCoverArt(QWidget *parent) :
	QWidget(parent),
	m_CoverArt(""),
	m_Rating(0),
	m_Favorite(0),
	m_Count(-1),
	ui(new Ui::FormCoverArt)
{
	ui->setupUi(this);

	ui->frameRating->hide();
	ui->labelCheck->hide();
	ui->labelFavorite->hide();
	ui->labelPlay->hide();

	ui->labelFavorite->installEventFilter(this);
	ui->labelPlay->installEventFilter(this);

	connect(ui->btnRating1, SIGNAL(clicked()), this, SLOT(SlotBtnRating1()));
	connect(ui->btnRating2, SIGNAL(clicked()), this, SLOT(SlotBtnRating2()));
	connect(ui->btnRating3, SIGNAL(clicked()), this, SLOT(SlotBtnRating3()));
	connect(ui->btnRating4, SIGNAL(clicked()), this, SLOT(SlotBtnRating4()));
	connect(ui->btnRating5, SIGNAL(clicked()), this, SLOT(SlotBtnRating5()));
}

FormCoverArt::~FormCoverArt()
{
	delete ui;
}

QString FormCoverArt::GetCoverArt()
{
	return m_CoverArt;
}

void FormCoverArt::SetCoverArt(QString coverArt)
{
	if (coverArt.isEmpty())
	{
		coverArt = ":/resource/playlist-img160-albumart-h@3x.png";
	}

	m_CoverArt = coverArt;

//	QImage image;
//	if (image.load(coverArt))
//	{
////			painter->drawImage(coverRect, image);
//		QPixmap pixmap = QPixmap::fromImage(image);
//		ui->labelCoverArt->setPixmap(pixmap.scaled(ui->labelCoverArt->width()
//												   , ui->labelCoverArt->height()
//												   , Qt::KeepAspectRatio));
//	}

	QString style = QString("QFrame { border-image: url('%1'); }").arg(coverArt);
	setStyleSheet(style);
}

int FormCoverArt::GetCount() const
{
	return m_Count;
}

void FormCoverArt::SetCount(int Count)
{
	ui->labelPlay->show();
	m_Count = Count;
	ui->labelPlay->setText(QString("%1").arg(m_Count));
}

int FormCoverArt::GetFavorite() const
{
	return m_Favorite;
}

void FormCoverArt::SetFavorite(int Favorite)
{
	ui->labelFavorite->show();
	m_Favorite = Favorite;

	QString style;

	if (m_Favorite == 0)
	{
		style = QString("QLabel	\
								{	\
								  border-image: url(\":/resource/mid-icon16-likeoff@3x.png\");	\
								}	\
								QLabel:hover	\
								{	\
								  border-image: url(\":/resource/mid-icon16-likeon@3x.png\");	\
								}");

	}
	else
	{
		style = QString("QLabel	\
								{	\
								  border-image: url(\":/resource/mid-icon16-likeon@3x.png\");	\
								}	\
								QLabel:hover	\
								{	\
								  border-image: url(\":/resource/mid-icon16-likeoff@3x.png\");	\
								}");

	}
	ui->labelFavorite->setStyleSheet(style);
}

int FormCoverArt::GetRating() const
{
	return m_Rating;
}

void FormCoverArt::SetRating(int Rating)
{
	ui->frameRating->show();
	m_Rating = Rating;

	QString style;

	if (m_Rating == 1)
	{
		style = "mid-icon16-rankon-h1@3x.png";
	}
	else if (m_Rating == 2)
	{
		style = "mid-icon16-rankon-h2@3x.png";
	}
	else if (m_Rating == 3)
	{
		style = "mid-icon16-rankon-h3@3x.png";
	}
	else if (m_Rating == 4)
	{
		style = "mid-icon16-rankon-h4@3x.png";
	}
	else if (m_Rating == 5)
	{
		style = "mid-icon16-rankon-h5@3x.png";
	}
	else
	{
		style = "mid-icon16-rankon-h@3x.png";
	}

	style = QString("QFrame	\
					{	\
					  border-image: url(\":/resource/%1\");	\
					}	\
					QFrame:hover	\
					{	\
					  border-image: url(\":/resource/mid-icon16-rankon-h@3x.png\");	\
					}").arg(style);

	ui->frameRating->setStyleSheet(style);

}

bool FormCoverArt::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->labelFavorite)
		{
			emit SigFavorite(m_Favorite);
		}
		else if (object == ui->labelPlay)
		{
			emit SigPlay();
		}
	}
	else if (event->type() == QMouseEvent::MouseButtonDblClick)
	{
		if (object == ui->frameCoverArt)
		{
			emit SigCoverArt();
		}
	}

	return QObject::eventFilter(object, event);

}

void FormCoverArt::SlotBtnRating1()
{
	SetRating(1);
	SigRating(m_Rating);
}

void FormCoverArt::SlotBtnRating2()
{
	SetRating(2);
	SigRating(m_Rating);
}

void FormCoverArt::SlotBtnRating3()
{
	SetRating(3);
	SigRating(m_Rating);
}

void FormCoverArt::SlotBtnRating4()
{
	SetRating(4);
	SigRating(m_Rating);
}

void FormCoverArt::SlotBtnRating5()
{
	SetRating(5);
	SigRating(m_Rating);
}
