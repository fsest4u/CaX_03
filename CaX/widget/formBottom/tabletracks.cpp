#include <QThread>
#include <QScrollBar>
#include <QHeaderView>

#include "tabletracks.h"
#include "ui_tabletracks.h"
#include "tabletracksdelegate.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"
#include "util/settingio.h"
#include "util/utilnovatron.h"

const QString SETTINGS_GROUP = "TableTracks";

TableTracks::TableTracks(QWidget *parent) :
	QWidget(parent),
	m_Model(new QStandardItemModel(0, TABLE_TRACKS_MAX)),
	m_Delegate(new TableTracksDelegate),
	m_ScrollBar(nullptr),
	m_Menu(new QMenu(this)),
	ui(new Ui::TableTracks)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	ReadSettings();
	Initialize();
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
	Q_UNUSED(service)

	int index = m_NodeList.count();
	m_NodeList.append(list);

	{
		foreach (CJsonNode node, list)
		{
			LogDebug("node [%s]", node.ToCompactByteArray().data());
			m_Model->setVerticalHeaderItem(index, new QStandardItem());

			int seconds = node.GetInt(KEY_TIME);
			QString hhmmss = UtilNovatron::CalcSecondToHMS(seconds);
//			QString artist;
//			if (!node.GetString(KEY_ARTIST).isEmpty())
//			{
//				artist = node.GetString(KEY_ARTIST);
//			}
//			else if (!node.GetString(KEY_SUBTITLE).isEmpty())
//			{
//				artist = node.GetString(KEY_SUBTITLE);
//			}
			int nID = node.GetString(KEY_ID_LOWER).toInt();
			QString extension = UtilNovatron::GetSuffix(node.GetString(KEY_FORMAT));
			QString bitrate = UtilNovatron::ConvertBitrate(node.GetString(KEY_BITRATE));
			QString samplerate = UtilNovatron::ConvertSamplerate(node.GetString(KEY_SAMPLERATE_CAP));

			m_Model->setData(m_Model->index(index, TABLE_TRACKS_ID), nID, Qt::DisplayRole);
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_SELECT), false);
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_TITLE), node.GetString(KEY_TITLE), Qt::DisplayRole);
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_FAVORITE), node.GetString(KEY_FAVORITE_CAP));
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_RATING), node.GetString(KEY_RATING));
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_TIME), hhmmss);
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_ARTIST), node.GetString(KEY_ARTIST));
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_ALBUM), node.GetString(KEY_ALBUM));
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_GENRE), node.GetString(KEY_GENRE));
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_ALBUM_ARTIST), node.GetString(KEY_ALBUM_ARTIST));
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_COMPOSER), node.GetString(KEY_COMPOSER));
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_YEAR), node.GetString(KEY_YEAR));
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_MOOD), node.GetString(KEY_MOOD));
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_TEMPO), node.GetString(KEY_TEMPO));
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_FORMAT), extension);
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_SAMPLE_RATE), samplerate);
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_BITRATE), bitrate);
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_TRACK_COUNT), node.GetString(KEY_COUNT));
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_INDEX), index);
			m_Model->setData(m_Model->index(index, TABLE_TRACKS_MENU), false);

			emit SigReqCoverArt(nID, index, 0);	// 0 is QListView::ListMode
			index++;
		}
	}

}

