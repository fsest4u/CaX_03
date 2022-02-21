#include <QPainter>
#include <QMouseEvent>

#include "icontracksdelegate.h"
//#include "icontrackseditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

//#include "widget/form/formcoverart.h"
//#include "widget/form/formtitle.h"

IconTracksDelegate::IconTracksDelegate()
{
	m_TypeMode = TYPE_MODE_MAX;
}

int IconTracksDelegate::GetTypeMode() const
{
	return m_TypeMode;
}

void IconTracksDelegate::SetTypeMode(int TypeMode)
{
	m_TypeMode = TypeMode;
}

//int IconTracksDelegate::GetResize() const
//{
//	return m_Resize;
//}

//void IconTracksDelegate::SetResize(int Resize)
//{
//	m_Resize = Resize;
//}

//void IconTracksDelegate::SlotClickCheck(int index)
//{
//	emit SigSelectCoverArt(index);
//}

//void IconTracksDelegate::SlotClickPlay(int nID)
//{
//	emit SigSelectPlay(nID, PLAY_CLEAR);
//}

//void IconTracksDelegate::SlotClickFavorite(int nID, int nFavorite)
//{
//	emit SigSelectFavorite(nID, nFavorite);
//}

//void IconTracksDelegate::SlotClickRating(int nID, int nRating)
//{
//	emit SigSelectRating(nID, nRating);

//}

//void IconTracksDelegate::SlotClickTitle(int nID, QString coverArt)
//{
//	emit SigSelectTitle(nID, coverArt);
//}

//void IconTracksDelegate::SlotClickSubtitle(int nID, QString coverArt)
//{
//	emit SigSelectSubtitle(nID, coverArt);
//}

void IconTracksDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyledItemDelegate::paint(painter, option, index);
	painter->save();

	QString cover = qvariant_cast<QString>(index.data(ICON_TRACKS_COVER));
	QString count = qvariant_cast<QString>(index.data(ICON_TRACKS_COUNT));
	QString title = qvariant_cast<QString>(index.data(ICON_TRACKS_TITLE));
	QString subtitle = qvariant_cast<QString>(index.data(ICON_TRACKS_SUBTITLE));
	int rating = qvariant_cast<int>(index.data(ICON_TRACKS_RATING));
	int favorite = qvariant_cast<int>(index.data(ICON_TRACKS_FAVORITE));
//	int nIndex = qvariant_cast<int>(index.data(ICON_TRACKS_INDEX));
	bool select = qvariant_cast<bool>(index.data(ICON_TRACKS_SELECT));

	QFont fontCount("Segoe UI", 12, QFont::Normal, false);
	QFont fontTitle("Segoe UI", 14, QFont::Bold, false);
	QFont fontSubtitle("Segoe UI", 14, QFont::Normal, false);
	QFontMetrics fmCount(fontCount);
	QFontMetrics fmTitle(fontTitle);
	QFontMetrics fmSubtitle(fontSubtitle);

	QRect rectOrig = option.rect;
	QRect rectBase = QRect(rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height());
	QRect rectCover = QRect(rectBase.x(), rectBase.y(), rectBase.width() * 0.9, rectBase.width() * 0.9);
	QRect rectCheck = QRect(rectCover.x() + rectCover.width() - 30 - 2, rectCover.y() + 2, 30, 30);
	QRect rectPlay = QRect(rectCover.x() + rectCover.width() - 30 -2, rectCover.y() + rectCover.height() - 30 - 2, 30, 30);
	QRect rectCount = QRect(rectPlay.x() + (rectPlay.width() - fmCount.width(count)) / 2, rectPlay.y() + (rectPlay.height() - fmCount.height()) / 2, fmCount.width(count), fmCount.height());
	QRect rectRating = QRect(rectCover.x() + 2, rectCover.y() + rectCover.height() - 16 - 80 - 2 - 2, 16, 80);
	QRect rectFavorite = QRect(rectCover.x() + 2, rectCover.y() + rectCover.height() - 16 - 2, 16, 16);
	QRect rectTitle = QRect(rectBase.x(), rectBase.y() + rectCover.height(), rectCover.width(), fmTitle.height());
	QRect rectSubtitle = QRect(rectBase.x(), rectTitle.y() + fmTitle.height(), rectCover.width(), fmSubtitle.height());

