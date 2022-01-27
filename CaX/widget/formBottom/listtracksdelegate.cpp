#include <QApplication>
#include <QPainter>
#include <QMouseEvent>

#include "listtracksdelegate.h"
#include "listtrackseditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

#include "widget/form/formcoverart.h"

ListTracksDelegate::ListTracksDelegate()
{

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

//void ListTracksDelegate::SlotClickCoverArt(int index)
//{
//	emit SigSelectCoverArt(index);
//}

//void ListTracksDelegate::SlotClickPlay(int nID)
//{
//	emit SigSelectPlay(nID, PLAY_CLEAR);
//}

//void ListTracksDelegate::SlotClickTitle(int nID, QString coverArt)
//{
//	emit SigSelectTitle(nID, coverArt);
//}

//void ListTracksDelegate::SlotClickFavorite(int nID, int nFavorite)
//{
//	emit SigSelectFavorite(nID, nFavorite);
//}

//void ListTracksDelegate::SlotClickTime(int nID)
//{
//	Q_UNUSED(nID);
//	LogDebug("click time");
//}

//void ListTracksDelegate::SlotClickArtist(int nID)
//{
//	Q_UNUSED(nID);
//	LogDebug("click artist");
//}

//void ListTracksDelegate::SlotClickAlbum(int nID)
//{
//	Q_UNUSED(nID);
//	LogDebug("click album");
//}

//void ListTracksDelegate::SlotClickGenre(int nID)
//{
//	Q_UNUSED(nID);
//	LogDebug("click genre");
//}

//void ListTracksDelegate::SlotMenuAction(int nID, int menuID)
//{
//	emit SigMenuAction(nID, menuID);
//}

void ListTracksDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyledItemDelegate::paint(painter, option, index);
	painter->save();

	QString cover = qvariant_cast<QString>(index.data(LIST_TRACKS_COVER));
	QString title = qvariant_cast<QString>(index.data(LIST_TRACKS_TITLE));
	QString duration = qvariant_cast<QString>(index.data(LIST_TRACKS_TIME));
	QString artist = qvariant_cast<QString>(index.data(LIST_TRACKS_ARTIST));
	QString album = qvariant_cast<QString>(index.data(LIST_TRACKS_ALBUM));
	QString genre = qvariant_cast<QString>(index.data(LIST_TRACKS_GENRE));
	bool select = qvariant_cast<bool>(index.data(LIST_TRACKS_SELECT));

	QFont fontTitle("Segoe UI", 14, QFont::Normal, false);
	QFontMetrics fmTitle(fontTitle);

	QRect rectOrig = option.rect;
	QRect rectBase = QRect(rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height());
	int gap = ( rectBase.width() - (60 + 40 + 20 + 200 + 200 + 200 + 200 + 200 + 30 + 60) ) / 6;
	QRect rectCover = QRect(rectBase.x() + 60, rectBase.y() + (rectBase.height() - 40) / 2, 40, 40);
	QRect rectCheck = QRect(rectCover.x() + rectCover.width() - 16 - 2, rectCover.y() + 2, 16, 16);
	QRect rectPlay = QRect(rectCover.x() + rectCover.width() + gap, rectBase.y() + (rectBase.height() - 16) / 2, 16, 16);
	QRect rectTitle = QRect(rectPlay.x() + rectPlay.width() + gap, rectBase.y() + (rectBase.height() - fmTitle.height()) / 2, 200, fmTitle.height());
	QRect rectDuration = QRect(rectTitle.x() + rectTitle.width() + gap, rectTitle.y(), 200, fmTitle.height());
	QRect rectArtist = QRect(rectDuration.x() + rectDuration.width() + gap, rectTitle.y(), 200, fmTitle.height());
	QRect rectAlbum = QRect(rectArtist.x() + rectArtist.width() + gap, rectTitle.y(), 200, fmTitle.height());
	QRect rectGenre = QRect(rectAlbum.x() + rectAlbum.width() + gap, rectTitle.y(), 200, fmTitle.height());
//	QRect rectMenu = QRect(rectGenre.x() + rectGenre.width() + gap, rectBase.y() + (rectBase.height() - 29) / 2, 29, 29);
	QRect rectMenu = QRect(rectBase.width() - 60 - 29, rectBase.y() + (rectBase.height() - 29) / 2, 29, 29);

//	LogDebug("orig x [%d] y [%d] w[%d] h[%d] row [%d]", rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height(), index.row());
//	painter->drawRect(rectOrig);
//	painter->drawRect(rectCover);
//	painter->drawRect(rectCheck);
//	painter->drawRect(rectPlay);
//	painter->drawRect(rectTitle);
//	painter->drawRect(rectDuration);
//	painter->drawRect(rectArtist);
//	painter->drawRect(rectAlbum);
//	painter->drawRect(rectGenre);
//	painter->drawRect(rectMenu);

	QPixmap pixCover;
	if (!cover.isEmpty() && pixCover.load(cover))
	{
		painter->drawPixmap(rectCover, pixCover);
	}

	QPixmap pixCheck;
	QString resCheck;
	if (select)
	{
		resCheck = QString(":/resource/playlist-btn30-selecton-h@2x.png");
	}
	else
	{
		resCheck = QString(":/resource/playlist-btn30-selecton-n@2x.png");
	}
	if (pixCheck.load(resCheck))
	{
		painter->drawPixmap(rectCheck, pixCheck);
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

QSize ListTracksDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option);
	Q_UNUSED(index);

	return QSize(ICON_ITEM_WIDTH, LIST_HEIGHT_MIN);
}

