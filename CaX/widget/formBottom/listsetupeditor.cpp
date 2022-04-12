#include <QMouseEvent>

#include "listsetupeditor.h"
#include "ui_listsetupeditor.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

ListSetupEditor::ListSetupEditor(QWidget *parent) :
	QWidget(parent),
	m_Menu(new QMenu(this)),
	ui(new Ui::ListSetupEditor)
{
	ui->setupUi(this);

	ConnectSigToSlot();

	Initialize();
}

ListSetupEditor::~ListSetupEditor()
{
	disconnect(m_Menu, SIGNAL(triggered(QAction*)));
	if (m_Menu)
	{
		delete m_Menu;
		m_Menu = nullptr;
	}

	delete ui;
}

QString ListSetupEditor::GetStrID() const
{
	return m_StrID;
}

void ListSetupEditor::SetStrID(const QString &StrID)
{
	m_StrID = StrID;
}

QString ListSetupEditor::GetTitle() const
{
	return ui->btnTitle->text();
}

void ListSetupEditor::SetTitle(const QString &title)
{
	ui->btnTitle->setText(title);
}

int ListSetupEditor::GetIndex() const
{
	return m_Index;
}

void ListSetupEditor::SetIndex(int Index)
{
	m_Index = Index;
}

void ListSetupEditor::ClearMenu()
{
	m_Menu->clear();
}

void ListSetupEditor::SetMenu(QMap<QString, CJsonNode> map)
{

#if 1	//  sort??
	QMap<QString, CJsonNode>::iterator i;
	for (i = map.begin(); i != map.end(); i++)
	{
#else
	QMapIterator<QString, CJsonNode> i(map);
	i.toBack();
	while (i.hasPrevious())
	{
		i.previous();
#endif

		CJsonNode node = i.value();
//		LogDebug("node [%s]", node.ToCompactByteArray().data());

		QString name = node.GetString(KEY_NAME_CAP);
		QStringList values = node.GetStringList(KEY_VALUES);
		if (values.isEmpty())
		{
			QAction *action = new QAction(name, this);
			action->setData(node.ToCompactByteArray().data());
			m_Menu->addAction(action);
		}
		else
		{
			QMenu *subMenu = new QMenu(name, this);

			foreach (QString value, values)
			{
//				LogDebug("value [%s]", value.toUtf8().data());

				QAction *action = new QAction(value, this);
				action->setData(node.ToCompactByteArray().data());
				subMenu->addAction(action);
			}

			QString style = QString("QMenu::item {	\
										width: 300px;	\
										height: 40px;	\
										color: rgb(90, 91, 94);	\
										font-size: 12pt;	\
										padding: 0px 20px 0px 20px;	\
									}	\
									QMenu::item:selected {	\
										background: rgba(201,237,248,255);	\
									}");

			subMenu->setStyleSheet(style);

			m_Menu->addAction(m_Menu->addMenu(subMenu));
			connect(subMenu, SIGNAL(triggered(QAction*)), this, SLOT(SlotSubMenuAction(QAction*)));

		}
	}
}

void ListSetupEditor::SlotClickTitle()
{
	emit SigClickTitle(m_StrID, m_Index);
}

void ListSetupEditor::SlotMenuAction(QAction *action)
{
	if (m_bSubMenu)
	{
		m_bSubMenu = false;
	}
	else
	{
//		LogDebug("menu action strID [%s] data [%s]", m_StrID.toUtf8().data(), action->data().toString().toUtf8().data());
		emit SigMenuAction(m_StrID, action->data().toString());
	}
}

void ListSetupEditor::SlotSubMenuAction(QAction *action)
{
	disconnect(m_Menu, SIGNAL(triggered(QAction*)));
	m_bSubMenu = true;

//	LogDebug("sub menu action strID [%s] data [%s]", action->text().toUtf8().data(), action->data().toString().toUtf8().data());
	emit SigSubMenuAction(action->text(), action->data().toString());
}

//bool ListSetupEditor::eventFilter(QObject *object, QEvent *event)
//{
//	if (event->type() == QMouseEvent::MouseButtonPress)
//	{
//		if (object == ui->btnTitle)
//		{
//			emit SigClickTitle(m_StrID);
//		}
//	}

//	return QObject::eventFilter(object, event);
//}

void ListSetupEditor::ConnectSigToSlot()
{
//	ui->btnTitle->installEventFilter(this);
	connect(ui->btnTitle, SIGNAL(pressed()), this, SLOT(SlotClickTitle()));
	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuAction(QAction*)));
}

void ListSetupEditor::Initialize()
{
	QString style = QString("QMenu::item {	\
								width: 300px;	\
								height: 40px;	\
								color: rgb(90, 91, 94);	\
								font-size: 12pt;	\
								padding: 0px 20px 0px 20px;	\
							}	\
							QMenu::item:selected {	\
								background: rgba(201,237,248,255);	\
							}");

	m_Menu->setStyleSheet(style);
	ui->btnTitle->setMenu(m_Menu);

	m_bSubMenu = false;
}

void ListSetupEditor::AddAction()
{

}

void ListSetupEditor::AddMenu()
{

}
