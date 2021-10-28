#include "topwindow.h"
#include "ui_topwindow.h"

#include "util/caxtranslate.h"

TopWindow::TopWindow(QWidget *parent) :
	QWidget(parent),
	m_Menu(new QMenu(this)),
	ui(new Ui::TopWindow)
{
	ui->setupUi(this);

	Initialize();
}

TopWindow::~TopWindow()
{
	if (m_Menu)
	{
		delete m_Menu;
		m_Menu = nullptr;
	}

	delete ui;
}

void TopWindow::AddTitle(QString title)
{
	m_TitleList.append(title);
}

void TopWindow::RemoveTitle()
{
	m_TitleList.removeLast();
}

void TopWindow::SetTitle(int index)
{
	int count = m_TitleList.count();
	if (count <= index)
	{
		return;
	}
	QString title = m_TitleList.at(index);
	ui->labelTitle->setText(title);
}

QPushButton *TopWindow::GetBtnMenu()
{
	return ui->btnMenu;
}

QPushButton *TopWindow::GetBtnHome()
{
	return ui->btnHome;
}

QPushButton *TopWindow::GetBtnPrev()
{
	return ui->btnPrev;
}

QPushButton *TopWindow::GetBtnNext()
{
	return ui->btnNext;
}

QPushButton *TopWindow::GetBtnSearch()
{
	return ui->btnSearch;
}

void TopWindow::ClearMenu()
{
	disconnect(m_Menu, SIGNAL(triggered(QAction*)));
	m_Menu->clear();
}

void TopWindow::SetMenu(QMap<int, QString> list)
{
	QMap<int, QString>::iterator i;
	for (i = list.begin(); i!= list.end(); i++)
	{
		QIcon icon = GetIcon(i.value());
		QAction *action = new QAction(icon, i.value(), this);
		action->setData(i.key());
		m_Menu->addAction(action);
	}
	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuAction(QAction*)));

}

void TopWindow::SlotMenu()
{
	emit SigMenu();
}

void TopWindow::SlotMenuAction(QAction *action)
{
	emit SigMenuAction(action->data().toInt());
}

void TopWindow::Initialize()
{
	connect(ui->btnMenu, SIGNAL(pressed()), this, SLOT(SlotMenu()));

	QString style = QString("QMenu::icon {	\
								padding: 0px 0px 0px 20px;	\
							}	\
							QMenu::item {	\
								width: 250px;	\
								height: 40px;	\
								color: rgb(255, 255, 255);	\
								font-size: 16pt;	\
								padding: 0px 20px 0px 20px;	\
							}	\
							QMenu::item:selected {	\
								background: rgba(201,237,248,255);	\
							}");
	m_Menu->setStyleSheet(style);
	ui->btnMenu->setMenu(m_Menu);

	m_TitleList.clear();

}

QIcon TopWindow::GetIcon(QString value)
{
//	if (value.contains(STR_MUSIC_DB))
//	{
//		return QIcon(":/resource/submenu-icon40-musicdb@3x.png");
//	}
//	else if (value.contains(STR_AUDIO_CD))
//	{
//		return QIcon(":/resource/submenu-icon40-audiocd@3x.png");
//	}
//	else if (value.contains(STR_PLAYLIST))
//	{
//		return QIcon(":/resource/submenu-icon40-playlist@3x.png");
//	}
//	else if (value.contains(STR_BROWSER))
//	{
//		return QIcon(":/resource/submenu-icon40-browser@3x.png");
//	}
//	else if (value.contains(STR_ISERVICE))
//	{
//		return QIcon(":/resource/submenu-icon40-internetservice@3x.png");
//	}
//	else if (value.contains(STR_INPUT))
//	{
//		return QIcon(":/resource/submenu-icon40-input@3x.png");
//	}
//	else if (value.contains(STR_FM_RADIO))
//	{
//		return QIcon(":/resource/submenu-icon40-fmradio@3x.png");
//	}
//	else if (value.contains(STR_DAB_RADIO))
//	{
//		return QIcon(":/resource/submenu-icon40-davradio@3x.png");
//	}
//	else if (value.contains(STR_GROUP_PLAY))
//	{
//		return QIcon(":/resource/submenu-icon40-groupplay@3x.png");
//	}
//	else if (value.contains(STR_SETUP))
//	{
//		return QIcon(":/resource/submenu-icon40-setup@3x.png");
//	}
//	else if (value.contains(STR_SELECT_DEVICE))
//	{
//		return QIcon(":/resource/submenu-icon40-selectdevice@3x.png");
//	}
//	else if (value.contains(STR_POWER_ON))
//	{
//		return QIcon(":/resource/submenu-icon40-wolpoweron@3x.png");
//	}
//	else if (value.contains(STR_POWER_OFF))
//	{
//		return QIcon(":/resource/submenu-icon40-poweroff@3x.png");
//	}
//	else if (value.contains(STR_ABOUT))
//	{
//		return QIcon(":/resource/submenu-icon40-aboutmusicx@3x.png");
//	}
//	else
	{
		return QIcon("");
	}
}
