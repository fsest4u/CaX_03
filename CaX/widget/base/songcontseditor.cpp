#include <QMouseEvent>

#include "songcontseditor.h"
#include "ui_songcontseditor.h"

SongContsEditor::SongContsEditor(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SongContsEditor)
{
	ui->setupUi(this);

	m_ID = -1;
	m_CoverArt.clear();

	ui->labelCoverArt->installEventFilter(this);
	ui->labelPlay->installEventFilter(this);
	ui->labelTitle->installEventFilter(this);
	ui->labelFavorite->installEventFilter(this);
	ui->labelTime->installEventFilter(this);
	ui->labelArtist->installEventFilter(this);
	ui->labelAlbum->installEventFilter(this);
	ui->labelGenre->installEventFilter(this);
	ui->labelMore->installEventFilter(this);

}

SongContsEditor::~SongContsEditor()
{
	delete ui;
}

int SongContsEditor::GetID() const
{
	return m_ID;
}

void SongContsEditor::SetID(int ID)
{
	m_ID = ID;
}

QString SongContsEditor::GetCoverArt() const
{
	return m_CoverArt;
}

void SongContsEditor::SetCoverArt(const QString &CoverArt)
{
	m_CoverArt = CoverArt;

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

QString SongContsEditor::GetTitle()
{
	return ui->labelTitle->text();
}

void SongContsEditor::SetTitle(const QString &value)
{
	ui->labelTitle->setText(value);
}

QString SongContsEditor::GetTime()
{
	return ui->labelTime->text();
}

void SongContsEditor::SetTime(const QString &value)
{
	ui->labelTime->setText(value);
}

QString SongContsEditor::GetArtist()
{
	return ui->labelArtist->text();
}

void SongContsEditor::SetArtist(const QString &value)
{
	ui->labelAlbum->setText(value);
}

QString SongContsEditor::GetAlbum()
{
	return ui->labelAlbum->text();
}

void SongContsEditor::SetAlbum(const QString &value)
{
	ui->labelAlbum->setText(value);
}

QString SongContsEditor::GetGenre()
{
	return ui->labelGenre->text();
}

void SongContsEditor::SetGenre(const QString &value)
{
	ui->labelGenre->setText(value);
}

bool SongContsEditor::eventFilter(QObject *object, QEvent *event)
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
		else if (object == ui->labelFavorite)
		{
			emit SigClickFavorite(m_ID);
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
