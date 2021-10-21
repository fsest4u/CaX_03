#include <QMouseEvent>

#include "listtrackseditor.h"
#include "ui_listtrackseditor.h"

#include "widget/form/formcoverart.h"

#include "util/log.h"

ListTracksEditor::ListTracksEditor(QWidget *parent) :
	QWidget(parent),
	m_pFormCoverArt(new FormCoverArt(this)),
	m_Favorite(0),
	ui(new Ui::ListTracksEditor)
{
	ui->setupUi(this);

	m_ID = -1;
//	m_CoverArt.clear();

	ConnectSigToSlot();

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

QString ListTracksEditor::GetTitle()
{
	return ui->labelTitle->text();
}

void ListTracksEditor::SetTitle(const QString &title)
{
	ui->labelTitle->setText(title);
}

int ListTracksEditor::GetFavorite() const
{
	return m_Favorite;
}

void ListTracksEditor::SetFavorite(int Favorite)
{
	ui->labelFavorite->show();
	m_Favorite = Favorite;

	QString style;

	if (m_Favorite == 0)
	{
		style = QString("QLabel	\
						{	\
						  border-image: url(\":/resource/play-btn12-like-n@3x.png\");	\
						}	\
						QLabel:hover	\
						{	\
						  border-image: url(\":/resource/play-btn12-like-h@3x.png\");	\
						}");

	}
	else
	{
		style = QString("QLabel	\
						{	\
						  border-image: url(\":/resource/play-btn12-like-u@3x.png\");	\
						}	\
						QLabel:hover	\
						{	\
						  border-image: url(\":/resource/play-btn12-like-h@3x.png\");	\
						}");

	}
	ui->labelFavorite->setStyleSheet(style);
}

QString ListTracksEditor::GetTime()
{
	return ui->labelTime->text();
}

void ListTracksEditor::SetTime(const QString &time)
{
	ui->labelTime->setText(time);
}

QString ListTracksEditor::GetArtist()
{
	return ui->labelArtist->text();
}

void ListTracksEditor::SetArtist(const QString &artist)
{
	ui->labelAlbum->setText(artist);
}

QString ListTracksEditor::GetAlbum()
{
	return ui->labelAlbum->text();
}

void ListTracksEditor::SetAlbum(const QString &album)
{
	ui->labelAlbum->setText(album);
}

QString ListTracksEditor::GetGenre()
{
	return ui->labelGenre->text();
}

void ListTracksEditor::SetGenre(const QString &genre)
{
	ui->labelGenre->setText(genre);
}

FormCoverArt *ListTracksEditor::GetFormCoverArt() const
{
	return m_pFormCoverArt;
}

bool ListTracksEditor::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{

		if (object == ui->labelPlay)
		{
			emit SigClickPlay(m_ID);
		}
		else if (object == ui->labelTitle)
		{
			emit SigClickTitle(m_ID);
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
			emit SigClickFavorite(m_ID, m_Favorite);
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

void ListTracksEditor::ConnectSigToSlot()
{
	ui->labelFavorite->hide();

	ui->labelPlay->installEventFilter(this);
	ui->labelTitle->installEventFilter(this);
	ui->labelFavorite->installEventFilter(this);
	ui->labelTime->installEventFilter(this);
	ui->labelArtist->installEventFilter(this);
	ui->labelAlbum->installEventFilter(this);
	ui->labelGenre->installEventFilter(this);
	ui->labelMore->installEventFilter(this);

	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);

}
