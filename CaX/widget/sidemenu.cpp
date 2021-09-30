#include <QMouseEvent>
#include <QListView>
#include <QStandardItemModel>

#include "sidemenu.h"
#include "ui_sidemenu.h"

#include "sidemenudelegate.h"

#include "util/log.h"

SideMenu::SideMenu(QWidget *parent)
	: QWidget(parent, Qt::FramelessWindowHint | Qt::Dialog)
	, m_ListView(new QListView)
	, m_Model(new QStandardItemModel)
	, m_Delegate(new SideMenuDelegate)
	, m_bAudioCD(false)
	, m_bInput(false)
	, m_bFMRadio(false)
	, m_bDABRadio(false)
	, m_bGroupPlay(false)
	, ui(new Ui::SideMenu)
{
	ui->setupUi(this);

	Initialize();
}

SideMenu::~SideMenu()
{
	delete ui;

	if (m_ListView)
	{
		delete m_ListView;
		m_ListView = nullptr;
	}
	if (m_Model)
	{
		delete m_Model;
		m_Model = nullptr;
	}
	if (m_Delegate)
	{
		delete m_Delegate;
		m_Delegate = nullptr;
	}
}

void SideMenu::ShowMenu()
{
	SetMenu();
	show();
}

void SideMenu::HideMenu()
{
	ClearMenu();
	hide();
}

void SideMenu::SetEnableAudioCD(bool bEnable)
{
	m_bAudioCD = bEnable;
}

void SideMenu::SetEnableInput(bool bEnable)
{
	m_bInput = bEnable;
}

void SideMenu::SetEnableFMRadio(bool bEnable)
{
	m_bFMRadio = bEnable;
	// like mobile-app
	m_bDABRadio = bEnable;
}

void SideMenu::SetEnableDABRadio(bool bEnable)
{
	m_bDABRadio = bEnable;
}

void SideMenu::SetEnableGroupPlay(bool bEnable)
{
	m_bGroupPlay = bEnable;
}

bool SideMenu::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{

		if (ui->frameMenu == object)
		{
//			LogDebug("click menu");
		}
		else
		{
//			LogDebug("click menu except");
			HideMenu();
		}
	}

	return QObject::eventFilter(object, event);
}

void SideMenu::SlotSelectItem(QModelIndex index)
{
	int id = qvariant_cast<int>(index.data(SideMenuDelegate::SIDEMENU_ID));
	QString title = qvariant_cast<QString>(index.data(SideMenuDelegate::SIDEMENU_TITLE));

	LogDebug("id [%d] title [%s]", id, title.toUtf8().data());
	emit SigSelectSideMenu(id);
	HideMenu();
}

void SideMenu::Initialize()
{
	setWindowModality(Qt::ApplicationModal);
	setAttribute(Qt::WA_TranslucentBackground);

	ui->frameMenu->installEventFilter(this);
	ui->frameTop->installEventFilter(this);
	ui->frameBottom->installEventFilter(this);
	ui->frameSpace->installEventFilter(this);

	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);

	connect(m_ListView, SIGNAL(clicked(QModelIndex)), this, SLOT(SlotSelectItem(QModelIndex)));
}

