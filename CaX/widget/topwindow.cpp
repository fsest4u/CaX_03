#include "topwindow.h"
#include "ui_topwindow.h"

#include "dialog/commondialog.h"

#include "util/caxconstants.h"
#include "util/caxtranslate.h"
#include "util/log.h"
#include "util/utilnovatron.h"

TopWindow::TopWindow(QWidget *parent) :
	QWidget(parent),
	m_Menu(new QMenu(this)),
	ui(new Ui::TopWindow)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();
}

TopWindow::~TopWindow()
{
	disconnect(m_Menu, SIGNAL(triggered(QAction*)));

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
	if (index < 0 || count <= 0 || index >= count)
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
	m_Menu->clear();
}

void TopWindow::SetMenu(QMap<int, QString> list)
{
	QMap<int, QString>::iterator i;
	for (i = list.begin(); i!= list.end(); i++)
	{
		QIcon icon = UtilNovatron::GetMenuIcon(i.value());
		QAction *action = new QAction(icon, i.value(), this);
		action->setData(i.key());
		m_Menu->addAction(action);
	}

}

void TopWindow::ShowCBSearch(bool show)
{
	if (show)
	{
		ui->cbSearch->show();
	}
	else
	{
		QString keyword = ui->cbSearch->currentText().toUtf8().data();

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

void TopWindow::SlotMenu()
{
	emit SigMenu();
}

void TopWindow::SlotMenuAction(QAction *action)
{
	emit SigMenuAction(action->data().toInt());
}

//void TopWindow::SlotInputSearchKeyword(const QString keyword)
//{
//	LogDebug("keyword [%s]", keyword.toUtf8().data());
//}

void TopWindow::ConnectSigToSlot()
{
	connect(ui->btnMenu, SIGNAL(pressed()), this, SLOT(SlotMenu()));
	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuAction(QAction*)));
//	connect(ui->cbSearch, SIGNAL(editTextChanged(const QString)), this, SLOT(SlotInputSearchKeyword(const QString)));
//	connect(ui->cbSearch, SIGNAL(currentTextChanged(const QString)), this, SLOT(SlotInputSearchKeyword(const QString)));
}

void TopWindow::Initialize()
{

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

	ui->cbSearch->hide();
}
