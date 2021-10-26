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
	, m_Delegate(new SubmenuDelegate)
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
	int id = qvariant_cast<int>(index.data(SubmenuDelegate::SUBMENU_ID));
	QString title = qvariant_cast<QString>(index.data(SubmenuDelegate::SUBMENU_TITLE));
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
//	QIcon icon0(":/resource/Icon-playbar-volume-160.png");
	//	item0->setData(CAT_GENRE, SubmenuDelegate::SUBMENU_ID);
//	item0->setData(icon0, SubmenuDelegate::SUBMENU_ICON);
//	item0->setData(STR_SORT_GENRE, SubmenuDelegate::SUBMENU_TITLE);
//	m_Model->appendRow(item0);

//	QStandardItem *item1 = new QStandardItem;
//	QIcon icon1(":/resource/Icon-playbar-volume-160.png");
//	item1->setData(CAT_MOOD, SubmenuDelegate::SUBMENU_ID);
//	item1->setData(icon1, SubmenuDelegate::SUBMENU_ICON);
//	item1->setData(STR_SORT_MOOD, SubmenuDelegate::SUBMENU_TITLE);
//	m_Model->appendRow(item1);

//	QStandardItem *item2 = new QStandardItem;
//	QIcon icon2(":/resource/Icon-playbar-volume-160.png");
//	item2->setData(CAT_FOLDER, SubmenuDelegate::SUBMENU_ID);
//	item2->setData(icon2, SubmenuDelegate::SUBMENU_ICON);
//	item2->setData(STR_SORT_FOLDER, SubmenuDelegate::SUBMENU_TITLE);
//	m_Model->appendRow(item2);

//	QStandardItem *item3 = new QStandardItem;
//	QIcon icon3(":/resource/Icon-playbar-volume-160.png");
//	item3->setData(CAT_YEAR, SubmenuDelegate::SUBMENU_ID);
//	item3->setData(icon3, SubmenuDelegate::SUBMENU_ICON);
//	item3->setData(STR_SORT_YEAR, SubmenuDelegate::SUBMENU_TITLE);
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
		item->setData(node.GetInt(KEY_ID_UPPER), SubmenuDelegate::SUBMENU_ID);
		item->setData(node.GetString(KEY_COVER_ART), SubmenuDelegate::SUBMENU_ICON);
		item->setData(node.GetString(KEY_NAME), SubmenuDelegate::SUBMENU_TITLE);
		m_Model->appendRow(item);
	}
	ui->gridLayout->addWidget(m_ListView);
}

void SubmenuDialog::ClearItemList()
{
	m_Model->clear();
	ui->gridLayout->removeWidget(m_ListView);
}