void SideMenu::SetMenu()
{
	QStandardItem *item0 = new QStandardItem;
	QIcon icon0(":/resource/Icon-playbar-volume-160.png");
	item0->setData(SideMenuDelegate::SIDEMENU_SERVICE_MUSIC_DB, SideMenuDelegate::SIDEMENU_ID);
	item0->setData(icon0, SideMenuDelegate::SIDEMENU_ICON);
	item0->setData("My Music", SideMenuDelegate::SIDEMENU_TITLE);
	m_Model->appendRow(item0);

	if (m_bAudioCD)
	{
		QStandardItem *item1 = new QStandardItem;
		QIcon icon1(":/resource/Icon-playbar-volume-160.png");
		item1->setData(SideMenuDelegate::SIDEMENU_SERVICE_AUDIO_CD, SideMenuDelegate::SIDEMENU_ID);
		item1->setData(icon1, SideMenuDelegate::SIDEMENU_ICON);
		item1->setData("Audio CD", SideMenuDelegate::SIDEMENU_TITLE);
		m_Model->appendRow(item1);
	}

	QStandardItem *item2 = new QStandardItem;
	QIcon icon2(":/resource/Icon-playbar-volume-160.png");
	item2->setData(SideMenuDelegate::SIDEMENU_SERVICE_PLAYLIST, SideMenuDelegate::SIDEMENU_ID);
	item2->setData(icon2, SideMenuDelegate::SIDEMENU_ICON);
	item2->setData("Playlist", SideMenuDelegate::SIDEMENU_TITLE);
	m_Model->appendRow(item2);

	QStandardItem *item3 = new QStandardItem;
	QIcon icon3(":/resource/Icon-playbar-volume-160.png");
	item3->setData(SideMenuDelegate::SIDEMENU_SERVICE_BROWSER, SideMenuDelegate::SIDEMENU_ID);
	item3->setData(icon3, SideMenuDelegate::SIDEMENU_ICON);
	item3->setData("Browser", SideMenuDelegate::SIDEMENU_TITLE);
	m_Model->appendRow(item3);

	QStandardItem *item4 = new QStandardItem;
	QIcon icon4(":/resource/Icon-playbar-volume-160.png");
	item4->setData(SideMenuDelegate::SIDEMENU_SERVICE_ISERVICE, SideMenuDelegate::SIDEMENU_ID);
	item4->setData(icon4, SideMenuDelegate::SIDEMENU_ICON);
	item4->setData("I-Service", SideMenuDelegate::SIDEMENU_TITLE);
	m_Model->appendRow(item4);

	if (m_bInput)
	{
		QStandardItem *item5 = new QStandardItem;
		QIcon icon5(":/resource/Icon-playbar-volume-160.png");
		item5->setData(SideMenuDelegate::SIDEMENU_SERVICE_INPUT, SideMenuDelegate::SIDEMENU_ID);
		item5->setData(icon5, SideMenuDelegate::SIDEMENU_ICON);
		item5->setData("Input", SideMenuDelegate::SIDEMENU_TITLE);
		m_Model->appendRow(item5);
	}
	if (m_bFMRadio)
	{
		QStandardItem *item6 = new QStandardItem;
		QIcon icon6(":/resource/Icon-playbar-volume-160.png");
		item6->setData(SideMenuDelegate::SIDEMENU_SERVICE_FM_RADIO, SideMenuDelegate::SIDEMENU_ID);
		item6->setData(icon6, SideMenuDelegate::SIDEMENU_ICON);
		item6->setData("FM Radio", SideMenuDelegate::SIDEMENU_TITLE);
		m_Model->appendRow(item6);
	}
	if (m_bDABRadio)
	{
		QStandardItem *item7 = new QStandardItem;
		QIcon icon7(":/resource/Icon-playbar-volume-160.png");
		item7->setData(SideMenuDelegate::SIDEMENU_SERVICE_DAB_RADIO, SideMenuDelegate::SIDEMENU_ID);
		item7->setData(icon7, SideMenuDelegate::SIDEMENU_ICON);
		item7->setData("DAB Radio", SideMenuDelegate::SIDEMENU_TITLE);
		m_Model->appendRow(item7);
	}
	if (m_bGroupPlay)
	{
		QStandardItem *item8 = new QStandardItem;
		QIcon icon8(":/resource/Icon-playbar-volume-160.png");
		item8->setData(SideMenuDelegate::SIDEMENU_SERVICE_GROUP_PLAY, SideMenuDelegate::SIDEMENU_ID);
		item8->setData(icon8, SideMenuDelegate::SIDEMENU_ICON);
		item8->setData("Group Play", SideMenuDelegate::SIDEMENU_TITLE);
		m_Model->appendRow(item8);
	}

	QStandardItem *item9 = new QStandardItem;
	QIcon icon9(":/resource/Icon-playbar-volume-160.png");
	item9->setData(SideMenuDelegate::SIDEMENU_SERVICE_SETUP, SideMenuDelegate::SIDEMENU_ID);
	item9->setData(icon9, SideMenuDelegate::SIDEMENU_ICON);
	item9->setData("Setup", SideMenuDelegate::SIDEMENU_TITLE);
	m_Model->appendRow(item9);

	QStandardItem *item10 = new QStandardItem;
	QIcon icon10(":/resource/Icon-playbar-volume-160.png");
	item10->setData(SideMenuDelegate::SIDEMENU_SERVICE_POWER_OFF, SideMenuDelegate::SIDEMENU_ID);
	item10->setData(icon10, SideMenuDelegate::SIDEMENU_ICON);
	item10->setData("Power Off", SideMenuDelegate::SIDEMENU_TITLE);
	m_Model->appendRow(item10);

	QStandardItem *item11 = new QStandardItem;
	QIcon icon11(":/resource/Icon-playbar-volume-160.png");
	item11->setData(SideMenuDelegate::SIDEMENU_SERVICE_POWER_ON, SideMenuDelegate::SIDEMENU_ID);
	item11->setData(icon11, SideMenuDelegate::SIDEMENU_ICON);
	item11->setData("Power on", SideMenuDelegate::SIDEMENU_TITLE);
	m_Model->appendRow(item11);

	ui->gridLayout->addWidget(m_ListView);
}

void SideMenu::ClearMenu()
{
	m_Model->clear();
	ui->gridLayout->removeWidget(m_ListView);
}
