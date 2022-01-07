#include "edittagdialog.h"
#include "ui_edittagdialog.h"

#include "dialog/inputnamedialog.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

EditTagDialog::EditTagDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::EditTagDialog)
{
	ui->setupUi(this);

	ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);


	m_Model = new QStandardItemModel(0, EDIT_TAG_MAX);

	m_Model->setHeaderData(EDIT_TAG_TITLE, Qt::Horizontal, KEY_TITLE);
//	m_Model->setHeaderData(EDIT_TAG_FAVORITE, Qt::Horizontal, KEY_FAVORITE);
	m_Model->setHeaderData(EDIT_TAG_ARTIST, Qt::Horizontal, KEY_ARTIST);
	m_Model->setHeaderData(EDIT_TAG_ALBUM, Qt::Horizontal, KEY_ALBUM);
	m_Model->setHeaderData(EDIT_TAG_GENRE, Qt::Horizontal, KEY_GENRE);
	m_Model->setHeaderData(EDIT_TAG_ALBUM_ARTIST, Qt::Horizontal, KEY_ALBUM_ARTIST);
	m_Model->setHeaderData(EDIT_TAG_COMPOSER, Qt::Horizontal, KEY_COMPOSER);
	m_Model->setHeaderData(EDIT_TAG_YEAR, Qt::Horizontal, KEY_YEAR);
	m_Model->setHeaderData(EDIT_TAG_MOOD, Qt::Horizontal, KEY_MOOD);
//	m_Model->setHeaderData(EDIT_TAG_RATING, Qt::Horizontal, KEY_RATING);

	ui->tableView->setModel(m_Model);
	ui->tableView->setColumnHidden(EDIT_TAG_ID, true);

	m_EnableChange = false;
	m_MapUpdateCell.clear();

	connect(ui->tableView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(SlotDataChanged(const QModelIndex&, const QModelIndex&)));
	connect(ui->tableView->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(SlotSectionClicked(int)));
}

EditTagDialog::~EditTagDialog()
{
	if (m_Model)
	{
		delete m_Model;
		m_Model = nullptr;
	}

	delete ui;
}

void EditTagDialog::SetNodeList(QList<CJsonNode> list)
{
	m_Model->setRowCount(list.count());

	int index = 0;
	foreach (CJsonNode node, list)
	{
//		LogDebug("node [%s]", node.ToCompactByteArray().data());
//		LogDebug("index [%d] title [%s]", index, node.GetString(KEY_FAVORITE).toUtf8().data());

		m_Model->setData(m_Model->index(index, EDIT_TAG_ID), node.GetString(KEY_ID_LOWER));
		m_Model->setData(m_Model->index(index, EDIT_TAG_TITLE), node.GetString(KEY_TITLE));
//		m_Model->setData(m_Model->index(index, EDIT_TAG_FAVORITE), node.GetString(KEY_FAVORITE_CAP));
		m_Model->setData(m_Model->index(index, EDIT_TAG_ARTIST), node.GetString(KEY_ARTIST));
		m_Model->setData(m_Model->index(index, EDIT_TAG_ALBUM), node.GetString(KEY_ALBUM));
		m_Model->setData(m_Model->index(index, EDIT_TAG_GENRE), node.GetString(KEY_GENRE));
		m_Model->setData(m_Model->index(index, EDIT_TAG_ALBUM_ARTIST), node.GetString(KEY_ALBUM_ARTIST));
		m_Model->setData(m_Model->index(index, EDIT_TAG_COMPOSER), node.GetString(KEY_COMPOSER));
		m_Model->setData(m_Model->index(index, EDIT_TAG_YEAR), node.GetString(KEY_YEAR));
		m_Model->setData(m_Model->index(index, EDIT_TAG_MOOD), node.GetString(KEY_MOOD));
//		m_Model->setData(m_Model->index(index, EDIT_TAG_RATING), node.GetString(KEY_RATING_CAP));

		m_Model->item(index, EDIT_TAG_TITLE)->setTextAlignment(Qt::AlignCenter);
//		m_Model->item(index, EDIT_TAG_FAVORITE)->setTextAlignment(Qt::AlignCenter);
		m_Model->item(index, EDIT_TAG_ARTIST)->setTextAlignment(Qt::AlignCenter);
		m_Model->item(index, EDIT_TAG_ALBUM)->setTextAlignment(Qt::AlignCenter);
		m_Model->item(index, EDIT_TAG_GENRE)->setTextAlignment(Qt::AlignCenter);
		m_Model->item(index, EDIT_TAG_ALBUM_ARTIST)->setTextAlignment(Qt::AlignCenter);
		m_Model->item(index, EDIT_TAG_COMPOSER)->setTextAlignment(Qt::AlignCenter);
		m_Model->item(index, EDIT_TAG_YEAR)->setTextAlignment(Qt::AlignCenter);
		m_Model->item(index, EDIT_TAG_MOOD)->setTextAlignment(Qt::AlignCenter);
//		m_Model->item(index, EDIT_TAG_RATING)->setTextAlignment(Qt::AlignCenter);

		index++;
	}

	m_EnableChange = true;
}

QStandardItemModel *EditTagDialog::GetModel() const
{
	return m_Model;
}

QMap<QStringList, QString> EditTagDialog::GetUpdateCell() const
{
	return m_MapUpdateCell;
}

void EditTagDialog::SlotDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{

	Q_UNUSED(bottomRight)

	if (!m_EnableChange)
	{
//		LogDebug("not yet ready to change");
		return;
	}

	int rowTopLeft = topLeft.row();
	int colTopLeft = topLeft.column();
	QModelIndex indexTopLeft = m_Model->index(rowTopLeft, colTopLeft);
	QStandardItem *itemTopLeft = m_Model->itemFromIndex(indexTopLeft);
//	LogDebug("top left row [%d] col [%d] value [%s]", rowTopLeft, colTopLeft, qvariant_cast<QString>(itemTopLeft->data(Qt::DisplayRole)).toUtf8().data());

//	int rowBottomRight = bottomRight.row();
//	int colBottomRight = bottomRight.column();
//	QModelIndex indexBottomRight = m_Model->index(rowBottomRight, colBottomRight);
//	QStandardItem *itemBottomRight = m_Model->itemFromIndex(indexBottomRight);
//	LogDebug("bottom right row [%d] col [%d] value [%s]", rowBottomRight, colBottomRight, qvariant_cast<QString>(itemBottomRight->data(Qt::DisplayRole)).toUtf8().data());

	QStringList list;
	list.append(QString("%1").arg(rowTopLeft));
	list.append(QString("%1").arg(colTopLeft));
	m_MapUpdateCell.insert(list, qvariant_cast<QString>(itemTopLeft->data(Qt::DisplayRole)));
}

void EditTagDialog::SlotSectionClicked(int logicalIndex)
{
	LogDebug("header index [%d]", logicalIndex);
	InputNameDialog dialog;
	if (dialog.exec() == QDialog::Accepted)
	{
		QString name = dialog.GetName();
		LogDebug("name [%s]", name.toUtf8().data());
		int rowCount = m_Model->rowCount();
		for (int row = 0; row < rowCount; row++)
		{
			m_Model->setData(m_Model->index(row, logicalIndex), name);
		}
	}
}

