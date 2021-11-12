#include <QMouseEvent>

#include "formsearchcoverart.h"
#include "ui_formsearchcoverart.h"

FormSearchCoverArt::FormSearchCoverArt(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FormSearchCoverArt)
{
	ui->setupUi(this);

	ui->frameCoverArt->installEventFilter(this);

}

FormSearchCoverArt::~FormSearchCoverArt()
{
	delete ui;
}


//QByteArray FormSearchCoverArt::GetCoverArt() const
//{
//	return m_CoverArt;
//}

//void FormSearchCoverArt::SetCoverArt(const QByteArray &CoverArt)
//{
//	m_CoverArt = CoverArt;
//	if (m_CoverArt.isEmpty())
//	{
////		m_CoverArt = ":/resource/playlist-img160-albumart-h@3x.png";
//	}

////	QImage image;
////	if (image.loadFromData(m_CoverArt))
////	{
//////		painter->drawImage(coverRect, image);
////		QPixmap pixmap = QPixmap::fromImage(image);
////		ui->labelCoverArt->setPixmap(pixmap.scaled(ui->labelCoverArt->width()
////												   , ui->labelCoverArt->height()
////												   , Qt::KeepAspectRatio));
////	}
//	QPixmap *pixmap = new QPixmap();
//	pixmap->loadFromData(m_CoverArt);
//	ui->labelCoverArt->setPixmap(pixmap->scaled(ui->labelCoverArt->width()
//												, ui->labelCoverArt->height()
//												, Qt::KeepAspectRatio));
//}

QString FormSearchCoverArt::GetCoverArt() const
{
	return m_CoverArt;
}

void FormSearchCoverArt::SetCoverArt(const QString &CoverArt)
{
	m_CoverArt = CoverArt;
	if (m_CoverArt.isEmpty())
	{
		m_CoverArt = ":/resource/playlist-img160-albumart-h@3x.png";
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

bool FormSearchCoverArt::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->frameCoverArt)
		{
			emit SigCoverArt();
		}

	}

	return QObject::eventFilter(object, event);

}
