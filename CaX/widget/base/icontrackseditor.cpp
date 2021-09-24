#include <QMouseEvent>

#include "icontrackseditor.h"
#include "ui_icontrackseditor.h"

#include "util/log.h"

IconTracksEditor::IconTracksEditor(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::IconTracksEditor)
{
	ui->setupUi(this);

	m_ID = -1;
	m_CoverArt.clear();

	ui->labelCoverArt->installEventFilter(this);
	ui->labelTitle->installEventFilter(this);
	ui->labelSubTitle->installEventFilter(this);
	ui->labelCount->installEventFilter(this);
	ui->labelFavorite->installEventFilter(this);
	ui->labelRating->installEventFilter(this);

}

IconTracksEditor::~IconTracksEditor()
{
	delete ui;
}

int IconTracksEditor::GetID()
{
	return m_ID;
}

void IconTracksEditor::SetID(int nID)
{
	m_ID = nID;
}

QString IconTracksEditor::GetCoverArt()
{
	return m_CoverArt;
}

void IconTracksEditor::SetCoverArt(const QString &value)
{
	m_CoverArt = value;

	bool bFoundImage = false;
	if (QFile::exists(m_CoverArt))
	{
		QImage image;
		if (image.load(m_CoverArt))
		{
//			painter->drawImage(coverRect, image);
			QPixmap pixmap = QPixmap::fromImage(image);
			ui->labelCoverArt->setPixmap(pixmap.scaled(ui->labelCoverArt->width()
													   , ui->labelCoverArt->height()
													   , Qt::KeepAspectRatio));
			bFoundImage = true;
		}
	}

	if (!bFoundImage)
	{
//		painter->drawImage(coverRect, m_Image);
	}
}

QString IconTracksEditor::GetTitle()
{
	return ui->labelTitle->text();
}

void IconTracksEditor::SetTitle(const QString &value)
{
	ui->labelTitle->setText(value);
}

QString IconTracksEditor::GetSubtitle()
{
	return ui->labelSubTitle->text();

}

void IconTracksEditor::SetSubtitle(const QString &value)
{
	ui->labelSubTitle->setText(value);
}

QString IconTracksEditor::GetCount()
{
	return ui->labelCount->text();

}

void IconTracksEditor::SetCount(const QString &value)
{
	ui->labelCount->setText(value);
}


int IconTracksEditor::GetRating() const
{
	return ui->labelRating->text().toInt();
}

void IconTracksEditor::SetRating(const int &value)
{
	ui->labelRating->setText(QString("%1").arg(value));
}

int IconTracksEditor::GetFavorite() const
{
	return ui->labelFavorite->text().toInt();
}

void IconTracksEditor::SetFavorite(const int &value)
{
	ui->labelFavorite->setText(QString("%1").arg(value));
}

bool IconTracksEditor::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->labelCoverArt)
		{
			emit SigClickCoverArt(m_ID);
		}
		else if (object == ui->labelTitle)
		{
			emit SigClickTitle(m_ID);
		}
		else if (object == ui->labelSubTitle)
		{
			emit SigClickSubtitle(m_ID);
		}
		else if (object == ui->labelCount)
		{
			emit SigClickCount(m_ID);
		}
		else if (object == ui->labelFavorite)
		{
			emit SigClickFavorite(m_ID, 1);
		}
		else if (object == ui->labelRating)
		{
			emit SigClickRating(m_ID, 5);
		}
	}
	return QObject::eventFilter(object, event);
}
