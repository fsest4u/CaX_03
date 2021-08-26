#include <QListView>
#include <QStandardItemModel>

#include "categorydialog.h"
#include "ui_categorydialog.h"
#include "categorydelegate.h"

#include "util/log.h"

CategoryDialog::CategoryDialog(QWidget *parent)
	:QDialog(parent)
	, m_ListView(new QListView)
	, m_Model(new QStandardItemModel)
	, m_Delegate(new CategoryDelegate)
	, ui(new Ui::CategoryDialog)
{
	ui->setupUi(this);

	Initialize();
}

CategoryDialog::~CategoryDialog()
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

int CategoryDialog::GetID() const
{
	return m_nID;
}

void CategoryDialog::SetID(int nID)
{
	m_nID = nID;
}

void CategoryDialog::SlotSelectItem(QModelIndex index)
{
	int id = qvariant_cast<int>(index.data(CategoryDelegate::CAT_ROLE_ID));
	QString title = qvariant_cast<QString>(index.data(CategoryDelegate::CAT_ROLE_TITLE));
	m_nID = id;

	LogDebug("id [%d] title [%s]", id, title.toUtf8().data());

	emit accept();
}

void CategoryDialog::Initialize()
{
	setWindowFlags(Qt::Popup);

	m_nID = -1;

	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);

	connect(m_ListView, SIGNAL(clicked(QModelIndex)), this, SLOT(SlotSelectItem(QModelIndex)));

	SetMenu();
}

void CategoryDialog::SetMenu()
{
	QStandardItem *item0 = new QStandardItem;
	QIcon icon0(":/resource/baseline_menu_black_24dp.png");
	item0->setData(CategoryDelegate::CAT_GENRE, CategoryDelegate::CAT_ROLE_ID);
	item0->setData(icon0, CategoryDelegate::CAT_ROLE_ICON);
	item0->setData("Sorted by Genre", CategoryDelegate::CAT_ROLE_TITLE);
	m_Model->appendRow(item0);

	QStandardItem *item1 = new QStandardItem;
	QIcon icon1(":/resource/baseline_menu_black_24dp.png");
	item1->setData(CategoryDelegate::CAT_MOOD, CategoryDelegate::CAT_ROLE_ID);
	item1->setData(icon1, CategoryDelegate::CAT_ROLE_ICON);
	item1->setData("Sorted by Mood", CategoryDelegate::CAT_ROLE_TITLE);
	m_Model->appendRow(item1);

	QStandardItem *item2 = new QStandardItem;
	QIcon icon2(":/resource/baseline_menu_black_24dp.png");
	item2->setData(CategoryDelegate::CAT_FOLDER, CategoryDelegate::CAT_ROLE_ID);
	item2->setData(icon2, CategoryDelegate::CAT_ROLE_ICON);
	item2->setData("Sorted by Folder", CategoryDelegate::CAT_ROLE_TITLE);
	m_Model->appendRow(item2);

	QStandardItem *item3 = new QStandardItem;
	QIcon icon3(":/resource/baseline_menu_black_24dp.png");
	item3->setData(CategoryDelegate::CAT_YEAR, CategoryDelegate::CAT_ROLE_ID);
	item3->setData(icon3, CategoryDelegate::CAT_ROLE_ICON);
	item3->setData("Sorted by Year", CategoryDelegate::CAT_ROLE_TITLE);
	m_Model->appendRow(item3);

	ui->gridLayout->addWidget(m_ListView);

}

void CategoryDialog::ClearMenu()
{
	m_Model->clear();
	ui->gridLayout->removeWidget(m_ListView);
}
