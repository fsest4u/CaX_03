#include "listsetupdelegate.h"
#include "listsetupeditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

ListSetupDelegate::ListSetupDelegate()
{

}

void ListSetupDelegate::SlotClickTitle(QString strID)
{
	emit SigSelectTitle(strID);
}

void ListSetupDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(painter)
	Q_UNUSED(option)
	Q_UNUSED(index)
}

QSize ListSetupDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	return QSize(LIST_ITEM_WIDTH, LIST_BROWSER_HEIGHT);
}

QWidget *ListSetupDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	ListSetupEditor *editor = new ListSetupEditor(parent);
	connect(editor, SIGNAL(SigClickTitle(QString)), this, SLOT(SlotClickTitle(QString)));

	return editor;
}

void ListSetupDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	ListSetupEditor *widget = static_cast<ListSetupEditor*>(editor);
	widget->blockSignals(true);
	widget->SetStrID(qvariant_cast<QString>(index.data(LIST_SETUP_ID)));
	widget->SetTitle(qvariant_cast<QString>(index.data(LIST_SETUP_TITLE)));
	widget->blockSignals(false);
}

void ListSetupDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	ListSetupEditor *widget = static_cast<ListSetupEditor*>(editor);
	model->setData(index, widget->GetStrID(), LIST_SETUP_ID);
	model->setData(index, widget->GetTitle(), LIST_SETUP_TITLE);
}

void ListSetupDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
	editor->setGeometry(option.rect);
}