//	LogDebug("orig x [%d] y [%d] w[%d] h[%d] row [%d]", rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height(), index.row());
//	LogDebug("base x [%d] y [%d] w[%d] h[%d]", rectBase.x(), rectBase.y(), rectBase.width(), rectBase.height());
//	painter->drawRect(rectOrig);
//	painter->drawRect(rectCheck);
//	painter->drawRect(rectPlay);
//	painter->drawRect(rectRating);
//	painter->drawRect(rectFavorite);

	QPixmap pixCover;
	if (cover.isEmpty())
	{
//		LogDebug("cover is empty [%d] - [%s] [%s]", nIndex, cover.toUtf8().data(), title.toUtf8().data());
		cover = QString(":/resource/playlist-img160-albumart-h@2x.png");
	}
	if (pixCover.load(cover))
	{
//		LogDebug("cover is load  [%d] - [%s] [%s]", nIndex, cover.toUtf8().data(), title.toUtf8().data());
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
	QString resPlay = QString(":/resource/mid-icon30-play-n@2x.png");
	if (pixPlay.load(resPlay) && count.toInt() > 0)
	{
		painter->drawPixmap(rectPlay, pixPlay);
	}

	QPixmap pixRating;
	QString resRating;
	switch (rating) {
	case 0:
		resRating = QString(":/resource/mid-icon16-rankon-h@2x.png");
		break;
	case 1:
		resRating = QString(":/resource/mid-icon16-rankon-h1@2x.png");
		break;
	case 2:
		resRating = QString(":/resource/mid-icon16-rankon-h2@2x.png");
		break;
	case 3:
		resRating = QString(":/resource/mid-icon16-rankon-h3@2x.png");
		break;
	case 4:
		resRating = QString(":/resource/mid-icon16-rankon-h4@2x.png");
		break;
	case 5:
		resRating = QString(":/resource/mid-icon16-rankon-h5@2x.png");
		break;
	}
	if (pixRating.load(resRating))
	{
		painter->drawPixmap(rectRating, pixRating);
	}

	QPixmap pixFavorite;
	QString resFavorite;
	if (favorite == 0)
	{
		resFavorite = QString(":/resource/mid-icon16-likeoff@2x.png");
	}
	else if (favorite == 1)
	{
		resFavorite = QString(":/resource/mid-icon16-likeon@2x.png");
	}
	if (pixFavorite.load(resFavorite))
	{
		painter->drawPixmap(rectFavorite, pixFavorite);
	}

	painter->setPen(QColor(52, 174, 214));
	if (!count.isEmpty() && count.toInt() > 0)
	{
		painter->setFont(fontCount);
		painter->drawText(rectCount, count);
	}

	painter->setPen(QColor(84, 84, 84));
	if (!title.isEmpty())
	{
		painter->setFont(fontTitle);
		painter->drawText(rectTitle, title);
	}
	if (!subtitle.isEmpty())
	{
		painter->setFont(fontSubtitle);
		painter->drawText(rectSubtitle, subtitle);
	}

	painter->restore();

}

QSize IconTracksDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)

//	LogDebug("sizeHint ~");
	return QSize(option.rect.width(), option.rect.height());
}

QWidget *IconTracksDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(parent)
	Q_UNUSED(option)
	Q_UNUSED(index)

//	LogDebug("createEditor ~");

//	IconTracksEditor *editor = new IconTracksEditor(parent);
//	connect(editor, SIGNAL(SigClickCoverArt(int)), this, SLOT(SlotClickCoverArt(int)));
//	connect(editor, SIGNAL(SigClickPlay(int)), this, SLOT(SlotClickPlay(int)));
//	connect(editor, SIGNAL(SigClickFavorite(int, int)), this, SLOT(SlotClickFavorite(int, int)));
//	connect(editor, SIGNAL(SigClickRating(int, int)), this, SLOT(SlotClickRating(int, int)));
//	connect(editor, SIGNAL(SigClickTitle(int, QString)), this, SLOT(SlotClickTitle(int, QString)));
//	connect(editor, SIGNAL(SigClickSubtitle(int, QString)), this, SLOT(SlotClickSubtitle(int, QString)));

	return nullptr;
}

bool IconTracksDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	int id = qvariant_cast<int>(index.data(ICON_TRACKS_ID));
	QString cover = qvariant_cast<QString>(index.data(ICON_TRACKS_COVER));

	QRect rectOrig = option.rect;
	QRect rectBase = QRect(rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height());
	QRect rectCover = QRect(rectBase.x(), rectBase.y(), rectBase.width() * 0.9, rectBase.width() * 0.9);
	QRect rectCheck = QRect(rectCover.x() + rectCover.width() - 30 - 2, rectCover.y() + 2, 30, 30);
	QRect rectPlay = QRect(rectCover.x() + rectCover.width() - 30 -2, rectCover.y() + rectCover.height() - 30 - 2, 30, 30);
	QRect rectRating = QRect(rectCover.x() + 2, rectCover.y() + rectCover.height() - 16 - 80 - 2 - 2, 16, 80);
	QRect rectFavorite = QRect(rectCover.x() + 2, rectCover.y() + rectCover.height() - 16 - 2, 16, 16);

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
			else if (rectRating.contains(curPoint))
			{
			}
			else if (rectFavorite.contains(curPoint))
			{
			}
			else if (rectCover.contains(curPoint))
			{
				emit SigSelectTitle(id, cover);
			}
		}
	}
	else if (event->type() == QMouseEvent::MouseMove)
	{
//		LogDebug("mouse move ~ x [%d] y [%d]", ((QMouseEvent*)event)->x(), ((QMouseEvent*)event)->y());
		if (rectPlay.contains(curPoint))
		{
//			LogDebug("rectPlay move ~ x [%d] y [%d]", ((QMouseEvent*)event)->x(), ((QMouseEvent*)event)->y());
//			m_HoverPlay = true;
		}
		else
		{
//			m_HoverPlay = false;
		}
	}

	return QStyledItemDelegate::editorEvent(event, model, option, index);
}

