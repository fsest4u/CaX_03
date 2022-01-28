#include <QScrollBar>

#include "listtracks.h"
#include "ui_listtracks.h"

#include "listtracksdelegate.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/caxtranslate.h"
#include "util/loading.h"
#include "util/log.h"
#include "util/utilnovatron.h"

ListTracks::ListTracks(QWidget *parent) :
	QWidget(parent),
	m_ListView(new QListView),
	m_Model(new QStandardItemModel),
	m_Delegate(new ListTracksDelegate),
	m_ScrollBar(nullptr),
//	m_pLoading(new Loading(this)),
	ui(new Ui::ListTracks)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();
}

ListTracks::~ListTracks()
{
	if (m_ScrollBar)
	{
		delete m_ScrollBar;
		m_ScrollBar = nullptr;
	}

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

//	if (m_pLoading)
//	{
//		delete m_pLoading;
//		m_pLoading = nullptr;
//	}

	delete ui;

}

QList<CJsonNode> ListTracks::GetNodeList() const
{
	return m_NodeList;
}

void ListTracks::SetNodeList(QList<CJsonNode> list, int service)
{
//	m_pLoading->Start();
	int index = m_NodeList.count();
	m_NodeList.append(list);

	if (SIDEMENU_AUDIO_CD == service)
	{
		foreach (CJsonNode node, list)
		{
//			LogDebug("node [%s]", node.ToCompactByteArray().data());
			int seconds = node.GetInt(KEY_TIME_CAP);
			QString hhmmss = UtilNovatron::CalcSecondToHMS(seconds);

			QStandardItem *item = new QStandardItem;
			int nID = node.GetString(KEY_TRACK).toInt();
			item->setData(nID, ListTracksDelegate::LIST_TRACKS_ID);
			item->setData(UtilNovatron::GetCoverArtIcon(SIDEMENU_AUDIO_CD), ListTracksDelegate::LIST_TRACKS_COVER);
			item->setData(node.GetString(KEY_TOP), ListTracksDelegate::LIST_TRACKS_TITLE);
			item->setData(hhmmss, ListTracksDelegate::LIST_TRACKS_TIME);
			item->setData(node.GetString(KEY_ARTIST), ListTracksDelegate::LIST_TRACKS_ARTIST);
			item->setData(node.GetString(KEY_ALBUM), ListTracksDelegate::LIST_TRACKS_ALBUM);
			item->setData(node.GetString(KEY_GENRE), ListTracksDelegate::LIST_TRACKS_GENRE);
			item->setData(index, ListTracksDelegate::LIST_TRACKS_INDEX);
			item->setData(false, ListTracksDelegate::LIST_TRACKS_SELECT);

			m_Model->appendRow(item);

			emit SigReqCoverArt(nID, index, QListView::ListMode);
			index++;
		}
	}
	else if (SIDEMENU_PLAYLIST == service)
	{
		foreach (CJsonNode node, list)
		{
//			LogDebug("node [%s]", node.ToCompactByteArray().data());
			int seconds = node.GetInt(KEY_TIME);
			QString hhmmss = UtilNovatron::CalcSecondToHMS(seconds);

			QStandardItem *item = new QStandardItem;
			int nID = node.GetString(KEY_ID_LOWER).toInt();
			item->setData(nID, ListTracksDelegate::LIST_TRACKS_ID);
			item->setData(node.GetString(KEY_TITLE), ListTracksDelegate::LIST_TRACKS_TITLE);
			item->setData(hhmmss, ListTracksDelegate::LIST_TRACKS_TIME);
			item->setData(node.GetString(KEY_ARTIST), ListTracksDelegate::LIST_TRACKS_ARTIST);
			item->setData(node.GetString(KEY_ALBUM), ListTracksDelegate::LIST_TRACKS_ALBUM);
			item->setData(node.GetString(KEY_GENRE), ListTracksDelegate::LIST_TRACKS_GENRE);
			item->setData(index, ListTracksDelegate::LIST_TRACKS_INDEX);
			item->setData(false, ListTracksDelegate::LIST_TRACKS_SELECT);

			m_Model->appendRow(item);

			emit SigReqCoverArt(nID, index, QListView::ListMode);
			index++;
		}
	}
//	else	// for music db
//	{

//		foreach (CJsonNode node, list)
//		{
////			LogDebug("node [%s]", node.ToCompactByteArray().data());
//			int seconds = node.GetInt(KEY_TIME);
//			QString hhmmss = UtilNovatron::CalcSecondToHMS(seconds);
//			QString artist;
//			if (!node.GetString(KEY_ARTIST).isEmpty())
//			{
//				artist = node.GetString(KEY_ARTIST);
//			}
//			else if (!node.GetString(KEY_SUBTITLE).isEmpty())
//			{
//				artist = node.GetString(KEY_SUBTITLE);
//			}

//			QStandardItem *item = new QStandardItem;
//			int nID = node.GetString(KEY_ID_LOWER).toInt();
//			item->setData(nID, ListTracksDelegate::LIST_TRACKS_ID);
//			item->setData(node.GetString(KEY_TITLE), ListTracksDelegate::LIST_TRACKS_TITLE);
//			item->setData(node.GetString(KEY_FAVORITE_CAP).toInt(), ListTracksDelegate::LIST_TRACKS_FAVORITE);
//			item->setData(hhmmss, ListTracksDelegate::LIST_TRACKS_TIME);
//			item->setData(artist, ListTracksDelegate::LIST_TRACKS_ARTIST);
//			item->setData(node.GetString(KEY_ALBUM), ListTracksDelegate::LIST_TRACKS_ALBUM);
//			item->setData(node.GetString(KEY_GENRE), ListTracksDelegate::LIST_TRACKS_GENRE);
//			item->setData(node.GetString(KEY_ALBUM_ARTIST), ListTracksDelegate::LIST_TRACKS_ALBUM_ARTIST);
//			item->setData(node.GetString(KEY_COMPOSER), ListTracksDelegate::LIST_TRACKS_COMPOSER);
//			item->setData(node.GetString(KEY_YEAR), ListTracksDelegate::LIST_TRACKS_YEAR);
//			item->setData(node.GetString(KEY_MOOD), ListTracksDelegate::LIST_TRACKS_MOOD);
//			item->setData(node.GetString(KEY_TEMPO), ListTracksDelegate::LIST_TRACKS_TEMPO);
//			item->setData(node.GetString(KEY_FORMAT), ListTracksDelegate::LIST_TRACKS_FORMAT);
//			item->setData(node.GetString(KEY_SAMPLERATE_CAP), ListTracksDelegate::LIST_TRACKS_SAMPLE_RATE);
//			item->setData(node.GetString(KEY_BITRATE), ListTracksDelegate::LIST_TRACKS_BIT_DEPTH);
//			item->setData(node.GetString(KEY_RATING_CAP).toInt(), ListTracksDelegate::LIST_TRACKS_RATING);
//			item->setData(index, ListTracksDelegate::LIST_TRACKS_INDEX);
//			item->setData(false, ListTracksDelegate::LIST_TRACKS_SELECT);

//			m_Model->appendRow(item);

//			emit SigReqCoverArt(nID, index, QListView::ListMode);
//			index++;
//		}
//	}

//	m_pLoading->Stop();
}

