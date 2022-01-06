#include "iconcoverartdelegate.h"
#include "iconcoverarteditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

#include "widget/form/formsearchcoverart.h"


IconCoverArtDelegate::IconCoverArtDelegate()
{

}

void IconCoverArtDelegate::SlotClickCoverArt(int index)
{
	emit SigSelectCoverArt(index);
}

void IconCoverArtDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(painter)
	Q_UNUSED(option)
	Q_UNUSED(index)
}

QSize IconCoverArtDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	return QSize(ICON_ITEM_WIDTH - 35, ICON_ITEM_HEIGHT - 20);
}

QWidget *IconCoverArtDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	IconCoverArtEditor *editor = new IconCoverArtEditor(parent);
	connect(editor, SIGNAL(SigClickCoverArt(int)), this, SLOT(SlotClickCoverArt(int)));

	return editor;
}

void IconCoverArtDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	IconCoverArtEditor *widget = static_cast<IconCoverArtEditor*>(editor);
	widget->blockSignals(true);
	widget->SetID(qvariant_cast<int>(index.data(ICON_COVER_ART_ID)));
	widget->GetFormSearchCoverArt()->SetCoverArt(qvariant_cast<QByteArray>(index.data(ICON_COVER_ART_COVER)));
//	widget->GetFormSearchCoverArt()->SetCoverArt(qvariant_cast<QString>(index.data(ICON_COVER_ART_COVER)));
	widget->blockSignals(false);

}

void IconCoverArtDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	IconCoverArtEditor *widget = static_cast<IconCoverArtEditor*>(editor);
	model->setData(index, widget->GetID(), ICON_COVER_ART_ID);
	model->setData(index, widget->GetFormSearchCoverArt()->GetCoverArt(), ICON_COVER_ART_COVER);

}

void IconCoverArtDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)

	QRect rect = option.rect;
//	rect.setWidth(m_Resize);
//	rect.setHeight(m_Resize * 1.25);

	IconCoverArtEditor *widget = static_cast<IconCoverArtEditor*>(editor);
	widget->setGeometry(rect);
}
