#include <QListView>
#include <QStandardItemModel>

#include "sortdialog.h"
#include "ui_sortdialog.h"
#include "sortdelegate.h"

#include "util/log.h"

SortDialog::SortDialog(QWidget *parent) :
	QDialog(parent)
	, m_ListView(new QListView)
	, m_Model(new QStandardItemModel)
	, m_Delegate(new SortDelegate)
	, ui(new Ui::SortDialog)
{
	ui->setupUi(this);

	Initialize();
}

SortDialog::~SortDialog()
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

int SortDialog::GetID() const
{
	return m_nID;
}

void SortDialog::SetID(int nID)
{
	m_nID = nID;
}

void SortDialog::SlotSelectItem(QModelIndex index)
{
	int id = qvariant_cast<int>(index.data(SortDelegate::SORT_ROLE_ID));
	QString title = qvariant_cast<QString>(index.data(SortDelegate::SORT_ROLE_TITLE));
	m_nID = id;

	LogDebug("id [%d] title [%s]", id, title.toUtf8().data());

	emit accept();
}

void SortDialog::Initialize()
{
	setWindowFlags(Qt::Popup);

	m_nID = -1;

	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);

	connect(m_ListView, SIGNAL(clicked(QModelIndex)), this, SLOT(SlotSelectItem(QModelIndex)));

	SetMenu();
}

void SortDialog::SetMenu()
{
	QStandardItem *item0 = new QStandardItem;
	QIcon icon0(":/resource/baseline_menu_black_24dp.png");
	item0->setData(SortDelegate::SORT_BY_GENRE, SortDelegate::SORT_ROLE_ID);
	item0->setData(icon0, SortDelegate::SORT_ROLE_ICON);
	item0->setData("Sorted by Genre", SortDelegate::SORT_ROLE_TITLE);
	m_Model->appendRow(item0);

	QStandardItem *item1 = new QStandardItem;
	QIcon icon1(":/resource/baseline_menu_black_24dp.png");
	item1->setData(SortDelegate::SORT_BY_MOOD, SortDelegate::SORT_ROLE_ID);
	item1->setData(icon1, SortDelegate::SORT_ROLE_ICON);
	item1->setData("Sorted by Mood", SortDelegate::SORT_ROLE_TITLE);
	m_Model->appendRow(item1);

	QStandardItem *item2 = new QStandardItem;
	QIcon icon2(":/resource/baseline_menu_black_24dp.png");
	item2->setData(SortDelegate::SORT_BY_FOLDER, SortDelegate::SORT_ROLE_ID);
	item2->setData(icon2, SortDelegate::SORT_ROLE_ICON);
	item2->setData("Sorted by Folder", SortDelegate::SORT_ROLE_TITLE);
	m_Model->appendRow(item2);

	QStandardItem *item3 = new QStandardItem;
	QIcon icon3(":/resource/baseline_menu_black_24dp.png");
	item3->setData(SortDelegate::SORT_BY_YEAR, SortDelegate::SORT_ROLE_ID);
	item3->setData(icon3, SortDelegate::SORT_ROLE_ICON);
	item3->setData("Sorted by Year", SortDelegate::SORT_ROLE_TITLE);
	m_Model->appendRow(item3);

	QStandardItem *item4 = new QStandardItem;
	QIcon icon4(":/resource/baseline_menu_black_24dp.png");
	item4->setData(SortDelegate::SORT_BY_RATING, SortDelegate::SORT_ROLE_ID);
	item4->setData(icon4, SortDelegate::SORT_ROLE_ICON);
	item4->setData("Sorted by Rating", SortDelegate::SORT_ROLE_TITLE);
	m_Model->appendRow(item4);

	QStandardItem *item5 = new QStandardItem;
	QIcon icon5(":/resource/baseline_menu_black_24dp.png");
	item5->setData(SortDelegate::SORT_BY_SAMPLE_RATE, SortDelegate::SORT_ROLE_ID);
	item5->setData(icon5, SortDelegate::SORT_ROLE_ICON);
	item5->setData("Sorted by Sample rate", SortDelegate::SORT_ROLE_TITLE);
	m_Model->appendRow(item5);

	ui->gridLayout->addWidget(m_ListView);
}

void SortDialog::ClearMenu()
{
	m_Model->clear();
	ui->gridLayout->removeWidget(m_ListView);
}
