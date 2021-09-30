#include "listservicedelegate.h"
#include "listserviceeditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

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

void ListServiceDelegate::SlotClickIcon(QString rawData)
{
	emit SigSelectIcon(rawData);
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
	connect(editor, SIGNAL(SigClickIcon(QString)), this, SLOT(SlotClickIcon(QString)));
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
	widget->SetTitle(qvariant_cast<QString>(index.data(LIST_SERVICE_TITLE)));
	widget->SetBottom(qvariant_cast<QString>(index.data(LIST_SERVICE_BOT)));
	widget->SetDuration(qvariant_cast<QString>(index.data(LIST_SERVICE_DURATION)));
	widget->SetIcon(qvariant_cast<QString>(index.data(LIST_SERVICE_ICON)));
	widget->SetArt(qvariant_cast<QString>(index.data(LIST_SERVICE_ART)));
	widget->SetRawData(qvariant_cast<QString>(index.data(LIST_SERVICE_RAW)));
	widget->blockSignals(false);
}

void ListServiceDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	ListServiceEditor *widget = static_cast<ListServiceEditor*>(editor);
	model->setData(index, widget->GetID(), LIST_SERVICE_ID);
	model->setData(index, widget->GetType(), LIST_SERVICE_TYPE);
	model->setData(index, widget->GetTitle(), LIST_SERVICE_TITLE);
	model->setData(index, widget->GetBottom(), LIST_SERVICE_BOT);
	model->setData(index, widget->GetDuration(), LIST_SERVICE_DURATION);
	model->setData(index, widget->GetIcon(), LIST_SERVICE_ICON);
	model->setData(index, widget->GetArt(), LIST_SERVICE_ART);
	model->setData(index, widget->GetRawData(), LIST_SERVICE_RAW);
}

void ListServiceDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
	editor->setGeometry(option.rect);
}
