#include <QMouseEvent>

#include "infohome.h"
#include "ui_infohome.h"

#include "widget/form/formplay.h"
#include "widget/form/formsort.h"
#include "widget/form/formclassify.h"

#include "dialog/submenudialog.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

InfoHome::InfoHome(QWidget *parent)	:
	QWidget(parent),
	m_pCatDlg(new SubmenuDialog(this)),
	m_pSortDlg(new SubmenuDialog(this)),
	m_pFormPlay(new FormPlay(this)),
	m_pFormClassify(new FormClassify(this)),
	m_pFormSort(new FormSort(this)),
	m_ClassifyMenu(new QMenu(this)),
	m_GenreMenu(new QMenu(tr("Genre"), this)),
	m_ArtistMenu(new QMenu(tr("Artist"), this)),
	m_ComposerMenu(new QMenu(tr("Composer"), this)),
	ui(new Ui::InfoHome)
{
	ui->setupUi(this);

	ConnectSigToSlot();

	ui->frameAlbum->installEventFilter(this);
	ui->frameArtist->installEventFilter(this);
	ui->frameTrack->installEventFilter(this);
	ui->frameGenre->installEventFilter(this);
	ui->frameSubmenu2->installEventFilter(this);

	ui->gridLayoutFormPlay->addWidget(m_pFormPlay);
	ui->gridLayoutFormClassify->addWidget(m_pFormClassify);
	ui->gridLayoutFormSort->addWidget(m_pFormSort);

	SetCategoryDialog();
	SetSortDialog();
	SetClassifyMenu();
}

InfoHome::~InfoHome()
{
	if (m_pCatDlg)
	{
		delete m_pCatDlg;
		m_pCatDlg = nullptr;
	}
	if (m_pSortDlg)
	{
		delete m_pSortDlg;
		m_pSortDlg = nullptr;
	}

	if (m_pFormPlay)
	{
		delete m_pFormPlay;
		m_pFormPlay = nullptr;
	}

	if (m_pFormClassify)
	{
		delete m_pFormClassify;
		m_pFormClassify = nullptr;
	}

	if (m_pFormSort)
	{
		delete m_pFormSort;
		m_pFormSort = nullptr;
	}

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

void InfoHome::SetAlbumCnt(const QString count)
{
	ui->labelAlbumCnt->setText(count);
}

void InfoHome::SetArtistCnt(const QString count)
{
	ui->labelArtistCnt->setText(count);

}

void InfoHome::SetTrackCnt(const QString count)
{
	ui->labelTrackCnt->setText(count);

}

void InfoHome::SetGenreCnt(const QString count)
{
	ui->labelGenreCnt->setText(count);

}

void InfoHome::SetSortName(const QString name)
{
//	ui->labelSort->setText(name);

}

bool InfoHome::eventFilter(QObject *object, QEvent *event)
{

	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->frameGenre)
		{
			emit SigGenreList();
		}
		else if (object == ui->frameAlbum)
		{
			emit SigAlbumList();
		}
		else if (object == ui->frameArtist)
		{
			emit SigArtistList();
		}
		else if (object == ui->frameTrack)
		{
			emit SigTrackList();
		}
		else if (object == ui->frameSubmenu2)
		{
			emit SigSubmenu2();
		}
//		else if (object == ui->frameClassifyArtist)
//		{
//			DoClickClassifyArtist();
//		}
//		else if (object == ui->frameClassifyGenre)
//		{
//			DoClickClassifyGenre();
//		}
//		else if (object == ui->frameClassifyComposer)
//		{
//			DoClickClassifyComposer();
//		}

	}

	return QObject::eventFilter(object, event);
}

void InfoHome::SlotArtistMenu(QAction *action)
{
	LogDebug("Artist id [%s][%s]", action->data().toString().toUtf8().data(), action->text().toUtf8().data());
//	ui->frameClassifyArtist->show();
//	ui->labelClassifyArtist->setText(action->text());
	emit SigClassifyArtist(true, action->data().toString());
}

void InfoHome::SlotGenreMenu(QAction *action)
{
	LogDebug("Genre id [%s][%s]", action->data().toString().toUtf8().data(), action->text().toUtf8().data());
//	ui->frameClassifyGenre->show();
//	ui->labelClassifyGenre->setText(action->text());
	emit SigClassifyGenre(true, action->data().toString());
}

