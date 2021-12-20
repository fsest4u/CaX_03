#include <QApplication>
//#include <QPainter>
//#include <QFile>
//#include <QLabel>

#include "listtracksdelegate.h"
#include "listtrackseditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

#include "widget/form/formcoverart.h"

ListTracksDelegate::ListTracksDelegate()
{
}

void ListTracksDelegate::SlotClickCoverArt(int index)
{
	emit SigSelectCoverArt(index);
}

void ListTracksDelegate::SlotClickPlay(int nID)
{
	emit SigSelectPlay(nID, PLAY_CLEAR);
}

void ListTracksDelegate::SlotClickTitle(int nID, QString coverArt)
{
	emit SigSelectTitle(nID, coverArt);
}

void ListTracksDelegate::SlotClickFavorite(int nID, int nFavorite)
{
	emit SigSelectFavorite(nID, nFavorite);
}

void ListTracksDelegate::SlotClickTime(int nID)
{
	Q_UNUSED(nID);
	LogDebug("click time");
}

void ListTracksDelegate::SlotClickArtist(int nID)
{
	Q_UNUSED(nID);
	LogDebug("click artist");
}

void ListTracksDelegate::SlotClickAlbum(int nID)
{
	Q_UNUSED(nID);
	LogDebug("click album");
}

void ListTracksDelegate::SlotClickGenre(int nID)
{
	Q_UNUSED(nID);
	LogDebug("click genre");
}

void ListTracksDelegate::SlotMenuAction(int nID, int menuID)
{
	emit SigMenuAction(nID, menuID);
}

void ListTracksDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(painter);
	Q_UNUSED(option);
	Q_UNUSED(index);

//	QStyledItemDelegate::paint(painter, option, index);
//	painter->save();

//	int nID = qvariant_cast<int>(index.data(LIST_TRACKS_ID));
//	QString cover = qvariant_cast<QString>(index.data(LIST_TRACKS_COVER));
//	QString title = qvariant_cast<QString>(index.data(LIST_TRACKS_TITLE));
//	bool favorite = qvariant_cast<bool>(index.data(LIST_TRACKS_FAVORITE));
//	QString time = qvariant_cast<QString>(index.data(LIST_TRACKS_TIME));
//	QString artist = qvariant_cast<QString>(index.data(LIST_TRACKS_ARTIST));
//	QString album = qvariant_cast<QString>(index.data(LIST_TRACKS_ALBUM));
//	QString genre = qvariant_cast<QString>(index.data(LIST_TRACKS_GENRE));

//	//LogDebug("rect [%d] - [%d][%d][%d][%d]", nID, option.rect.top(), option.rect.bottom(), option.rect.left(), option.rect.right());

//	QRect coverRect = option.rect;
//	QRect titleRect = option.rect;
//	QRect favoriteRect = option.rect;
//	QRect timeRect = option.rect;
//	QRect artistRect = option.rect;
//	QRect albumRect = option.rect;
//	QRect genreRect = option.rect;

//	if (m_ViewMode == QListView::ViewMode::ListMode)
//	{
//		coverRect.setWidth(coverRect.height());
//	}

//	bool bFoundImage = false;
//	if (!cover.isEmpty() || QFile::exists(cover))
//	{
//		QImage image;
//		if (image.load(cover))
//		{
//			painter->drawImage(coverRect, image);
//			bFoundImage = true;
//		}
//	}

//	if (!bFoundImage)
//	{
//		painter->drawImage(coverRect, m_Image);
//	}

//	QFont font = QApplication::font();

//	painter->setFont(font);
//	painter->drawText(titleRect, title);
////	painter->drawText(favoriteRect, favorite);
//	painter->drawText(timeRect, time);
//	painter->drawText(artistRect, artist);
//	painter->drawText(albumRect, album);
//	painter->drawText(genreRect, genre);

//	painter->restore();
}

QSize ListTracksDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option);
	Q_UNUSED(index);

	return QSize(LIST_ITEM_WIDTH, LIST_TRACKS_HEIGHT);
}

