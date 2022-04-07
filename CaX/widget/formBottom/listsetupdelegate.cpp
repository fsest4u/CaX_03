#include <QPainter>

#include "listsetupdelegate.h"
#include "listsetupeditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

#include "widget/setup.h"

ListSetupDelegate::ListSetupDelegate()
{

}

void ListSetupDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyledItemDelegate::paint(painter, option, index);
	painter->save();

	int type = qvariant_cast<int>(index.data(LIST_SETUP_TYPE));
	QString title = qvariant_cast<QString>(index.data(LIST_SETUP_TITLE));
	QString subtitle = qvariant_cast<QString>(index.data(LIST_SETUP_SUBTITLE));
//	LogDebug("title [%s] subtitle [%s]", title.toUtf8().data(), subtitle.toUtf8().data());

	int indent = 0;
	if (iAppSetupType_Mask_Sub & type)
	{
		indent = 40;
	}

	QFont fontTitle("Segoe UI", 14, QFont::Normal, false);
	QFontMetrics fmTitle(fontTitle);

	QRect rectOrig = option.rect;
	QRect rectBase = QRect(rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height());
	QRect rectTitle = QRect(rectBase.x() + 60 + indent, rectBase.y() + (rectBase.height() - fmTitle.height()) / 2, fmTitle.width(title) + 10, fmTitle.height());
	QRect rectSubtitle = QRect(rectBase.width() - fmTitle.width(subtitle) - 60, rectBase.y() + (rectBase.height() - fmTitle.height()) / 2, fmTitle.width(subtitle) + 10, fmTitle.height());

//	LogDebug("orig x [%d] y [%d] w[%d] h[%d] row [%d]", rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height(), index.row());
//	painter->drawRect(rectOrig);
//	painter->drawRect(rectTitle);
//	painter->drawRect(rectSubtitle);

	if (iAppSetupType_Mask_Disable & type)
	{
		painter->setPen(QColor(230, 230, 230));
	}
	else
	{
		painter->setPen(QColor(84, 84, 84));
	}

	if (!title.isEmpty())
	{
		painter->setFont(fontTitle);
		painter->drawText(rectTitle, title);
	}
	if (!subtitle.isEmpty() && !(iAppSetupType_Mask_HideValue & type))
	{
		painter->setFont(fontTitle);
		painter->drawText(rectSubtitle, subtitle);
	}

	painter->restore();
}

QSize ListSetupDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	return QSize(LIST_ITEM_WIDTH, LIST_BROWSER_HEIGHT);
}

QWidget *ListSetupDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(parent)
	Q_UNUSED(option)
	Q_UNUSED(index)

//	ListSetupEditor *editor = new ListSetupEditor(parent);
//	connect(editor, SIGNAL(SigClickTitle(QString, int)), this, SLOT(SlotClickTitle(QString, int)));
//	connect(editor, SIGNAL(SigMenuAction(QString, QString)), this, SLOT(SlotMenuAction(QString, QString)));
//	connect(editor, SIGNAL(SigSubMenuAction(QString, QString)), this, SLOT(SlotSubMenuAction(QString, QString)));

	return nullptr;
}

bool ListSetupDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	int type = qvariant_cast<int>(index.data(LIST_SETUP_TYPE));

//	QFont fontTitle("Segoe UI", 14, QFont::Normal, false);
//	QFontMetrics fmTitle(fontTitle);

//	QRect rectOrig = option.rect;
//	QRect rectBase = QRect(rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height());
//	QRect rectTitle = QRect(rectBase.x() + 60, rectBase.y() + (rectBase.height() - fmTitle.height()) / 2, 200, fmTitle.height());

	QPoint curPoint(((QMouseEvent*)event)->x(), ((QMouseEvent*)event)->y());
//	LogDebug("editorEvent ~ x [%d] y [%d] w [%d] h [%d] row [%d] ", rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height(), index.row());

	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (((QMouseEvent*)event)->button() == Qt::LeftButton)
		{
			if (iAppSetupType_Mask_Disable & type)
			{
				// nothing
			}
			else
			{
				//if (rectTitle.contains(curPoint))
				{
					emit SigSelectMenu(index, curPoint);
					return true;
				}
			}

		}
	}

//	return QStyledItemDelegate::editorEvent(event, model, option, index);
	return true;
}

//void ListSetupDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
//{
//	ListSetupEditor *widget = static_cast<ListSetupEditor*>(editor);
//	widget->blockSignals(true);
//	widget->SetStrID(qvariant_cast<QString>(index.data(LIST_SETUP_ID)));
//	widget->SetTitle(qvariant_cast<QString>(index.data(LIST_SETUP_TITLE)));
//	widget->SetIndex(qvariant_cast<int>(index.data(LIST_SETUP_INDEX)));
//	widget->blockSignals(false);

//	widget->ClearMenu();
//	widget->SetMenu(m_MenuMap);
//}

//void ListSetupDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
//{
//	ListSetupEditor *widget = static_cast<ListSetupEditor*>(editor);
//	model->setData(index, widget->GetStrID(), LIST_SETUP_ID);
//	model->setData(index, widget->GetTitle(), LIST_SETUP_TITLE);
//	model->setData(index, widget->GetIndex(), LIST_SETUP_INDEX);
//}

//void ListSetupDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//	Q_UNUSED(index)
//	editor->setGeometry(option.rect);
//}
