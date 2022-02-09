#include <QPainter>
#include <QMouseEvent>

#include "searchtrackdelegate.h"
#include "searchtrackeditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

#include "widget/form/formcoverart.h"

SearchTrackDelegate::SearchTrackDelegate()
{

}

//QMap<int, QString> SearchTrackDelegate::GetOptionMenuMap() const
//{
//	return m_OptionMenuMap;
//}

//void SearchTrackDelegate::SetOptionMenuMap(const QMap<int, QString> &OptionMenuMap)
//{
//	m_OptionMenuMap = OptionMenuMap;
//}

//void SearchTrackDelegate::SlotClickPlay(int nID)
//{
//	emit SigSelectPlay(nID, PLAY_CLEAR);
//}

//void SearchTrackDelegate::SlotMenuAction(int nID, int menuID, QString coverArt)
//{
//	emit SigMenuAction(nID, menuID, coverArt);
//}

void SearchTrackDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyledItemDelegate::paint(painter, option, index);
	painter->save();

	QString cover = qvariant_cast<QString>(index.data(SEARCH_TRACKS_COVER));
	QString title = qvariant_cast<QString>(index.data(SEARCH_TRACKS_TITLE));
	QString duration = qvariant_cast<QString>(index.data(SEARCH_TRACKS_TIME));
	QString artist = qvariant_cast<QString>(index.data(SEARCH_TRACKS_ARTIST));
	QString album = qvariant_cast<QString>(index.data(SEARCH_TRACKS_ALBUM));
	QString genre = qvariant_cast<QString>(index.data(SEARCH_TRACKS_GENRE));

	QFont fontTitle("Segoe UI", 14, QFont::Normal, false);
	QFontMetrics fmTitle(fontTitle);

	QRect rectOrig = option.rect;
	QRect rectBase = QRect(rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height());
	int coverSize = rectBase.height() * 0.9;
	int gap = ( rectBase.width() - (coverSize + 20 + 200 + 100 + 200 + 200 + 100 + 30 + 10) ) / 7;
	QRect rectCover = QRect(rectBase.x(), rectBase.y() + (rectBase.height() - coverSize) / 2, coverSize, coverSize);
	QRect rectPlay = QRect(rectCover.x() + rectCover.width() + gap, rectBase.y() + (rectBase.height() - 16) / 2, 16, 16);
	QRect rectTitle = QRect(rectPlay.x() + rectPlay.width() + gap, rectBase.y() + (rectBase.height() - fmTitle.height()) / 2, 200, fmTitle.height());
	QRect rectDuration = QRect(rectTitle.x() + rectTitle.width() + gap, rectTitle.y(), 100, fmTitle.height());
	QRect rectArtist = QRect(rectDuration.x() + rectDuration.width() + gap, rectTitle.y(), 200, fmTitle.height());
	QRect rectAlbum = QRect(rectArtist.x() + rectArtist.width() + gap, rectTitle.y(), 200, fmTitle.height());
	QRect rectGenre = QRect(rectAlbum.x() + rectAlbum.width() + gap, rectTitle.y(), 100, fmTitle.height());
//	QRect rectMenu = QRect(rectGenre.x() + rectGenre.width() + gap, rectBase.y() + (rectBase.height() - 29) / 2, 29, 29);
	QRect rectMenu = QRect(rectBase.width() - 10 - 29, rectBase.y() + (rectBase.height() - 29) / 2, 29, 29);

//	LogDebug("orig x [%d] y [%d] w[%d] h[%d] row [%d]", rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height(), index.row());
//	painter->drawRect(rectOrig);
//	painter->drawRect(rectCover);
//	painter->drawRect(rectPlay);
//	painter->drawRect(rectTitle);
//	painter->drawRect(rectDuration);
//	painter->drawRect(rectArtist);
//	painter->drawRect(rectAlbum);
//	painter->drawRect(rectGenre);
//	painter->drawRect(rectMenu);

	QPixmap pixCover;
	if (cover.isEmpty())
	{
		cover = QString(":/resource/playlist-img160-albumart-h@2x.png");
	}
	if (pixCover.load(cover))
	{
		painter->drawPixmap(rectCover, pixCover);
	}

	QPixmap pixPlay;
	QString resPlay = QString(":/resource/browser-icon16-playnow@2x.png");
	if (pixPlay.load(resPlay))
	{
		painter->drawPixmap(rectPlay, pixPlay);
	}

	QPixmap pixMenu;
	QString resMenu = QString(":/resource/play-btn28-popupmenu-n@2x.png");
	if (pixMenu.load(resMenu))
	{
		painter->drawPixmap(rectMenu, pixMenu);
	}

	painter->setPen(QColor(84, 84, 84));
	if (!title.isEmpty())
	{
		painter->setFont(fontTitle);
		painter->drawText(rectTitle, title);
	}
	if (!duration.isEmpty())
	{
		painter->setFont(fontTitle);
		painter->drawText(rectDuration, duration);
	}
	if (!artist.isEmpty())
	{
		painter->setFont(fontTitle);
		painter->drawText(rectArtist, artist);
	}
	if (!album.isEmpty())
	{
		painter->setFont(fontTitle);
		painter->drawText(rectAlbum, album);
	}
	if (!genre.isEmpty())
	{
		painter->setFont(fontTitle);
		painter->drawText(rectGenre, genre);
	}

	painter->restore();

}

QSize SearchTrackDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option);
	Q_UNUSED(index);

	return QSize(LIST_ITEM_WIDTH, LIST_TRACKS_HEIGHT);
}

QWidget *SearchTrackDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

//	SearchTrackEditor *editor = new SearchTrackEditor(parent);
//	connect(editor, SIGNAL(SigClickPlay(int)), this, SLOT(SlotClickPlay(int)));
//	connect(editor, SIGNAL(SigMenuAction(int, int, QString)), this, SLOT(SlotMenuAction(int, int, QString)));

