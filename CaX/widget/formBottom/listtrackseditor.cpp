#include <QMouseEvent>

#include "listtrackseditor.h"
#include "ui_listtrackseditor.h"

#include "util/caxtranslate.h"
#include "util/log.h"
#include "util/utilnovatron.h"

#include "widget/form/formcoverart.h"

ListTracksEditor::ListTracksEditor(QWidget *parent) :
	QWidget(parent),
	m_pFormCoverArt(new FormCoverArt(this)),
	m_Menu(new QMenu(this)),
	m_Favorite(0),
	ui(new Ui::ListTracksEditor)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();

}

ListTracksEditor::~ListTracksEditor()
{
	if (m_pFormCoverArt)
	{
		delete m_pFormCoverArt;
		m_pFormCoverArt = nullptr;
	}

	disconnect(m_Menu, SIGNAL(triggered(QAction*)));
	if (m_Menu)
	{
		delete m_Menu;
		m_Menu = nullptr;
	}

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
	ui->labelArtist->setText(artist);
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

void ListTracksEditor::ClearMenu()
{
	m_Menu->clear();
}

void ListTracksEditor::SetMenu(QMap<int, QString> map)
{
	QMap<int, QString>::iterator i;
	for (i = map.begin(); i != map.end(); i++)
	{
		QIcon icon = UtilNovatron::GetMenuIcon(i.value());
		QAction *action = new QAction(icon, i.value(), this);
		action->setData(i.key());
		m_Menu->addAction(action);
	}
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
			emit SigClickTitle(m_ID, m_pFormCoverArt->GetCoverArt());
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

	}

	return QObject::eventFilter(object, event);
}

void ListTracksEditor::SlotMenuAction(QAction *action)
{
	emit SigMenuAction(m_ID, action->data().toInt());
}

void ListTracksEditor::ConnectSigToSlot()
{
	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuAction(QAction*)));
}

void ListTracksEditor::Initialize()
{
	QString style = QString("QMenu::icon {	\
								padding: 0px 0px 0px 20px;	\
							}	\
							QMenu::item {	\
								width: 260px;	\
								height: 40px;	\
								color: rgb(90, 91, 94);	\
								font-size: 14pt;	\
								padding: 0px 20px 0px 20px;	\
							}	\
							QMenu::item:selected {	\
								background: rgba(201,237,248,255);	\
							}");

	m_Menu->setStyleSheet(style);
	ui->btnMenu->setMenu(m_Menu);

	ui->labelFavorite->hide();

	ui->labelPlay->installEventFilter(this);
	ui->labelTitle->installEventFilter(this);
	ui->labelFavorite->installEventFilter(this);
	ui->labelTime->installEventFilter(this);
	ui->labelArtist->installEventFilter(this);
	ui->labelAlbum->installEventFilter(this);
	ui->labelGenre->installEventFilter(this);

	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);

	m_ID = -1;
//	m_CoverArt.clear();
}
