#include <QMouseEvent>

#include "formplay.h"
#include "ui_formplay.h"

#include "util/caxtranslate.h"
#include "util/log.h"

FormPlay::FormPlay(QWidget *parent) :
	QWidget(parent),
	m_Menu(new QMenu(this)),
	m_Favorite(0),
	m_Rating(0),
	ui(new Ui::FormPlay)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();

}

FormPlay::~FormPlay()
{
	disconnect(m_Menu, SIGNAL(triggered(QAction*)));
	if (m_Menu)
	{
		delete m_Menu;
		m_Menu = nullptr;
	}

	delete ui;
}

void FormPlay::ShowPlayAll()
{
	ui->labelPlayAll->show();
}

void FormPlay::ShowPlayRandom()
{
	ui->labelPlayRandom->show();
}

void FormPlay::ShowFavorite()
{
	ui->labelFavorite->show();
}

void FormPlay::ShowRating()
{
	ui->frameRating->show();
}

void FormPlay::ShowMenu()
{
	ui->btnMenu->show();
}

int FormPlay::GetRating() const
{
	return m_Rating;
}

void FormPlay::SetRating(int Rating)
{
	m_Rating = Rating;

	QString style;

	if (m_Rating == 1)
	{
		style = "play-btn28-rank-sel1@3x.png";
	}
	else if (m_Rating == 2)
	{
		style = "play-btn28-rank-sel2@3x.png";
	}
	else if (m_Rating == 3)
	{
		style = "play-btn28-rank-sel3@3x.png";
	}
	else if (m_Rating == 4)
	{
		style = "play-btn28-rank-sel4@3x.png";
	}
	else if (m_Rating == 5)
	{
		style = "play-btn28-rank-sel5@3x.png";
	}
	else
	{
		style = "play-btn28-rank-sel0@3x.png";
	}

	style = QString("QFrame	\
					{	\
					  border-image: url(\":/resource/%1\");	\
					}	\
					QFrame:hover	\
					{	\
					  border-image: url(\":/resource/play-btn28-rank-sel0@3x.png\");	\
					}").arg(style);

	ui->frameRating->setStyleSheet(style);
}

void FormPlay::ClearMenu()
{
	m_Menu->clear();
}

void FormPlay::SetMenu(QMap<int, QString> map)
{
	QMap<int, QString>::iterator i;
	for (i = map.begin(); i != map.end(); i++)
	{
		QIcon icon = GetIcon(i.value());
		QAction *action = new QAction(icon, i.value(), this);
		action->setData(i.key());
		m_Menu->addAction(action);
	}
}

int FormPlay::GetFavorite() const
{
	return m_Favorite;
}

void FormPlay::SetFavorite(int Favorite)
{
	m_Favorite = Favorite;

	QString style;

	if (m_Favorite == 0)
	{
		style = QString("QLabel	\
						{	\
						  border-image: url(\":/resource/play-btn28-like-n@3x.png\");	\
						}	\
						QLabel:hover	\
						{	\
						  border-image: url(\":/resource/play-btn28-like-h@3x.png\");	\
						}");

	}
	else
	{
		style = QString("QLabel	\
						{	\
						  border-image: url(\":/resource/play-btn28-like-h@3x.png\");	\
						}	\
						QLabel:hover	\
						{	\
						  border-image: url(\":/resource/play-btn28-like-n@3x.png\");	\
						}");

	}
	ui->labelFavorite->setStyleSheet(style);
}

bool FormPlay::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->labelPlayAll)
		{
			emit SigPlayAll();
		}
		else if (object == ui->labelPlayRandom)
		{
			emit SigPlayRandom();
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
			emit SigFavorite(m_Favorite);
		}

	}

	return QObject::eventFilter(object, event);

}

void FormPlay::SlotBtnRating1()
{
	SetRating(1);
	emit SigRating(m_Rating);
}

void FormPlay::SlotBtnRating2()
{
	SetRating(2);
	emit SigRating(m_Rating);
}

void FormPlay::SlotBtnRating3()
{
	SetRating(3);
	emit SigRating(m_Rating);
}

void FormPlay::SlotBtnRating4()
{
	SetRating(4);
	emit SigRating(m_Rating);
}

void FormPlay::SlotBtnRating5()
{
	SetRating(5);
	emit SigRating(m_Rating);
}

void FormPlay::SlotMenu()
{
	emit SigMenu();
}

void FormPlay::SlotMenuAction(QAction *action)
{
	emit SigMenuAction(action->data().toInt());
}

void FormPlay::ConnectSigToSlot()
{
	connect(ui->btnRating1, SIGNAL(clicked()), this, SLOT(SlotBtnRating1()));
	connect(ui->btnRating2, SIGNAL(clicked()), this, SLOT(SlotBtnRating2()));
	connect(ui->btnRating3, SIGNAL(clicked()), this, SLOT(SlotBtnRating3()));
	connect(ui->btnRating4, SIGNAL(clicked()), this, SLOT(SlotBtnRating4()));
	connect(ui->btnRating5, SIGNAL(clicked()), this, SLOT(SlotBtnRating5()));

	connect(ui->btnMenu, SIGNAL(pressed()), this, SLOT(SlotMenu()));

}

