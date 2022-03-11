#include <QMouseEvent>

#include "formclassify.h"
#include "ui_formclassify.h"

#include "util/caxkeyvalue.h"
#include "util/caxtranslate.h"
#include "util/log.h"


FormClassify::FormClassify(QWidget *parent) :
	QWidget(parent),
	m_Menu(new QMenu(this)),
	m_GenreMenu(new QMenu(STR_GENRE, this)),
	m_ArtistMenu(new QMenu(STR_ARTIST, this)),
	m_ComposerMenu(new QMenu(STR_COMPOSER, this)),
	m_FormatMenu(new QMenu(STR_AUDIO_FORMAT, this)),
	m_MostPlayedMenu(new QMenu(STR_MOST_PLAYED, this)),
	m_Favorite(0),
	m_Rating(0),
	ui(new Ui::FormClassify)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();

}

FormClassify::~FormClassify()
{
	disconnect(m_ArtistMenu, SIGNAL(triggered(QAction*)));
	disconnect(m_GenreMenu, SIGNAL(triggered(QAction*)));
	disconnect(m_ComposerMenu, SIGNAL(triggered(QAction*)));
	disconnect(m_FormatMenu, SIGNAL(triggered(QAction*)));
	disconnect(m_MostPlayedMenu, SIGNAL(triggered(QAction*)));

	if (m_Menu)
	{
		delete m_Menu;
		m_Menu = nullptr;
	}

	if (m_GenreMenu)
	{
		delete m_GenreMenu;
		m_GenreMenu = nullptr;
	}

	if (m_ArtistMenu)
	{
		delete m_ArtistMenu;
		m_ArtistMenu = nullptr;
	}

	if (m_ComposerMenu)
	{
		delete m_ComposerMenu;
		m_ComposerMenu = nullptr;
	}

	if (m_FormatMenu)
	{
		delete m_FormatMenu;
		m_FormatMenu = nullptr;
	}

	if (m_MostPlayedMenu)
	{
		delete m_MostPlayedMenu;
		m_MostPlayedMenu = nullptr;
	}
	delete ui;
}

void FormClassify::ClearClassifyArtistMenu()
{
	m_ArtistMenu->clear();
}

void FormClassify::ClearClassifyGenreMenu()
{
	m_GenreMenu->clear();
}

void FormClassify::ClearClassifyComposerMenu()
{
	m_ComposerMenu->clear();
}

void FormClassify::ClearClassifyFormatMenu()
{
	m_FormatMenu->clear();
}

void FormClassify::ClearClassifyMostPlayedMenu()
{
	m_MostPlayedMenu->clear();
}

void FormClassify::SetClassifyArtistMenu(QList<CJsonNode> list)
{
	foreach (CJsonNode node, list)
	{
//		LogDebug("node [%s]", node.ToCompactByteArray().data());

		QAction *action = new QAction(node.GetString(KEY_NAME), this);
		action->setData(node.GetString(KEY_ID_LOWER));
		m_ArtistMenu->addAction(action);
	}
}

void FormClassify::SetClassifyGenreMenu(QList<CJsonNode> list)
{
	foreach (CJsonNode node, list)
	{
//		LogDebug("node [%s]", node.ToCompactByteArray().data());

		QAction *action = new QAction(node.GetString(KEY_NAME), this);
		action->setData(node.GetString(KEY_ID_LOWER));
		m_GenreMenu->addAction(action);
	}
}

void FormClassify::SetClassifyComposerMenu(QList<CJsonNode> list)
{
	foreach (CJsonNode node, list)
	{
//		LogDebug("node [%s]", node.ToCompactByteArray().data());

		QAction *action = new QAction(node.GetString(KEY_NAME), this);
		action->setData(node.GetString(KEY_ID_LOWER));
		m_ComposerMenu->addAction(action);
	}
}

void FormClassify::SetClassifyFormatMenu(QList<CJsonNode> list)
{
	foreach (CJsonNode node, list)
	{
//		LogDebug("node [%s]", node.ToCompactByteArray().data());

		QAction *action = new QAction(node.GetString(KEY_NAME), this);
		action->setData(node.GetString(KEY_ID_LOWER));
		m_FormatMenu->addAction(action);
	}
}

