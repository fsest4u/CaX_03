#include <QThread>
#include <QScrollBar>
#include <QHeaderView>

#include "tabletracks.h"
#include "ui_tabletracks.h"
#include "tabletracksdelegate.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/utilnovatron.h"


TableTracks::TableTracks(QWidget *parent) :
	QWidget(parent),
	m_Model(new QStandardItemModel(0, TABLE_TRACKS_MAX)),
	m_Delegate(new TableTracksDelegate),
	m_ScrollBar(nullptr),
	m_Menu(new QMenu(this)),
	ui(new Ui::TableTracks)
{
	ui->setupUi(this);

	Initialize();
	ConnectSigToSlot();
}

TableTracks::~TableTracks()
{
	if (m_ScrollBar)
	{
		delete m_ScrollBar;
		m_ScrollBar = nullptr;
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

	disconnect(m_Menu, SIGNAL(triggered(QAction*)));
	if (m_Menu)
	{
		delete m_Menu;
		m_Menu = nullptr;
	}

	delete ui;
}

QList<CJsonNode> TableTracks::GetNodeList() const
{
	return m_NodeList;
}

void TableTracks::SetNodeList(QList<CJsonNode> list, int service)
{
	int index = m_NodeList.count();
	m_NodeList.append(list);

	{
		foreach (CJsonNode node, list)
		{
//			LogDebug("node [%s]", node.ToCompactByteArray().data());
			m_Model->setVerticalHeaderItem(index, new QStandardItem());

			int seconds = node.GetInt(KEY_TIME);
			QString hhmmss = UtilNovatron::CalcSecondToHMS(seconds);
			QString artist;
			if (!node.GetString(KEY_ARTIST).isEmpty())
			{
				artist = node.GetString(KEY_ARTIST);
			}
			else if (!node.GetString(KEY_SUBTITLE).isEmpty())
			{
				artist = node.GetString(KEY_SUBTITLE);
			}
			int nID = node.GetString(KEY_ID_LOWER).toInt();

			QString extension = UtilNovatron::GetSuffix(node.GetString(KEY_FORMAT));

			m_Model->setData(m_Model->index(index, TABLE_TRACKS_ID), nID, Qt::DisplayRole);
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_SELECT), false);
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_TITLE), node.GetString(KEY_TITLE), Qt::DisplayRole);
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_FAVORITE), node.GetString(KEY_FAVORITE_CAP));
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_TIME), hhmmss);
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_ARTIST), artist);
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_ALBUM), node.GetString(KEY_ALBUM));
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_GENRE), node.GetString(KEY_GENRE));
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_ALBUM_ARTIST), node.GetString(KEY_ALBUM_ARTIST));
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_COMPOSER), node.GetString(KEY_COMPOSER));
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_YEAR), node.GetString(KEY_YEAR));
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_MOOD), node.GetString(KEY_MOOD));
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_TEMPO), node.GetString(KEY_TEMPO));
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_FORMAT), extension);
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_SAMPLE_RATE), node.GetString(KEY_SAMPLERATE_CAP));
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_BIT_DEPTH), node.GetString(KEY_BITRATE));
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_RATING), node.GetString(KEY_RATING_CAP));
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_INDEX), index);
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_MENU), false);

			index++;
		}
	}

}

void TableTracks::ClearNodeList()
{
	m_Model->clear();
	m_NodeList.clear();
	m_SelectMap.clear();
}

void TableTracks::ClearSelectMap()
{
	int count = m_Model->rowCount();

	for (int i = 0; i < count; i++)
	{
		m_Model->setData(m_Model->index(i, TABLE_TRACKS_SELECT), false);
		int id = qvariant_cast<int>(m_Model->data(m_Model->index(i, TABLE_TRACKS_ID)));
		m_SelectMap.remove(id);
	}
}

void TableTracks::SetAllSelectMap()
{
	int count = m_Model->rowCount();

	for (int i = 0; i < count; i++)
	{
		m_Model->setData(m_Model->index(i, TABLE_TRACKS_SELECT), true);
		int id = qvariant_cast<int>(m_Model->data(m_Model->index(i, TABLE_TRACKS_ID)));
		m_SelectMap.insert(id, true);
	}
}

QMap<int, bool> TableTracks::GetSelectMap() const
{
	return m_SelectMap;
}

void TableTracks::SetSelectMap(const QMap<int, bool> &SelectMap)
{
	m_SelectMap = SelectMap;
}

QMap<int, QString> TableTracks::GetOptionMenuMap() const
{
	return m_OptionMenuMap;
}

