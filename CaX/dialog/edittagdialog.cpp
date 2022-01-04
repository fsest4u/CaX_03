#include "edittagdialog.h"
#include "ui_edittagdialog.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

EditTagDialog::EditTagDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::EditTagDialog)
{
	ui->setupUi(this);

	ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	ui->tableView->verticalHeader()->setDefaultSectionSize(60);

	m_Model = new QStandardItemModel(0, EDIT_TAG_MAX);

	m_Model->setHeaderData(EDIT_TAG_TITLE, Qt::Horizontal, KEY_TITLE);
	m_Model->setHeaderData(EDIT_TAG_FAVORITE, Qt::Horizontal, KEY_FAVORITE);
	m_Model->setHeaderData(EDIT_TAG_ARTIST, Qt::Horizontal, KEY_ARTIST);
	m_Model->setHeaderData(EDIT_TAG_ALBUM, Qt::Horizontal, KEY_ALBUM);
	m_Model->setHeaderData(EDIT_TAG_GENRE, Qt::Horizontal, KEY_GENRE);
	m_Model->setHeaderData(EDIT_TAG_ALBUM_ARTIST, Qt::Horizontal, KEY_ALBUM_ARTIST);
	m_Model->setHeaderData(EDIT_TAG_COMPOSER, Qt::Horizontal, KEY_COMPOSER);
	m_Model->setHeaderData(EDIT_TAG_YEAR, Qt::Horizontal, KEY_YEAR);
	m_Model->setHeaderData(EDIT_TAG_MOOD, Qt::Horizontal, KEY_MOOD);
	m_Model->setHeaderData(EDIT_TAG_RATING, Qt::Horizontal, KEY_RATING);

	ui->tableView->setModel(m_Model);
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
		LogDebug("node [%s]", node.ToCompactByteArray().data());
		LogDebug("index [%d] title [%s]", index, node.GetString(KEY_FAVORITE).toUtf8().data());

		m_Model->setData(m_Model->index(index, EDIT_TAG_TITLE), node.GetString(KEY_TITLE));
		m_Model->setData(m_Model->index(index, EDIT_TAG_FAVORITE), node.GetString(KEY_FAVORITE_CAP));
		m_Model->setData(m_Model->index(index, EDIT_TAG_ARTIST), node.GetString(KEY_ARTIST));
		m_Model->setData(m_Model->index(index, EDIT_TAG_ALBUM), node.GetString(KEY_ALBUM));
		m_Model->setData(m_Model->index(index, EDIT_TAG_GENRE), node.GetString(KEY_GENRE));
		m_Model->setData(m_Model->index(index, EDIT_TAG_ALBUM_ARTIST), node.GetString(KEY_ALBUM_ARTIST));
		m_Model->setData(m_Model->index(index, EDIT_TAG_COMPOSER), node.GetString(KEY_COMPOSER));
		m_Model->setData(m_Model->index(index, EDIT_TAG_YEAR), node.GetString(KEY_YEAR));
		m_Model->setData(m_Model->index(index, EDIT_TAG_MOOD), node.GetString(KEY_MOOD));
		m_Model->setData(m_Model->index(index, EDIT_TAG_RATING), node.GetString(KEY_RATING_CAP));

		index++;
	}
}