QWidget *ListTracksDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

//	ListTracksEditor *editor = new ListTracksEditor(parent);
//	connect(editor, SIGNAL(SigClickCoverArt(int)), this, SLOT(SlotClickCoverArt(int)));
//	connect(editor, SIGNAL(SigClickPlay(int)), this, SLOT(SlotClickPlay(int)));
//	connect(editor, SIGNAL(SigClickTitle(int, QString)), this, SLOT(SlotClickTitle(int, QString)));
//	connect(editor, SIGNAL(SigClickFavorite(int, int)), this, SLOT(SlotClickFavorite(int, int)));
////	connect(editor, SIGNAL(SigClickTime(int)), this, SLOT(SlotClickTime(int)));
////	connect(editor, SIGNAL(SigClickArtist(int)), this, SLOT(SlotClickArtist(int)));
////	connect(editor, SIGNAL(SigClickAlbum(int)), this, SLOT(SlotClickAlbum(int)));
////	connect(editor, SIGNAL(SigClickGenre(int)), this, SLOT(SlotClickGenre(int)));
//	connect(editor, SIGNAL(SigMenuAction(int, int)), this, SLOT(SlotMenuAction(int, int)));

//	editor->ClearMenu();
//	editor->SetMenu(m_OptionMenuMap);

	return nullptr;
}

bool ListTracksDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	int id = qvariant_cast<int>(index.data(LIST_TRACKS_ID));
	QString cover = qvariant_cast<QString>(index.data(LIST_TRACKS_COVER));


	QFont fontTitle("Segoe UI", 14, QFont::Normal, false);
	QFontMetrics fmTitle(fontTitle);

	QRect rectOrig = option.rect;
	QRect rectBase = QRect(rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height());
	int gap = ( rectBase.width() - (60 + 40 + 20 + 200 + 200 + 200 + 200 + 200 + 30 + 60) ) / 6;
	QRect rectCover = QRect(rectBase.x() + 60, rectBase.y() + (rectBase.height() - 40) / 2, 40, 40);
	QRect rectCheck = QRect(rectCover.x() + rectCover.width() - 16 - 2, rectCover.y() + 2, 16, 16);
	QRect rectPlay = QRect(rectCover.x() + rectCover.width() + gap, rectBase.y() + (rectBase.height() - 16) / 2, 16, 16);
	QRect rectTitle = QRect(rectPlay.x() + rectPlay.width() + gap, rectBase.y() + (rectBase.height() - fmTitle.height()) / 2, 200, fmTitle.height());
	QRect rectDuration = QRect(rectTitle.x() + rectTitle.width() + gap, rectTitle.y(), 200, fmTitle.height());
	QRect rectArtist = QRect(rectDuration.x() + rectDuration.width() + gap, rectTitle.y(), 200, fmTitle.height());
	QRect rectAlbum = QRect(rectArtist.x() + rectArtist.width() + gap, rectTitle.y(), 200, fmTitle.height());
	QRect rectGenre = QRect(rectAlbum.x() + rectAlbum.width() + gap, rectTitle.y(), 200, fmTitle.height());
