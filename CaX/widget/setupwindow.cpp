#include "setupwindow.h"
#include "ui_setupwindow.h"

#include "dialog/commondialog.h"
#include "dialog/setup/alarmdialog.h"
#include "dialog/setup/analoginvolumedialog.h"
#include "dialog/setup/customeqdialog.h"
#include "dialog/setup/formdialog.h"
#include "dialog/setup/maxvolumedialog.h"
#include "dialog/setup/poweronvolumedialog.h"
#include "dialog/setup/setuplogindialog.h"
#include "dialog/setup/timemanualdialog.h"
#include "dialog/setup/wiredlansetupdialog.h"

#include "manager/setupmanager.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/settingio.h"
#include "util/utilnovatron.h"

#include "widget/browserwindow.h"
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

void SetupWindow::SlotAddWidget(QWidget *widget, QString title)
{
	emit SigAddWidget(widget, title);		// recursive
}

void SetupWindow::SlotRemoveWidget(QWidget *widget)
{
	emit SigRemoveWidget(widget);

}

void SetupWindow::SlotSelectMenu(const QModelIndex &modelIndex, QPoint point)
{
	m_StrID = qvariant_cast<QString>(modelIndex.data(ListSetupDelegate::LIST_SETUP_ID));
//	LogDebug("setup main menu [%s]", m_StrID.toUtf8().data());

	m_pMgr->RequestSetupGroup(m_EventID, m_StrID);
}

