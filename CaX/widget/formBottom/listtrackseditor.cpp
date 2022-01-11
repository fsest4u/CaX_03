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

void ListTracksEditor::SetFavorite(int value)
{
	m_Favorite = value;

	QString style;

	if (m_Favorite == 0)
	{
		style = QString("QLabel	\
						{	\
						  border-image: url(\":/resource/play-btn12-like-n.png\");	\
						}	\
						QLabel:hover	\
						{	\
						  border-image: url(\":/resource/play-btn12-like-h.png\");	\
						}");

	}
	else
	{
		style = QString("QLabel	\
						{	\
						  border-image: url(\":/resource/play-btn12-like-u.png\");	\
						}	\
						QLabel:hover	\
						{	\
						  border-image: url(\":/resource/play-btn12-like-h.png\");	\
						}");

	}
	ui->labelFavorite->setStyleSheet(style);
}

void ListTracksEditor::ShowFavorite(bool show)
{
	if (show)
	{
		ui->frameFavorite->show();
	}
	else
	{
		ui->frameFavorite->hide();
	}
}

QString ListTracksEditor::GetTime()
{
	return ui->labelTime->text();
}

void ListTracksEditor::SetTime(const QString &title)
{
	ui->labelTime->setText(title);
}

void ListTracksEditor::ShowTime(bool show)
{
	if (show)
	{
		ui->labelTime->show();
	}
	else
	{
		ui->labelTime->hide();
	}
}

QString ListTracksEditor::GetArtist()
{
	return ui->labelArtist->text();
}

void ListTracksEditor::SetArtist(const QString &title)
{
	ui->labelArtist->setText(title);
}

void ListTracksEditor::ShowArtist(bool show)
{
	if (show)
	{
		ui->labelArtist->show();
	}
	else
	{
		ui->labelArtist->hide();
	}
}

QString ListTracksEditor::GetAlbum()
{
	return ui->labelAlbum->text();
}

void ListTracksEditor::SetAlbum(const QString &title)
{
	ui->labelAlbum->setText(title);
}

void ListTracksEditor::ShowAlbum(bool show)
{
	if (show)
	{
		ui->labelAlbum->show();
	}
	else
	{
		ui->labelAlbum->hide();
	}
}

QString ListTracksEditor::GetGenre()
{
	return ui->labelGenre->text();
}

void ListTracksEditor::SetGenre(const QString &title)
{
	ui->labelGenre->setText(title);
}

void ListTracksEditor::ShowGenre(bool show)
{
	if (show)
	{
		ui->labelGenre->show();
	}
	else
	{
		ui->labelGenre->hide();
	}
}

QString ListTracksEditor::GetAlbumArtist()
{
	return ui->labelAlbumArtist->text();
}

void ListTracksEditor::SetAlbumArtist(const QString &title)
{
	ui->labelAlbumArtist->setText(title);
}

void ListTracksEditor::ShowAlbumArtist(bool show)
{
	if (show)
	{
		ui->labelAlbumArtist->show();
	}
	else
	{
		ui->labelAlbumArtist->hide();
	}
}

QString ListTracksEditor::GetComposer()
{
	return ui->labelComposer->text();
}

void ListTracksEditor::SetComposer(const QString &title)
{
	ui->labelComposer->setText(title);
}

void ListTracksEditor::ShowComposer(bool show)
{
	if (show)
	{
		ui->labelComposer->show();
	}
	else
	{
		ui->labelComposer->hide();
	}
}

QString ListTracksEditor::GetYear()
{
	return ui->labelYear->text();
}

void ListTracksEditor::SetYear(const QString &title)
{
	ui->labelYear->setText(title);
}

void ListTracksEditor::ShowYear(bool show)
{
	if (show)
	{
		ui->labelYear->show();
	}
	else
	{
		ui->labelYear->hide();
	}
}

QString ListTracksEditor::GetMood()
{
	return ui->labelMood->text();
}

void ListTracksEditor::SetMood(const QString &title)
{
	ui->labelMood->setText(title);
}

void ListTracksEditor::ShowMood(bool show)
{
	if (show)
	{
		ui->labelMood->show();
	}
	else
	{
		ui->labelMood->hide();
	}
}

QString ListTracksEditor::GetTempo()
{
	return ui->labelTempo->text();
}

void ListTracksEditor::SetTempo(const QString &title)
{
	ui->labelTempo->setText(title);
}

