#include "setupwindow.h"
#include "ui_setupwindow.h"

#include "dialog/commondialog.h"
#include "dialog/setup/analoginvolumedialog.h"
#include "dialog/setup/formdialog.h"
#include "dialog/setup/maxvolumedialog.h"
#include "dialog/setup/poweronvolumedialog.h"
#include "dialog/setup/setuplogindialog.h"

#include "manager/setupmanager.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/settingio.h"
#include "util/utilnovatron.h"

#include "widget/setup.h"
#include "widget/formTop/infoservice.h"
#include "widget/formBottom/listsetup.h"
#include "widget/formBottom/listsetupdelegate.h"

const QString SETTINGS_GROUP = "Setup";

SetupWindow::SetupWindow(QWidget *parent, const QString &addr) :
	QWidget(parent),
	m_pMgr(new SetupManager),
	m_pInfoService(new InfoService(this)),
	m_pListSetup(new ListSetup(this)),\
	m_StrID(""),
	m_StrIDSub(""),
	m_MenuSub(new QMenu(this)),
	ui(new Ui::SetupWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
	ReadSettings();
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

	disconnect(m_MenuSub, SIGNAL(triggered(QAction*)));
	if (m_MenuSub)
	{
		delete m_MenuSub;
		m_MenuSub = nullptr;
	}

	delete ui;

}

void SetupWindow::SetupHome(QList<CJsonNode> list, int eventID)
{
	m_EventID = eventID;

	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pListSetup);

	m_pInfoService->SetTitle(SETTINGS_GROUP);
	m_pListSetup->SetNodeList(list);
}

void SetupWindow::SlotSelectMenu(const QModelIndex &modelIndex, QPoint point)
{
	LogDebug("setup main menu");
//	m_ModelIndex = modelIndex;
//	m_Point = point;

	m_StrID = qvariant_cast<QString>(modelIndex.data(ListSetupDelegate::LIST_SETUP_ID));

	m_pMgr->RequestSetupGroup(m_EventID, m_StrID);
}

void SetupWindow::SlotSelectMenuSub(const QModelIndex &modelIndex, QPoint point)
{
	LogDebug("setup sub menu");
	m_ModelIndex = modelIndex;
	m_Point = point;

	m_StrIDSub = qvariant_cast<QString>(m_ModelIndex.data(ListSetupDelegate::LIST_SETUP_ID));
	int type = qvariant_cast<int>(m_ModelIndex.data(ListSetupDelegate::LIST_SETUP_TYPE));
	UtilNovatron::DebugTypeForSetup("SlotSelectMenuSub", type);

	if (type & iAppSetupType_Mask_Enum
			|| type & iAppSetupType_Mask_Select)
	{
		QString json = qvariant_cast<QString>(m_ModelIndex.data(ListSetupDelegate::LIST_SETUP_RAW));
		CJsonNode node;
		if (!node.SetContent(json))
		{
			return;
		}

		QStringList values = node.GetStringList(KEY_VALUES);
		SetMenuSubMap(values);

		m_MenuSub->clear();

		QMap<int, QString>::iterator i;
		for (i = m_MenuSubMap.begin(); i != m_MenuSubMap.end(); i++)
		{
			QAction *action = new QAction(i.value(), this);
			action->setData(i.value());
			m_MenuSub->addAction(action);
		}

		m_MenuSub->popup(m_pListSetup->GetListViewSub()->viewport()->mapToGlobal(m_Point));
	}
	else if (type & iAppSetupType_Mask_List)
	{

	}
	else if (type & iAppSetupType_Mask_FormSelect)
	{

	}
	else if (type & iAppSetupType_Mask_Exec
			 || type & iAppSetupType_Mask_App)
	{
		QString json = qvariant_cast<QString>(m_ModelIndex.data(ListSetupDelegate::LIST_SETUP_RAW));
		CJsonNode node;
		if (!node.SetContent(json))
		{
			return;
		}

		CJsonNode nodeForm = node.GetObject(KEY_FORM);

		if (nodeForm.IsNull())
		{
			LogDebug("form is null");
			if (m_StrIDSub.contains("MaxVolume"))
			{
				int volume = qvariant_cast<int>(m_ModelIndex.data(ListSetupDelegate::LIST_SETUP_SUBTITLE));

				DoMaxVolume(volume);
			}

		}
		else
		{
			LogDebug("form is exist");
			if (m_StrIDSub.contains("AUD_ANALOG_IN_VOLUME"))
			{
				DoAnalogInVolume(nodeForm);
			}
			else if (m_StrIDSub.contains("IS_QB_USER")
					 || m_StrIDSub.contains("IS_AIRABLE_TIDAL_USER")
					 || m_StrIDSub.contains("IS_AIRABLE_DEEZER_USER")
					 || m_StrIDSub.contains("IS_AIRABLE_NAPSTER_USER")
					 || m_StrIDSub.contains("IS_AIRABLE_HIGHRESAUDIO_USER")
					 || m_StrIDSub.contains("IS_AIRABLE_AMAZON_USER"))
			{
				DoLogin(nodeForm);
			}
		}
	}
}