void FormClassify::SetClassifyMostPlayedMenu(QList<CJsonNode> list)
{
	foreach (CJsonNode node, list)
	{
//		LogDebug("node [%s]", node.ToCompactByteArray().data());

		QAction *action = new QAction(node.GetString(KEY_NAME), this);
		action->setData(node.GetString(KEY_ID_LOWER));
		m_MostPlayedMenu->addAction(action);
	}
}

int FormClassify::GetFavorite() const
{
	return m_Favorite;
}

void FormClassify::SetFavorite(int Favorite)
{
	m_Favorite = Favorite;

	QString style;

	if (m_Favorite == 0)
	{
		style = QString("QLabel	\
						{	\
						  border-image: url(\":/resource/play-btn28-like-n.png\");	\
						}");

	}
	else
	{
		style = QString("QLabel	\
						{	\
						  border-image: url(\":/resource/play-btn28-like-n.png\");	\
						}");

	}
	ui->labelFavorite->setStyleSheet(style);

}

int FormClassify::GetRating() const
{
	return m_Rating;
}

void FormClassify::SetRating(int Rating)
{
	m_Rating = Rating;

	QString style;

	if (m_Rating == 1)
	{
		style = "play-btn28-rank-sel1.png";
	}
	else if (m_Rating == 2)
	{
		style = "play-btn28-rank-sel2.png";
	}
	else if (m_Rating == 3)
	{
		style = "play-btn28-rank-sel3.png";
	}
	else if (m_Rating == 4)
	{
		style = "play-btn28-rank-sel4.png";
	}
	else if (m_Rating == 5)
	{
		style = "play-btn28-rank-sel5.png";
	}
	else
	{
		style = "play-btn28-rank-sel0.png";
	}

	style = QString("QFrame	\
					{	\
					  border-image: url(\":/resource/%1\");	\
					}").arg(style);

	ui->frameRating->setStyleSheet(style);

}

bool FormClassify::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{

		if (object == ui->labelFavorite)
		{
			if (m_Favorite == 0)
			{
				SetFavorite(1);
			}
			else
			{
				SetFavorite(0);
			}
			emit SigClassifyFavorite(m_Favorite);
			return true;
		}
		else if (object == ui->labelArtist)
		{
			ui->labelArtist->hide();
			emit SigClassifyArtist(false, "");
			return true;
		}
		else if (object == ui->labelGenre)
		{
			ui->labelGenre->hide();
			emit SigClassifyGenre(false, "");
			return true;
		}
		else if (object == ui->labelComposer)
		{
			ui->labelComposer->hide();
			emit SigClassifyComposer(false, "");
			return true;
		}
		else if (object == ui->labelFormat)
		{
			ui->labelFormat->hide();
			emit SigClassifyFormat(false, "");
			return true;
		}
		else if (object == ui->labelMostPlayed)
		{
			ui->labelMostPlayed->hide();
			emit SigClassifyMostPlayed(false, "");
			return true;
		}
		else if (object == ui->frameRating)
		{
			SlotBtnRating0();
			return true;
		}

	}

	return QObject::eventFilter(object, event);
}

void FormClassify::SlotArtistMenu(QAction *action)
{
//	LogDebug("Artist id [%s][%s]", action->data().toString().toUtf8().data(), action->text().toUtf8().data());
	ui->labelArtist->setText(action->text());
	ui->labelArtist->show();
	emit SigClassifyArtist(true, action->data().toString());
}

void FormClassify::SlotGenreMenu(QAction *action)
{
//	LogDebug("Genre id [%s][%s]", action->data().toString().toUtf8().data(), action->text().toUtf8().data());
	ui->labelGenre->setText(action->text());
	ui->labelGenre->show();
	emit SigClassifyGenre(true, action->data().toString());
}

void FormClassify::SlotComposerMenu(QAction *action)
{
//	LogDebug("Composer id [%s][%s]", action->data().toString().toUtf8().data(), action->text().toUtf8().data());
	ui->labelComposer->setText(action->text());
	ui->labelComposer->show();
	emit SigClassifyComposer(true, action->data().toString());
}

void FormClassify::SlotFormatMenu(QAction *action)
{
//	LogDebug("Format id [%s][%s]", action->data().toString().toUtf8().data(), action->text().toUtf8().data());
	ui->labelFormat->setText(action->text());
	ui->labelFormat->show();
	emit SigClassifyFormat(true, action->data().toString());
}