void ListTracksEditor::ShowTempo(bool show)
{
	if (show)
	{
		ui->labelTempo->show();
	}
	else
	{
		ui->labelTempo->hide();
	}
}

QString ListTracksEditor::GetFormat()
{
	return ui->labelFormat->text();
}

void ListTracksEditor::SetFormat(const QString &title)
{
	int index = title.length() - title.lastIndexOf('.') - 1;
	QString extension = title.right(index);
	ui->labelFormat->setText(extension);
}

void ListTracksEditor::ShowFormat(bool show)
{
	if (show)
	{
		ui->labelFormat->show();
	}
	else
	{
		ui->labelFormat->hide();
	}
}

QString ListTracksEditor::GetSampleRate()
{
	return ui->labelSampleRate->text();
}

void ListTracksEditor::SetSampleRate(const QString &title)
{
	ui->labelSampleRate->setText(title);
}

void ListTracksEditor::ShowSampleRate(bool show)
{
	if (show)
	{
		ui->labelSampleRate->show();
	}
	else
	{
		ui->labelSampleRate->hide();
	}
}

QString ListTracksEditor::GetBitDepth()
{
	return ui->labelBitDepth->text();
}

void ListTracksEditor::SetBitDepth(const QString &title)
{
	ui->labelBitDepth->setText(title);
}

void ListTracksEditor::ShowBitDepth(bool show)
{
	if (show)
	{
		ui->labelBitDepth->show();
	}
	else
	{
		ui->labelBitDepth->hide();
	}
}

int ListTracksEditor::GetRating() const
{
	return m_Rating;
}

void ListTracksEditor::SetRating(int value)
{
	m_Rating = value;

	// temp_code
	ui->labelRating->setText(QString("%1").arg(value));

}

void ListTracksEditor::ShowRating(bool show)
{
	if (show)
	{
		ui->labelRating->show();
	}
	else
	{
		ui->labelRating->hide();
	}
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

void ListTracksEditor::SetFrameSizeFormCoverArt(int size)
{
	ui->frameFormCoverArt->setMaximumWidth(size);
	ui->frameFormCoverArt->setMaximumHeight(size);
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
//		else if (object == ui->labelTime)
//		{
//			emit SigClickTime(m_ID);
//		}
//		else if (object == ui->labelArtist)
//		{
//			emit SigClickArtist(m_ID);
//		}
//		else if (object == ui->labelAlbum)
//		{
//			emit SigClickAlbum(m_ID);
//		}
//		else if (object == ui->labelGenre)
//		{
//			emit SigClickGenre(m_ID);
//		}

	}

	return QObject::eventFilter(object, event);
}

void ListTracksEditor::SlotMenuAction(QAction *action)
{
	emit SigMenuAction(m_ID, action->data().toInt());
}

void ListTracksEditor::SlotCoverArt(int index)
{
	Q_UNUSED(index)
	emit SigClickTitle(m_ID, m_pFormCoverArt->GetCoverArt());
}

void ListTracksEditor::SlotCheck(int index)
{
	emit SigClickCoverArt(index);
}

void ListTracksEditor::ConnectSigToSlot()
{
	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuAction(QAction*)));
	connect(m_pFormCoverArt, SIGNAL(SigCoverArt(int)), this, SLOT(SlotCoverArt(int)));
	connect(m_pFormCoverArt, SIGNAL(SigCheck(int)), this, SLOT(SlotCheck(int)));
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

	// only music db
	ui->frameFavorite->hide();
	ui->labelTime->hide();
	ui->labelArtist->hide();
	ui->labelAlbum->hide();
	ui->labelGenre->hide();
	ui->labelAlbumArtist->hide();
	ui->labelComposer->hide();
	ui->labelYear->hide();

	// option
	ui->labelMood->hide();
	ui->labelTempo->hide();
	ui->labelFormat->hide();
	ui->labelSampleRate->hide();
	ui->labelBitDepth->hide();
	ui->labelRating->hide();

	ui->labelPlay->installEventFilter(this);
	ui->labelTitle->installEventFilter(this);
	ui->labelFavorite->installEventFilter(this);
//	ui->labelTime->installEventFilter(this);
//	ui->labelArtist->installEventFilter(this);
//	ui->labelAlbum->installEventFilter(this);
//	ui->labelGenre->installEventFilter(this);

	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);

	m_ID = -1;
//	m_CoverArt.clear();
}