void SetupWindow::SlotSelectMenuSub(const QModelIndex &modelIndex, QPoint point)
{
	m_ModelIndex = modelIndex;

	m_StrIDSub = qvariant_cast<QString>(m_ModelIndex.data(ListSetupDelegate::LIST_SETUP_ID));
//	LogDebug("setup sub menu [%s]", m_StrIDSub.toUtf8().data());
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

		QStringList keys = node.GetStringList(KEY_KEYS);
		QStringList values = node.GetStringList(KEY_VALUES);
		SetMenuSubMap(keys, values);

		m_MenuSub->clear();

		QMap<QString, QString>::iterator i;
		for (i = m_MenuSubMap.begin(); i != m_MenuSubMap.end(); i++)
		{
			QAction *action = new QAction(i.value(), this);
			action->setData(i.key());
			m_MenuSub->addAction(action);
		}

		m_MenuSub->popup(m_pListSetup->GetListViewSub()->viewport()->mapToGlobal(point));
	}
	else if (type & iAppSetupType_Mask_List)
	{

	}
	else if (type & iAppSetupType_Mask_FormSelect)
	{
		QString json = qvariant_cast<QString>(m_ModelIndex.data(ListSetupDelegate::LIST_SETUP_RAW));
		CJsonNode node;
		if (!node.SetContent(json))
		{
			return;
		}

		QStringList keys;
		QStringList values;
		CJsonNode nodeForms = node.GetArray(KEY_FORMS);
		for(int i = 0; i < nodeForms.ArraySize(); i++)
		{
			CJsonNode nodeForm = nodeForms.GetArrayAt(i);
			keys.append(QString::number(i));
			values.append(nodeForm.GetString(KEY_TITLE_CAP));
		}
		SetMenuSubMap(keys, values);

		m_MenuSub->clear();

		QMap<QString, QString>::iterator i;
		for (i = m_MenuSubMap.begin(); i != m_MenuSubMap.end(); i++)
		{
			QAction *action = new QAction(i.value(), this);
			action->setData(i.key());
			m_MenuSub->addAction(action);
		}

		m_MenuSub->popup(m_pListSetup->GetListViewSub()->viewport()->mapToGlobal(point));
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
//			LogDebug("form is null");
			if (m_StrIDSub.contains(ID_MAX_VOLUME))
			{
				int volume = qvariant_cast<int>(m_ModelIndex.data(ListSetupDelegate::LIST_SETUP_SUBTITLE));

				DoMaxVolume(volume);
			}
			else if (m_StrIDSub.contains(ID_NET_NETWORK_INFO)
					 || m_StrIDSub.contains(ID_SY_CHECKFIRMWARE)
					 || m_StrIDSub.contains(ID_LI_GRACENOTE_CD)
					 || m_StrIDSub.contains(ID_LI_GRACENOTE_COVER)
					 || m_StrIDSub.contains(ID_LI_GRACENOTE_FINGER)
					 || m_StrIDSub.contains(ID_LI_GRACENOTE_PLS)
					 || m_StrIDSub.contains(ID_MDB_RESCAN))
			{
				CJsonNode node(JSON_OBJECT);
				node.AddInt(KEY_EVENT_ID, m_EventID);
				node.Add(KEY_ID_UPPER, m_StrIDSub);
				m_pMgr->RequestSetupSet(node);
			}
			else if (type & iAppSetupType_Mask_Browser)
			{
				QString ext = node.GetString(KEY_EXT);

				BrowserWindow *widget = new BrowserWindow(this, m_pMgr->GetAddr(), m_EventID);
				widget->RequestRoot(ext);

				emit widget->SigAddWidget(widget, STR_BROWSER);
			}
		}
		else
		{
//			LogDebug("form is exist");
			if (m_StrIDSub.contains(ID_AUD_ANALOG_IN_VOLUME))
			{
				DoAnalogInVolume(nodeForm);
			}
			else if (m_StrIDSub.contains(ID_IS_QB_USER)
					 || m_StrIDSub.contains(ID_IS_AIRABLE_TIDAL_USER)
					 || m_StrIDSub.contains(ID_IS_AIRABLE_DEEZER_USER)
					 || m_StrIDSub.contains(ID_IS_AIRABLE_NAPSTER_USER)
					 || m_StrIDSub.contains(ID_IS_AIRABLE_HIGHRESAUDIO_USER)
					 || m_StrIDSub.contains(ID_IS_AIRABLE_AMAZON_USER)
					 || m_StrIDSub.contains(ID_NET_SAMBA_HOST_NAME)
					 || m_StrIDSub.contains(ID_NET_SAMBA_WORKGROUP)
					 || m_StrIDSub.contains(ID_NET_FTP_PASS))
			{
				DoLogin(nodeForm);
			}
			else if (m_StrIDSub.contains(ID_TM_MANUAL))
			{
				DoTimeManual(nodeForm);
			}
			else if (m_StrIDSub.contains(ID_SY_FACTORY_RESET)
					 || m_StrIDSub.contains(ID_MDB_INIT)
					 || m_StrIDSub.contains(ID_RIP_FREE_DB_UPDATE)
					 || m_StrIDSub.contains(ID_RIP_FREE_DB_DELETE))
			{
				FormDialog dialog;
				dialog.SetNodeForm(nodeForm);
				if (dialog.exec() == QDialog::Accepted)
				{
					CJsonNode node = dialog.GetNodeForm();
					node.AddInt(KEY_EVENT_ID, m_EventID);
					node.Add(KEY_ID_UPPER, m_StrIDSub);
					node.Add(KEY_OK, true);
					m_pMgr->RequestSetupSet(node);
				}
			}
		}
	}
	else if (type & iAppSetupType_Mask_Sub)
	{

	}
	else if (type & iAppSetupType_Mask_PlayDisable)
	{

	}
	else if (type & iAppSetupType_Mask_Disable)
	{

	}
	else if (type & iAppSetupType_Mask_HideValue)
	{

	}
	else if (type & iAppSetupType_Mask_Event)
	{
		if (m_StrIDSub.contains(ID_MDB_SCAN_STATUS))
		{
			CJsonNode node(JSON_OBJECT);
			node.AddInt(KEY_EVENT_ID, m_EventID);
			node.Add(KEY_ID_UPPER, m_StrIDSub);
			m_pMgr->RequestSetupSet(node);
		}
	}
}

