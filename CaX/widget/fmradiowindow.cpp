#include "fmradiowindow.h"
#include "ui_fmradiowindow.h"

#include "dialog/addradiodialog.h"
#include "dialog/commondialog.h"
#include "dialog/progressfmdialog.h"
#include "dialog/setupreservationrecordingdialog.h"

#include "manager/fmradiomanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/caxtranslate.h"
#include "util/log.h"
#include "util/utilnovatron.h"

#include "widget/form/formplay.h"
#include "widget/form/formsort.h"

#include "widget/formTop/infoservice.h"
#include "widget/formBottom/iconservice.h"
#include "widget/formBottom/iconservicedelegate.h"

#define MAIN_TITLE	"FM Radio"
#define RESERVE_TITLE	"Reserved record list"

FMRadioWindow::FMRadioWindow(QWidget *parent, const QString &addr, const int &eventID) :
	QWidget(parent),
	m_pMgr(new FmRadioManager),
	m_pInfoService(new InfoService(this)),
	m_pIconService(new IconService(this)),
	m_ProgressDialog(new ProgressFmDialog(this)),
	m_EventID(eventID),
	m_FreqMax(0),
	m_FreqMin(0),
	m_FreqStep(0),
	ui(new Ui::FMRadioWindow)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
	Initialize();

}

FMRadioWindow::~FMRadioWindow()
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

	if (m_pIconService)
	{
		delete m_pIconService;
		m_pIconService = nullptr;
	}

	if (m_ProgressDialog)
	{
		delete m_ProgressDialog;
		m_ProgressDialog = nullptr;
	}

	delete ui;

}

void FMRadioWindow::AddWidgetFMRadioHome()
{
	ui->gridLayoutTop->addWidget(m_pInfoService);
	ui->gridLayoutBottom->addWidget(m_pIconService);
}

void FMRadioWindow::RequestList()
{
	m_pMgr->RequestList();
}

void FMRadioWindow::RequestRecordList()
{
	m_pMgr->RequestRecordList();
}

void FMRadioWindow::SlotTopMenu()
{
	m_SelectMap = m_pIconService->GetSelectMap();
	if (m_SelectMap.count() > 0)
	{
		SetSelectOnTopMenu();
	}
	else
	{
		SetSelectOffTopMenu();
	}
}

void FMRadioWindow::SlotTopMenuAction(int menuID)
{
	switch (menuID) {
	case TOP_MENU_SELECT_ALL:
		DoTopMenuSelectAll();
		break;
	case TOP_MENU_CLEAR_ALL:
		DoTopMenuClearAll();
		break;
	case TOP_MENU_CLEAR_AND_SEEK_ALL:
		DoTopMenuSeekAll(true);
		break;
	case TOP_MENU_SEEK_ALL:
		DoTopMenuSeekAll(false);
		break;
	case TOP_MENU_ADD_NEW_STATION:
		DoTopMenuAddNewStation();
		break;
	case TOP_MENU_DELETE:
		DoTopMenuDelete();
		break;
	case TOP_MENU_EDIT:
		DoTopMenuEdit();
		break;
	case TOP_MENU_RESERVED_RECORD_LIST:
		DoTopMenuReservedRecordList();
		break;
	case TOP_MENU_SETUP_RESERVED_RECORD:
		DoTopMenuSetupReservedRecord();
		break;
	}

}

//void FMRadioWindow::SlotResize()
//{
//	LogDebug("click resize");
//}

void FMRadioWindow::SlotRespError(QString errMsg)
{
	CommonDialog dialog(this, STR_WARNING, errMsg);
	dialog.exec();
}

void FMRadioWindow::SlotSelectTitle(int nType)
{
	m_pMgr->RequestPlay(nType);
}

void FMRadioWindow::SlotRespList(CJsonNode node)
{
	CJsonNode result;
	if (!node.GetArray(VAL_RESULT, result) || result.ArraySize() <= 0)
	{
		SlotRespError(STR_NO_RESULT);
		return;
	}

	m_FreqMax = node.GetInt(KEY_FREQ_MAX);
	m_FreqMin = node.GetInt(KEY_FREQ_MIN);
	m_FreqStep = node.GetInt(KEY_FREQ_STEP);

	m_NodeList.clear();
	for (int i = 0; i < result.ArraySize(); i++)
	{
		m_NodeList.append(result.GetArrayAt(i));
	}

	SetHome(m_NodeList);

	m_pInfoService->SetTitle(MAIN_TITLE);
	m_pIconService->SetNodeList(m_NodeList, IconService::ICON_SERVICE_FM_RADIO);
}

