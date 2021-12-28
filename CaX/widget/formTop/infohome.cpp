#include <QMouseEvent>
#include <QThread>

#include "infohome.h"
#include "ui_infohome.h"

#include "widget/form/formtitle.h"
#include "widget/form/formplay.h"
#include "widget/form/formsort.h"
#include "widget/form/formclassify.h"


#include "manager/sqlmanager.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

InfoHome::InfoHome(QWidget *parent)	:
	QWidget(parent),
//	m_pFormTitle(new FormTitle(this)),
	m_pFormPlay(new FormPlay(this)),
	m_pFormClassify(new FormClassify(this)),
	m_pFormSort(new FormSort(this)),
	m_Menu(new QMenu(this)),
	ui(new Ui::InfoHome)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();
}

InfoHome::~InfoHome()
{

//	if (m_pFormTitle)
//	{
//		delete m_pFormTitle;
//		m_pFormTitle = nullptr;
//	}

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

	disconnect(m_Menu, SIGNAL(triggered(QAction*)));
	if (m_Menu)
	{
		delete m_Menu;
		m_Menu = nullptr;
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
	case SQLManager::CATEGORY_ALBUM_ARTIST:
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

//	m_pFormTitle->SetTitleFont(FONT_SIZE_INFO_TITLE);
//	m_pFormTitle->SetTitle(title);
	ui->labelTitle->setText(title);

}

void InfoHome::SetAlbumCnt(const QString count)
{
	ui->labelAlbumCnt->setText(count);
}

void InfoHome::SetAlbumArtistCnt(const QString count)
{
	ui->labelAlbumArtistCnt->setText(count);
}

void InfoHome::SetArtistCnt(const QString count)
{
	ui->labelArtistCnt->setText(count);
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

void InfoHome::ClearMenu()
{
	m_Menu->clear();
}

void InfoHome::SetMenu(QMap<int, QString> list)
{
	QMap<int, QString>::iterator i;
	for (i = list.begin(); i!= list.end(); i++)
	{
		QAction *action = new QAction(i.value(), this);
		action->setData(i.key());
		m_Menu->addAction(action);
	}
}

bool InfoHome::eventFilter(QObject *object, QEvent *event)
{

	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->frameAlbum)
		{
			emit SigAlbumList();
		}
		else if (object == ui->frameAlbumArtist)
		{
			emit SigAlbumArtistList();
		}
		else if (object == ui->frameArtist)
		{
			emit SigArtistList();
		}
		else if (object == ui->frameGenre)
		{
			emit SigGenreList();
		}
	}
	else if (event->type() == QMouseEvent::Enter)
	{
		if (object == ui->labelTitle)
		{
			QThread::sleep(1);
			ui->labelTitle->startTimer();
		}

	}
	else if (event->type() == QMouseEvent::Leave)
	{
		if (object == ui->labelTitle)
		{
			ui->labelTitle->stopTimer();
		}

	}

	return QObject::eventFilter(object, event);
}

void InfoHome::SlotMenu()
{
	emit SigMenu();
}

void InfoHome::SlotMenuAction(QAction *action)
{
	emit SigMenuAction(action->data().toInt(), action->text());
}

void InfoHome::ConnectSigToSlot()
{
	connect(ui->btnMenu, SIGNAL(pressed()), this, SLOT(SlotMenu()));
	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuAction(QAction*)));
}

void InfoHome::Initialize()
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

	ui->frameAlbum->installEventFilter(this);
	ui->frameAlbumArtist->installEventFilter(this);
	ui->frameArtist->installEventFilter(this);
	ui->frameGenre->installEventFilter(this);

	ui->labelTitle->installEventFilter(this);

//	ui->gridLayoutFormTitle->addWidget(m_pFormTitle);
	ui->gridLayoutFormPlay->addWidget(m_pFormPlay);
	ui->gridLayoutFormClassify->addWidget(m_pFormClassify);
	ui->gridLayoutFormSort->addWidget(m_pFormSort);

}