void SetupWindow::SlotMenuActionSub(QAction *action)
{
	QString key = action->data().toString();
	QString value = action->text();
//	LogDebug("eventID [%d] strID [%s] key [%s] value [%s]", m_EventID, m_StrIDSub.toUtf8().data(), key.toUtf8().data(), value.toUtf8().data());
	if (m_StrIDSub.contains(ID_NET_WIRED_LAN_SETUP))
	{
		QString json = qvariant_cast<QString>(m_ModelIndex.data(ListSetupDelegate::LIST_SETUP_RAW));
		CJsonNode node;
		if (!node.SetContent(json))
		{
			return;
		}

		QStringList values;
		CJsonNode nodeForms = node.GetArray(KEY_FORMS);
		for(int i = 0; i < nodeForms.ArraySize(); i++)
		{
			CJsonNode nodeForm = nodeForms.GetArrayAt(i);
			if (!nodeForm.GetString(KEY_TITLE_CAP).compare(value))
			{
				DoWiredLanSetup(nodeForm);
				break;
			}
		}
	}
	else if (m_StrIDSub.contains(ID_SY_ALARM)
			 || m_StrIDSub.contains(ID_SY_AUTO_SHUTDOWN)
			 || m_StrIDSub.contains(ID_SY_HDD_FORMAT)
			 || m_StrIDSub.contains(ID_SY_NTFS_FIX))
	{
		CJsonNode node(JSON_OBJECT);
		node.AddInt(KEY_EVENT_ID, m_EventID);
		node.Add(KEY_ID_UPPER, m_StrIDSub);
		if (!key.isEmpty())
		{
			node.Add(KEY_VALUE,		key);
		}
		m_pMgr->RequestSetupSet(node);
	}
	else
	{
		CJsonNode node(JSON_OBJECT);
		node.AddInt(KEY_EVENT_ID, m_EventID);
		node.Add(KEY_ID_UPPER, m_StrIDSub);
		if (!value.isEmpty())
		{
			node.Add(KEY_VALUE,		value);
		}
		m_pMgr->RequestSetupSet(node);
	}
}

