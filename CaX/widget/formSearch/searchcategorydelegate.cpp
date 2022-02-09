#include <QPainter>
#include <QMouseEvent>

#include "searchcategorydelegate.h"
#include "searchcategoryeditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

#include "widget/form/formcoverart.h"
#include "widget/form/formtitle.h"


SearchCategoryDelegate::SearchCategoryDelegate()
{

}

//void SearchCategoryDelegate::SlotClickTitle(int id, QString coverArt)
//{
//	emit SigSelectTitle(id, coverArt);
//}

void SearchCategoryDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyledItemDelegate::paint(painter, option, index);
	painter->save();

	QString cover = qvariant_cast<QString>(index.data(SEARCH_CATEGORY_COVER));
	QString title = qvariant_cast<QString>(index.data(SEARCH_CATEGORY_TITLE));
	QString subtitle = qvariant_cast<QString>(index.data(SEARCH_CATEGORY_SUBTITLE));

	QFont fontTitle("Segoe UI", 14, QFont::Normal, false);
	QFontMetrics fmTitle(fontTitle);

	QRect rectOrig = option.rect;
	QRect rectBase = QRect(rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height());
	QRect rectCover = QRect(rectBase.x(), rectBase.y() + (rectBase.height() - 100) / 2, 100, 100);
	QRect rectTitle = QRect(rectCover.x() + rectCover.width() + 20,
							rectBase.y() + (rectBase.height() / 2 - fmTitle.height()) / 2 + 10,
							rectBase.width() - (rectCover.width() + 20),
							fmTitle.height());
	QRect rectSubtitle = QRect(rectTitle.x(),
							   rectBase.y() + rectBase.height() / 2 + (rectBase.height() / 2 - fmTitle.height()) / 2 - 10,
							   rectBase.width() - (rectCover.width() + 20),
							   fmTitle.height());

//	LogDebug("orig x [%d] y [%d] w[%d] h[%d] row [%d]", rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height(), index.row());
//	painter->drawRect(rectOrig);
//	painter->drawRect(rectCover);
//	painter->drawRect(rectTitle);
//	painter->drawRect(rectSubtitle);

	QPixmap pixCover;
	if (cover.isEmpty())
	{
		cover = QString(":/resource/playlist-img160-albumart-h@2x.png");
	}
	if (pixCover.load(cover))
	{
		painter->drawPixmap(rectCover, pixCover);
	}

	painter->setPen(QColor(84, 84, 84));
	if (!title.isEmpty())
	{
		painter->setFont(fontTitle);
		painter->drawText(rectTitle, title);
	}
	if (!subtitle.isEmpty())
	{
		painter->setFont(fontTitle);
		painter->drawText(rectSubtitle, subtitle);
	}

	painter->restore();

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

//	SearchCategoryEditor *editor = new SearchCategoryEditor(parent);
//	connect(editor, SIGNAL(SigClickTitle(int, QString)), this, SLOT(SlotClickTitle(int, QString)));

	return nullptr;
}

bool SearchCategoryDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	int id = qvariant_cast<int>(index.data(SEARCH_CATEGORY_ID));
	QString cover = qvariant_cast<QString>(index.data(SEARCH_CATEGORY_COVER));

	QFont fontTitle("Segoe UI", 14, QFont::Normal, false);
	QFontMetrics fmTitle(fontTitle);

	QRect rectOrig = option.rect;
	QRect rectBase = QRect(rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height());
	QRect rectCover = QRect(rectBase.x(), rectBase.y() + (rectBase.height() - 100) / 2, 100, 100);
	QRect rectTitle = QRect(rectCover.x() + rectCover.width() + 20,
							rectBase.y() + (rectBase.height() / 2 - fmTitle.height()) / 2 + 10,
							rectBase.width() - (rectCover.width() + 20),
							fmTitle.height());
	QRect rectSubtitle = QRect(rectTitle.x(),
							   rectBase.y() + rectBase.height() / 2 + (rectBase.height() / 2 - fmTitle.height()) / 2 - 10,
							   rectBase.width() - (rectCover.width() + 20),
							   fmTitle.height());

	QPoint curPoint(((QMouseEvent*)event)->x(), ((QMouseEvent*)event)->y());
//	LogDebug("editorEvent ~ x [%d] y [%d] w [%d] h [%d] row [%d] ", rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height(), index.row());

	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (((QMouseEvent*)event)->button() == Qt::LeftButton)
		{
			if (rectCover.contains(curPoint))
			{
			}
			else if (rectTitle.contains(curPoint))
			{
				emit SigSelectTitle(id, cover);
			}
			else if (rectSubtitle.contains(curPoint))
			{
				emit SigSelectTitle(id, cover);
			}
		}
	}

	return QStyledItemDelegate::editorEvent(event, model, option, index);
}

//void SearchCategoryDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
//{
//	SearchCategoryEditor *widget = static_cast<SearchCategoryEditor*>(editor);
//	widget->blockSignals(true);
//	widget->SetID(qvariant_cast<int>(index.data(SEARCH_CATEGORY_ID)));
//	widget->GetFormCoverArt()->SetCoverArt(qvariant_cast<QString>(index.data(SEARCH_CATEGORY_COVER)));
//	widget->GetFormTitle()->SetTitleFont(FONT_SIZE_SEARCH_TITLE, FONT_COLOR_NORMAL, FONT_WEIGHT_NORMAL);
//	widget->GetFormTitle()->SetSubtitleFont(FONT_SIZE_SEARCH_SUBTITLE, FONT_COLOR_NORMAL, FONT_WEIGHT_NORMAL);
//	widget->GetFormTitle()->SetTitle(qvariant_cast<QString>(index.data(SEARCH_CATEGORY_TITLE)));
//	widget->GetFormTitle()->SetSubtitle(qvariant_cast<QString>(index.data(SEARCH_CATEGORY_SUBTITLE)));
//	widget->blockSignals(false);

//}

//void SearchCategoryDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
//{
//	SearchCategoryEditor *widget = static_cast<SearchCategoryEditor*>(editor);
//	model->setData(index, widget->GetID(), SEARCH_CATEGORY_ID);
//	model->setData(index, widget->GetFormCoverArt()->GetCoverArt(), SEARCH_CATEGORY_COVER);
//	model->setData(index, widget->GetFormTitle()->GetTitle(), SEARCH_CATEGORY_TITLE);
//	model->setData(index, widget->GetFormTitle()->GetSubtitle(), SEARCH_CATEGORY_SUBTITLE);
//}

//void SearchCategoryDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//	Q_UNUSED(index)
//	editor->setGeometry(option.rect);
//}