//	QRect rectMenu = QRect(rectGenre.x() + rectGenre.width() + gap, rectBase.y() + (rectBase.height() - 29) / 2, 29, 29);
	QRect rectMenu = QRect(rectBase.width() - 60 - 29, rectBase.y() + (rectBase.height() - 29) / 2, 29, 29);

	QPoint curPoint(((QMouseEvent*)event)->x(), ((QMouseEvent*)event)->y());
//	LogDebug("editorEvent ~ x [%d] y [%d] w [%d] h [%d] row [%d] ", rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height(), index.row());

	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (((QMouseEvent*)event)->button() == Qt::LeftButton)
		{
			if (rectCheck.contains(curPoint))
			{
				emit SigSelectCheck(index);
			}
			else if (rectPlay.contains(curPoint))
			{
				emit SigSelectPlay(id, PLAY_CLEAR);
			}
			else if (rectTitle.contains(curPoint))
			{
				emit SigSelectTitle(id, cover);
			}
			else if (rectDuration.contains(curPoint))
			{
				emit SigSelectTitle(id, cover);
			}
			else if (rectArtist.contains(curPoint))
			{
				emit SigSelectTitle(id, cover);
			}
			else if (rectAlbum.contains(curPoint))
			{
				emit SigSelectTitle(id, cover);
			}
			else if (rectGenre.contains(curPoint))
			{
				emit SigSelectTitle(id, cover);
			}
			else if (rectMenu.contains(curPoint))
			{
			}
			else if (rectCover.contains(curPoint))
			{
				emit SigSelectCheck(index);
			}
		}
	}

	return QStyledItemDelegate::editorEvent(event, model, option, index);
}

//void ListTracksDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
//{
//	ListTracksEditor *widget = static_cast<ListTracksEditor*>(editor);
//	widget->blockSignals(true);
//	widget->SetID(qvariant_cast<int>(index.data(LIST_TRACKS_ID)));
//	widget->GetFormCoverArt()->SetCoverArt(qvariant_cast<QString>(index.data(LIST_TRACKS_COVER)));
//	widget->GetFormCoverArt()->SetIndex(qvariant_cast<int>(index.data(LIST_TRACKS_INDEX)));
//	widget->GetFormCoverArt()->SetSelect(qvariant_cast<bool>(index.data(LIST_TRACKS_SELECT)));
//	widget->SetTitle(qvariant_cast<QString>(index.data(LIST_TRACKS_TITLE)));
//	widget->SetFavorite(qvariant_cast<int>(index.data(LIST_TRACKS_FAVORITE)));
//	widget->SetTime(qvariant_cast<QString>(index.data(LIST_TRACKS_TIME)));
//	widget->SetArtist(qvariant_cast<QString>(index.data(LIST_TRACKS_ARTIST)));
//	widget->SetAlbum(qvariant_cast<QString>(index.data(LIST_TRACKS_ALBUM)));
//	widget->SetGenre(qvariant_cast<QString>(index.data(LIST_TRACKS_GENRE)));
//	widget->SetAlbumArtist(qvariant_cast<QString>(index.data(LIST_TRACKS_ALBUM_ARTIST)));
//	widget->SetComposer(qvariant_cast<QString>(index.data(LIST_TRACKS_COMPOSER)));
//	widget->SetYear(qvariant_cast<QString>(index.data(LIST_TRACKS_YEAR)));
//	widget->SetMood(qvariant_cast<QString>(index.data(LIST_TRACKS_MOOD)));
//	widget->SetTempo(qvariant_cast<QString>(index.data(LIST_TRACKS_TEMPO)));
//	widget->SetFormat(qvariant_cast<QString>(index.data(LIST_TRACKS_FORMAT)));
//	widget->SetSampleRate(qvariant_cast<QString>(index.data(LIST_TRACKS_SAMPLE_RATE)));
//	widget->SetBitDepth(qvariant_cast<QString>(index.data(LIST_TRACKS_BIT_DEPTH)));
//	widget->SetRating(qvariant_cast<int>(index.data(LIST_TRACKS_RATING)));