void TableTracks::ClearNodeList()
{
	int rowCnt = m_Model->rowCount();
	m_Model->removeRows(0, rowCnt);
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
	resize = 60;

	int colCnt = m_Model->columnCount();
	if (colCnt <= 0 )
	{
		return;
	}

	int showColCnt = GetCntColumnShow();

	if (m_ColWidthSelect <= 0)
	{
		m_ColWidthSelect = resize;
	}
	if (m_ColWidthCover <= 0)
	{
		m_ColWidthCover = resize * 2;
	}
	if (m_ColWidthPlay <= 0)
	{
		m_ColWidthPlay = resize;
	}
	if (m_ColWidthTitle <= 0)
	{
		m_ColWidthTitle = resize * (colCnt - showColCnt);
	}
	if (m_ColWidthFavorite <= 0)
	{
		m_ColWidthFavorite = resize;
	}
	if (m_ColWidthTime <= 0)
	{
		m_ColWidthTime = resize;
	}
	if (m_ColWidthArtist <= 0)
	{
		m_ColWidthArtist = resize;
	}
	if (m_ColWidthAlbum <= 0)
	{
		m_ColWidthAlbum = resize;
	}
	if (m_ColWidthGenre <= 0)
	{
		m_ColWidthGenre = resize;
	}
	if (m_ColWidthAlbumArtist <= 0)
	{
		m_ColWidthAlbumArtist = resize;
	}
	if (m_ColWidthComposer <= 0)
	{
		m_ColWidthComposer = resize;
	}
	if (m_ColWidthYear <= 0)
	{
		m_ColWidthYear = resize;
	}
	if (m_ColWidthMood <= 0)
	{
		m_ColWidthMood = resize;
	}
	if (m_ColWidthTempo <= 0)
	{
		m_ColWidthTempo = resize;
	}
	if (m_ColWidthFormat <= 0)
	{
		m_ColWidthFormat = resize;
	}
	if (m_ColWidthSampleRate <= 0)
	{
		m_ColWidthSampleRate = resize;
	}
	if (m_ColWidthBitrate <= 0)
	{
		m_ColWidthBitrate = resize;
	}
	if (m_ColWidthRating <= 0)
	{
		m_ColWidthRating = resize;
	}
	if (m_ColWidthMenu <= 0)
	{
		m_ColWidthMenu = resize;
	}

	ui->tableView->setColumnWidth(TABLE_TRACKS_SELECT, m_ColWidthSelect);
	ui->tableView->setColumnWidth(TABLE_TRACKS_COVER, m_ColWidthCover);
	ui->tableView->setColumnWidth(TABLE_TRACKS_PLAY, m_ColWidthPlay);
	ui->tableView->setColumnWidth(TABLE_TRACKS_TITLE, m_ColWidthTitle);
	ui->tableView->setColumnWidth(TABLE_TRACKS_FAVORITE, m_ColWidthFavorite);
	ui->tableView->setColumnWidth(TABLE_TRACKS_RATING, m_ColWidthRating);
	ui->tableView->setColumnWidth(TABLE_TRACKS_TIME, m_ColWidthTime);
	ui->tableView->setColumnWidth(TABLE_TRACKS_ARTIST, m_ColWidthArtist);
	ui->tableView->setColumnWidth(TABLE_TRACKS_ALBUM, m_ColWidthAlbum);
	ui->tableView->setColumnWidth(TABLE_TRACKS_GENRE, m_ColWidthGenre);
	ui->tableView->setColumnWidth(TABLE_TRACKS_ALBUM_ARTIST, m_ColWidthAlbumArtist);
	ui->tableView->setColumnWidth(TABLE_TRACKS_COMPOSER, m_ColWidthComposer);
	ui->tableView->setColumnWidth(TABLE_TRACKS_YEAR, m_ColWidthYear);
	ui->tableView->setColumnWidth(TABLE_TRACKS_MOOD, m_ColWidthMood);
	ui->tableView->setColumnWidth(TABLE_TRACKS_TEMPO, m_ColWidthTempo);
	ui->tableView->setColumnWidth(TABLE_TRACKS_FORMAT, m_ColWidthFormat);
	ui->tableView->setColumnWidth(TABLE_TRACKS_SAMPLE_RATE, m_ColWidthSampleRate);
	ui->tableView->setColumnWidth(TABLE_TRACKS_BITRATE, m_ColWidthBitrate);
	ui->tableView->setColumnWidth(TABLE_TRACKS_TRACK_COUNT, m_ColWidthTrackCount);
	ui->tableView->setColumnWidth(TABLE_TRACKS_MENU, m_ColWidthMenu);

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

void TableTracks::resizeEvent(QResizeEvent *event)
{
	LogDebug("resizeEvent");

	SetColResize(0);
}

void TableTracks::SlotScrollValueChanged(int value)
{
	int min = m_ScrollBar->minimum();
	int max = m_ScrollBar->maximum();
//	LogDebug("value [%d] min [%d] max [%d]", value, min, max);
	if (value >= max && max != 0)
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
		int nIndex = qvariant_cast<int>(m_Model->data(m_Model->index(row, TableTracks::TABLE_TRACKS_INDEX)));
		int favorite = qvariant_cast<int>(m_Model->data(m_Model->index(row, TableTracks::TABLE_TRACKS_FAVORITE)));
//		favorite = favorite == 0 ? 1 : 0;
//		m_Model->setData(index, favorite);
		emit SigSelectFavorite(m_ID, nIndex, favorite);
	}
	else if (col == TableTracks::TABLE_TRACKS_RATING)
	{

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

void TableTracks::SlotSectionResize(int logicalIndex, int oldWidth, int newWidth)
{
//	LogDebug("col [%d] old [%d] new [%d]", logicalIndex, oldWidth, newWidth);
	if (newWidth <= 0)
	{
		return;
	}

	switch (logicalIndex)
	{
	case TABLE_TRACKS_ID:
		m_ColWidthID = newWidth;
		break;
	case TABLE_TRACKS_SELECT:
		m_ColWidthSelect = newWidth;
		break;
	case TABLE_TRACKS_COVER:
		m_ColWidthCover = newWidth;
		break;
	case TABLE_TRACKS_PLAY:
		m_ColWidthPlay = newWidth;
		break;
	case TABLE_TRACKS_TITLE:
		m_ColWidthTitle = newWidth;
		break;
	case TABLE_TRACKS_FAVORITE:
		m_ColWidthFavorite = newWidth;
		break;
	case TABLE_TRACKS_RATING:
		m_ColWidthRating = newWidth;
		break;
	case TABLE_TRACKS_TIME:
		m_ColWidthTime = newWidth;
		break;
	case TABLE_TRACKS_ARTIST:
		m_ColWidthArtist = newWidth;
		break;
	case TABLE_TRACKS_ALBUM:
		m_ColWidthAlbum = newWidth;
		break;
	case TABLE_TRACKS_GENRE:
		m_ColWidthGenre = newWidth;
		break;
	case TABLE_TRACKS_ALBUM_ARTIST:
		m_ColWidthAlbumArtist = newWidth;
		break;
	case TABLE_TRACKS_COMPOSER:
		m_ColWidthComposer = newWidth;
		break;
	case TABLE_TRACKS_YEAR:
		m_ColWidthYear = newWidth;
		break;
	case TABLE_TRACKS_MOOD:
		m_ColWidthMood = newWidth;
		break;
	case TABLE_TRACKS_TEMPO:
		m_ColWidthTempo = newWidth;
		break;
	case TABLE_TRACKS_FORMAT:
		m_ColWidthFormat = newWidth;
		break;
	case TABLE_TRACKS_SAMPLE_RATE:
		m_ColWidthSampleRate = newWidth;
		break;
	case TABLE_TRACKS_BITRATE:
		m_ColWidthBitrate = newWidth;
		break;
	case TABLE_TRACKS_TRACK_COUNT:
		m_ColWidthTrackCount = newWidth;
		break;
	case TABLE_TRACKS_INDEX:
		m_ColWidthIndex = newWidth;
		break;
	case TABLE_TRACKS_MENU:
		m_ColWidthMenu = newWidth;
		break;

	}

	WriteSettings();

}

void TableTracks::SlotMenuAction(QAction *action)
{
	emit SigMenuAction(m_ID, action->data().toInt());
}

void TableTracks::ReadSettings()
{
	SettingIO settings;
	settings.beginGroup(SETTINGS_GROUP);

	m_ColWidthID = settings.value("col_width_id").toInt();
	m_ColWidthSelect = settings.value("col_width_select").toInt();
	m_ColWidthCover = settings.value("col_width_cover").toInt();
	m_ColWidthPlay = settings.value("col_width_play").toInt();
	m_ColWidthTitle = settings.value("col_width_title").toInt();
	m_ColWidthFavorite = settings.value("col_width_favorite").toInt();
	m_ColWidthTime = settings.value("col_width_time").toInt();
	m_ColWidthArtist = settings.value("col_width_artist").toInt();
	m_ColWidthAlbum = settings.value("col_width_album").toInt();
	m_ColWidthGenre = settings.value("col_width_genre").toInt();
	m_ColWidthAlbumArtist = settings.value("col_width_album_artist").toInt();
	m_ColWidthComposer = settings.value("col_width_composer").toInt();
	m_ColWidthYear = settings.value("col_width_year").toInt();
	m_ColWidthMood = settings.value("col_width_mood").toInt();
	m_ColWidthTempo = settings.value("col_width_tempo").toInt();
	m_ColWidthFormat = settings.value("col_width_format").toInt();
	m_ColWidthSampleRate = settings.value("col_width_sample_rate").toInt();
	m_ColWidthBitrate = settings.value("col_width_bitrate").toInt();
	m_ColWidthRating = settings.value("col_width_rating").toInt();
	m_ColWidthIndex = settings.value("col_width_index").toInt();
	m_ColWidthMenu = settings.value("col_width_menu").toInt();

	settings.endGroup();

}

void TableTracks::WriteSettings()
{
	SettingIO settings;
	settings.beginGroup(SETTINGS_GROUP);

	settings.setValue("col_width_id", m_ColWidthID);
	settings.setValue("col_width_select", m_ColWidthSelect);
	settings.setValue("col_width_cover", m_ColWidthCover);
	settings.setValue("col_width_play", m_ColWidthPlay);
	settings.setValue("col_width_title", m_ColWidthTitle);
	settings.setValue("col_width_favorite", m_ColWidthFavorite);
	settings.setValue("col_width_time", m_ColWidthTime);
	settings.setValue("col_width_artist", m_ColWidthArtist);
	settings.setValue("col_width_album", m_ColWidthAlbum);
	settings.setValue("col_width_genre", m_ColWidthGenre);
	settings.setValue("col_width_album_artist", m_ColWidthAlbumArtist);
	settings.setValue("col_width_composer", m_ColWidthComposer);
	settings.setValue("col_width_year", m_ColWidthYear);
	settings.setValue("col_width_mood", m_ColWidthMood);
	settings.setValue("col_width_tempo", m_ColWidthTempo);
	settings.setValue("col_width_format", m_ColWidthFormat);
	settings.setValue("col_width_sample_rate", m_ColWidthSampleRate);
	settings.setValue("col_width_bitrate", m_ColWidthBitrate);
	settings.setValue("col_width_rating", m_ColWidthRating);
	settings.setValue("col_width_index", m_ColWidthIndex);
	settings.setValue("col_width_menu", m_ColWidthMenu);

	settings.endGroup();

}

void TableTracks::ConnectSigToSlot()
{
	connect(ui->tableView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(SlotClickCell(const QModelIndex&)));
	connect(ui->tableView->horizontalHeader(), SIGNAL(sectionResized(int, int, int)), this, SLOT(SlotSectionResize(int, int, int)));
	connect(m_Model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(SlotDataChanged(const QModelIndex&, const QModelIndex&)));
	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuAction(QAction*)));

}