//void IconTracksDelegate::destroyEditor(QWidget *editor, const QModelIndex &index) const
//{
//	LogDebug("destroyEditor ~");
//	QStyledItemDelegate::destroyEditor(editor, index);
//}

//bool IconTracksDelegate::helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index)
//{
//	LogDebug("helpEvent ~");
//	return QStyledItemDelegate::helpEvent(event, view, option, index);
//}

//void IconTracksDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
//{
//	LogDebug("setEditorData ~");

//	IconTracksEditor *widget = static_cast<IconTracksEditor*>(editor);
//	widget->blockSignals(true);
//	widget->SetID(qvariant_cast<int>(index.data(ICON_TRACKS_ID)));
//	widget->GetFormCoverArt()->SetCoverArt(qvariant_cast<QString>(index.data(ICON_TRACKS_COVER)));
//	widget->GetFormCoverArt()->SetCount(qvariant_cast<int>(index.data(ICON_TRACKS_COUNT)));
//	if (m_TypeMode == TYPE_MODE_ITEM_TRACK || m_TypeMode == TYPE_MODE_TRACK)
//	{
//		widget->GetFormCoverArt()->SetFavorite(qvariant_cast<int>(index.data(ICON_TRACKS_FAVORITE)));
//		widget->GetFormCoverArt()->SetRating(qvariant_cast<int>(index.data(ICON_TRACKS_RATING)));
//	}
//	widget->GetFormCoverArt()->SetSelect(qvariant_cast<bool>(index.data(ICON_TRACKS_SELECT)));
//	widget->GetFormCoverArt()->SetIndex(qvariant_cast<int>(index.data(ICON_TRACKS_INDEX)));
//	widget->GetFormTitle()->SetTitleFont(FONT_SIZE_ICON_TITLE, FONT_COLOR_NORMAL, FONT_WEIGHT_BOLD);
//	widget->GetFormTitle()->SetSubtitleFont(FONT_SIZE_ICON_SUBTITLE, FONT_COLOR_NORMAL, FONT_WEIGHT_NORMAL);
//	widget->GetFormTitle()->SetTitle(qvariant_cast<QString>(index.data(ICON_TRACKS_TITLE)));
//	widget->GetFormTitle()->SetSubtitle(qvariant_cast<QString>(index.data(ICON_TRACKS_SUBTITLE)));
//	widget->blockSignals(false);
//}

//void IconTracksDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
//{
//	LogDebug("setModelData ~");

//	IconTracksEditor *widget = static_cast<IconTracksEditor*>(editor);
//	model->setData(index, widget->GetID(), ICON_TRACKS_ID);
//	model->setData(index, widget->GetFormCoverArt()->GetCoverArt(), ICON_TRACKS_COVER);
//	model->setData(index, widget->GetFormCoverArt()->GetCount(), ICON_TRACKS_COUNT);
//	if (m_TypeMode == TYPE_MODE_ITEM_TRACK || m_TypeMode == TYPE_MODE_TRACK)
//	{
//		model->setData(index, widget->GetFormCoverArt()->GetFavorite(), ICON_TRACKS_FAVORITE);
//		model->setData(index, widget->GetFormCoverArt()->GetRating(), ICON_TRACKS_RATING);
//	}
//	model->setData(index, widget->GetFormCoverArt()->GetSelect(), ICON_TRACKS_SELECT);
//	model->setData(index, widget->GetFormCoverArt()->GetIndex(), ICON_TRACKS_INDEX);
//	model->setData(index, widget->GetFormTitle()->GetTitle(), ICON_TRACKS_TITLE);
//	model->setData(index, widget->GetFormTitle()->GetSubtitle(), ICON_TRACKS_SUBTITLE);
//}

//void IconTracksDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//	Q_UNUSED(index)

//	LogDebug("updateEditorGeometry ~");

//	QRect rect = option.rect;
//	rect.setWidth(m_Resize);
//	rect.setHeight(m_Resize * 1.2);

//	IconTracksEditor *widget = static_cast<IconTracksEditor*>(editor);
//	widget->setGeometry(rect);
//}

