#include <QKeyEvent>
#include <QRadioButton>
#include <QWidgetAction>

#include "topwindow.h"
#include "ui_topwindow.h"

#include "dialog/commondialog.h"

#include "util/caxconstants.h"
#include "util/caxtranslate.h"
#include "util/log.h"
#include "util/utilnovatron.h"

TopWindow::TopWindow(QWidget *parent) :
	QWidget(parent),
	m_SideMenu(new QMenu(this)),
	m_DeviceMenu(new QMenu(this)),
	m_DeviceName(""),
	ui(new Ui::TopWindow)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();
}

TopWindow::~TopWindow()
{
	disconnect(m_SideMenu, SIGNAL(triggered(QAction*)));

	if (m_SideMenu)
	{
		delete m_SideMenu;
		m_SideMenu = nullptr;
	}

	disconnect(m_DeviceMenu, SIGNAL(triggered(QAction*)));

	if (m_DeviceMenu)
	{
		delete m_DeviceMenu;
		m_DeviceMenu = nullptr;
	}

	delete ui;
}

void TopWindow::AddTitle(int index, QString title)
{
	m_TitleList.insert(index, title);
//	for (int i = 0; i < m_TitleList.count(); i++)
//	{
//		LogDebug("add title [%d] [%s]", i, m_TitleList.at(i).toUtf8().data());
//	}
}

void TopWindow::RemoveTitle()
{
	if (m_TitleList.count() > 0)
	{
		m_TitleList.removeLast();
	}
//	for (int i = 0; i < m_TitleList.count(); i++)
//	{
//		LogDebug("remove title [%d] [%s]", i, m_TitleList.at(i).toUtf8().data());
//	}
}

void TopWindow::SetTitle(int index)
{
	int count = m_TitleList.count();
//	LogDebug("set title index [%d] count [%d]", index, count);

	if (index < 0 || count <= 0 || index >= count)
	{
//		LogDebug("return set title");
		return;
	}

	QString title = m_TitleList.at(index);
//	LogDebug("title [%s]", title.toUtf8().data());
	ui->labelTitle->setText(title);
}

