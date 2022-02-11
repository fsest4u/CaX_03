#include "setupwindow.h"
#include "ui_setupwindow.h"

#include "dialog/commondialog.h"
#include "dialog/formdialog.h"

#include "manager/setupmanager.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/utilnovatron.h"

#include "widget/setup.h"
#include "widget/formTop/infoservice.h"
#include "widget/formBottom/listsetup.h"
#include "widget/formBottom/listsetupdelegate.h"

#define MAIN_TITLE	"Setup"


SetupWindow::SetupWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new SetupManager),
	m_pInfoService(new InfoService(this)),
	m_pListSetup(new ListSetup(this)),\
	m_StrID(""),
	m_Menu(new QMenu(this)),
	ui(new Ui::SetupWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
	Initialize();
}

SetupWindow::~SetupWindow()
{
	if (m_pMgr)
	{
		delete m_pMgr;
		m_pMgr = nullptr;
	}

	if (m_pInfoService)
	{
		delete m_pInfoService;
		m_pInfoService = nullptr;
	}

	if (m_pListSetup)
	{
		delete m_pListSetup;
		m_pListSetup = nullptr;
	}

	disconnect(m_Menu, SIGNAL(triggered(QAction*)));
	if (m_Menu)
	{
		delete m_Menu;
		m_Menu = nullptr;
	}

	delete ui;

}

void SetupWindow::SetupHome(QList<CJsonNode> list, int eventID)
{
	m_EventID = eventID;

	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pListSetup);

//	SetSetupHome(list);

	m_pInfoService->SetTitle(MAIN_TITLE);
	m_pListSetup->SetNodeList(list);
}

void SetupWindow::SlotSelectMenu(const QModelIndex &modelIndex, QPoint point)
{
	LogDebug("setup main menu");
	m_ModelIndex = modelIndex;
	m_Point = point;

	QString strID = qvariant_cast<QString>(m_ModelIndex.data(ListSetupDelegate::LIST_SETUP_ID));
	int index = qvariant_cast<int>(m_ModelIndex.data(ListSetupDelegate::LIST_SETUP_INDEX));

	m_pMgr->RequestSetupGroup(m_EventID, strID, index);
}

void SetupWindow::SlotSelectMenuSub(const QModelIndex &modelIndex, QPoint point)
{
	LogDebug("setup sub menu");
	m_ModelIndex = modelIndex;
	m_Point = point;

	QStringList values = qvariant_cast<QStringList>(m_ModelIndex.data(ListSetupDelegate::LIST_SETUP_VALUES));
	if (values.isEmpty())
	{
		QString strID = qvariant_cast<QString>(m_ModelIndex.data(ListSetupDelegate::LIST_SETUP_ID));
		m_pMgr->RequestSetupSet(m_EventID, strID);
	}
	else
	{
		SetMenuMap(values);

		m_Menu->clear();

		QMap<QString, QString>::iterator i;
		for (i = m_MenuMap.begin(); i != m_MenuMap.end(); i++)
		{
			QAction *action = new QAction(i.key(), this);
			action->setData(i.value());
			m_Menu->addAction(action);
		}

		m_Menu->popup(m_pListSetup->GetListViewSub()->viewport()->mapToGlobal(m_Point));
	}
}

void SetupWindow::SlotSelectMenuDetail(const QModelIndex &modelIndex, QPoint point)
{
	LogDebug("setup detail menu");
}

void SetupWindow::SlotMenuAction(QAction *action)
{
	QString rawData = action->data().toString();
	LogDebug("rawData [%s]", rawData.toUtf8().data());
}

//void SetupWindow::SlotSelectTitle(QString strID, int index)
//{
//	m_pMgr->RequestSetupGroup(m_EventID, strID, index);
//}

//void SetupWindow::SlotMenuAction(QString strID, QString json)
//{
//	LogDebug("strID [%s] json [%s] ", strID.toUtf8().data(), json.toUtf8().data());
//	CJsonNode node;
//	if (!node.SetContent(json))
//	{
//		return;
//	}

//	m_StrID = node.GetString(KEY_ID_UPPER);

//	int type = node.GetInt(KEY_TYPE);
//	UtilNovatron::DebugTypeForSetup("SlotMenuAction", type);

//	CJsonNode form = node.GetObject(KEY_FORM);
//	if (form.IsNull())
//	{
//		LogDebug("menu is not form~~");

//		if (type & iAppSetupType_Mask_Exec)
//		{
//			m_pMgr->RequestSetupSet(m_EventID, m_StrID);
//		}
//		else if (type & iAppSetupType_Mask_Event)
//		{
//			m_pMgr->RequestSetupSet(m_EventID, m_StrID);
//		}
//	}
//	else
//	{
//		LogDebug("menu is form~~");
//		QString title = form.GetString(KEY_TITLE_CAP);
//		CJsonNode nodeOK = form.GetObject(KEY_OK);
//		CJsonNode nodeCancel = form.GetObject(KEY_CANCEL);
//		CJsonNode arrInput = form.GetArray(KEY_INPUTS);

