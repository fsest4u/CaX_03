#include <QApplication>
#include <QPainter>
#include <QFile>
#include <QLabel>

#include "listtracksdelegate.h"
#include "listtrackseditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

#include "network/tcpclient.h"


ListTracksDelegate::ListTracksDelegate()
{
	m_Image = QImage(":/resource/outline_arrow_forward_black_24dp.png");
}

void ListTracksDelegate::SlotClickCoverArt(int nID)
{
	Q_UNUSED(nID);
	LogDebug("click cover art");
}

void ListTracksDelegate::SlotClickPlay(int nID)
{
	emit SigSelectPlay(nID);
}

void ListTracksDelegate::SlotClickTitle(int nID)
{
	Q_UNUSED(nID);
	LogDebug("click title");
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

void ListTracksDelegate::SlotClickMore(int nID)
{
	emit SigSelectMore(nID);
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
//	int nIndex = qvariant_cast<int>(index.data(LIST_TRACKS_INDEX));

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

	return QSize(SONG_CONT_WIDTH, SONG_CONT_HEIGHT + 50);
}

QWidget *ListTracksDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	ListTracksEditor *editor = new ListTracksEditor(parent);
//	connect(editor, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));
	connect(editor, SIGNAL(SigClickCoverArt(int)), this, SLOT(SlotClickCoverArt(int)));
	connect(editor, SIGNAL(SigClickPlay(int)), this, SLOT(SlotClickPlay(int)));
	connect(editor, SIGNAL(SigClickTitle(int)), this, SLOT(SlotClickTitle(int)));
	connect(editor, SIGNAL(SigClickTime(int)), this, SLOT(SlotClickTime(int)));
	connect(editor, SIGNAL(SigClickArtist(int)), this, SLOT(SlotClickArtist(int)));
	connect(editor, SIGNAL(SigClickAlbum(int)), this, SLOT(SlotClickAlbum(int)));
	connect(editor, SIGNAL(SigClickGenre(int)), this, SLOT(SlotClickGenre(int)));
	connect(editor, SIGNAL(SigClickMore(int)), this, SLOT(SlotClickMore(int)));

	return editor;
}

void ListTracksDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	ListTracksEditor *widget = static_cast<ListTracksEditor*>(editor);
	widget->blockSignals(true);
	widget->SetID(qvariant_cast<int>(index.data(LIST_TRACKS_ID)));
	widget->SetCoverArt(qvariant_cast<QString>(index.data(LIST_TRACKS_COVER)));
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
	model->setData(index, widget->GetCoverArt(), LIST_TRACKS_COVER);
	model->setData(index, widget->GetTitle(), LIST_TRACKS_TITLE);
	model->setData(index, widget->GetTime(), LIST_TRACKS_TIME);
	model->setData(index, widget->GetArtist(), LIST_TRACKS_ARTIST);
	model->setData(index, widget->GetAlbum(), LIST_TRACKS_ALBUM);
	model->setData(index, widget->GetGenre(), LIST_TRACKS_GENRE);
}

void ListTracksDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
	editor->setGeometry(option.rect);
}

QListView::ViewMode ListTracksDelegate::GetViewMode() const
{
	return m_ViewMode;
}

void ListTracksDelegate::SetViewMode(const QListView::ViewMode &ViewMode)
{
	m_ViewMode = ViewMode;
}