void FormPlay::Initialize()
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

	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuAction(QAction*)));

	ui->labelPlayAll->hide();
	ui->labelPlayRandom->hide();
	ui->labelFavorite->hide();
	ui->frameRating->hide();
	ui->btnMenu->hide();

	ui->labelPlayAll->installEventFilter(this);
	ui->labelPlayRandom->installEventFilter(this);
	ui->labelFavorite->installEventFilter(this);

}

QIcon FormPlay::GetIcon(QString value)
{
	if (value.contains(STR_PLAY_NOW))
	{
		return QIcon(":/resource/audiocd-popup-icon16-play@3x.png");
	}
	else if (value.contains(STR_PLAY_LAST))
	{
		return QIcon(":/resource/audiocd-popup-icon16-play@3x.png");
	}
	else if (value.contains(STR_PLAY_NEXT))
	{
		return QIcon(":/resource/audiocd-popup-icon16-play@3x.png");
	}
	else if (value.contains(STR_PLAY_CLEAR))
	{
		return QIcon(":/resource/audiocd-popup-icon16-play@3x.png");
	}
	else if (value.contains(STR_RELOAD))
	{
		return QIcon(":/resource/play-popup-icon16-reload@3x.png");
	}
	else if (value.contains(STR_LOAD_COUNT))
	{
		return QIcon(":/resource/play-popup-icon16-loadalloff@3x.png");
	}
	else if (value.contains(STR_SELECT_ALL))
	{
		return QIcon(":/resource/play-popup-icon16-selectall@3x.png");
	}
	else if (value.contains(STR_CLEAR_ALL))
	{
		return QIcon(":/resource/playlist-icon16-clearall@3x.png");
	}
	else if (value.contains(STR_GAIN_SET))
	{
		return QIcon(":/resource/play-popup-icon16-replaygain@3x.png");
	}
	else if (value.contains(STR_GAIN_CLEAR))
	{
		return QIcon(":/resource/play-popup-icon16-cleanreplaygain@3x.png");
	}
	else if (value.contains(STR_CD_RIPPING))
	{
		return QIcon(":/resource/audiocd-popup-icon16-cdrip@3x.png");
	}
	else if (value.contains(STR_EJECT_CD))
	{
		return QIcon(":/resource/audiocd-popup-icon16-eject@3x.png");
	}
	else if (value.contains(STR_ADD_PLAYLIST))
	{
		return QIcon(":/resource/playlist-icon16-newplaylist@3x.png");
	}
	else if (value.contains(STR_RENAME_PLAYLIST))
	{
		return QIcon(":/resource/play-popup-icon16-nameedit@3x.png");
	}
	else if (value.contains(STR_ADD_TO_PLAYLIST))
	{
		return QIcon(":/resource/play-popup-icon16-addplaylist@3x.png");
	}
	else if (value.contains(STR_DELETE_ITEM))
	{
		return QIcon(":/resource/playlist-icon16-delete@3x.png");
	}
	else if (value.contains(STR_EXPORT_TRACK))
	{
		return QIcon(":/resource/playlist-icon16-exportsong@3x.png");
	}
	else if (value.contains(STR_OPTION_PLAY_SUBDIR))
	{
		return QIcon("");
	}
	else if (value.contains(STR_CONVERT_FORMAT))
	{
		return QIcon(":/resource/browser-icon16-convertaudio@3x.png");
	}
	else if (value.contains(STR_NEW_DIR))
	{
		return QIcon(":/resource/browser-icon16-newfolder@3x.png");
	}
	else if (value.contains(STR_OPTION_OVERWRITE))
	{
		return QIcon("");
	}
	else if (value.contains(STR_MOVE))
	{
		return QIcon(":/resource/browser-icon16-move@3x.png");
	}
	else if (value.contains(STR_COPY))
	{
		return QIcon(":/resource/browser-icon16-copy@3x.png");
	}
	else if (value.contains(STR_SEARCH_COVERART))
	{
		return QIcon(":/resource/play-popup-icon16-searchcoverart@3x.png");
	}
	else if (value.contains(STR_EDIT_TAG))
	{
		return QIcon(":/resource/browser-icon16-settag@3x.png");
	}
	else if (value.contains(STR_SEARCH_ALL_N_DELETE))
	{
		return QIcon("");
	}
	else if (value.contains(STR_SEARCH_ALL))
	{
		return QIcon("");
	}
	else if (value.contains(STR_RESERVE_RECORD_LIST))
	{
		return QIcon("");
	}
	else
	{
		LogDebug("There is no icon corresponding to the string.");
		return QIcon("");
	}
}