//		FormDialog dialog;
//		dialog.SetWindowTitle(title);
//		dialog.SetOK(nodeOK.GetString(KEY_NAME_CAP), nodeOK.GetInt(KEY_ACTION));
//		dialog.SetCancel(nodeCancel.GetString(KEY_NAME_CAP), nodeCancel.GetInt(KEY_ACTION));
//		dialog.SetLabelTitle(arrInput);
//		if (dialog.exec() == QDialog::Accepted)
//		{
//			m_pMgr->RequestSetupSet(m_EventID, m_StrID, true);
//		}
//	}
//}

//void SetupWindow::SlotSubMenuAction(QString value, QString json)
//{
//	LogDebug("value [%s] json [%s] ", value.toUtf8().data(), json.toUtf8().data());
//	CJsonNode node;
//	if (!node.SetContent(json))
//	{
//		return;
//	}
//	int type = node.GetInt(KEY_TYPE);
//	UtilNovatron::DebugTypeForSetup("SlotSubMenuAction", type);

//	if (type & iAppSetupType_Mask_Enum)
//	{
//		QString id = node.GetString(KEY_ID_UPPER);
//		m_pMgr->RequestSetupSet(m_EventID, id, value);
//	}
//	else if (type & iAppSetupType_Mask_Select)
//	{
//		QString id = node.GetString(KEY_ID_UPPER);
//		m_pMgr->RequestSetupSet(m_EventID, id, value);
//	}
//}

void SetupWindow::SlotRespError(QString errMsg)
{
	CommonDialog dialog(this, STR_WARNING, errMsg);
	dialog.exec();
}

void SetupWindow::SlotRespGroup(QList<CJsonNode> list, int index)
{
	m_pListSetup->ClearNodeListSub();
	m_pListSetup->SetNodeListSub(list);
}

void SetupWindow::SlotRespSet(CJsonNode node)
{
	LogDebug("node [%s]", node.ToCompactByteArray().data());
	CJsonNode nodeForm = node.GetObject(KEY_FORM);
	if (nodeForm.IsNull())
	{
		LogDebug("resp is not form~~");
	}
	else
	{
		LogDebug("resp is form~~");

		FormDialog dialog;
		dialog.SetNodeForm(nodeForm);
		if (dialog.exec() == QDialog::Accepted)
		{
			m_pMgr->RequestSetupSet(m_EventID, m_StrID, true);
		}
	}
}

void SetupWindow::ConnectSigToSlot()
{
//	connect(m_pListSetup->GetDelegate(), SIGNAL(SigSelectTitle(QString, int)), this, SLOT(SlotSelectTitle(QString, int)));
//	connect(m_pListSetup->GetDelegate(), SIGNAL(SigMenuAction(QString, QString)), this, SLOT(SlotMenuAction(QString, QString)));
//	connect(m_pListSetup->GetDelegate(), SIGNAL(SigSubMenuAction(QString, QString)), this, SLOT(SlotSubMenuAction(QString, QString)));
	connect(m_pListSetup->GetDelegate(), SIGNAL(SigSelectMenu(const QModelIndex&, QPoint)), this, SLOT(SlotSelectMenu(const QModelIndex&, QPoint)));
	connect(m_pListSetup->GetDelegateSub(), SIGNAL(SigSelectMenu(const QModelIndex&, QPoint)), this, SLOT(SlotSelectMenuSub(const QModelIndex&, QPoint)));
	connect(m_pListSetup->GetDelegateDetail(), SIGNAL(SigSelectMenu(const QModelIndex&, QPoint)), this, SLOT(SlotSelectMenuDetail(const QModelIndex&, QPoint)));

	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespGroup(QList<CJsonNode>, int)), this, SLOT(SlotRespGroup(QList<CJsonNode>, int)));
	connect(m_pMgr, SIGNAL(SigRespSet(CJsonNode)), this, SLOT(SlotRespSet(CJsonNode)));

	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuAction(QAction*)));

}

void SetupWindow::Initialize()
{
	m_MenuMap.clear();

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
}

void SetupWindow::SetMenuMap(QStringList values)
{
	m_MenuMap.clear();
	foreach (QString value, values)
	{
		LogDebug("value [%s]", value.toUtf8().data());
		m_MenuMap.insert(value, value);
	}
}

//void SetupWindow::SetSetupHome(QList<CJsonNode> &list)
//{
//	QList<CJsonNode> tempList;
//	int index = 0;
//	QString strCover = "";

//	foreach (CJsonNode node, list)
//	{
//		node.Add(KEY_COVER_ART, strCover);
//		node.AddInt(KEY_TYPE, index);

//		tempList.append(node);
//		index++;
//	}

//	list = tempList;
//}