void FMRadioWindow::SlotRespRecordList(QList<CJsonNode> list)
{
	m_NodeList = list;
	SetHome(m_NodeList);

	m_pInfoService->SetTitle(RESERVE_TITLE);
	m_pIconService->SetNodeList(m_NodeList, IconService::ICON_SERVICE_FM_RADIO_RECORD);
}

void FMRadioWindow::SlotRespRecordSet(CJsonNode node)
{
	QString desc = node.GetString(KEY_DESC);
	if (!desc.isEmpty())
	{
		CommonDialog dialog(this, STR_WARNING, desc);
		dialog.exec();
	}
}

void FMRadioWindow::SlotEventFmSeeking(CJsonNode node)
{
	QString title = node.GetString(KEY_NAME_CAP);
	if (m_ProgressDialog->isHidden() && !title.isEmpty())
	{
		m_ProgressDialog->show();
	}
	m_ProgressDialog->SetTitle(title);
}

void FMRadioWindow::SlotEventFmSeek(CJsonNode node)
{
	SlotRespList(node);
}

void FMRadioWindow::SlotEventFmSeekStop(CJsonNode node)
{
	if (!m_ProgressDialog->isHidden())
	{
		m_ProgressDialog->hide();
	}
	SlotRespList(node);
}

void FMRadioWindow::SlotEventFmAdd(CJsonNode node)
{
	SlotRespList(node);
}

void FMRadioWindow::SlotEventFmDel(CJsonNode node)
{
	SlotRespList(node);
}

void FMRadioWindow::SlotEventFmSet(CJsonNode node)
{
	SlotRespList(node);
}

void FMRadioWindow::ConnectSigToSlot()
{
	connect(this, SIGNAL(SigAddWidget(QWidget*, QString)), parent(), SLOT(SlotAddWidget(QWidget*, QString)));

//	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectPlay(int)), this, SLOT(SlotSelectPlay(int)));
	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int)), this, SLOT(SlotSelectTitle(int)));
//	connect(m_pIconService->GetDelegate(), SIGNAL(SigSelectTitle(int, QString)), this, SLOT(SlotSelectTitle(int, QString)));

	connect(m_pMgr, SIGNAL(SigRespError(QString)), this, SLOT(SlotRespError(QString)));
	connect(m_pMgr, SIGNAL(SigRespList(CJsonNode)), this, SLOT(SlotRespList(CJsonNode)));
	connect(m_pMgr, SIGNAL(SigRespRecordList(QList<CJsonNode>)), this, SLOT(SlotRespRecordList(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespRecordSet(CJsonNode)), this, SLOT(SlotRespRecordSet(CJsonNode)));

	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigMenu()), this, SLOT(SlotTopMenu()));
	connect(m_pInfoService->GetFormPlay(), SIGNAL(SigMenuAction(int)), this, SLOT(SlotTopMenuAction(int)));
//	connect(m_pInfoService->GetFormSort(), SIGNAL(SigResize()), this, SLOT(SlotResize()));

}

void FMRadioWindow::Initialize()
{

	m_pInfoService->GetFormPlay()->ShowMenu();
//	m_pInfoService->GetFormSort()->ShowResize();

	m_TopMenuMap.clear();
	m_SelectMap.clear();
}

void FMRadioWindow::SetSelectOffTopMenu()
{
	m_TopMenuMap.clear();

	m_TopMenuMap.insert(TOP_MENU_CLEAR_AND_SEEK_ALL, STR_CLEAR_AND_SEEK_ALL);
	m_TopMenuMap.insert(TOP_MENU_SEEK_ALL, STR_SEEK_ALL);
	m_TopMenuMap.insert(TOP_MENU_SELECT_ALL, STR_SELECT_ALL);
	m_TopMenuMap.insert(TOP_MENU_ADD_NEW_STATION, STR_ADD_NEW_STATION);
	m_TopMenuMap.insert(TOP_MENU_RESERVED_RECORD_LIST, STR_RESERVE_RECORD_LIST);

	m_pInfoService->GetFormPlay()->ClearMenu();
	m_pInfoService->GetFormPlay()->SetMenu(m_TopMenuMap);
}

void FMRadioWindow::SetSelectOnTopMenu()
{
	m_TopMenuMap.clear();

	m_TopMenuMap.insert(TOP_MENU_CLEAR_ALL, STR_CLEAR_ALL);
	m_TopMenuMap.insert(TOP_MENU_DELETE, STR_DELETE);
	m_TopMenuMap.insert(TOP_MENU_EDIT, STR_EDIT);
	m_TopMenuMap.insert(TOP_MENU_SETUP_RESERVED_RECORD, STR_SETUP_RESERVE_RECORD);

	m_pInfoService->GetFormPlay()->ClearMenu();
	m_pInfoService->GetFormPlay()->SetMenu(m_TopMenuMap);
}

