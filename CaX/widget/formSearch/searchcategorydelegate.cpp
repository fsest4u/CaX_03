#include "searchcategorydelegate.h"
#include "searchcategoryeditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

#include "widget/form/formcoverart.h"
#include "widget/form/formtitle.h"


SearchCategoryDelegate::SearchCategoryDelegate()
{

}

void SearchCategoryDelegate::SlotClickTitle(int id, QString coverArt)
{
	emit SigSelectTitle(id, coverArt);
}

void SearchCategoryDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(painter)
	Q_UNUSED(option)
	Q_UNUSED(index)
}

QSize SearchCategoryDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	return QSize(SEARCH_ITEM_WIDTH, SEARCH_ITEM_HEIGHT);
}

QWidget *SearchCategoryDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	SearchCategoryEditor *editor = new SearchCategoryEditor(parent);
	connect(editor, SIGNAL(SigClickTitle(int, QString)), this, SLOT(SlotClickTitle(int, QString)));

	return editor;
}

void SearchCategoryDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	SearchCategoryEditor *widget = static_cast<SearchCategoryEditor*>(editor);
	widget->blockSignals(true);
	widget->SetID(qvariant_cast<int>(index.data(SEARCH_CATEGORY_ID)));
	widget->GetFormCoverArt()->SetCoverArt(qvariant_cast<QString>(index.data(SEARCH_CATEGORY_COVER)));
	widget->GetFormTitle()->SetTitle(qvariant_cast<QString>(index.data(SEARCH_CATEGORY_TITLE)));
	widget->GetFormTitle()->SetSubtitle(qvariant_cast<QString>(index.data(SEARCH_CATEGORY_SUBTITLE)));
	widget->blockSignals(false);

}

void SearchCategoryDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	SearchCategoryEditor *widget = static_cast<SearchCategoryEditor*>(editor);
	model->setData(index, widget->GetID(), SEARCH_CATEGORY_ID);
	model->setData(index, widget->GetFormCoverArt()->GetCoverArt(), SEARCH_CATEGORY_COVER);
	model->setData(index, widget->GetFormTitle()->GetTitle(), SEARCH_CATEGORY_TITLE);
	model->setData(index, widget->GetFormTitle()->GetSubtitle(), SEARCH_CATEGORY_SUBTITLE);
}

void SearchCategoryDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
	editor->setGeometry(option.rect);
}