void TableTracks::Initialize()
{
	ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
	ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);
	ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft|Qt::AlignVCenter);
	ui->tableView->horizontalHeader()->sizeHint().setHeight(100);

	ui->tableView->setModel(m_Model);

	m_Model->setHeaderData(TABLE_TRACKS_SELECT, Qt::Horizontal, "");
	m_Model->setHeaderData(TABLE_TRACKS_COVER, Qt::Horizontal, "");
	m_Model->setHeaderData(TABLE_TRACKS_PLAY, Qt::Horizontal, "");
	m_Model->setHeaderData(TABLE_TRACKS_TITLE, Qt::Horizontal, KEY_TITLE_CAP);
	m_Model->setHeaderData(TABLE_TRACKS_FAVORITE, Qt::Horizontal, KEY_FAVORITE_CAP);
	m_Model->setHeaderData(TABLE_TRACKS_RATING, Qt::Horizontal, KEY_RATING_CAP);
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
	m_Model->setHeaderData(TABLE_TRACKS_BITRATE, Qt::Horizontal, KEY_BITRATE);
	m_Model->setHeaderData(TABLE_TRACKS_TRACK_COUNT, Qt::Horizontal, KEY_TRACK);
	m_Model->setHeaderData(TABLE_TRACKS_MENU, Qt::Horizontal, KEY_MENU);

	ui->tableView->setColumnHidden(TABLE_TRACKS_ID, true);
