#include "listservicedelegate.h"
#include "listserviceeditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

#include "widget/form/formcoverart.h"

ListServiceDelegate::ListServiceDelegate()
{

}

QListView::ViewMode ListServiceDelegate::GetViewMode() const
{
	return m_ViewMode;
}

void ListServiceDelegate::SetViewMode(const QListView::ViewMode &ViewMode)
{
	m_ViewMode = ViewMode;
}

void ListServiceDelegate::SlotClickCoverArt(QString rawData)
{
	emit SigSelectCoverArt(rawData);
}

void ListServiceDelegate::SlotClickTitle(QString rawData)
{
	emit SigSelectTitle(rawData);
}

void ListServiceDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(painter)
	Q_UNUSED(option)
	Q_UNUSED(index)
}

QSize ListServiceDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	return QSize(LIST_ITEM_WIDTH, LIST_ITEM_HEIGHT + 50);
}

QWidget *ListServiceDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	ListServiceEditor *editor = new ListServiceEditor(parent);
//	connect(editor, SIGNAL(SigClickCoverArt(QString)), this, SLOT(SlotClickCoverArt(QString)));
	connect(editor, SIGNAL(SigClickTitle(QString)), this, SLOT(SlotClickTitle(QString)));

	return editor;
}

void ListServiceDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
//	LogDebug("type : [%d]", qvariant_cast<int>(index.data(LIST_SERVICE_TYPE)));
//	LogDebug("icon : [%s]", qvariant_cast<QString>(index.data(LIST_SERVICE_ICON)).toUtf8().data());
//	LogDebug("title : [%s]", qvariant_cast<QString>(index.data(LIST_SERVICE_TITLE)).toUtf8().data());
//	LogDebug("url : [%s]", qvariant_cast<QString>(index.data(LIST_SERVICE_URL)).toUtf8().data());
	ListServiceEditor *widget = static_cast<ListServiceEditor*>(editor);
	widget->blockSignals(true);
	widget->SetID(qvariant_cast<QString>(index.data(LIST_SERVICE_ID)));
	widget->SetType(qvariant_cast<int>(index.data(LIST_SERVICE_TYPE)));
	widget->GetFormCoverArt()->SetCoverArt(qvariant_cast<QString>(index.data(LIST_SERVICE_COVER_ART)));
	widget->GetFormCoverArt()->SetSelect(qvariant_cast<bool>(index.data(LIST_SERVICE_SELECT)));
	widget->SetTitle(qvariant_cast<QString>(index.data(LIST_SERVICE_TITLE)));
	widget->SetSubtitle(qvariant_cast<QString>(index.data(LIST_SERVICE_SUBTITLE)));
	widget->SetDuration(qvariant_cast<QString>(index.data(LIST_SERVICE_TIME)));
	widget->SetRawData(qvariant_cast<QString>(index.data(LIST_SERVICE_RAW)));
	widget->blockSignals(false);
}

void ListServiceDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	ListServiceEditor *widget = static_cast<ListServiceEditor*>(editor);
	model->setData(index, widget->GetID(), LIST_SERVICE_ID);
	model->setData(index, widget->GetType(), LIST_SERVICE_TYPE);
	model->setData(index, widget->GetFormCoverArt()->GetCoverArt(), LIST_SERVICE_COVER_ART);
	model->setData(index, widget->GetFormCoverArt()->GetSelect(), LIST_SERVICE_SELECT);
	model->setData(index, widget->GetTitle(), LIST_SERVICE_TITLE);
	model->setData(index, widget->GetSubtitle(), LIST_SERVICE_SUBTITLE);
	model->setData(index, widget->GetDuration(), LIST_SERVICE_TIME);
	model->setData(index, widget->GetRawData(), LIST_SERVICE_RAW);
}

void ListServiceDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
	editor->setGeometry(option.rect);
}
