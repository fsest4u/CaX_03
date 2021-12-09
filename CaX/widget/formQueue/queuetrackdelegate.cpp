#include "queuetrackdelegate.h"
#include "queuetrackeditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

QueueTrackDelegate::QueueTrackDelegate()
{

}

void QueueTrackDelegate::SlotClickPlay(int index)
{
	emit SigSelectPlay(index, PLAY_CLEAR);
}

void QueueTrackDelegate::SlotClickTitle(int index)
{
	Q_UNUSED(index);
//	emit SigSelectTitle(index);
}

void QueueTrackDelegate::SlotClickTime(int index)
{
	Q_UNUSED(index);
//	emit SigSelectTime(index);
}

void QueueTrackDelegate::SlotClickArtist(int index)
{
	Q_UNUSED(index);
//	emit SigSelectArtist(index);
}

void QueueTrackDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(painter);
	Q_UNUSED(option);
	Q_UNUSED(index);
}

QSize QueueTrackDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option);
	Q_UNUSED(index);

	return QSize(LIST_ITEM_WIDTH, LIST_BROWSER_HEIGHT);
}

QWidget *QueueTrackDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	QueueTrackEditor *editor = new QueueTrackEditor(parent);
	connect(editor, SIGNAL(SigClickPlay(int)), this, SLOT(SlotClickPlay(int)));
	connect(editor, SIGNAL(SigClickTitle(int)), this, SLOT(SlotClickTitle(int)));
	connect(editor, SIGNAL(SigClickTime(int)), this, SLOT(SlotClickTime(int)));
	connect(editor, SIGNAL(SigClickArtist(int)), this, SLOT(SlotClickArtist(int)));

	return editor;
}

void QueueTrackDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QueueTrackEditor *widget = static_cast<QueueTrackEditor*>(editor);
	widget->blockSignals(true);
	widget->SetID(qvariant_cast<int>(index.data(QUEUE_TRACKS_ID)));
	widget->SetTitle(qvariant_cast<QString>(index.data(QUEUE_TRACKS_TITLE)));
	widget->SetTime(qvariant_cast<QString>(index.data(QUEUE_TRACKS_TIME)));
	widget->SetArtist(qvariant_cast<QString>(index.data(QUEUE_TRACKS_ARTIST)));
	widget->SetIndex(qvariant_cast<int>(index.data(QUEUE_TRACKS_INDEX)));
	widget->blockSignals(false);
}

void QueueTrackDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QueueTrackEditor *widget = static_cast<QueueTrackEditor*>(editor);
	model->setData(index, widget->GetID(), QUEUE_TRACKS_ID);
	model->setData(index, widget->GetTitle(), QUEUE_TRACKS_TITLE);
	model->setData(index, widget->GetTime(), QUEUE_TRACKS_TIME);
	model->setData(index, widget->GetArtist(), QUEUE_TRACKS_ARTIST);
	model->setData(index, widget->GetIndex(), QUEUE_TRACKS_INDEX);

}

void QueueTrackDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
	editor->setGeometry(option.rect);
}
