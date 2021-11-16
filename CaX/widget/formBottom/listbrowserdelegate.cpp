#include "listbrowserdelegate.h"
#include "listbrowsereditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

#include "widget/form/formcoverart.h"

ListBrowserDelegate::ListBrowserDelegate()
{

}

void ListBrowserDelegate::SlotClickTitle(int nType, QString rawData)
{
	emit SigSelectTitle(nType, rawData);
}

void ListBrowserDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(painter)
	Q_UNUSED(option)
	Q_UNUSED(index)
}

QSize ListBrowserDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	return QSize(LIST_ITEM_WIDTH, LIST_BROWSER_HEIGHT);
}

QWidget *ListBrowserDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	ListBrowserEditor *editor = new ListBrowserEditor(parent);
//	connect(editor, SIGNAL(SigClickCoverArt(QString)), this, SLOT(SlotClickCoverArt(QString)));
	connect(editor, SIGNAL(SigClickTitle(int, QString)), this, SLOT(SlotClickTitle(int, QString)));

	return editor;
}

void ListBrowserDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	ListBrowserEditor *widget = static_cast<ListBrowserEditor*>(editor);
	widget->blockSignals(true);
	widget->SetID(qvariant_cast<QString>(index.data(LIST_BROWSER_ID)));
	widget->SetType(qvariant_cast<int>(index.data(LIST_BROWSER_TYPE)));
	widget->GetFormCoverArt()->SetCoverArt(qvariant_cast<QString>(index.data(LIST_BROWSER_COVER)));
	widget->GetFormCoverArt()->SetSelect(qvariant_cast<bool>(index.data(LIST_BROWSER_SELECT)));
	widget->SetTitle(qvariant_cast<QString>(index.data(LIST_BROWSER_TITLE)));
	widget->SetSubtitle(qvariant_cast<QString>(index.data(LIST_BROWSER_SUBTITLE)));
	widget->SetDuration(qvariant_cast<QString>(index.data(LIST_BROWSER_DURATION)));
	widget->SetFileSize(qvariant_cast<QString>(index.data(LIST_BROWSER_FILESIZE)));
	widget->SetRawData(qvariant_cast<QString>(index.data(LIST_BROWSER_RAW)));
	widget->blockSignals(false);
}

void ListBrowserDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	ListBrowserEditor *widget = static_cast<ListBrowserEditor*>(editor);
	model->setData(index, widget->GetID(), LIST_BROWSER_ID);
	model->setData(index, widget->GetType(), LIST_BROWSER_TYPE);
	model->setData(index, widget->GetFormCoverArt()->GetCoverArt(), LIST_BROWSER_COVER);
	model->setData(index, widget->GetFormCoverArt()->GetSelect(), LIST_BROWSER_SELECT);
	model->setData(index, widget->GetTitle(), LIST_BROWSER_TITLE);
	model->setData(index, widget->GetSubtitle(), LIST_BROWSER_SUBTITLE);
	model->setData(index, widget->GetDuration(), LIST_BROWSER_DURATION);
	model->setData(index, widget->GetFilesize(), LIST_BROWSER_FILESIZE);
	model->setData(index, widget->GetRawData(), LIST_BROWSER_RAW);
}

void ListBrowserDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
	editor->setGeometry(option.rect);
}