void SetupWindow::SlotMenuActionSub(QAction *action)
{
	QString value = action->data().toString();
	LogDebug("eventID [%d] strID [%s] value [%s]", m_EventID, m_StrIDSub.toUtf8().data(), value.toUtf8().data());
	m_pMgr->RequestSetupSet(m_EventID, m_StrIDSub, value);
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

void SetupWindow::SlotRespGroup(QList<CJsonNode> list)
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
		m_pMgr->RequestSetupGroup(m_EventID, m_StrID);
	}
	else
	{
		LogDebug("resp is form~~");
		QString title = nodeForm.GetString(KEY_TITLE_CAP);
		if (title.contains("Power On Volume"))
		{
			DoPowerOnVolume(nodeForm);
		}
		else
		{
			FormDialog dialog;
			dialog.SetNodeForm(nodeForm);
			if (dialog.exec() == QDialog::Accepted)
			{
				m_pMgr->RequestSetupSet(m_EventID, m_StrIDSub, true);
			}
		}
	}
}

void SetupWindow::ReadSettings()
{
	SettingIO settings;
	settings.beginGroup(SETTINGS_GROUP);

	m_MaxVolume = settings.value("setup_max_volume").toInt();

	settings.endGroup();

	if (m_MaxVolume <= 0)
	{
		m_MaxVolume = 30;
	}

}

void SetupWindow::WriteSettings()
{
	SettingIO settings;
	settings.beginGroup(SETTINGS_GROUP);

	settings.setValue("setup_max_volume", m_MaxVolume);

	settings.endGroup();

}

void SetupWindow::ConnectSigToSlot()
{
//	connect(m_pListSetup->GetDelegate(), SIGNAL(SigSelectTitle(QString, int)), this, SLOT(SlotSelectTitle(QString, int)));
//	connect(m_pListSetup->GetDelegate(), SIGNAL(SigMenuAction(QString, QString)), this, SLOT(SlotMenuAction(QString, QString)));
//	connect(m_pListSetup->GetDelegate(), SIGNAL(SigSubMenuAction(QString, QString)), this, SLOT(SlotSubMenuAction(QString, QString)));
	connect(m_pListSetup->GetDelegate(), SIGNAL(SigSelectMenu(const QModelIndex&, QPoint)), this, SLOT(SlotSelectMenu(const QModelIndex&, QPoint)));
	connect(m_pListSetup->GetDelegateSub(), SIGNAL(SigSelectMenu(const QModelIndex&, QPoint)), this, SLOT(SlotSelectMenuSub(const QModelIndex&, QPoint)));

	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespGroup(QList<CJsonNode>)), this, SLOT(SlotRespGroup(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespSet(CJsonNode)), this, SLOT(SlotRespSet(CJsonNode)));

	connect(m_MenuSub, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuActionSub(QAction*)));

}

void SetupWindow::Initialize()
{
	m_MenuSubMap.clear();

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

	m_MenuSub->setStyleSheet(style);
}

void SetupWindow::SetMenuSubMap(QStringList values)
{
	m_MenuSubMap.clear();
	int index = 0;
	foreach (QString value, values)
	{
		LogDebug("value [%s]", value.toUtf8().data());
		m_MenuSubMap.insert(index, value);
		index++;
	}
}

void SetupWindow::DoAnalogInVolume(CJsonNode node)
{
	AnalogInVolumeDialog dialog;
	dialog.SetNodeForm(node);
	if (dialog.exec() == QDialog::Accepted)
	{
		int analog = dialog.GetSliderValue0();
		int aux = dialog.GetSliderValue1();
		int phono = dialog.GetSliderValue2();

		m_pMgr->RequestSetupSet(m_EventID, m_StrIDSub, true, analog, aux, phono);
	}
}

void SetupWindow::DoLogin(CJsonNode node)
{
	SetupLoginDialog dialog;
	dialog.SetNodeForm(node);
	if (dialog.exec() == QDialog::Accepted)
	{
		QString usernameKey = dialog.GetUsernameKey();
		QString passwordKey = dialog.GetPasswordKey();

		QString username = dialog.GetUsername();
		QString password = dialog.GetPassword();

		m_pMgr->RequestSetupSet(m_EventID, m_StrIDSub, true, usernameKey, username, passwordKey, password);
	}
}

void SetupWindow::DoMaxVolume(int volume)
{
	MaxVolumeDialog dialog;
	dialog.SetMaxVolume(m_MaxVolume);
	dialog.SetCurVolume(volume);
	if (dialog.exec() == QDialog::Accepted)
	{
		m_MaxVolume = dialog.GetMaxVolume();
		volume = dialog.GetCurVolume();

		m_pMgr->RequestVolume(m_EventID, volume);
		WriteSettings();
		m_pMgr->RequestSetupGroup(m_EventID, m_StrID);
	}
}

void SetupWindow::DoPowerOnVolume(CJsonNode node)
{
	PowerOnVolumeDialog dialog;
	dialog.SetNodeForm(node);
	if (dialog.exec() == QDialog::Accepted)
	{
		int volume = dialog.GetSliderValue0();
		QString key = dialog.GetHiddenKey();
		QString value = dialog.GetHiddenValue();

		m_pMgr->RequestSetupSet(m_EventID, m_StrIDSub, true, volume, key, value);
	}
}