QPushButton *TopWindow::GetBtnSideMenu()
{
	return ui->btnSideMenu;
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

void TopWindow::ClearSideMenu()
{
	m_SideMenu->clear();
}

void TopWindow::SetSideMenu(QMap<int, QString> map)
{
	QMap<int, QString>::iterator i;
	for (i = map.begin(); i!= map.end(); i++)
	{
		QString icon = UtilNovatron::GetSideMenuIcon(i.value());

		QString style = QString("QRadioButton::indicator {	\
									width: 40px;	\
									height: 40px;	\
								}	\
								QRadioButton::indicator:unchecked {	\
									image: url(\"%1\");	\
								}	\
								QRadioButton::indicator:checked {	\
									image: url(\"%1\");	\
								}	\
								QRadioButton {	\
									padding: 0px 10px 0px 30px;\
									width: 250px;   \
									height: 40px;   \
									color: rgb(255, 255, 255);  \
									font: 16pt \"Segoe UI\";	\
									spacing: 15px;	\
								}	\
								QRadioButton:hover {	\
									background: rgba(201,237,248,255);  \
								}").arg(icon);

		QRadioButton *button=new QRadioButton(m_SideMenu);
		button->setFixedSize(250, 40);
		button->setText(i.value());
		button->setStyleSheet(style);
		QWidgetAction *action=new QWidgetAction(this);
		action->setDefaultWidget(button);
		m_SideMenu->addAction(action);

		switch (i.key())
		{
		case SIDEMENU_MUSIC_DB:
			connect(button, SIGNAL(clicked()), this, SLOT(SlotSideMenuMusicDB()));
			break;
		case SIDEMENU_AUDIO_CD:
			connect(button, SIGNAL(clicked()), this, SLOT(SlotSideMenuAudioCD()));
			break;
		case SIDEMENU_PLAYLIST:
			connect(button, SIGNAL(clicked()), this, SLOT(SlotSideMenuPlaylist()));
			break;
		case SIDEMENU_BROWSER:
			connect(button, SIGNAL(clicked()), this, SLOT(SlotSideMenuBrowser()));
			break;
		case SIDEMENU_ISERVICE:
			connect(button, SIGNAL(clicked()), this, SLOT(SlotSideMenuIService()));
			break;
		case SIDEMENU_INPUT:
			connect(button, SIGNAL(clicked()), this, SLOT(SlotSideMenuInput()));
			break;
		case SIDEMENU_FM_RADIO:
			connect(button, SIGNAL(clicked()), this, SLOT(SlotSideMenuFmRadio()));
			break;
		case SIDEMENU_DAB_RADIO:
			connect(button, SIGNAL(clicked()), this, SLOT(SlotSideMenuDabRadio()));
			break;
		case SIDEMENU_GROUP_PLAY:
			connect(button, SIGNAL(clicked()), this, SLOT(SlotSideMenuGroupPlay()));
			break;
		case SIDEMENU_SETUP:
			connect(button, SIGNAL(clicked()), this, SLOT(SlotSideMenuSetup()));
			break;
		case SIDEMENU_SELECT_DEVICE:
			connect(button, SIGNAL(clicked()), this, SLOT(SlotSideMenuSelectDevice()));
			break;
		case SIDEMENU_POWER_ON:
			connect(button, SIGNAL(clicked()), this, SLOT(SlotSideMenuPowerOn()));
			break;
		case SIDEMENU_POWER_OFF:
			connect(button, SIGNAL(clicked()), this, SLOT(SlotSideMenuPowerOff()));
			break;
		case SIDEMENU_ABOUT:
			connect(button, SIGNAL(clicked()), this, SLOT(SlotSideMenuAbout()));
			break;
		}
	}

}

void TopWindow::ClearDeviceMenu()
{
	m_DeviceMenu->clear();

}

void TopWindow::SetDeviceMenu(QMap<QString, QString> map)
{

	QMap<QString, QString>::iterator i;
	for (i = map.begin(); i != map.end(); i++)
	{
		QAction *action = new QAction(i.value(), this);
		action->setData(i.key());
		m_DeviceMenu->addAction(action);
	}
}

QString TopWindow::GetDeviceName() const
{
	return m_DeviceName;
}

void TopWindow::SetDeviceName(const QString &DeviceName)
{
	m_DeviceName = DeviceName;
	ui->btnDeviceMenu->setText(m_DeviceName);
}

void TopWindow::ShowCBSearch(bool show)
{
	if (show)
	{
		ui->cbSearch->show();
	}
	else
	{
		QString keyword = ui->cbSearch->currentText();

		if (keyword.length() < SEARCH_WORD_LIMIT_COUNT)
		{
			CommonDialog dialog(this, STR_WARNING, STR_ENTER_SEARCH_WORD);
			dialog.exec();
			return;
		}
		ui->cbSearch->addItem(keyword);
		ui->cbSearch->hide();

		emit SigSearchKeyword(keyword);
	}
}

bool TopWindow::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent* key = static_cast<QKeyEvent*>(event);
		if ( (key->key()==Qt::Key_Enter) || (key->key()==Qt::Key_Return) )
		{
			if (object == ui->cbSearch)
			{
				ShowCBSearch(false);
			}
		}
	}

	return QObject::eventFilter(object, event);
}

//void TopWindow::ClearCBSearch()
//{
//	ui->cbSearch->clear();
//}

//void TopWindow::SetCBSearch(QStringList list)
//{
//	int count = list.count();
//	for (int i = 0; i < count; i++)
//	{
//		ui->cbSearch->setItemText(i, list.at(i));
//	}
//}

//QStringList TopWindow::GetCBSearch()
//{
//	int count = ui->cbSearch->count();
//	QStringList list;

//	for (int i = 0; i < count; i++)
//	{
//		list.append(ui->cbSearch->itemText(i));
//	}

//	return list;
//}

void TopWindow::SlotSideMenu()
{
	emit SigSideMenu();
}