void InfoHome::SlotComposerMenu(QAction *action)
{
	LogDebug("Composer id [%s][%s]", action->data().toString().toUtf8().data(), action->text().toUtf8().data());
//	ui->frameClassifyComposer->show();
//	ui->labelClassifyComposer->setText(action->text());
	emit SigClassifyComposer(true, action->data().toString());
}

void InfoHome::SlotPlayAll()
{
	LogDebug("good choice play all");

}

void InfoHome::SlotPlayRandom()
{
	LogDebug("good choice play random");

}

void InfoHome::SlotFavorite()
{
	LogDebug("good choice favorite");

}

void InfoHome::SlotRating()
{
	LogDebug("good choice rating");

}

void InfoHome::SlotSubmenu()
{
	LogDebug("good choice submenu");

}

void InfoHome::SlotSort()
{
	LogDebug("good choice sort");

}

void InfoHome::SlotIncDec()
{
	LogDebug("good choice inc dec");

}

void InfoHome::SlotResize()
{
	LogDebug("good choice resize");

}

void InfoHome::SlotFilterClassify()
{
	LogDebug("good choice filter classify");

}

void InfoHome::SlotFilterFavorite()
{
	LogDebug("good choice filter favorite");

}

void InfoHome::SlotFilterRating()
{
	LogDebug("good choice filter rating");

}

void InfoHome::SlotClassifyArtist()
{
	LogDebug("good choice classify artist");

}

void InfoHome::SlotClassifyGenre()
{
	LogDebug("good choice classify genre");

}

void InfoHome::SlotClassifyComposer()
{
	LogDebug("good choice classify composer");

}


void InfoHome::ConnectSigToSlot()
{
	connect(m_pFormPlay, SIGNAL(SigPlayAll()), this, SLOT(SlotPlayAll()));
	connect(m_pFormPlay, SIGNAL(SigPlayRandom()), this, SLOT(SlotPlayRandom()));
	connect(m_pFormPlay, SIGNAL(SigFavorite()), this, SLOT(SlotFavorite()));
	connect(m_pFormPlay, SIGNAL(SigRating()), this, SLOT(SlotRating()));
	connect(m_pFormPlay, SIGNAL(SigSubmenu()), this, SLOT(SlotSubmenu()));
	connect(m_pFormSort, SIGNAL(SigSort()), this, SLOT(SlotSort()));
	connect(m_pFormSort, SIGNAL(SigIncDec()), this, SLOT(SlotIncDec()));
	connect(m_pFormSort, SIGNAL(SigResize()), this, SLOT(SlotResize()));
	connect(m_pFormClassify, SIGNAL(SigClassify()), this, SLOT(SlotFilterClassify()));
	connect(m_pFormClassify, SIGNAL(SigFavorite()), this, SLOT(SlotFilterFavorite()));
	connect(m_pFormClassify, SIGNAL(SigRating()), this, SLOT(SlotFilterRating()));
	connect(m_pFormClassify, SIGNAL(SigArtist()), this, SLOT(SlotClassifyArtist()));
	connect(m_pFormClassify, SIGNAL(SigGenre()), this, SLOT(SlotClassifyGenre()));
	connect(m_pFormClassify, SIGNAL(SigComposer()), this, SLOT(SlotClassifyComposer()));

}


void InfoHome::SetCategoryDialog()
{
	QList<CJsonNode> list;

	QList<int> listID = {
		CATEGORY_GENRE,
		CATEGORY_MOOD,
		CATEGORY_FOLDER,
		CATEGORY_YEAR
	};
	QList<QString> listName = {
		"Sorted by Genre",
		"Sorted by Mood",
		"Sorted by Folder",
		"Sorted by Year"
	};

	CJsonNode node(JSON_OBJECT);
	for (int i = 0; i < listID.count(); i++)
	{
		node.AddInt(KEY_ID_UPPER, listID.at(i));
		node.Add(KEY_COVER_ART, ":/resource/Icon-playbar-volume-160.png");
		node.Add(KEY_NAME, listName.at(i));
		list.append(node);
	}
	m_pCatDlg->SetItemList(list);
}