void ListTracks::ClearNodeList()
{
	m_Model->clear();
	m_NodeList.clear();
	m_SelectMap.clear();
}

void ListTracks::ClearSelectMap()
{
	int count = m_Model->rowCount();

	for (int i = 0; i < count; i++)
	{
		QModelIndex modelIndex = m_Model->index(i, 0);
		m_Model->setData(modelIndex, false, ListTracksDelegate::LIST_TRACKS_SELECT);
		int id = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_ID));
		m_SelectMap.remove(id);
	}
}

void ListTracks::SetAllSelectMap()
{
	int count = m_Model->rowCount();

	for (int i = 0; i < count; i++)
	{
		QModelIndex modelIndex = m_Model->index(i, 0);
		m_Model->setData(modelIndex, true, ListTracksDelegate::LIST_TRACKS_SELECT);
		int id = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_ID));
		m_SelectMap.insert(id, true);
	}
}

QMap<int, bool> ListTracks::GetSelectMap() const
{
	return m_SelectMap;
}

void ListTracks::SetSelectMap(const QMap<int, bool> &SelectMap)
{
	m_SelectMap = SelectMap;
}

void ListTracks::SetResize(int resize)
{
	m_Delegate->SetResize(resize);
	m_ListView->setGridSize(QSize(resize, resize));
}

//void ListTracks::SetEditor(int index)
//{
//	QModelIndex modelIndex = m_Model->index(index, 0);

//	QStandardItem *item = m_Model->itemFromIndex(modelIndex);
//	QString nothing = qvariant_cast<QString>(item->data(ListTracksDelegate::LIST_TRACKS_MAX));
//	item->setData(nothing + " ", ListTracksDelegate::LIST_TRACKS_MAX);

//	m_ListView->openPersistentEditor(modelIndex);
//}