void TableTracks::SetOptionMenuMap(const QMap<int, QString> &OptionMenuMap)
{
	m_OptionMenuMap = OptionMenuMap;
}

void TableTracks::SetColResize(int resize)
{
//	ui->tableView->horizontalHeader()->setDefaultSectionSize(resize);
	resize = 60;

	int colCnt = m_Model->columnCount();
	int showColCnt = GetCntColumnShow();

	int coverWidth = resize * 2;
	int titleWidth = resize * (colCnt - showColCnt);

	ui->tableView->setColumnWidth(TABLE_TRACKS_ID, resize);
	ui->tableView->setColumnWidth(TABLE_TRACKS_SELECT, resize);
	ui->tableView->setColumnWidth(TABLE_TRACKS_COVER, coverWidth);
	ui->tableView->setColumnWidth(TABLE_TRACKS_PLAY, resize);
	ui->tableView->setColumnWidth(TABLE_TRACKS_TITLE, titleWidth);
	ui->tableView->setColumnWidth(TABLE_TRACKS_FAVORITE, resize);
	ui->tableView->setColumnWidth(TABLE_TRACKS_TIME, resize);
	ui->tableView->setColumnWidth(TABLE_TRACKS_ARTIST, resize);
	ui->tableView->setColumnWidth(TABLE_TRACKS_ALBUM, resize);
	ui->tableView->setColumnWidth(TABLE_TRACKS_GENRE, resize);
	ui->tableView->setColumnWidth(TABLE_TRACKS_ALBUM_ARTIST, resize);
	ui->tableView->setColumnWidth(TABLE_TRACKS_COMPOSER, resize);
	ui->tableView->setColumnWidth(TABLE_TRACKS_YEAR, resize);

	ui->tableView->setColumnWidth(TABLE_TRACKS_MOOD, resize);
	ui->tableView->setColumnWidth(TABLE_TRACKS_TEMPO, resize);
	ui->tableView->setColumnWidth(TABLE_TRACKS_FORMAT, resize);
	ui->tableView->setColumnWidth(TABLE_TRACKS_SAMPLE_RATE, resize);
	ui->tableView->setColumnWidth(TABLE_TRACKS_BIT_DEPTH, resize);
	ui->tableView->setColumnWidth(TABLE_TRACKS_RATING, resize);
	ui->tableView->setColumnWidth(TABLE_TRACKS_INDEX, resize);
	ui->tableView->setColumnWidth(TABLE_TRACKS_MENU, resize);

}

void TableTracks::SetRowResize(int resize)
{

	ui->tableView->verticalHeader()->setDefaultSectionSize(resize);
}

QStandardItemModel *TableTracks::GetModel()
{
	return m_Model;
}

TableTracksDelegate *TableTracks::GetDelegate()
{
	return m_Delegate;
}

void TableTracks::SetBackgroundTask(QThread *thread)
{
	connect(thread, SIGNAL(started()), this, SLOT(SlotReqCoverArt()));
	connect(thread, SIGNAL(finished()), this, SLOT(SlotFinishThread()));
}

void TableTracks::SetHeaderTitle(QString title)
{
	m_Model->setHeaderData(TABLE_TRACKS_TITLE, Qt::Horizontal, title);
}

int TableTracks::GetCntColumnShow()
{
	int showColCnt = 0;
	int colCnt = m_Model->columnCount();
	for (int i = 0; i < colCnt; i++)
	{
		if (!ui->tableView->isColumnHidden(i))
		{
			showColCnt++;
		}
	}

	return showColCnt;
}

bool TableTracks::GetColumnShow(int column)
{
	return !ui->tableView->isColumnHidden(column);
}

void TableTracks::SetColumnShow(int column, bool show)
{
	ui->tableView->setColumnHidden(column, !show);
}

void TableTracks::SlotReqCoverArt()
{
	int index = 0;
	foreach (CJsonNode node, m_NodeList)
	{
		int nID = node.GetString(KEY_ID_LOWER).toInt();
		QThread::msleep(5);
		emit SigReqCoverArt(nID, index, 0);	// 0 is QListView::ListMode
		index++;
	}
}

void TableTracks::SlotFinishThread()
{
//	LogDebug("thread finish good");
}

void TableTracks::SlotScrollValueChanged(int value)
{
//	LogDebug("value [%d] min [%d] max [%d]", value, m_ScrollBar->minimum(), m_ScrollBar->maximum());

	if (value >= m_ScrollBar->maximum())
	{
		emit SigAppendList();
	}
}

