#include "edittagdialog.h"
#include "ui_edittagdialog.h"
#include "edittagdelegate.h"

#include "dialog/inputfavoritedialog.h"
#include "dialog/inputtagdialog.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

EditTagDialog::EditTagDialog(QWidget *parent) :
	QDialog(parent),
	m_Model(new QStandardItemModel(0, EDIT_TAG_MAX)),
	m_Delegate(new EditTagDelegate),
	ui(new Ui::EditTagDialog)
{
	ui->setupUi(this);

	Initialize();
	ConnectSigToSlot();
}

EditTagDialog::~EditTagDialog()
{
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

	delete ui;
}

void EditTagDialog::SetNodeList(QList<CJsonNode> list)
{
//	m_Model->setRowCount(list.count());

	int index = 0;
	foreach (CJsonNode node, list)
	{
//		LogDebug("node [%s]", node.ToCompactByteArray().data());
//		LogDebug("index [%d] title [%s]", index, node.GetString(KEY_FAVORITE).toUtf8().data());
		m_Model->setVerticalHeaderItem(index, new QStandardItem(QString("%1").arg(index+1)));

		m_Model->setData(m_Model->index(index, EDIT_TAG_ID), node.GetString(KEY_ID_LOWER));
		m_Model->setData(m_Model->index(index, EDIT_TAG_TITLE), node.GetString(KEY_TITLE));
		m_Model->setData(m_Model->index(index, EDIT_TAG_FAVORITE), node.GetString(KEY_FAVORITE_CAP));
		m_Model->setData(m_Model->index(index, EDIT_TAG_ARTIST), node.GetString(KEY_ARTIST));
		m_Model->setData(m_Model->index(index, EDIT_TAG_ALBUM), node.GetString(KEY_ALBUM));
		m_Model->setData(m_Model->index(index, EDIT_TAG_GENRE), node.GetString(KEY_GENRE));
		m_Model->setData(m_Model->index(index, EDIT_TAG_ALBUM_ARTIST), node.GetString(KEY_ALBUM_ARTIST));
		m_Model->setData(m_Model->index(index, EDIT_TAG_COMPOSER), node.GetString(KEY_COMPOSER));
		m_Model->setData(m_Model->index(index, EDIT_TAG_YEAR), node.GetString(KEY_YEAR));
		m_Model->setData(m_Model->index(index, EDIT_TAG_MOOD), node.GetString(KEY_MOOD));
//		m_Model->setData(m_Model->index(index, EDIT_TAG_RATING), node.GetString(KEY_RATING_CAP));

		m_Model->item(index, EDIT_TAG_TITLE)->setTextAlignment(Qt::AlignCenter);
		m_Model->item(index, EDIT_TAG_FAVORITE)->setTextAlignment(Qt::AlignCenter);
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

void EditTagDialog::resizeEvent(QResizeEvent *event)
{
	int width = event->size().width();
	int height = event->size().height();

	int colCnt = m_Model->columnCount();
	int rowCnt = m_Model->rowCount();

	if (colCnt <= 0 || rowCnt <= 0)
	{
		return;
	}

	int colSize = width / colCnt;
	int rowSize = height / rowCnt;
	LogDebug("resize colSize [%d] rowSize [%d]", colSize, rowSize);

//	for (int i = 0; i < colCnt; i++)
//	{
//		ui->tableView->setColumnWidth(i, colSize);
//	}
//	for (int i = 0; i < rowCnt; i++)
//	{
//		ui->tableView->setRowHeight(i, rowSize);
//	}

	ui->tableView->horizontalHeader()->setDefaultSectionSize(colSize);
	ui->tableView->verticalHeader()->setDefaultSectionSize(rowSize);
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
	if (rowTopLeft < 0 || colTopLeft < 0)
	{
		return;
	}
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
//	LogDebug("header index [%d]", logicalIndex);
	if (logicalIndex == EDIT_TAG_FAVORITE)
	{
		InputFavoriteDialog dialog;
		if (dialog.exec() == QDialog::Accepted)
		{
			bool favorite = dialog.GetFavorite();
//			LogDebug("favorite [%d]", favorite);
			int rowCount = m_Model->rowCount();
			for (int row = 0; row < rowCount; row++)
			{
				m_Model->setData(m_Model->index(row, logicalIndex), QString("%1").arg(favorite));
			}
		}
	}
	else
	{
		QStringList tagList = GetTagList(logicalIndex);

		InputTagDialog dialog;
		dialog.SetTagList(tagList);
		if (dialog.exec() == QDialog::Accepted)
		{
			QString name = dialog.GetTagName();
//			LogDebug("name [%s]", name.toUtf8().data());
			int rowCount = m_Model->rowCount();
			for (int row = 0; row < rowCount; row++)
			{
				m_Model->setData(m_Model->index(row, logicalIndex), name);
			}
		}
	}

}

void EditTagDialog::SlotClickFavorite(const QModelIndex &index)
{
	int data = index.data().toString().toInt() == 0 ? 1 : 0;
//	LogDebug("slot event row [%d] index.data [%d] data [%d]", index.row(), index.data().toString().toInt(), data);
	m_Model->setData(index, data);
}

void EditTagDialog::ConnectSigToSlot()
{
	connect(m_Model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(SlotDataChanged(const QModelIndex&, const QModelIndex&)));
	connect(ui->tableView->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(SlotSectionClicked(int)));
	connect(m_Delegate, SIGNAL(SigClickFavorite(const QModelIndex&)), this, SLOT(SlotClickFavorite(const QModelIndex&)));
}

void EditTagDialog::Initialize()
{
	ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
	ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);

	ui->tableView->setModel(m_Model);

	m_Model->setHeaderData(EDIT_TAG_TITLE, Qt::Horizontal, KEY_TITLE_CAP);
	m_Model->setHeaderData(EDIT_TAG_FAVORITE, Qt::Horizontal, KEY_FAVORITE_CAP);
	m_Model->setHeaderData(EDIT_TAG_ARTIST, Qt::Horizontal, KEY_ARTIST);
	m_Model->setHeaderData(EDIT_TAG_ALBUM, Qt::Horizontal, KEY_ALBUM);
	m_Model->setHeaderData(EDIT_TAG_GENRE, Qt::Horizontal, KEY_GENRE);
	m_Model->setHeaderData(EDIT_TAG_ALBUM_ARTIST, Qt::Horizontal, KEY_ALBUM_ARTIST);
	m_Model->setHeaderData(EDIT_TAG_COMPOSER, Qt::Horizontal, KEY_COMPOSER);
	m_Model->setHeaderData(EDIT_TAG_YEAR, Qt::Horizontal, KEY_YEAR);
	m_Model->setHeaderData(EDIT_TAG_MOOD, Qt::Horizontal, KEY_MOOD);
//	m_Model->setHeaderData(EDIT_TAG_RATING, Qt::Horizontal, KEY_RATING);

	ui->tableView->setColumnHidden(EDIT_TAG_ID, true);

	ui->tableView->setItemDelegateForColumn(EDIT_TAG_FAVORITE, m_Delegate);

	m_EnableChange = false;
	m_MapUpdateCell.clear();

	Qt::WindowFlags flags = 0;
	flags |= Qt::WindowMinimizeButtonHint;
	flags |= Qt::WindowMaximizeButtonHint;
	flags |= Qt::WindowCloseButtonHint;
//	flags |= Qt::WindowContextHelpButtonHint;
	setWindowFlags( flags );

	m_AlbumList.clear();
	m_AlbumArtistList.clear();
	m_ArtistList.clear();
	m_GenreList.clear();
	m_ComposerList.clear();
	m_MoodList.clear();

}

QStringList EditTagDialog::GetTagList(int index)
{
	QStringList tagList;

	switch (index)
	{
	case EDIT_TAG_ARTIST:
		tagList = m_ArtistList;
		break;
	case EDIT_TAG_ALBUM:
		tagList = m_AlbumList;
		break;
	case EDIT_TAG_GENRE:
		tagList = m_GenreList;
		break;
	case EDIT_TAG_ALBUM_ARTIST:
		tagList = m_AlbumArtistList;
		break;
	case EDIT_TAG_COMPOSER:
		tagList = m_ComposerList;
		break;
	case EDIT_TAG_MOOD:
		tagList = m_MoodList;
		break;
	}

	return tagList;
}

QStringList EditTagDialog::GetMoodList() const
{
	return m_MoodList;
}

void EditTagDialog::SetMoodList(const QStringList &MoodList)
{
	m_MoodList = MoodList;
}

QStringList EditTagDialog::GetComposerList() const
{
	return m_ComposerList;
}

void EditTagDialog::SetComposerList(const QStringList &ComposerList)
{
	m_ComposerList = ComposerList;
}

QStringList EditTagDialog::GetGenreList() const
{
	return m_GenreList;
}

void EditTagDialog::SetGenreList(const QStringList &GenreList)
{
	m_GenreList = GenreList;
}

QStringList EditTagDialog::GetArtistList() const
{
	return m_ArtistList;
}

void EditTagDialog::SetArtistList(const QStringList &ArtistList)
{
	m_ArtistList = ArtistList;
}

QStringList EditTagDialog::GetAlbumArtistList() const
{
	return m_AlbumArtistList;
}

void EditTagDialog::SetAlbumArtistList(const QStringList &AlbumArtistList)
{
	m_AlbumArtistList = AlbumArtistList;
}

QStringList EditTagDialog::GetAlbumList() const
{
	return m_AlbumList;
}

void EditTagDialog::SetAlbumList(const QStringList &AlbumList)
{
	m_AlbumList = AlbumList;
}