void InfoHome::SetSortDialog()
{
	QList<CJsonNode> list;

	QList<int> listID = {
		SORT_GENRE,
		SORT_MOOD,
		SORT_FOLDER,
		SORT_YEAR,
		SORT_RATING,
		SORT_SAMPLE_RATE
	};
	QList<QString> listName = {
		"Sorted by Genre",
		"Sorted by Mood",
		"Sorted by Folder",
		"Sorted by Year",
		"Sorted by Rating",
		"Sorted by Sample rate"
	};

	CJsonNode node(JSON_OBJECT);
	for (int i = 0; i < listID.count(); i++)
	{
		node.AddInt(KEY_ID_UPPER, listID.at(i));
		node.Add(KEY_COVER_ART, ":/resource/Icon-playbar-volume-160.png");
		node.Add(KEY_NAME, listName.at(i));
		list.append(node);
	}
	m_pSortDlg->SetItemList(list);
}

void InfoHome::SetClassifyMenu()
{
	m_ClassifyMenu->setStyleSheet("QMenu { menu-scrollable: 1; }");

	m_ClassifyMenu->addAction(m_ClassifyMenu->addMenu(m_ArtistMenu));
	m_ClassifyMenu->addAction(m_ClassifyMenu->addMenu(m_GenreMenu));
	m_ClassifyMenu->addAction(m_ClassifyMenu->addMenu(m_ComposerMenu));

//	ui->btnClassify->setMenu(m_ClassifyMenu);

//	ui->frameClassifyArtist->hide();
//	ui->frameClassifyGenre->hide();
//	ui->frameClassifyComposer->hide();
//	ui->frameClassifyTemp->hide();
//	ui->frameClassifyTemp_2->hide();
}

void InfoHome::DoClickClassifyArtist()
{
//	ui->frameClassifyArtist->hide();
//	emit SigClassifyArtist(false, "");
}

void InfoHome::DoClickClassifyGenre()
{
//	ui->frameClassifyGenre->hide();
//	emit SigClassifyGenre(false, "");
}

void InfoHome::DoClickClassifyComposer()
{
//	ui->frameClassifyComposer->hide();
//	emit SigClassifyComposer(false, "");
}

void InfoHome::SetClassifyArtistMenu(QList<CJsonNode> list)
{
	foreach (CJsonNode node, list)
	{
		QAction *action = new QAction(node.GetString(KEY_NAME_LOWER), this);
		action->setData(node.GetString(KEY_ID_LOWER));
		m_ArtistMenu->addAction(action);
	}
	connect(m_ArtistMenu, SIGNAL(triggered(QAction*)), this, SLOT(SlotArtistMenu(QAction*)));
}

void InfoHome::SetClassifyGenreMenu(QList<CJsonNode> list)
{
	foreach (CJsonNode node, list)
	{
		QAction *action = new QAction(node.GetString(KEY_NAME_LOWER), this);
		action->setData(node.GetString(KEY_ID_LOWER));
		m_GenreMenu->addAction(action);
	}
	connect(m_GenreMenu, SIGNAL(triggered(QAction*)), this, SLOT(SlotGenreMenu(QAction*)));

}

void InfoHome::SetClassifyComposerMenu(QList<CJsonNode> list)
{
	foreach (CJsonNode node, list)
	{
		QAction *action = new QAction(node.GetString(KEY_NAME_LOWER), this);
		action->setData(node.GetString(KEY_ID_LOWER));
		m_ComposerMenu->addAction(action);
	}
	connect(m_ComposerMenu, SIGNAL(triggered(QAction*)), this, SLOT(SlotComposerMenu(QAction*)));

}

void InfoHome::ShowCategoryDialog()
{
//	if (m_pCatDlg->isHidden())
//	{
//		m_pCatDlg->move(mapToGlobal(ui->frameSubmenu->geometry().bottomRight()).x() - m_pCatDlg->width()
//						, mapToGlobal(ui->frameSubmenu->geometry().bottomRight()).y());

//		if (m_pCatDlg->exec())
//		{
//			int id = m_pCatDlg->GetID();
//			LogDebug("sort type id [%d]", id);
//			// emit signal sort...
//		}
//	}
//	else
//	{
//		m_pCatDlg->close();
//	}
}

void InfoHome::ShowSortDialog()
{
	if (m_pSortDlg->isHidden())
	{
		m_pSortDlg->move(mapToGlobal(this->geometry().bottomRight()).x() - m_pSortDlg->width()
						 , mapToGlobal(this->geometry().bottomRight()).y());

		if (m_pSortDlg->exec())
		{
			int id = m_pSortDlg->GetID();
			LogDebug("sort type id [%d]", id);
			// emit signal sort...
		}
	}
	else
	{
		m_pSortDlg->close();
	}
}