void FMRadioWindow::DoTopMenuSeekAll(bool bDelete)
{
	m_pMgr->RequestSeek(bDelete);
}

void FMRadioWindow::DoTopMenuSelectAll()
{
	m_pIconService->SetAllSelectMap();
}

void FMRadioWindow::DoTopMenuClearAll()
{
	m_pIconService->ClearSelectMap();
}

void FMRadioWindow::DoTopMenuAddNewStation()
{
	AddRadioDialog dialog;
	dialog.SetName(STR_MY_RADIO);
	dialog.SetRange(m_FreqMin/100.0, m_FreqMax/100.0, m_FreqStep/100.0);

	if (dialog.exec() == QDialog::Accepted)
	{
		QString name = dialog.GetName();
		double freq = dialog.GetFrequency();
		m_pMgr->RequestAdd(name, freq);
	}
}

void FMRadioWindow::DoTopMenuDelete()
{
	if (m_SelectMap.count() > 0)
	{
		m_pMgr->RequestDelete(m_SelectMap);
	}
	m_pIconService->ClearSelectMap();
}

void FMRadioWindow::DoTopMenuEdit()
{
	if (m_SelectMap.count() == 1)
	{
		int index = -1;
		QMap<int, bool>::iterator i;
		for (i = m_SelectMap.begin(); i!= m_SelectMap.end(); i++)
		{
			index = (int64_t)i.key();
		}

		CJsonNode node = m_NodeList.at(index);
		LogDebug("node [%s]", node.ToCompactByteArray().data());

		AddRadioDialog dialog;
		dialog.SetTitle(STR_EDIT);
		dialog.SetName(node.GetString(KEY_RIGHT));
		dialog.SetFrequency((node.GetString(KEY_LEFT).toDouble() * 1000));
		dialog.SetRange(m_FreqMin/100.0, m_FreqMax/100.0, m_FreqStep/100.0);

		if (dialog.exec() == QDialog::Accepted)
		{
			QString name = dialog.GetName();
			double freq = dialog.GetFrequency();
			m_pMgr->RequestSet(name, freq, index);
		}

	}
	else
	{
		CommonDialog dialog(this, STR_WARNING, STR_SELECT_ONE_ITEM);
		dialog.exec();
	}
}

void FMRadioWindow::DoTopMenuReservedRecordList()
{
	FMRadioWindow *widget = new FMRadioWindow(this, m_pMgr->GetAddr());
	widget->AddWidgetFMRadioHome();
	emit SigAddWidget(widget, STR_FM_RADIO);

	widget->RequestRecordList();
}

void FMRadioWindow::DoTopMenuSetupReservedRecord()
{
	if (m_SelectMap.count() == 1)
	{
		int index = -1;
		QMap<int, bool>::iterator i;
		for (i = m_SelectMap.begin(); i!= m_SelectMap.end(); i++)
		{
			index = (int64_t)i.key();
		}

		CJsonNode tempNode = m_NodeList.at(index);
		LogDebug("node [%s]", tempNode.ToCompactByteArray().data());

		CJsonNode node(JSON_OBJECT);
		node.Add(KEY_TOP, tempNode.GetString(KEY_RIGHT));
		node.Add(KEY_ACTIVE, false);
		node.AddInt(KEY_DURATION, 5);

		SetupReservationRecordingDialog dialog;
		dialog.SetNodeData(node);

		if (dialog.exec() == QDialog::Accepted)
		{
			node.Clear();
			node = dialog.GetNodeData();
			node.AddInt(KEY_FREQ, tempNode.GetString(KEY_LEFT).toDouble() * 100);
			node.AddInt(KEY_EVENT_ID, m_EventID);

			m_pMgr->RequestRecordSet(node);
		}

	}
	else
	{
		CommonDialog dialog(this, STR_WARNING, STR_SELECT_ONE_ITEM);
		dialog.exec();
	}
}

void FMRadioWindow::SetHome(QList<CJsonNode> &list)
{
	QList<CJsonNode> tempList;
	int index = 0;

	foreach (CJsonNode node, list)
	{
		node.Add(KEY_COVER_ART, UtilNovatron::GetCoverArtIcon(SIDEMENU_FM_RADIO));
		node.AddInt(KEY_ID_UPPER, index);
		node.AddInt(KEY_TYPE, index);

		tempList.append(node);
		index++;
	}
	list = tempList;
}
