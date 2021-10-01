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
	ui->labelTop->installEventFilter(this);
	ui->labelBottom->installEventFilter(this);
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

QString IconTracksEditor::GetTop()
{
	return ui->labelTop->text();
}

void IconTracksEditor::SetTop(const QString &value)
{
	ui->labelTop->setText(value);
}

QString IconTracksEditor::GetBottom()
{
	return ui->labelBottom->text();

}

void IconTracksEditor::SetBottom(const QString &value)
{
	ui->labelBottom->setText(value);
}

QString IconTracksEditor::GetCount()
{
	return ui->labelCount->text();

}

void IconTracksEditor::SetCount(const QString &value)
{
	if (value.isEmpty())
	{
		ui->labelCount->hide();
	}

	ui->labelCount->setText(value);
}


int IconTracksEditor::GetRating() const
{
	return ui->labelRating->text().toInt();
}

void IconTracksEditor::SetRating(const int &value)
{
	if (value < 0)
	{
		ui->labelRating->hide();
	}

	ui->labelRating->setText(QString("%1").arg(value));
}

int IconTracksEditor::GetFavorite() const
{
	return ui->labelFavorite->text().toInt();
}

void IconTracksEditor::SetFavorite(const int &value)
{
	if (value < 0)
	{
		ui->labelFavorite->hide();
	}

	ui->labelFavorite->setText(QString("%1").arg(value));
}

bool IconTracksEditor::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->labelCoverArt)
		{
			emit SigClickCoverArt(m_ID, m_CoverArt);
		}
		else if (object == ui->labelTop)
		{
			emit SigClickTop(m_ID);
		}
		else if (object == ui->labelBottom)
		{
			emit SigClickBottom(m_ID);
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
