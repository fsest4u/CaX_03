#include <QMouseEvent>

#include "infohome.h"
#include "ui_infohome.h"

#include "widget/form/formplay.h"
#include "widget/form/formsort.h"
#include "widget/form/formclassify.h"


#include "manager/sqlmanager.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

InfoHome::InfoHome(QWidget *parent)	:
	QWidget(parent),
	m_pFormPlay(new FormPlay(this)),
	m_pFormClassify(new FormClassify(this)),
	m_pFormSort(new FormSort(this)),
	m_CategoryMenu(new QMenu(this)),
	ui(new Ui::InfoHome)
{
	ui->setupUi(this);

	Initialize();
}

InfoHome::~InfoHome()
{

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

	if (m_CategoryMenu)
	{
		delete m_CategoryMenu;
		m_CategoryMenu = nullptr;
	}

	delete ui;
}

void InfoHome::SetTitle(const int nCategory)
{
	QString title = KEY_ALBUM;
	switch (nCategory)
	{
	case SQLManager::CATEGORY_ALBUM:
		title = KEY_ALBUM;
		break;
	case SQLManager::CATEGORY_ALBUMARTIST:
		title = KEY_ALBUM_ARTIST;
		break;
	case SQLManager::CATEGORY_ARTIST:
		title = KEY_ARTIST;
		break;
	case SQLManager::CATEGORY_GENRE:
		title = KEY_GENRE;
		break;
	case SQLManager::CATEGORY_COMPOSER:
		title = KEY_COMPOSER;
		break;
	case SQLManager::CATEGORY_MOOD:
		title = KEY_MOOD;
		break;
	case SQLManager::CATEGORY_FOLDER:
		title = KEY_FOLDER;
		break;
	case SQLManager::CATEGORY_YEAR:
		title = KEY_YEAR;
		break;
	case SQLManager::CATEGORY_TRACK:
		title = KEY_TRACK;
		break;
	}

	ui->labelTitle->setText(title);
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

FormPlay *InfoHome::GetFormPlay()
{
	return m_pFormPlay;
}

FormSort *InfoHome::GetFormSort()
{
	return m_pFormSort;
}

FormClassify *InfoHome::GetFormClassify()
{
	return m_pFormClassify;
}

void InfoHome::ClearCategoryMenu()
{
	disconnect(m_CategoryMenu, SIGNAL(triggered(QAction*)));
	m_CategoryMenu->clear();
}

void InfoHome::SetCategoryMenu(QMap<int, QString> list)
{
	QMap<int, QString>::iterator i;
	for (i = list.begin(); i!= list.end(); i++)
	{
		QAction *action = new QAction(i.value(), this);
		action->setData(i.key());
		m_CategoryMenu->addAction(action);
	}
	connect(m_CategoryMenu, SIGNAL(triggered(QAction*)), this, SLOT(SlotCategoryMenuAction(QAction*)));
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
	}

	return QObject::eventFilter(object, event);
}

void InfoHome::SlotBtnCategoryMenu()
{
	emit SigCategoryMenu();
}

void InfoHome::SlotCategoryMenuAction(QAction *action)
{
	emit SigCategoryMenuAction(action->data().toInt(), action->text());
}

void InfoHome::Initialize()
{
	connect(ui->btnCategoryMenu, SIGNAL(pressed()), this, SLOT(SlotBtnCategoryMenu()));

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

	m_CategoryMenu->setStyleSheet(style);
	ui->btnCategoryMenu->setMenu(m_CategoryMenu);

	ui->frameAlbum->installEventFilter(this);
	ui->frameArtist->installEventFilter(this);
	ui->frameTrack->installEventFilter(this);
	ui->frameGenre->installEventFilter(this);

	ui->gridLayoutFormPlay->addWidget(m_pFormPlay);
	ui->gridLayoutFormClassify->addWidget(m_pFormClassify);
	ui->gridLayoutFormSort->addWidget(m_pFormSort);

}