QWidget *ListTracksDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	ListTracksEditor *editor = new ListTracksEditor(parent);
	connect(editor, SIGNAL(SigClickCoverArt(int)), this, SLOT(SlotClickCoverArt(int)));
	connect(editor, SIGNAL(SigClickPlay(int)), this, SLOT(SlotClickPlay(int)));
	connect(editor, SIGNAL(SigClickTitle(int, QString)), this, SLOT(SlotClickTitle(int, QString)));
	connect(editor, SIGNAL(SigClickFavorite(int, int)), this, SLOT(SlotClickFavorite(int, int)));
	connect(editor, SIGNAL(SigClickTime(int)), this, SLOT(SlotClickTime(int)));
	connect(editor, SIGNAL(SigClickArtist(int)), this, SLOT(SlotClickArtist(int)));
	connect(editor, SIGNAL(SigClickAlbum(int)), this, SLOT(SlotClickAlbum(int)));
	connect(editor, SIGNAL(SigClickGenre(int)), this, SLOT(SlotClickGenre(int)));
	connect(editor, SIGNAL(SigMenuAction(int, int)), this, SLOT(SlotMenuAction(int, int)));

	editor->ClearMenu();
	editor->SetMenu(m_OptionMenuMap);

	return editor;
}

void ListTracksDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	ListTracksEditor *widget = static_cast<ListTracksEditor*>(editor);
	widget->blockSignals(true);
	widget->SetID(qvariant_cast<int>(index.data(LIST_TRACKS_ID)));
	widget->GetFormCoverArt()->SetCoverArt(qvariant_cast<QString>(index.data(LIST_TRACKS_COVER)));
	widget->GetFormCoverArt()->SetIndex(qvariant_cast<int>(index.data(LIST_TRACKS_INDEX)));
	widget->GetFormCoverArt()->SetSelect(qvariant_cast<bool>(index.data(LIST_TRACKS_SELECT)));
	widget->SetTitle(qvariant_cast<QString>(index.data(LIST_TRACKS_TITLE)));
	widget->SetTime(qvariant_cast<QString>(index.data(LIST_TRACKS_TIME)));
	widget->SetArtist(qvariant_cast<QString>(index.data(LIST_TRACKS_ARTIST)));
	widget->SetAlbum(qvariant_cast<QString>(index.data(LIST_TRACKS_ALBUM)));
	widget->SetGenre(qvariant_cast<QString>(index.data(LIST_TRACKS_GENRE)));
	widget->blockSignals(false);
}

void ListTracksDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	ListTracksEditor *widget = static_cast<ListTracksEditor*>(editor);
	model->setData(index, widget->GetID(), LIST_TRACKS_ID);
	model->setData(index, widget->GetFormCoverArt()->GetCoverArt(), LIST_TRACKS_COVER);
	model->setData(index, widget->GetFormCoverArt()->GetIndex(), LIST_TRACKS_INDEX);
	model->setData(index, widget->GetFormCoverArt()->GetSelect(), LIST_TRACKS_SELECT);
	model->setData(index, widget->GetTitle(), LIST_TRACKS_TITLE);
	model->setData(index, widget->GetTime(), LIST_TRACKS_TIME);
	model->setData(index, widget->GetArtist(), LIST_TRACKS_ARTIST);
	model->setData(index, widget->GetAlbum(), LIST_TRACKS_ALBUM);
	model->setData(index, widget->GetGenre(), LIST_TRACKS_GENRE);
}

void ListTracksDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)

	QRect rect = option.rect;
//	rect.setWidth(m_Resize);
	rect.setHeight(m_Resize * 1.25);

	ListTracksEditor *widget = static_cast<ListTracksEditor*>(editor);
	widget->setGeometry(rect);
}

QMap<int, QString> ListTracksDelegate::GetOptionMenuMap() const
{
	return m_OptionMenuMap;
}

void ListTracksDelegate::SetOptionMenuMap(const QMap<int, QString> &OptionMenuMap)
{
	m_OptionMenuMap = OptionMenuMap;
}

int ListTracksDelegate::GetResize() const
{
	return m_Resize;
}

void ListTracksDelegate::SetResize(int Resize)
{
	m_Resize = Resize;
}