void TableTracks::SlotDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
	Q_UNUSED(bottomRight)

	int rowTopLeft = topLeft.row();
	int colTopLeft = topLeft.column();
	if (rowTopLeft < 0 || colTopLeft < 0)
	{
		return;
	}
	QModelIndex indexTopLeft = m_Model->index(rowTopLeft, colTopLeft);
	QStandardItem *itemTopLeft = m_Model->itemFromIndex(indexTopLeft);
//	LogDebug("top left row [%d] col [%d] value [%s]", rowTopLeft, colTopLeft, qvariant_cast<QString>(itemTopLeft->data(Qt::DisplayRole)).toUtf8().data());
}

void TableTracks::SlotClickCell(const QModelIndex &index)
{
	int row = index.row();
	int col = index.column();

//	LogDebug("row [%d] col [%d]", row, col);
	m_ID = qvariant_cast<int>(m_Model->data(m_Model->index(row, TableTracks::TABLE_TRACKS_ID)));
	if (col == TABLE_TRACKS_SELECT)
	{
		bool bSelect = !qvariant_cast<bool>(m_Model->data(m_Model->index(row, TableTracks::TABLE_TRACKS_SELECT)));
		m_Model->setData(index, bSelect);
		if (bSelect)
		{
			m_SelectMap.insert(m_ID, bSelect);
		}
		else
		{
			m_SelectMap.remove(m_ID);
		}
	}
	else if (col == TableTracks::TABLE_TRACKS_PLAY)
	{
		emit SigSelectPlay(m_ID, PLAY_CLEAR);
	}
	else if (col == TableTracks::TABLE_TRACKS_TITLE)
	{
		QString coverArt = qvariant_cast<QString>(m_Model->data(m_Model->index(row, TableTracks::TABLE_TRACKS_COVER)));
		emit SigSelectTitle(m_ID, coverArt);
	}
	else if (col == TableTracks::TABLE_TRACKS_FAVORITE)
	{
		int favorite = qvariant_cast<int>(m_Model->data(m_Model->index(row, TableTracks::TABLE_TRACKS_FAVORITE)));
		favorite = favorite == 0 ? 1 : 0;
		m_Model->setData(index, favorite);
		emit SigSelectFavorite(m_ID, favorite);
	}
	else if (col == TableTracks::TABLE_TRACKS_MENU)
	{
		QRect rect = ui->tableView->visualRect(index);
		QPoint point = ui->tableView->mapToGlobal(rect.center());
		point.setX(point.x() + 30);
//		LogDebug("[%d] [%d]", point.x(), point.y());

		m_Menu->clear();

		QMap<int, QString>::iterator i;
		for (i = m_OptionMenuMap.begin(); i != m_OptionMenuMap.end(); i++)
		{
			QIcon icon = UtilNovatron::GetMenuIcon(i.value());
			QAction *action = new QAction(icon, i.value(), this);
			action->setData(i.key());
			m_Menu->addAction(action);
		}

		m_Menu->popup(ui->tableView->viewport()->mapToParent(point));
	}
}

void TableTracks::SlotMenuAction(QAction *action)
{
	emit SigMenuAction(m_ID, action->data().toInt());
}

void TableTracks::ConnectSigToSlot()
{
	connect(ui->tableView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(SlotClickCell(const QModelIndex&)));
	connect(m_Model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(SlotDataChanged(const QModelIndex&, const QModelIndex&)));
	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuAction(QAction*)));

}

void TableTracks::Initialize()
{
	ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
	ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);
