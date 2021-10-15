#include <QMouseEvent>

#include "formcoverart.h"
#include "ui_formcoverart.h"

FormCoverArt::FormCoverArt(QWidget *parent) :
	QWidget(parent),
	m_CoverArt(""),
	m_Rating(-1),
	m_Favorite(-1),
	m_Count(-1),
	ui(new Ui::FormCoverArt)
{
	ui->setupUi(this);

	ui->labelCheck->hide();
	ui->labelRating->hide();
	ui->labelFavorite->hide();
	ui->labelPlay->hide();

	ui->labelRating->installEventFilter(this);
	ui->labelFavorite->installEventFilter(this);
	ui->labelPlay->installEventFilter(this);
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
}

int FormCoverArt::GetRating() const
{
	return m_Rating;
}

void FormCoverArt::SetRating(int Rating)
{
	ui->labelRating->show();
	m_Rating = Rating;
}

bool FormCoverArt::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->labelRating)
		{
			emit SigRating();
		}
		else if (object == ui->labelFavorite)
		{
			emit SigFavorite();
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
