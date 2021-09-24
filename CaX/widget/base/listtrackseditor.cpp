#include <QMouseEvent>

#include "listtrackseditor.h"
#include "ui_listtrackseditor.h"

#include "util/log.h"

ListTracksEditor::ListTracksEditor(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ListTracksEditor)
{
	ui->setupUi(this);

	m_ID = -1;
	m_CoverArt.clear();

	ui->labelCoverArt->installEventFilter(this);
	ui->labelPlay->installEventFilter(this);
	ui->labelTitle->installEventFilter(this);
	ui->labelTime->installEventFilter(this);
	ui->labelArtist->installEventFilter(this);
	ui->labelAlbum->installEventFilter(this);
	ui->labelGenre->installEventFilter(this);
	ui->labelMore->installEventFilter(this);

}

ListTracksEditor::~ListTracksEditor()
{
	delete ui;
}

int ListTracksEditor::GetID() const
{
	return m_ID;
}

void ListTracksEditor::SetID(int ID)
{
	m_ID = ID;
}

QString ListTracksEditor::GetCoverArt() const
{
	return m_CoverArt;
}

void ListTracksEditor::SetCoverArt(const QString &value)
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

QString ListTracksEditor::GetTitle()
{
	return ui->labelTitle->text();
}

void ListTracksEditor::SetTitle(const QString &value)
{
	ui->labelTitle->setText(value);
}

QString ListTracksEditor::GetTime()
{
	return ui->labelTime->text();
}

void ListTracksEditor::SetTime(const QString &value)
{
	ui->labelTime->setText(value);
}

QString ListTracksEditor::GetArtist()
{
	return ui->labelArtist->text();
}

void ListTracksEditor::SetArtist(const QString &value)
{
	ui->labelAlbum->setText(value);
}

QString ListTracksEditor::GetAlbum()
{
	return ui->labelAlbum->text();
}

void ListTracksEditor::SetAlbum(const QString &value)
{
	ui->labelAlbum->setText(value);
}

QString ListTracksEditor::GetGenre()
{
	return ui->labelGenre->text();
}

void ListTracksEditor::SetGenre(const QString &value)
{
	ui->labelGenre->setText(value);
}

bool ListTracksEditor::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->labelCoverArt)
		{
			emit SigClickCoverArt(m_ID);
		}
		else if (object == ui->labelPlay)
		{
			emit SigClickPlay(m_ID);
		}
		else if (object == ui->labelTitle)
		{
			emit SigClickTitle(m_ID);
		}
		else if (object == ui->labelTime)
		{
			emit SigClickTime(m_ID);
		}
		else if (object == ui->labelArtist)
		{
			emit SigClickArtist(m_ID);
		}
		else if (object == ui->labelAlbum)
		{
			emit SigClickAlbum(m_ID);
		}
		else if (object == ui->labelGenre)
		{
			emit SigClickGenre(m_ID);
		}
		else if (object == ui->labelMore)
		{
			emit SigClickMore(m_ID);
		}
	}

	return QObject::eventFilter(object, event);
}
