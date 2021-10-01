#include "iconservicedelegate.h"
#include "iconserviceeditor.h"

#include "util/caxconstants.h"
#include "util/log.h"
#include "util/CJsonNode.h"

IconServiceDelegate::IconServiceDelegate()
{

}

void IconServiceDelegate::SlotClickCoverArt(int nType, QString rawData)
{
	LogDebug("click cover art");
	CJsonNode node(JSON_OBJECT);
	if (!node.SetContent(rawData))
	{
		return;
	}

//	if (node.GetString("ItemType").isEmpty())
//	{
//		emit SigSelectCoverArt(nType);

//	}
//	else
	{
		// for i-service
		emit SigSelectCoverArt(nType, rawData);
	}

}

void IconServiceDelegate::SlotClickTitle(int nType)
{
	Q_UNUSED(nType)
	LogDebug("click title");
}

void IconServiceDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(painter)
	Q_UNUSED(option)
	Q_UNUSED(index)
}

QSize IconServiceDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	return QSize(ICON_ITEM_WIDTH, ICON_ITEM_HEIGHT + 50);
}

QWidget *IconServiceDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	IconServiceEditor *editor = new IconServiceEditor(parent);
	connect(editor, SIGNAL(SigClickCoverArt(int, QString)), this, SLOT(SlotClickCoverArt(int, QString)));
	connect(editor, SIGNAL(SigClickTitle(int)), this, SLOT(SlotClickTitle(int)));

	return editor;
}

void IconServiceDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	IconServiceEditor *widget = static_cast<IconServiceEditor*>(editor);
	widget->blockSignals(true);

	widget->SetID(qvariant_cast<int>(index.data(ICON_SERVICE_ID)));
	widget->SetType(qvariant_cast<int>(index.data(ICON_SERVICE_TYPE)));
	widget->SetCoverArt(qvariant_cast<QString>(index.data(ICON_SERVICE_COVER)));
	widget->SetTitle(qvariant_cast<QString>(index.data(ICON_SERVICE_TITLE)));
	widget->SetSubtitle(qvariant_cast<QString>(index.data(ICON_SERVICE_SUBTITLE)));
	widget->SetRawData(qvariant_cast<QString>(index.data(ICON_SERVICE_RAW)));

	widget->blockSignals(false);
}

void IconServiceDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	IconServiceEditor *widget = static_cast<IconServiceEditor*>(editor);
	model->setData(index, widget->GetID(), ICON_SERVICE_ID);
	model->setData(index, widget->GetType(), ICON_SERVICE_TYPE);
	model->setData(index, widget->GetCoverArt(), ICON_SERVICE_COVER);
	model->setData(index, widget->GetTitle(), ICON_SERVICE_TITLE);
	model->setData(index, widget->GetSubtitle(), ICON_SERVICE_SUBTITLE);
	model->setData(index, widget->GetRawData(), ICON_SERVICE_RAW);
}

void IconServiceDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
	editor->setGeometry(option.rect);
}

QListView::ViewMode IconServiceDelegate::GetViewMode() const
{
	return m_ViewMode;
}

void IconServiceDelegate::SetViewMode(const QListView::ViewMode &ViewMode)
{
	m_ViewMode = ViewMode;
}
