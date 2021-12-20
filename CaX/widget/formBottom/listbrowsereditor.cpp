#include <QMouseEvent>

#include "listbrowsereditor.h"
#include "ui_listbrowsereditor.h"

#include "util/CJsonNode.h"
#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/caxtranslate.h"
#include "util/log.h"
#include "util/utilnovatron.h"

#include "widget/airable.h"
#include "widget/browser.h"
#include "widget/qobuz.h"

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

	if (SIDEMENU_BROWSER == m_Service)
	{
		ui->frameMenu->show();

		if (iFolderType_Mask_Play_Select & m_Type)
		{
			ui->labelPlay->show();
		}
		else
		{
			ui->labelPlay->hide();
		}
	}
	else if (SIDEMENU_ISERVICE == m_Service)
	{
		if (m_Type & iAirableType_Mask_Sub)
		{
			ui->horizontalSpacerIndent->changeSize(LIST_BROWSER_INDENT_1, LIST_BROWSER_HEIGHT);
		}
		else
		{
			ui->horizontalSpacerIndent->changeSize(LIST_BROWSER_INDENT_0, LIST_BROWSER_HEIGHT);
		}

		if (m_Type & iAirableType_Mask_Art)
		{
			ui->frameMenu->show();
		}
		else
		{
			ui->frameMenu->hide();
		}

		if (m_Type & iAirableType_Mask_Track
				|| m_Type & iAirableType_Mask_Program
				|| m_Type & iAirableType_Mask_Radio
				|| m_Type & iAirableType_Mask_Feed
				|| m_Type & iAirableType_Mask_Play)
		{
			ui->labelPlay->show();
		}
		else
		{
			ui->labelPlay->hide();
		}
	}
	else	// qobuz
	{
		if ((iQobuzType_Mask_UserPlaylist | iQobuzType_Mask_Playlist) == m_Type)
		{
			ui->frameMenu->show();
		}
		else
		{
			ui->frameMenu->hide();
		}

		if (m_Type & iQobuzType_Mask_Track)
		{
			ui->labelPlay->show();
		}
		else
		{
			ui->labelPlay->hide();
		}
	}
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

int ListBrowserEditor::GetService() const
{
	return m_Service;
}

void ListBrowserEditor::SetService(int Service)
{
	m_Service = Service;
}


void ListBrowserEditor::ClearMenu()
{
	m_Menu->clear();
}

void ListBrowserEditor::SetMenu(QMap<int, QString> map)
{
//	if (map.count() > 0)
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
//	else
//	{
//		ui->frameMenu->hide();
//	}
}

FormCoverArt *ListBrowserEditor::GetFormCoverArt() const
{
	return m_pFormCoverArt;
}

bool ListBrowserEditor::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		CJsonNode node;
		if (!node.SetContent(m_RawData))
		{
			return false;
		}
		node.AddInt(KEY_ID_LOWER, m_ID);
		node.AddString(KEY_ART, m_pFormCoverArt->GetCoverArt());

		if (object == ui->labelPlay)
		{
			emit SigClickPlay(m_Type, node);
		}
		else if (object == ui->labelTitle)
		{
			emit SigClickTitle(m_Type, node);
		}
	}

	return QObject::eventFilter(object, event);
}

void ListBrowserEditor::SlotMenu()
{
	if (SIDEMENU_BROWSER == m_Service)
	{
		emit SigMenu(m_pFormCoverArt->GetIndex(), m_Type);
	}
	else if (SIDEMENU_ISERVICE == m_Service)
	{
		CJsonNode node;
		if (!node.SetContent(m_RawData))
		{
			LogCritical("invalid json");
			return;
		}
		CJsonNode acts = node.GetArray(KEY_ACTS);

		if (acts.ArraySize() <= 0)
		{
			LogCritical("array node is empty");
			return;
		}

		CJsonNode act = acts.GetArrayAt(0);
		QString name = act.GetString(KEY_NAME_CAP);
		if (!name.isEmpty())
		{
			emit SigMenu(m_pFormCoverArt->GetIndex(), m_Type, name);
		}
	}
	else
	{
		emit SigMenu(m_pFormCoverArt->GetIndex(), m_Type);
	}
}

void ListBrowserEditor::SlotMenuAction(QAction *action)
{
	if (SIDEMENU_BROWSER == m_Service)
	{
		CJsonNode node;
		if (!node.SetContent(m_RawData))
		{
			return;
		}
		node.AddInt(KEY_ID_LOWER, m_ID);

		emit SigMenuAction(node, m_Type, action->data().toInt());
	}
	else if (SIDEMENU_ISERVICE == m_Service)
	{
		CJsonNode node;
		if (!node.SetContent(m_RawData))
		{
			LogCritical("invalid json");
			return;
		}
		CJsonNode acts = node.GetArray(KEY_ACTS);

		if (acts.ArraySize() <= 0)
		{
			LogCritical("array node is empty");
			return;
		}

		CJsonNode act = acts.GetArrayAt(0);
		QString url = act.GetString(KEY_URL);
		if (!url.isEmpty())
		{
			emit SigMenuAction(url, m_Type, action->data().toInt());
		}
	}
	else
	{
		CJsonNode node;
		if (!node.SetContent(m_RawData))
		{
			return;
		}
		QString id = node.GetString(KEY_ID_UPPER);
		emit SigMenuAction(id, m_Type, action->data().toInt());
	}
}

void ListBrowserEditor::SlotCoverArt(int index)
{
	emit SigClickCoverArt(index);
}

void ListBrowserEditor::ConnectSigToSlot()
{
	connect(ui->btnMenu, SIGNAL(pressed()), this, SLOT(SlotMenu()));
	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuAction(QAction*)));

	connect(m_pFormCoverArt, SIGNAL(SigCoverArt(int)), this, SLOT(SlotCoverArt(int)));

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

	ui->labelPlay->hide();
	ui->labelSubtitle->hide();
	ui->labelDuration->hide();
	ui->labelFilesize->hide();
	ui->frameMenu->hide();

	ui->labelPlay->installEventFilter(this);
	ui->labelTitle->installEventFilter(this);

	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);
}