void FormClassify::SlotMostPlayedMenu(QAction *action)
{
//	LogDebug("Most play id [%s][%s]", action->data().toString().toUtf8().data(), action->text().toUtf8().data());
	ui->labelMostPlayed->setText(action->text());
	ui->labelMostPlayed->show();
	emit SigClassifyMostPlayed(true, action->data().toString());
}

void FormClassify::SlotBtnRating0()
{
	SetRating(0);
	emit SigClassifyRating(m_Rating);
}

void FormClassify::SlotBtnRating1()
{
	SetRating(1);
	emit SigClassifyRating(m_Rating);
}

void FormClassify::SlotBtnRating2()
{
	SetRating(2);
	emit SigClassifyRating(m_Rating);
}

void FormClassify::SlotBtnRating3()
{
	SetRating(3);
	emit SigClassifyRating(m_Rating);
}

void FormClassify::SlotBtnRating4()
{
	SetRating(4);
	emit SigClassifyRating(m_Rating);
}

void FormClassify::SlotBtnRating5()
{
	SetRating(5);
	emit SigClassifyRating(m_Rating);
}

void FormClassify::ConnectSigToSlot()
{
	connect(ui->btnRating1, SIGNAL(clicked()), this, SLOT(SlotBtnRating1()));
	connect(ui->btnRating2, SIGNAL(clicked()), this, SLOT(SlotBtnRating2()));
	connect(ui->btnRating3, SIGNAL(clicked()), this, SLOT(SlotBtnRating3()));
	connect(ui->btnRating4, SIGNAL(clicked()), this, SLOT(SlotBtnRating4()));
	connect(ui->btnRating5, SIGNAL(clicked()), this, SLOT(SlotBtnRating5()));
}

void FormClassify::Initialize()
{

	m_Menu->addAction(m_Menu->addMenu(m_ArtistMenu));
	m_Menu->addAction(m_Menu->addMenu(m_GenreMenu));
	m_Menu->addAction(m_Menu->addMenu(m_ComposerMenu));
	m_Menu->addAction(m_Menu->addMenu(m_FormatMenu));
	m_Menu->addAction(m_Menu->addMenu(m_MostPlayedMenu));

	QString styleCat = QString("QMenu::item {	\
									width: 200px;	\
									height: 30px;	\
									color: rgb(90, 91, 94);	\
									font-size: 14pt;	\
									padding: 0px 20px 0px 20px;	\
								}	\
								QMenu::item:selected {	\
									background: rgba(201,237,248,255);	\
								}");
	QString style = QString("QMenu::item {	\
								width: 200px;	\
								height: 40px;	\
								color: rgb(90, 91, 94);	\
								font-size: 14pt;	\
								padding: 0px 20px 0px 20px;	\
							}	\
							QMenu::item:selected {	\
								background: rgba(201,237,248,255);	\
							}");

	m_ArtistMenu->setStyleSheet(styleCat);
	m_GenreMenu->setStyleSheet(styleCat);
	m_ComposerMenu->setStyleSheet(styleCat);
	m_FormatMenu->setStyleSheet(styleCat);
	m_MostPlayedMenu->setStyleSheet(styleCat);
	m_Menu->setStyleSheet(style);
	ui->btnClassify->setMenu(m_Menu);

	connect(m_ArtistMenu, SIGNAL(triggered(QAction*)), this, SLOT(SlotArtistMenu(QAction*)));
	connect(m_GenreMenu, SIGNAL(triggered(QAction*)), this, SLOT(SlotGenreMenu(QAction*)));
	connect(m_ComposerMenu, SIGNAL(triggered(QAction*)), this, SLOT(SlotComposerMenu(QAction*)));
	connect(m_FormatMenu, SIGNAL(triggered(QAction*)), this, SLOT(SlotFormatMenu(QAction*)));
	connect(m_MostPlayedMenu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMostPlayedMenu(QAction*)));

	ui->labelArtist->hide();
	ui->labelGenre->hide();
	ui->labelComposer->hide();
	ui->labelFormat->hide();
	ui->labelMostPlayed->hide();

	ui->labelFavorite->installEventFilter(this);
	ui->labelArtist->installEventFilter(this);
	ui->labelGenre->installEventFilter(this);
	ui->labelComposer->installEventFilter(this);
	ui->labelFormat->installEventFilter(this);
	ui->labelMostPlayed->installEventFilter(this);
	ui->frameRating->installEventFilter(this);

}

