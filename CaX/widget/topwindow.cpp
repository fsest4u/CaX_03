#include <QKeyEvent>

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
}

void TopWindow::RemoveTitle()
{
	if (m_TitleList.count() > 0)
	{
		m_TitleList.removeLast();
	}
}

void TopWindow::SetTitle(int index)
{
	int count = m_TitleList.count();
	if (index < 0 || count <= 0 || index >= count)
	{
		return;
	}
	QString title = m_TitleList.at(index);
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
		QIcon icon = UtilNovatron::GetMenuIcon(i.value());
		QAction *action = new QAction(icon, i.value(), this);
		action->setData(i.key());
		m_SideMenu->addAction(action);
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

void TopWindow::ConnectSigToSlot()
{
	connect(ui->btnSideMenu, SIGNAL(pressed()), this, SLOT(SlotSideMenu()));
	connect(m_SideMenu, SIGNAL(triggered(QAction*)), this, SLOT(SlotSideMenuAction(QAction*)));
	connect(ui->btnDeviceMenu, SIGNAL(pressed()), this, SLOT(SlotDeviceMenu()));
	connect(m_DeviceMenu, SIGNAL(triggered(QAction*)), this, SLOT(SlotDeviceMenuAction(QAction*)));
}

void TopWindow::Initialize()
{

	QString styleSide = QString("QMenu::icon {	\
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
	m_SideMenu->setStyleSheet(styleSide);
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

