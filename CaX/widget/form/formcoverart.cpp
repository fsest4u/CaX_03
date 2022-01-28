#include <QMouseEvent>

#include "formcoverart.h"
#include "ui_formcoverart.h"

#include "util/log.h"

FormCoverArt::FormCoverArt(QWidget *parent) :
	QWidget(parent),
	m_CoverArt(""),
	m_Rating(-1),
	m_Favorite(-1),
	m_Count(-1),
	ui(new Ui::FormCoverArt)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();
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
	m_CoverArt = coverArt;
	if (m_CoverArt.isEmpty())
	{
		m_CoverArt = ":/resource/playlist-img160-albumart-h.png";
	}

//	QImage image;
//	if (image.load(coverArt))
//	{
////			painter->drawImage(coverRect, image);
//		QPixmap pixmap = QPixmap::fromImage(image);
//		ui->labelCoverArt->setPixmap(pixmap.scaled(ui->labelCoverArt->width()
//												   , ui->labelCoverArt->height()
//												   , Qt::KeepAspectRatio));
//	}

	QString style = QString("QFrame { border-image: url('%1'); }").arg(m_CoverArt);
	setStyleSheet(style);
}

int FormCoverArt::GetCount() const
{
	return m_Count;
}

void FormCoverArt::SetCount(int Count)
{
	if (Count > 0)
	{
		ui->labelPlay->show();
		m_Count = Count;
		ui->labelPlay->setText(QString::number(m_Count));
	}
}

int FormCoverArt::GetFavorite() const
{
	return m_Favorite;
}

void FormCoverArt::SetFavorite(int Favorite)
{
	if (Favorite >= 0)
	{
//		ui->widgetCoverLeft->show();
		ui->labelFavorite->show();
		m_Favorite = Favorite;
	}

	QString style;

	if (m_Favorite == 0)
	{
//		style = QString("QLabel	\
//						{	\
//						  border-image: url(\":/resource/mid-icon16-likeoff.png\");	\
//						}	\
//						QLabel:hover	\
//						{	\
//						  border-image: url(\":/resource/mid-icon16-likeon.png\");	\
//						}");

		style = QString("QLabel	\
						{	\
						  border-image: url(\":/resource/mid-icon16-likeoff.png\");	\
						}");
	}
	else
	{
//		style = QString("QLabel	\
//						{	\
//						  border-image: url(\":/resource/mid-icon16-likeon.png\");	\
//						}	\
//						QLabel:hover	\
//						{	\
//						  border-image: url(\":/resource/mid-icon16-likeoff.png\");	\
//						}");

		style = QString("QLabel	\
						{	\
						  border-image: url(\":/resource/mid-icon16-likeon.png\");	\
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
	if (Rating >= 0)
	{
//		ui->widgetCoverLeft->show();
		ui->frameRating->show();
		m_Rating = Rating;
	}

	QString style;

	if (m_Rating == 1)
	{
		style = "mid-icon16-rankon-h1.png";
	}
	else if (m_Rating == 2)
	{
		style = "mid-icon16-rankon-h2.png";
	}
	else if (m_Rating == 3)
	{
		style = "mid-icon16-rankon-h3.png";
	}
	else if (m_Rating == 4)
	{
		style = "mid-icon16-rankon-h4.png";
	}
	else if (m_Rating == 5)
	{
		style = "mid-icon16-rankon-h5.png";
	}
	else
	{
		style = "mid-icon16-rankon-h.png";
	}

//	style = QString("QFrame	\
//					{	\
//					  border-image: url(\":/resource/%1\");	\
//					}	\
//					QFrame:hover	\
//					{	\
//					  border-image: url(\":/resource/mid-icon16-rankon-h.png\");	\
//					}").arg(style);
	style = QString("QFrame	\
					{	\
						border-image: url(\":/resource/%1\");	\
					}").arg(style);

	ui->frameRating->setStyleSheet(style);

}

bool FormCoverArt::GetSelect() const
{
	return m_Select;
}

void FormCoverArt::SetSelect(bool Select)
{
	ui->labelCheck->show();
	m_Select = Select;

	QString style;

	if (m_Select)
	{
		style = "playlist-btn30-selecton-h.png";
	}
	else
	{
		style = "playlist-btn30-selecton-n.png";
	}

	style = QString("QLabel	\
					{	\
						border-image: url(\":/resource/%1\");	\
					}").arg(style);

	ui->labelCheck->setStyleSheet(style);

}

bool FormCoverArt::GetMute() const
{
	return m_Mute;
}

void FormCoverArt::SetMute(bool Mute)
{
	m_Mute = Mute;
	ui->labelPlay->show();

	QString style;
	if (m_Mute)
	{
		style = QString("QLabel	\
						{	\
							border-image: url(\":/resource/groupp-icon30-volume-mute.png\");	\
						}");
	}
	else
	{
		style = QString("QLabel	\
						{	\
							border-image: url(\":/resource/groupp-icon30-volume.png\");	\
						}");

	}
	ui->labelPlay->setStyleSheet(style);
}

int FormCoverArt::GetIndex() const
{
	return m_Index;
}

void FormCoverArt::SetIndex(int Index)
{
	m_Index = Index;
}

void FormCoverArt::SetMinimumSizeCheck(int size)
{
	ui->labelCheck->setMinimumWidth(size);
	ui->labelCheck->setMinimumHeight(size);
}

bool FormCoverArt::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->labelCheck)
		{
			emit SigCheck(m_Index);
			return true;
		}
		else if (object == ui->labelPlay)
		{
			emit SigPlay();
			return true;
		}
//		else if (object == ui->labelFavorite)
//		{
//			emit SigFavorite(m_Favorite);
//			return true;
//		}
		else if (object == ui->frameCoverArt)
		{
			if (((QMouseEvent*)event)->button() == Qt::LeftButton)
			{
				emit SigCoverArt(m_Index);
				return true;
			}

			return true;
		}

	}

	return QObject::eventFilter(object, event);;

}

