#include <QMouseEvent>

#include "formclassify.h"
#include "ui_formclassify.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"


FormClassify::FormClassify(QWidget *parent) :
	QWidget(parent),
	m_ClassifyMenu(new QMenu(this)),
	m_GenreMenu(new QMenu(tr("Genre"), this)),
	m_ArtistMenu(new QMenu(tr("Artist"), this)),
	m_ComposerMenu(new QMenu(tr("Composer"), this)),
	ui(new Ui::FormClassify)
{
	ui->setupUi(this);

	ui->btnClassify->installEventFilter(this);
	ui->labelFavorite->installEventFilter(this);
	ui->labelRating->installEventFilter(this);
	ui->labelArtist->installEventFilter(this);
	ui->labelGenre->installEventFilter(this);
	ui->labelComposer->installEventFilter(this);

	SetClassifyMenu();

}

FormClassify::~FormClassify()
{
	if (m_ClassifyMenu)
	{
		delete m_ClassifyMenu;
		m_ClassifyMenu = nullptr;
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

	delete ui;
}

void FormClassify::SetClassifyArtistMenu(QList<CJsonNode> list)
{
	foreach (CJsonNode node, list)
	{
		QAction *action = new QAction(node.GetString(KEY_NAME_LOWER), this);
		action->setData(node.GetString(KEY_ID_LOWER));
		m_ArtistMenu->addAction(action);
	}
	connect(m_ArtistMenu, SIGNAL(triggered(QAction*)), this, SLOT(SlotArtistMenu(QAction*)));
}

void FormClassify::SetClassifyGenreMenu(QList<CJsonNode> list)
{
	foreach (CJsonNode node, list)
	{
		QAction *action = new QAction(node.GetString(KEY_NAME_LOWER), this);
		action->setData(node.GetString(KEY_ID_LOWER));
		m_GenreMenu->addAction(action);
	}
	connect(m_GenreMenu, SIGNAL(triggered(QAction*)), this, SLOT(SlotGenreMenu(QAction*)));

}

void FormClassify::SetClassifyComposerMenu(QList<CJsonNode> list)
{
	foreach (CJsonNode node, list)
	{
		QAction *action = new QAction(node.GetString(KEY_NAME_LOWER), this);
		action->setData(node.GetString(KEY_ID_LOWER));
		m_ComposerMenu->addAction(action);
	}
	connect(m_ComposerMenu, SIGNAL(triggered(QAction*)), this, SLOT(SlotComposerMenu(QAction*)));

}

bool FormClassify::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->btnClassify)
		{
			emit SigClassify();
		}
		else if (object == ui->labelFavorite)
		{
			emit SigFavorite();
		}
		else if (object == ui->labelRating)
		{
			emit SigRating();
		}
		else if (object == ui->labelArtist)
		{
			ui->labelArtist->hide();
			emit SigClassifyArtist(false, "");
		}
		else if (object == ui->labelGenre)
		{
			ui->labelGenre->hide();
			emit SigClassifyGenre(false, "");
		}
		else if (object == ui->labelComposer)
		{
			ui->labelComposer->hide();
			emit SigClassifyComposer(false, "");
		}

	}

	return QObject::eventFilter(object, event);
}

void FormClassify::SlotArtistMenu(QAction *action)
{
	LogDebug("Artist id [%s][%s]", action->data().toString().toUtf8().data(), action->text().toUtf8().data());
	ui->labelArtist->setText(action->text());
	ui->labelArtist->show();
	emit SigClassifyArtist(true, action->data().toString());
}

void FormClassify::SlotGenreMenu(QAction *action)
{
	LogDebug("Genre id [%s][%s]", action->data().toString().toUtf8().data(), action->text().toUtf8().data());
	ui->labelGenre->setText(action->text());
	ui->labelGenre->show();
	emit SigClassifyGenre(true, action->data().toString());
}

void FormClassify::SlotComposerMenu(QAction *action)
{
	LogDebug("Composer id [%s][%s]", action->data().toString().toUtf8().data(), action->text().toUtf8().data());
	ui->labelComposer->setText(action->text());
	ui->labelComposer->show();
	emit SigClassifyComposer(true, action->data().toString());
}

void FormClassify::SetClassifyMenu()
{
//	m_ClassifyMenu->setStyleSheet("QMenu { menu-scrollable: 1; }");

	m_ClassifyMenu->addAction(m_ClassifyMenu->addMenu(m_ArtistMenu));
	m_ClassifyMenu->addAction(m_ClassifyMenu->addMenu(m_GenreMenu));
	m_ClassifyMenu->addAction(m_ClassifyMenu->addMenu(m_ComposerMenu));

	ui->btnClassify->setMenu(m_ClassifyMenu);

	ui->labelArtist->hide();
	ui->labelGenre->hide();
	ui->labelComposer->hide();
	ui->labelTemp->hide();
	ui->labelTemp_2->hide();
}
