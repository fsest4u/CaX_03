#include <QListView>
#include <QStandardItemModel>

#include "submenudialog.h"
#include "ui_submenudialog.h"
#include "submenudelegate.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

SubmenuDialog::SubmenuDialog(QWidget *parent)
	:QDialog(parent)
	, m_ListView(new QListView)
	, m_Model(new QStandardItemModel)
	, m_Delegate(new CategoryDelegate)
	, ui(new Ui::SubmenuDialog)
{
	ui->setupUi(this);

	Initialize();
}

SubmenuDialog::~SubmenuDialog()
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

int SubmenuDialog::GetID() const
{
	return m_nID;
}

void SubmenuDialog::SetID(int nID)
{
	m_nID = nID;
}

void SubmenuDialog::SlotSelectItem(QModelIndex index)
{
	int id = qvariant_cast<int>(index.data(CategoryDelegate::CAT_ROLE_ID));
	QString title = qvariant_cast<QString>(index.data(CategoryDelegate::CAT_ROLE_TITLE));
	m_nID = id;

	LogDebug("id [%d] title [%s]", id, title.toUtf8().data());

	emit accept();
}

void SubmenuDialog::Initialize()
{
	setWindowFlags(Qt::Popup);

	m_nID = -1;

	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);

	connect(m_ListView, SIGNAL(clicked(QModelIndex)), this, SLOT(SlotSelectItem(QModelIndex)));

//	SetMenu();
}

void SubmenuDialog::SetMenu()
{
//	QStandardItem *item0 = new QStandardItem;
//	QIcon icon0(":/resource/baseline_menu_black_24dp.png");
//	item0->setData(CAT_GENRE, CategoryDelegate::CAT_ROLE_ID);
//	item0->setData(icon0, CategoryDelegate::CAT_ROLE_ICON);
//	item0->setData("Sorted by Genre", CategoryDelegate::CAT_ROLE_TITLE);
//	m_Model->appendRow(item0);

//	QStandardItem *item1 = new QStandardItem;
//	QIcon icon1(":/resource/baseline_menu_black_24dp.png");
//	item1->setData(CAT_MOOD, CategoryDelegate::CAT_ROLE_ID);
//	item1->setData(icon1, CategoryDelegate::CAT_ROLE_ICON);
//	item1->setData("Sorted by Mood", CategoryDelegate::CAT_ROLE_TITLE);
//	m_Model->appendRow(item1);

//	QStandardItem *item2 = new QStandardItem;
//	QIcon icon2(":/resource/baseline_menu_black_24dp.png");
//	item2->setData(CAT_FOLDER, CategoryDelegate::CAT_ROLE_ID);
//	item2->setData(icon2, CategoryDelegate::CAT_ROLE_ICON);
//	item2->setData("Sorted by Folder", CategoryDelegate::CAT_ROLE_TITLE);
//	m_Model->appendRow(item2);

//	QStandardItem *item3 = new QStandardItem;
//	QIcon icon3(":/resource/baseline_menu_black_24dp.png");
//	item3->setData(CAT_YEAR, CategoryDelegate::CAT_ROLE_ID);
//	item3->setData(icon3, CategoryDelegate::CAT_ROLE_ICON);
//	item3->setData("Sorted by Year", CategoryDelegate::CAT_ROLE_TITLE);
//	m_Model->appendRow(item3);

//	ui->gridLayout->addWidget(m_ListView);

}

void SubmenuDialog::ClearMenu()
{
	m_Model->clear();
	ui->gridLayout->removeWidget(m_ListView);
}

QList<CJsonNode> SubmenuDialog::GetItemList() const
{
	return m_ItemList;
}

void SubmenuDialog::SetItemList(const QList<CJsonNode> &ItemList)
{
	m_ItemList = ItemList;
	foreach (CJsonNode node, m_ItemList)
	{
		QStandardItem *item = new QStandardItem;
		item->setData(node.GetInt(KEY_ID_UPPER), CategoryDelegate::CAT_ROLE_ID);
		item->setData(node.GetString(KEY_COVER_ART), CategoryDelegate::CAT_ROLE_ICON);
		item->setData(node.GetString(KEY_NAME), CategoryDelegate::CAT_ROLE_TITLE);
		m_Model->appendRow(item);
	}
	ui->gridLayout->addWidget(m_ListView);
}

void SubmenuDialog::ClearItemList()
{
	m_Model->clear();
	ui->gridLayout->removeWidget(m_ListView);
}
