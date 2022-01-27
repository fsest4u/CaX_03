#include <QPainter>
#include <QMouseEvent>

#include "queuetrackdelegate.h"
#include "queuetrackeditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

QueueTrackDelegate::QueueTrackDelegate()
{

}

//void QueueTrackDelegate::SlotClickPlay(int index)
//{
//	emit SigSelectPlay(index, PLAY_CLEAR);
//}

//void QueueTrackDelegate::SlotClickTitle(int index)
//{
//	Q_UNUSED(index);
////	emit SigSelectTitle(index);
//}

//void QueueTrackDelegate::SlotClickTime(int index)
//{
//	Q_UNUSED(index);
////	emit SigSelectTime(index);
//}

//void QueueTrackDelegate::SlotClickArtist(int index)
//{
//	Q_UNUSED(index);
////	emit SigSelectArtist(index);
//}

void QueueTrackDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyledItemDelegate::paint(painter, option, index);
	painter->save();

	QString title = qvariant_cast<QString>(index.data(QUEUE_TRACKS_TITLE));
	QString duration = qvariant_cast<QString>(index.data(QUEUE_TRACKS_TIME));
	QString artist = qvariant_cast<QString>(index.data(QUEUE_TRACKS_ARTIST));

	QFont fontTitle("Segoe UI", 14, QFont::Normal, false);
	QFontMetrics fmTitle(fontTitle);

	QRect rectOrig = option.rect;
	QRect rectBase = QRect(rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height());
	int gap = ( rectBase.width() - (20 + 200 + 200 + 200) ) / 4;
	QRect rectPlay = QRect(rectBase.x(), rectBase.y() + (rectBase.height() - 16) / 2, 16, 16);
	QRect rectTitle = QRect(rectPlay.x() + rectPlay.width() + gap, rectBase.y() + (rectBase.height() - fmTitle.height()) / 2, 200, fmTitle.height());
	QRect rectDuration = QRect(rectTitle.x() + rectTitle.width() + gap, rectTitle.y(), 200, fmTitle.height());
	QRect rectArtist = QRect(rectBase.width() - 200 - 20, rectTitle.y(), 200, fmTitle.height());

	painter->setPen(QColor(255, 255, 255));

//	LogDebug("orig x [%d] y [%d] w[%d] h[%d] row [%d]", rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height(), index.row());
//	painter->drawRect(rectOrig);
//	painter->drawRect(rectPlay);
//	painter->drawRect(rectTitle);
//	painter->drawRect(rectDuration);
//	painter->drawRect(rectArtist);

	QPixmap pixPlay;
	QString resPlay = QString(":/resource/btm-btn40-play-n@2x.png");
	if (pixPlay.load(resPlay))
	{
		painter->drawPixmap(rectPlay, pixPlay);
	}

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

	painter->restore();
}

QSize QueueTrackDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index);
//	LogDebug("sizeHint ~ w[%d] h[%d] ", option.rect.width(), option.rect.height());

	return QSize(ICON_ITEM_WIDTH, LIST_HEIGHT_MIN);
}

QWidget *QueueTrackDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

//	QueueTrackEditor *editor = new QueueTrackEditor(parent);
//	connect(editor, SIGNAL(SigClickPlay(int)), this, SLOT(SlotClickPlay(int)));
//	connect(editor, SIGNAL(SigClickTitle(int)), this, SLOT(SlotClickTitle(int)));
//	connect(editor, SIGNAL(SigClickTime(int)), this, SLOT(SlotClickTime(int)));
//	connect(editor, SIGNAL(SigClickArtist(int)), this, SLOT(SlotClickArtist(int)));

	return nullptr;
}

bool QueueTrackDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	int nIndex = qvariant_cast<int>(index.data(QUEUE_TRACKS_INDEX));

	QFont fontTitle("Segoe UI", 14, QFont::Normal, false);
	QFontMetrics fmTitle(fontTitle);

	QRect rectOrig = option.rect;
	QRect rectBase = QRect(rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height());
	int gap = ( rectBase.width() - (20 + 200 + 200 + 200) ) / 4;
	QRect rectPlay = QRect(rectBase.x(), rectBase.y() + (rectBase.height() - 16) / 2, 16, 16);
	QRect rectTitle = QRect(rectPlay.x() + rectPlay.width() + gap, rectBase.y() + (rectBase.height() - fmTitle.height()) / 2, 200, fmTitle.height());
	QRect rectDuration = QRect(rectTitle.x() + rectTitle.width() + gap, rectTitle.y(), 200, fmTitle.height());
	QRect rectArtist = QRect(rectBase.width() - 200 - 20, rectTitle.y(), 200, fmTitle.height());

	QPoint curPoint(((QMouseEvent*)event)->x(), ((QMouseEvent*)event)->y());
//	LogDebug("editorEvent ~ x [%d] y [%d] w [%d] h [%d] row [%d] ", rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height(), index.row());

	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (((QMouseEvent*)event)->button() == Qt::LeftButton)
		{
			if (rectPlay.contains(curPoint))
			{
				emit SigSelectPlay(nIndex, PLAY_CLEAR);
			}
			else if (rectTitle.contains(curPoint))
			{
//				emit SigSelectTitle(nIndex, cover);
			}
			else if (rectDuration.contains(curPoint))
			{
//				emit SigSelectTitle(nIndex, cover);
			}
			else if (rectArtist.contains(curPoint))
			{
//				emit SigSelectTitle(nIndex, cover);
			}
		}
	}

	return QStyledItemDelegate::editorEvent(event, model, option, index);
}

//void QueueTrackDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
//{
//	QueueTrackEditor *widget = static_cast<QueueTrackEditor*>(editor);
//	widget->blockSignals(true);
//	widget->SetID(qvariant_cast<int>(index.data(QUEUE_TRACKS_ID)));
//	widget->SetTitle(qvariant_cast<QString>(index.data(QUEUE_TRACKS_TITLE)));
//	widget->SetTime(qvariant_cast<QString>(index.data(QUEUE_TRACKS_TIME)));
//	widget->SetArtist(qvariant_cast<QString>(index.data(QUEUE_TRACKS_ARTIST)));
//	widget->SetIndex(qvariant_cast<int>(index.data(QUEUE_TRACKS_INDEX)));
//	widget->blockSignals(false);
//}

//void QueueTrackDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
//{
//	QueueTrackEditor *widget = static_cast<QueueTrackEditor*>(editor);
//	model->setData(index, widget->GetID(), QUEUE_TRACKS_ID);
//	model->setData(index, widget->GetTitle(), QUEUE_TRACKS_TITLE);
//	model->setData(index, widget->GetTime(), QUEUE_TRACKS_TIME);
//	model->setData(index, widget->GetArtist(), QUEUE_TRACKS_ARTIST);
//	model->setData(index, widget->GetIndex(), QUEUE_TRACKS_INDEX);

//}

//void QueueTrackDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//	Q_UNUSED(index)
//	editor->setGeometry(option.rect);
//}