//	ui->tableView->setColumnHidden(TABLE_TRACKS_SELECT, true);
//	ui->tableView->setColumnHidden(TABLE_TRACKS_COVER, true);
//	ui->tableView->setColumnHidden(TABLE_TRACKS_PLAY, true);
//	ui->tableView->setColumnHidden(TABLE_TRACKS_TITLE, true);
	ui->tableView->setColumnHidden(TABLE_TRACKS_FAVORITE, true);
	ui->tableView->setColumnHidden(TABLE_TRACKS_RATING, true);
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
	ui->tableView->setColumnHidden(TABLE_TRACKS_BITRATE, true);
	ui->tableView->setColumnHidden(TABLE_TRACKS_TRACK_COUNT, true);
	ui->tableView->setColumnHidden(TABLE_TRACKS_INDEX, true);
//	ui->tableView->setColumnHidden(TABLE_TRACKS_MENU, true);

	ui->tableView->setItemDelegateForColumn(TABLE_TRACKS_SELECT, m_Delegate);
	ui->tableView->setItemDelegateForColumn(TABLE_TRACKS_COVER, m_Delegate);
	ui->tableView->setItemDelegateForColumn(TABLE_TRACKS_PLAY, m_Delegate);
	ui->tableView->setItemDelegateForColumn(TABLE_TRACKS_FAVORITE, m_Delegate);
	ui->tableView->setItemDelegateForColumn(TABLE_TRACKS_RATING, m_Delegate);
	ui->tableView->setItemDelegateForColumn(TABLE_TRACKS_MENU, m_Delegate);

	SetColResize(0);
	SetRowResize(TABLE_HEIGHT_MIN);

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

