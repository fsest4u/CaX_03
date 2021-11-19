#include <QMouseEvent>

#include "listbrowsereditor.h"
#include "ui_listbrowsereditor.h"

#include "util/caxtranslate.h"
#include "util/log.h"
#include "util/utilnovatron.h"

#include "widget/form/formcoverart.h"

#define ICON_DIR		"dir"
#define ICON_SEARCH		"search"
#define ICON_REC_LIST	"rec_list"


ListBrowserEditor::ListBrowserEditor(QWidget *parent) :
	QWidget(parent),
	m_pFormCoverArt(new FormCoverArt(this)),
	m_Menu(new QMenu(this)),
	ui(new Ui::ListBrowserEditor)
{
	ui->setupUi(this);

	ConnectSigToSlot();

	Initialize();
}

ListBrowserEditor::~ListBrowserEditor()
{
	if (m_pFormCoverArt)
	{
		delete m_pFormCoverArt;
		m_pFormCoverArt = nullptr;
	}

	disconnect(m_Menu, SIGNAL(triggered(QAction*)));
	if (m_Menu)
	{
		delete m_Menu;
		m_Menu = nullptr;
	}


	delete ui;
}

int ListBrowserEditor::GetID() const
{
	return m_ID;
}

void ListBrowserEditor::SetID(int ID)
{
	m_ID = ID;
}

int ListBrowserEditor::GetType() const
{
	return m_Type;
}

void ListBrowserEditor::SetType(int Type)
{
	m_Type = Type;
}

QString ListBrowserEditor::GetTitle() const
{
	return ui->labelTitle->text();
}

void ListBrowserEditor::SetTitle(const QString &title)
{
	ui->labelTitle->setText(title);
}

QString ListBrowserEditor::GetSubtitle() const
{
	return ui->labelSubtitle->text();
}

void ListBrowserEditor::SetSubtitle(const QString &subtitle)
{
	ui->labelSubtitle->show();
	ui->labelSubtitle->setText(subtitle);
}

QString ListBrowserEditor::GetDuration() const
{
	return ui->labelDuration->text();
}

void ListBrowserEditor::SetDuration(const QString &duration)
{
	ui->labelDuration->show();
	ui->labelDuration->setText(duration);
}

QString ListBrowserEditor::GetFilesize() const
{
	return ui->labelFilesize->text();
}

void ListBrowserEditor::SetFileSize(const QString &filesize)
{
	ui->labelFilesize->show();
	ui->labelFilesize->setText(filesize);
}

QString ListBrowserEditor::GetRawData() const
{
	return m_RawData;
}

void ListBrowserEditor::SetRawData(const QString &rawData)
{
	m_RawData = rawData;
}

void ListBrowserEditor::ClearMenu()
{
	m_Menu->clear();
}

void ListBrowserEditor::SetMenu(QMap<int, QString> map)
{
	QMap<int, QString>::iterator i;
	for (i = map.begin(); i != map.end(); i++)
	{
		QIcon icon = UtilNovatron::GetMenuIcon(i.value());
		QAction *action = new QAction(icon, i.value(), this);
		action->setData(i.key());
		m_Menu->addAction(action);
	}
}

FormCoverArt *ListBrowserEditor::GetFormCoverArt() const
{
	return m_pFormCoverArt;
}

bool ListBrowserEditor::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->labelTitle)
		{
			emit SigClickTitle(m_Type, m_RawData);
		}
	}

	return QObject::eventFilter(object, event);
}

void ListBrowserEditor::SlotMenu()
{
	emit SigMenu(m_ID, m_Type);
}

void ListBrowserEditor::SlotMenuAction(QAction *action)
{
	emit SigMenuAction(ui->labelTitle->text(), m_Type, action->data().toInt());
}

void ListBrowserEditor::ConnectSigToSlot()
{
	connect(ui->btnMenu, SIGNAL(pressed()), this, SLOT(SlotMenu()));
	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuAction(QAction*)));
}

void ListBrowserEditor::Initialize()
{
	m_Type = -1;
	m_RawData.clear();

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

	ui->labelSubtitle->hide();
	ui->labelDuration->hide();
	ui->labelFilesize->hide();

	ui->labelTitle->installEventFilter(this);

	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);
}
