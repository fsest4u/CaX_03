#include <QPainter>
#include <QMouseEvent>

#include "iconcoverartdelegate.h"
#include "iconcoverarteditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

#include "widget/form/formsearchcoverart.h"


IconCoverArtDelegate::IconCoverArtDelegate()
{

}

//void IconCoverArtDelegate::SlotClickCoverArt(int index)
//{
//	emit SigSelectCoverArt(index);
//}

void IconCoverArtDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyledItemDelegate::paint(painter, option, index);
	painter->save();

	QString cover = qvariant_cast<QString>(index.data(ICON_COVER_ART_COVER));

	QRect rectOrig = option.rect;
	QRect rectBase = QRect(rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height());
	QRect rectCover = QRect(rectBase.x(), rectBase.y(), rectBase.width() * 0.9, rectBase.width() * 0.9);

//	LogDebug("orig x [%d] y [%d] w[%d] h[%d] row [%d]", rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height(), index.row());
//	painter->drawRect(rectOrig);

	QPixmap pixCover;
	if (!cover.isEmpty() && pixCover.load(cover))
	{
		painter->drawPixmap(rectCover, pixCover);
	}

	painter->restore();

}

QSize IconCoverArtDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)

//	LogDebug("sizeHint ~");
	return QSize(option.rect.width(), option.rect.height());
}

QWidget *IconCoverArtDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

//	IconCoverArtEditor *editor = new IconCoverArtEditor(parent);
//	connect(editor, SIGNAL(SigClickCoverArt(int)), this, SLOT(SlotClickCoverArt(int)));

	return nullptr;
}

bool IconCoverArtDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	QRect rectOrig = option.rect;
	QRect rectBase = QRect(rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height());
	QRect rectCover = QRect(rectBase.x(), rectBase.y(), rectBase.width() * 0.9, rectBase.width() * 0.9);

	QPoint curPoint(((QMouseEvent*)event)->x(), ((QMouseEvent*)event)->y());
//	LogDebug("editorEvent ~ x [%d] y [%d] w [%d] h [%d] row [%d] ", rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height(), index.row());

	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (rectCover.contains(curPoint))
		{
			emit SigSelectCoverArt(index);
		}
	}

	return QStyledItemDelegate::editorEvent(event, model, option, index);
}

//void IconCoverArtDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
//{
//	IconCoverArtEditor *widget = static_cast<IconCoverArtEditor*>(editor);
//	widget->blockSignals(true);
//	widget->SetID(qvariant_cast<int>(index.data(ICON_COVER_ART_ID)));
////	widget->GetFormSearchCoverArt()->SetCoverArt(qvariant_cast<QByteArray>(index.data(ICON_COVER_ART_COVER)));
//	widget->GetFormSearchCoverArt()->SetCoverArt(qvariant_cast<QString>(index.data(ICON_COVER_ART_COVER)));
//	widget->blockSignals(false);

//}

//void IconCoverArtDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
//{
//	IconCoverArtEditor *widget = static_cast<IconCoverArtEditor*>(editor);
//	model->setData(index, widget->GetID(), ICON_COVER_ART_ID);
//	model->setData(index, widget->GetFormSearchCoverArt()->GetCoverArt(), ICON_COVER_ART_COVER);

//}

//void IconCoverArtDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//	Q_UNUSED(index)

//	QRect rect = option.rect;
////	rect.setWidth(m_Resize);
////	rect.setHeight(m_Resize * 1.25);

//	IconCoverArtEditor *widget = static_cast<IconCoverArtEditor*>(editor);
//	widget->setGeometry(rect);
//}