void FormCoverArt::SlotBtnRating1()
{
//	SetRating(1);
//	emit SigRating(m_Rating);
}

void FormCoverArt::SlotBtnRating2()
{
//	SetRating(2);
//	emit SigRating(m_Rating);
}

void FormCoverArt::SlotBtnRating3()
{
//	SetRating(3);
//	emit SigRating(m_Rating);
}

void FormCoverArt::SlotBtnRating4()
{
//	SetRating(4);
//	emit SigRating(m_Rating);
}

void FormCoverArt::SlotBtnRating5()
{
//	SetRating(5);
//	emit SigRating(m_Rating);
}

void FormCoverArt::ConnectSigToSlot()
{
//	connect(ui->btnRating1, SIGNAL(clicked()), this, SLOT(SlotBtnRating1()));
//	connect(ui->btnRating2, SIGNAL(clicked()), this, SLOT(SlotBtnRating2()));
//	connect(ui->btnRating3, SIGNAL(clicked()), this, SLOT(SlotBtnRating3()));
//	connect(ui->btnRating4, SIGNAL(clicked()), this, SLOT(SlotBtnRating4()));
//	connect(ui->btnRating5, SIGNAL(clicked()), this, SLOT(SlotBtnRating5()));
}

void FormCoverArt::Initialize()
{
//	ui->widgetCoverLeft->hide();
	ui->frameRating->hide();
	ui->labelFavorite->hide();
	ui->labelCheck->hide();
	ui->labelPlay->hide();

//	ui->frameRating->installEventFilter(this);
//	ui->labelFavorite->installEventFilter(this);
	ui->labelCheck->installEventFilter(this);
	ui->labelPlay->installEventFilter(this);
	ui->frameCoverArt->installEventFilter(this);

	ui->labelCheck->setMinimumWidth(30);
	ui->labelCheck->setMinimumHeight(30);

}