void TopWindow::SlotSideMenuAction(QAction *action)
{
	emit SigSideMenuAction(action->data().toInt());
}

void TopWindow::SlotDeviceMenu()
{
	emit SigDeviceMenu();
}

void TopWindow::SlotDeviceMenuAction(QAction *action)
{
	emit SigDeviceMenuAction(action->data().toString());
}

void TopWindow::SlotSideMenuMusicDB()
{
	m_SideMenu->close();
	emit SigSideMenuAction(SIDEMENU_MUSIC_DB);
}

void TopWindow::SlotSideMenuAudioCD()
{
	m_SideMenu->close();
	emit SigSideMenuAction(SIDEMENU_AUDIO_CD);
}

void TopWindow::SlotSideMenuPlaylist()
{
	m_SideMenu->close();
	emit SigSideMenuAction(SIDEMENU_PLAYLIST);
}

void TopWindow::SlotSideMenuBrowser()
{
	m_SideMenu->close();
	emit SigSideMenuAction(SIDEMENU_BROWSER);
}

void TopWindow::SlotSideMenuIService()
{
	m_SideMenu->close();
	emit SigSideMenuAction(SIDEMENU_ISERVICE);
}

void TopWindow::SlotSideMenuInput()
{
	m_SideMenu->close();
	emit SigSideMenuAction(SIDEMENU_INPUT);
}

void TopWindow::SlotSideMenuFmRadio()
{
	m_SideMenu->close();
	emit SigSideMenuAction(SIDEMENU_FM_RADIO);
}

void TopWindow::SlotSideMenuDabRadio()
{
	m_SideMenu->close();
	emit SigSideMenuAction(SIDEMENU_DAB_RADIO);
}

void TopWindow::SlotSideMenuGroupPlay()
{
	m_SideMenu->close();
	emit SigSideMenuAction(SIDEMENU_GROUP_PLAY);
}

void TopWindow::SlotSideMenuSetup()
{
	m_SideMenu->close();
	emit SigSideMenuAction(SIDEMENU_SETUP);
}

void TopWindow::SlotSideMenuSelectDevice()
{
	m_SideMenu->close();
	emit SigSideMenuAction(SIDEMENU_SELECT_DEVICE);
}

void TopWindow::SlotSideMenuPowerOn()
{
	m_SideMenu->close();
	emit SigSideMenuAction(SIDEMENU_POWER_ON);
}

void TopWindow::SlotSideMenuPowerOff()
{
	m_SideMenu->close();
	emit SigSideMenuAction(SIDEMENU_POWER_OFF);
}

void TopWindow::SlotSideMenuAbout()
{
	m_SideMenu->close();
	emit SigSideMenuAction(SIDEMENU_ABOUT);
}

void TopWindow::ConnectSigToSlot()
{
	connect(ui->btnSideMenu, SIGNAL(pressed()), this, SLOT(SlotSideMenu()));
//	connect(m_SideMenu, SIGNAL(triggered(QAction*)), this, SLOT(SlotSideMenuAction(QAction*)));
	connect(ui->btnDeviceMenu, SIGNAL(pressed()), this, SLOT(SlotDeviceMenu()));
	connect(m_DeviceMenu, SIGNAL(triggered(QAction*)), this, SLOT(SlotDeviceMenuAction(QAction*)));
}

void TopWindow::Initialize()
{

	ui->btnSideMenu->setMenu(m_SideMenu);

	QString styleDevice = QString("QMenu {	\
								  background-color: rgb(0, 0, 0);	\
							  }	\
							  QMenu::item {	\
								  width: 160px;	\
								  height: 40px;	\
								  color: rgb(174,176,179);	\
								  font-size: 16pt;	\
								  padding: 0px 20px 0px 20px;	\
							  }	\
							  QMenu::item:selected {	\
								  background: rgba(238,238,238,255);	\
							  }");

	m_DeviceMenu->setStyleSheet(styleDevice);
	ui->btnDeviceMenu->setMenu(m_DeviceMenu);


	m_TitleList.clear();

	ui->cbSearch->hide();
	ui->cbSearch->installEventFilter(this);

}