void SetupWindow::SlotBrowserPath(QString path)
{
	CJsonNode node(JSON_OBJECT);
	node.AddInt(KEY_EVENT_ID, m_EventID);
	node.Add(KEY_ID_UPPER, m_StrIDSub);
	if (!path.isEmpty())
	{
		node.Add(KEY_PATH,		path);
	}

	m_pMgr->RequestSetupSet(node);
}

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
//	LogDebug("node [%s]", node.ToCompactByteArray().data());
	CJsonNode nodeForm = node.GetObject(KEY_FORM);
	if (nodeForm.IsNull())
	{
//		LogDebug("resp is not form~~");
		m_pMgr->RequestSetupGroup(m_EventID, m_StrID);
	}
	else
	{
//		LogDebug("resp is form~~");
		QString title = nodeForm.GetString(KEY_TITLE_CAP);
		if (title.contains(SETUP_POWER_ON_VOLUME))
		{
			DoPowerOnVolume(nodeForm);
		}
		else if (title.contains(SETUP_CUSTOM_EQ))
		{
			DoCustomEQ(nodeForm);
		}
		else if (title.contains(SETUP_ALRAM)
				 || title.contains(SETUP_AUTO_SHUTDOWN))
		{
			DoAlarm(nodeForm);
		}
		else
		{
			FormDialog dialog;
			dialog.SetNodeForm(nodeForm);
			if (dialog.exec() == QDialog::Accepted)
			{
				CJsonNode node = dialog.GetNodeForm();
				node.AddInt(KEY_EVENT_ID, m_EventID);
				node.Add(KEY_ID_UPPER, m_StrIDSub);
				node.Add(KEY_OK, true);
				m_pMgr->RequestSetupSet(node);
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
	connect(this, SIGNAL(SigAddWidget(QWidget*, QString)), parent(), SLOT(SlotAddWidget(QWidget*, QString)));		// recursive
	connect(this, SIGNAL(SigRemoveWidget(QWidget*)), parent(), SLOT(SlotRemoveWidget(QWidget*)));

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

void SetupWindow::SetMenuSubMap(QStringList keys, QStringList values)
{
	m_MenuSubMap.clear();
	QString key;
	QString value;
	for (int i = 0; i < values.count(); i++)
	{
		if (!keys.isEmpty() && keys.count() > i)
		{
			key = keys.at(i);
		}
		if (!values.isEmpty() && values.count() > i)
		{
			value = values.at(i);
		}
//		LogDebug("key [%s] value [%s]", key.toUtf8().data(), value.toUtf8().data());
		if (key.isEmpty())
		{
			m_MenuSubMap.insert(QString::number(i), value);
		}
		else
		{
			m_MenuSubMap.insert(key, value);
		}
	}
}

void SetupWindow::DoAlarm(CJsonNode node)
{
	AlarmDialog dialog;
	dialog.SetNodeForm(node);
	if (dialog.exec() == QDialog::Accepted)
	{
		CJsonNode node = dialog.GetNodeForm();
		node.AddInt(KEY_EVENT_ID, m_EventID);
		node.Add(KEY_ID_UPPER, m_StrIDSub);
		node.Add(KEY_OK, true);
		m_pMgr->RequestSetupSet(node);
	}
}

void SetupWindow::DoAnalogInVolume(CJsonNode node)
{
	AnalogInVolumeDialog dialog;
	dialog.SetNodeForm(node);
	if (dialog.exec() == QDialog::Accepted)
	{
		CJsonNode node = dialog.GetNodeForm();
		node.AddInt(KEY_EVENT_ID, m_EventID);
		node.Add(KEY_ID_UPPER, m_StrIDSub);
		node.Add(KEY_OK, true);
		m_pMgr->RequestSetupSet(node);
	}
}

void SetupWindow::DoCustomEQ(CJsonNode node)
{
	CustomEQDialog dialog;
	dialog.SetNodeForm(node);
	if (dialog.exec() == QDialog::Accepted)
	{
		CJsonNode node = dialog.GetNodeForm();
		node.Add(KEY_VALUE,		node.GetString(KEY_TITLE_CAP));
		node.AddInt(KEY_EVENT_ID, m_EventID);
		node.Add(KEY_ID_UPPER, m_StrIDSub);
		node.Add(KEY_OK, true);
		m_pMgr->RequestSetupSet(node);
	}
}

void SetupWindow::DoLogin(CJsonNode node)
{
	SetupLoginDialog dialog;
	dialog.SetNodeForm(node);
	if (dialog.exec() == QDialog::Accepted)
	{
		CJsonNode node = dialog.GetNodeForm();
		node.AddInt(KEY_EVENT_ID, m_EventID);
		node.Add(KEY_ID_UPPER, m_StrIDSub);
		node.Add(KEY_OK, true);
		m_pMgr->RequestSetupSet(node);
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
		CJsonNode node = dialog.GetNodeForm();
		node.AddInt(KEY_EVENT_ID, m_EventID);
		node.Add(KEY_ID_UPPER, m_StrIDSub);
		node.Add(KEY_OK, true);
		m_pMgr->RequestSetupSet(node);
	}
}

void SetupWindow::DoTimeManual(CJsonNode node)
{
	TimeManualDialog dialog;
	dialog.SetNodeForm(node);
	if (dialog.exec() == QDialog::Accepted)
	{
		CJsonNode node = dialog.GetNodeForm();
		node.AddInt(KEY_EVENT_ID, m_EventID);
		node.Add(KEY_ID_UPPER, m_StrIDSub);
		node.Add(KEY_OK, true);
		m_pMgr->RequestSetupSet(node);
	}
}

void SetupWindow::DoWiredLanSetup(CJsonNode node)
{
	WiredLanSetupDialog dialog;
	dialog.SetNodeForm(node);
	if (dialog.exec() == QDialog::Accepted)
	{
		CJsonNode node = dialog.GetNodeForm();
		node.AddInt(KEY_EVENT_ID, m_EventID);
		node.Add(KEY_ID_UPPER, m_StrIDSub);
		node.Add(KEY_OK, true);
		m_pMgr->RequestSetupSet(node);
	}
}
