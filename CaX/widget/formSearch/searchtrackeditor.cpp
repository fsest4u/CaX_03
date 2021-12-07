#include <QMouseEvent>

#include "searchtrackeditor.h"
#include "ui_searchtrackeditor.h"

#include "util/caxtranslate.h"
#include "util/log.h"
#include "util/utilnovatron.h"

#include "widget/form/formcoverart.h"

SearchTrackEditor::SearchTrackEditor(QWidget *parent) :
	QWidget(parent),
	m_pFormCoverArt(new FormCoverArt(this)),
	m_Menu(new QMenu(this)),
	ui(new Ui::SearchTrackEditor)
{
	ui->setupUi(this);

	m_ID = -1;
	m_AlbumID = -1;

	ConnectSigToSlot();

	Initialize();
}

SearchTrackEditor::~SearchTrackEditor()
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

int SearchTrackEditor::GetID() const
{
	return m_ID;
}

void SearchTrackEditor::SetID(int ID)
{
	m_ID = ID;
}

QString SearchTrackEditor::GetTitle() const
{
	return ui->labelTitle->text();
}

void SearchTrackEditor::SetTitle(const QString &title)
{
	ui->labelTitle->setText(title);
}

QString SearchTrackEditor::GetTime() const
{
	return ui->labelTime->text();
}

void SearchTrackEditor::SetTime(const QString &time)
{
	ui->labelTime->setText(time);
}

QString SearchTrackEditor::GetArtist() const
{
	return ui->labelArtist->text();
}

void SearchTrackEditor::SetArtist(const QString &artist)
{
	ui->labelArtist->setText(artist);
}

int SearchTrackEditor::GetAlbumID() const
{
	return m_AlbumID;
}

void SearchTrackEditor::SetAlbumID(const int id)
{
	m_AlbumID = id;
}

QString SearchTrackEditor::GetAlbum() const
{
	return ui->labelAlbum->text();
}

void SearchTrackEditor::SetAlbum(const QString &album)
{
	ui->labelAlbum->setText(album);
}

QString SearchTrackEditor::GetGenre() const
{
	return ui->labelGenre->text();
}

void SearchTrackEditor::SetGenre(const QString &genre)
{
	ui->labelGenre->setText(genre);
}

void SearchTrackEditor::ClearMenu()
{
	m_Menu->clear();
}

void SearchTrackEditor::SetMenu(QMap<int, QString> map)
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

FormCoverArt *SearchTrackEditor::GetFormCoverArt() const
{
	return m_pFormCoverArt;
}

bool SearchTrackEditor::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{

		if (object == ui->labelPlay)
		{
			emit SigClickPlay(m_ID);
		}

	}

	return QObject::eventFilter(object, event);
}

void SearchTrackEditor::SlotMenuAction(QAction *action)
{
	emit SigMenuAction(m_AlbumID, action->data().toInt(), m_pFormCoverArt->GetCoverArt());
}

void SearchTrackEditor::ConnectSigToSlot()
{
	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuAction(QAction*)));
}

void SearchTrackEditor::Initialize()
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

	ui->labelPlay->installEventFilter(this);

	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);

}