QListView *ListTracks::GetListView()
{
	return m_ListView;
}

QStandardItemModel *ListTracks::GetModel()
{
	return m_Model;
}

ListTracksDelegate *ListTracks::GetDelegate()
{
	return m_Delegate;
}

//void ListTracks::SetHeaderTitle(QString title)
//{
//	ui->labelHeaderTitle->setText(title);
//}

//void ListTracks::ShowHeaderTime(bool show)
//{
//	if (show)
//	{
//		ui->labelHeaderTime->show();
//	}
//	else
//	{
//		ui->labelHeaderTime->hide();
//	}
//}

//void ListTracks::ShowHeaderArtist(bool show)
//{
//	if (show)
//	{
//		ui->labelHeaderArtist->show();
//	}
//	else
//	{
//		ui->labelHeaderArtist->hide();
//	}
//}

//void ListTracks::ShowHeaderAlbum(bool show)
//{
//	if (show)
//	{
//		ui->labelHeaderAlbum->show();
//	}
//	else
//	{
//		ui->labelHeaderAlbum->hide();
//	}
//}

//void ListTracks::ShowHeaderGenre(bool show)
//{
//	if (show)
//	{
//		ui->labelHeaderGenre->show();
//	}
//	else
//	{
//		ui->labelHeaderGenre->hide();
//	}
//}

void ListTracks::SlotScrollValueChanged(int value)
{
	int min = m_ScrollBar->minimum();
	int max = m_ScrollBar->maximum();
//	LogDebug("value [%d] min [%d] max [%d]", value, min, max);
	if (value >= max)
	{
		emit SigAppendList();
	}
}

void ListTracks::SlotSelectCheck(const QModelIndex &modelIndex)
{
	int id = qvariant_cast<int>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_ID));
	bool bSelect = !qvariant_cast<bool>(modelIndex.data(ListTracksDelegate::LIST_TRACKS_SELECT));
	m_Model->setData(modelIndex, bSelect, ListTracksDelegate::LIST_TRACKS_SELECT);

	if (bSelect)
	{
		m_SelectMap.insert(id, bSelect);
	}
	else
	{
		m_SelectMap.remove(id);
	}
}

//void ListTracks::SlotEditAllArtist()
//{
//	emit SigEditAllArtist(ui->lineEditHeaderArtist->text());
//	ui->lineEditHeaderArtist->setText("");
//}

//void ListTracks::SlotEditAllAlbum()
//{
//	emit SigEditAllAlbum(ui->lineEditHeaderAlbum->text());
//	ui->lineEditHeaderAlbum->setText("");
//}

//void ListTracks::SlotEditAllGenre()
//{
//	emit SigEditAllGenre(ui->lineEditHeaderGenre->text());
//	ui->lineEditHeaderGenre->setText("");
//}

void ListTracks::ConnectSigToSlot()
{
//	connect(ui->lineEditHeaderArtist, SIGNAL(returnPressed()), this, SLOT(SlotEditAllArtist()));
//	connect(ui->lineEditHeaderAlbum, SIGNAL(returnPressed()), this, SLOT(SlotEditAllAlbum()));
//	connect(ui->lineEditHeaderGenre, SIGNAL(returnPressed()), this, SLOT(SlotEditAllGenre()));
}

void ListTracks::Initialize()
{
	m_ListView->setItemDelegate(m_Delegate);
	m_ListView->setModel(m_Model);
	m_ListView->setResizeMode(QListView::Adjust);
	m_ListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_ListView->setViewMode(QListView::ListMode);
	m_ListView->setEditTriggers(QAbstractItemView::EditTrigger::AllEditTriggers);
	m_ListView->setMouseTracking(true);
	SetResize(LIST_HEIGHT_MIN);

	m_ScrollBar = m_ListView->verticalScrollBar();
	connect(m_ScrollBar, SIGNAL(valueChanged(int)), this, SLOT(SlotScrollValueChanged(int)));
	connect(m_Delegate, SIGNAL(SigSelectCheck(const QModelIndex&)), this, SLOT(SlotSelectCheck(const QModelIndex&)));

	ui->gridLayout->addWidget(m_ListView);

//	ui->labelHeaderTitle->setText(STR_TITLE);
//	ui->labelHeaderMenu->setText(STR_MENU);

	ui->labelHeaderTitle->hide();
	ui->labelHeaderTime->hide();
	ui->labelHeaderArtist->hide();
	ui->labelHeaderAlbum->hide();
	ui->labelHeaderGenre->hide();
	ui->labelHeaderMenu->hide();

}

