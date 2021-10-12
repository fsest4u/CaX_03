#include <QMouseEvent>

#include "listtrackseditor.h"
#include "ui_listtrackseditor.h"

#include "widget/form/formcoverart.h"

#include "util/log.h"

ListTracksEditor::ListTracksEditor(QWidget *parent) :
	QWidget(parent),
	m_pFormCoverArt(new FormCoverArt(this)),
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

QString ListTracksEditor::GetCoverArt() const
{
//	return m_CoverArt;
	return m_pFormCoverArt->GetCoverArt();
}

void ListTracksEditor::SetCoverArt(const QString &coverArt)
{
//	m_CoverArt = value;

//	if (m_CoverArt.isEmpty())
//	{
//		m_CoverArt = ":/resource/Icon-playbar-volume-160.png";
//	}

//	QImage image;
//	if (image.load(m_CoverArt))
//	{
////		painter->drawImage(coverRect, image);
//		QPixmap pixmap = QPixmap::fromImage(image);
//		ui->labelCoverArt->setPixmap(pixmap.scaled(ui->labelCoverArt->width()
//												   , ui->labelCoverArt->height()
//												   , Qt::KeepAspectRatio));
//	}

	m_pFormCoverArt->SetCoverArt(coverArt);
}

QString ListTracksEditor::GetTitle()
{
	return ui->labelTitle->text();
}

void ListTracksEditor::SetTitle(const QString &title)
{
	ui->labelTitle->setText(title);
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

bool ListTracksEditor::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
//		if (object == ui->labelCoverArt)
//		{
//			emit SigClickCoverArt(m_ID);
//		}
//		else
		if (object == ui->labelPlay)
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

void ListTracksEditor::SlotCoverArt()
{
	emit SigClickCoverArt(m_ID);
}

void ListTracksEditor::ConnectSigToSlot()
{
//	ui->labelCoverArt->installEventFilter(this);
	ui->labelPlay->installEventFilter(this);
	ui->labelTitle->installEventFilter(this);
	ui->labelTime->installEventFilter(this);
	ui->labelArtist->installEventFilter(this);
	ui->labelAlbum->installEventFilter(this);
	ui->labelGenre->installEventFilter(this);
	ui->labelMore->installEventFilter(this);

	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);

	connect(m_pFormCoverArt, SIGNAL(SigCoverArt()), this, SLOT(SlotCoverArt()));
}