//	editor->ClearMenu();
//	editor->SetMenu(m_OptionMenuMap);

	return nullptr;
}

bool SearchTrackDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	int id = qvariant_cast<int>(index.data(SEARCH_TRACKS_ID));
	QString cover = qvariant_cast<QString>(index.data(SEARCH_TRACKS_COVER));

	QFont fontTitle("Segoe UI", 14, QFont::Normal, false);
	QFontMetrics fmTitle(fontTitle);

	QRect rectOrig = option.rect;
	QRect rectBase = QRect(rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height());
	int coverSize = rectBase.height() * 0.9;
	int gap = ( rectBase.width() - (coverSize + 20 + 200 + 200 + 200 + 200 + 200 + 30 + 10) ) / 7;
	QRect rectCover = QRect(rectBase.x(), rectBase.y() + (rectBase.height() - coverSize) / 2, coverSize, coverSize);
	QRect rectPlay = QRect(rectCover.x() + rectCover.width() + gap, rectBase.y() + (rectBase.height() - 16) / 2, 16, 16);
	QRect rectTitle = QRect(rectPlay.x() + rectPlay.width() + gap, rectBase.y() + (rectBase.height() - fmTitle.height()) / 2, 200, fmTitle.height());
	QRect rectDuration = QRect(rectTitle.x() + rectTitle.width() + gap, rectTitle.y(), 200, fmTitle.height());
	QRect rectArtist = QRect(rectDuration.x() + rectDuration.width() + gap, rectTitle.y(), 200, fmTitle.height());
	QRect rectAlbum = QRect(rectArtist.x() + rectArtist.width() + gap, rectTitle.y(), 200, fmTitle.height());
	QRect rectGenre = QRect(rectAlbum.x() + rectAlbum.width() + gap, rectTitle.y(), 200, fmTitle.height());
//	QRect rectMenu = QRect(rectGenre.x() + rectGenre.width() + gap, rectBase.y() + (rectBase.height() - 29) / 2, 29, 29);
	QRect rectMenu = QRect(rectBase.width() - 10 - 29, rectBase.y() + (rectBase.height() - 29) / 2, 29, 29);

	QPoint curPoint(((QMouseEvent*)event)->x(), ((QMouseEvent*)event)->y());
//	LogDebug("editorEvent ~ x [%d] y [%d] w [%d] h [%d] row [%d] ", rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height(), index.row());

	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (((QMouseEvent*)event)->button() == Qt::LeftButton)
		{
			if (rectPlay.contains(curPoint))
			{
				emit SigSelectPlay(id, PLAY_CLEAR);
			}
			else if (rectTitle.contains(curPoint))
			{
				emit SigSelectPlay(id, PLAY_CLEAR);
			}
			else if (rectDuration.contains(curPoint))
			{
				emit SigSelectPlay(id, PLAY_CLEAR);
			}
			else if (rectArtist.contains(curPoint))
			{
				emit SigSelectPlay(id, PLAY_CLEAR);
			}
			else if (rectAlbum.contains(curPoint))
			{
				emit SigSelectPlay(id, PLAY_CLEAR);
			}
			else if (rectGenre.contains(curPoint))
			{
				emit SigSelectPlay(id, PLAY_CLEAR);
			}
			else if (rectMenu.contains(curPoint))
			{
				emit SigSelectMenu(index, curPoint);
			}
		}
	}

	return QStyledItemDelegate::editorEvent(event, model, option, index);

}

//void SearchTrackDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
//{
//	SearchTrackEditor *widget = static_cast<SearchTrackEditor*>(editor);
//	widget->blockSignals(true);
//	widget->SetID(qvariant_cast<int>(index.data(SEARCH_TRACKS_ID)));
//	widget->GetFormCoverArt()->SetCoverArt(qvariant_cast<QString>(index.data(SEARCH_TRACKS_COVER)));
//	widget->SetTitle(qvariant_cast<QString>(index.data(SEARCH_TRACKS_TITLE)));
//	widget->SetTime(qvariant_cast<QString>(index.data(SEARCH_TRACKS_TIME)));
//	widget->SetArtist(qvariant_cast<QString>(index.data(SEARCH_TRACKS_ARTIST)));
//	widget->SetAlbumID(qvariant_cast<int>(index.data(SEARCH_TRACKS_ALBUM_ID)));
//	widget->SetAlbum(qvariant_cast<QString>(index.data(SEARCH_TRACKS_ALBUM)));
//	widget->SetGenre(qvariant_cast<QString>(index.data(SEARCH_TRACKS_GENRE)));
//	widget->blockSignals(false);
//}

//void SearchTrackDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
//{
//	SearchTrackEditor *widget = static_cast<SearchTrackEditor*>(editor);
//	model->setData(index, widget->GetID(), SEARCH_TRACKS_ID);
//	model->setData(index, widget->GetFormCoverArt()->GetCoverArt(), SEARCH_TRACKS_COVER);
//	model->setData(index, widget->GetTitle(), SEARCH_TRACKS_TITLE);
//	model->setData(index, widget->GetTime(), SEARCH_TRACKS_TIME);
//	model->setData(index, widget->GetArtist(), SEARCH_TRACKS_ARTIST);
//	model->setData(index, widget->GetAlbumID(), SEARCH_TRACKS_ALBUM_ID);
//	model->setData(index, widget->GetAlbum(), SEARCH_TRACKS_ALBUM);
//	model->setData(index, widget->GetGenre(), SEARCH_TRACKS_GENRE);

//}

//void SearchTrackDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//	Q_UNUSED(index)
//	editor->setGeometry(option.rect);
//}