//	widget->ShowFavorite(m_ShowFavorite);
//	widget->ShowTime(m_ShowTime);
//	widget->ShowArtist(m_ShowArtist);
//	widget->ShowAlbum(m_ShowAlbum);
//	widget->ShowGenre(m_ShowGenre);
//	widget->ShowAlbumArtist(m_ShowAlbumArtist);
//	widget->ShowComposer(m_ShowComposer);
//	widget->ShowYear(m_ShowYear);

//	widget->ShowMood(m_ShowMood);
//	widget->ShowTempo(m_ShowTempo);
//	widget->ShowFormat(m_ShowFormat);
//	widget->ShowSampleRate(m_ShowSampleRate);
//	widget->ShowBitDepth(m_ShowBitDepth);
//	widget->ShowRating(m_ShowRating);

//	widget->blockSignals(false);
//}

//void ListTracksDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
//{
//	ListTracksEditor *widget = static_cast<ListTracksEditor*>(editor);
//	model->setData(index, widget->GetID(), LIST_TRACKS_ID);
//	model->setData(index, widget->GetFormCoverArt()->GetCoverArt(), LIST_TRACKS_COVER);
//	model->setData(index, widget->GetFormCoverArt()->GetIndex(), LIST_TRACKS_INDEX);
//	model->setData(index, widget->GetFormCoverArt()->GetSelect(), LIST_TRACKS_SELECT);
//	model->setData(index, widget->GetTitle(), LIST_TRACKS_TITLE);
//	model->setData(index, widget->GetFavorite(), LIST_TRACKS_FAVORITE);
//	model->setData(index, widget->GetTime(), LIST_TRACKS_TIME);
//	model->setData(index, widget->GetArtist(), LIST_TRACKS_ARTIST);
//	model->setData(index, widget->GetAlbum(), LIST_TRACKS_ALBUM);
//	model->setData(index, widget->GetGenre(), LIST_TRACKS_GENRE);
//	model->setData(index, widget->GetAlbumArtist(), LIST_TRACKS_ALBUM_ARTIST);
//	model->setData(index, widget->GetComposer(), LIST_TRACKS_COMPOSER);
//	model->setData(index, widget->GetYear(), LIST_TRACKS_YEAR);
//	model->setData(index, widget->GetMood(), LIST_TRACKS_MOOD);
//	model->setData(index, widget->GetTempo(), LIST_TRACKS_TEMPO);
//	model->setData(index, widget->GetFormat(), LIST_TRACKS_FORMAT);
//	model->setData(index, widget->GetSampleRate(), LIST_TRACKS_SAMPLE_RATE);
//	model->setData(index, widget->GetBitDepth(), LIST_TRACKS_BIT_DEPTH);
//	model->setData(index, widget->GetRating(), LIST_TRACKS_RATING);
//}

//void ListTracksDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//	Q_UNUSED(index)

//	QRect rect = option.rect;
////	rect.setWidth(m_Resize);
//	rect.setHeight(m_Resize * 1.2);

//	ListTracksEditor *widget = static_cast<ListTracksEditor*>(editor);
//	widget->setGeometry(rect);
//	widget->SetFrameSizeFormCoverArt(m_Resize);
////	LogDebug("resize [%d], height [%d]", m_Resize, rect.height());
//}