//	ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft|Qt::AlignVCenter);
	ui->tableView->horizontalHeader()->sizeHint().setHeight(100);

	ui->tableView->setModel(m_Model);

	m_Model->setHeaderData(TABLE_TRACKS_SELECT, Qt::Horizontal, "");
	m_Model->setHeaderData(TABLE_TRACKS_COVER, Qt::Horizontal, "");
	m_Model->setHeaderData(TABLE_TRACKS_PLAY, Qt::Horizontal, "");
	m_Model->setHeaderData(TABLE_TRACKS_TITLE, Qt::Horizontal, KEY_TITLE_CAP);
	m_Model->setHeaderData(TABLE_TRACKS_FAVORITE, Qt::Horizontal, KEY_FAVORITE_CAP);
	m_Model->setHeaderData(TABLE_TRACKS_TIME, Qt::Horizontal, KEY_TIME_CAP);
	m_Model->setHeaderData(TABLE_TRACKS_ARTIST, Qt::Horizontal, KEY_ARTIST);
	m_Model->setHeaderData(TABLE_TRACKS_ALBUM, Qt::Horizontal, KEY_ALBUM);
	m_Model->setHeaderData(TABLE_TRACKS_GENRE, Qt::Horizontal, KEY_GENRE);
	m_Model->setHeaderData(TABLE_TRACKS_ALBUM_ARTIST, Qt::Horizontal, KEY_ALBUM_ARTIST);
	m_Model->setHeaderData(TABLE_TRACKS_COMPOSER, Qt::Horizontal, KEY_COMPOSER);
	m_Model->setHeaderData(TABLE_TRACKS_YEAR, Qt::Horizontal, KEY_YEAR);

	m_Model->setHeaderData(TABLE_TRACKS_MOOD, Qt::Horizontal, KEY_MOOD);
	m_Model->setHeaderData(TABLE_TRACKS_TEMPO, Qt::Horizontal, KEY_TEMPO);
	m_Model->setHeaderData(TABLE_TRACKS_FORMAT, Qt::Horizontal, KEY_FORMAT);
	m_Model->setHeaderData(TABLE_TRACKS_SAMPLE_RATE, Qt::Horizontal, KEY_SAMPLERATE_CAP);
	m_Model->setHeaderData(TABLE_TRACKS_BIT_DEPTH, Qt::Horizontal, KEY_BIT_DEPTH);
	m_Model->setHeaderData(TABLE_TRACKS_RATING, Qt::Horizontal, KEY_RATING);
	m_Model->setHeaderData(TABLE_TRACKS_MENU, Qt::Horizontal, KEY_MENU);

	ui->tableView->setColumnHidden(TABLE_TRACKS_ID, true);
//	ui->tableView->setColumnHidden(TABLE_TRACKS_SELECT, true);
//	ui->tableView->setColumnHidden(TABLE_TRACKS_COVER, true);
//	ui->tableView->setColumnHidden(TABLE_TRACKS_PLAY, true);
//	ui->tableView->setColumnHidden(TABLE_TRACKS_TITLE, true);
	ui->tableView->setColumnHidden(TABLE_TRACKS_FAVORITE, true);
	ui->tableView->setColumnHidden(TABLE_TRACKS_TIME, true);
	ui->tableView->setColumnHidden(TABLE_TRACKS_ARTIST, true);
	ui->tableView->setColumnHidden(TABLE_TRACKS_ALBUM, true);
	ui->tableView->setColumnHidden(TABLE_TRACKS_GENRE, true);
	ui->tableView->setColumnHidden(TABLE_TRACKS_ALBUM_ARTIST, true);
	ui->tableView->setColumnHidden(TABLE_TRACKS_COMPOSER, true);
	ui->tableView->setColumnHidden(TABLE_TRACKS_YEAR, true);

	ui->tableView->setColumnHidden(TABLE_TRACKS_MOOD, true);
	ui->tableView->setColumnHidden(TABLE_TRACKS_TEMPO, true);
	ui->tableView->setColumnHidden(TABLE_TRACKS_FORMAT, true);
	ui->tableView->setColumnHidden(TABLE_TRACKS_SAMPLE_RATE, true);
	ui->tableView->setColumnHidden(TABLE_TRACKS_BIT_DEPTH, true);
	ui->tableView->setColumnHidden(TABLE_TRACKS_RATING, true);
	ui->tableView->setColumnHidden(TABLE_TRACKS_INDEX, true);
//	ui->tableView->setColumnHidden(TABLE_TRACKS_MENU, true);

	ui->tableView->setItemDelegateForColumn(TABLE_TRACKS_SELECT, m_Delegate);
	ui->tableView->setItemDelegateForColumn(TABLE_TRACKS_COVER, m_Delegate);
	ui->tableView->setItemDelegateForColumn(TABLE_TRACKS_PLAY, m_Delegate);
	ui->tableView->setItemDelegateForColumn(TABLE_TRACKS_FAVORITE, m_Delegate);
//	ui->tableView->setItemDelegateForColumn(TABLE_TRACKS_RATING, m_Delegate);
	ui->tableView->setItemDelegateForColumn(TABLE_TRACKS_MENU, m_Delegate);

	SetColResize(0);
	SetRowResize(LIST_HEIGHT_MIN);

	m_ScrollBar = ui->tableView->verticalScrollBar();
	connect(m_ScrollBar, SIGNAL(valueChanged(int)), this, SLOT(SlotScrollValueChanged(int)));

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

	m_Menu->setStyleSheet(style);

	m_ScrollBar = ui->tableView->verticalScrollBar();
	connect(m_ScrollBar, SIGNAL(valueChanged(int)), this, SLOT(SlotScrollValueChanged(int)));
}

