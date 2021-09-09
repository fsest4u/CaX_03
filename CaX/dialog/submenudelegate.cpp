#include <QApplication>
#include <QPainter>

#include "submenudelegate.h"

CategoryDelegate::CategoryDelegate()
{

}

void CategoryDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyledItemDelegate::paint(painter, option, index);

	painter->save();

	QFont font = QApplication::font();
	QFont subFont = QApplication::font();

	font.setBold(true);
	subFont.setWeight(subFont.weight() - 2);
	QFontMetrics fm(font);

//	QIcon icon = qvariant_cast<QIcon>(index.data(CAT_ROLE_ICON));
//	QSize iconSize = icon.actualSize(option.decorationSize);
//	QRect iconRect = option.rect;

//	iconRect.setRight(iconSize.width() + 30);
//	iconRect.setTop(iconRect.top() + 5);

//	painter->drawPixmap(QPoint(iconRect.left() + iconSize.width() / 2 + 2,
//							   iconRect.top() + iconSize.height() / 2 + 3),
//						icon.pixmap(iconSize.width(), iconSize.height()));

	QString icon = qvariant_cast<QString>(index.data(CAT_ROLE_ICON));
	QRect iconRect = option.rect;
	iconRect.setRight(20 + 30);
	iconRect.setTop(iconRect.top() + 5);

	QImage image;
	if (image.load(icon))
	{
		painter->drawImage(iconRect, image);
	}

	QString title = qvariant_cast<QString>(index.data(CAT_ROLE_TITLE));
	QRect titleRect = option.rect;

	titleRect.setLeft(iconRect.right());
	titleRect.setTop(titleRect.top() + 5);
	titleRect.setBottom(titleRect.top() + fm.height());

	painter->setFont(font);
	painter->drawText(titleRect, title);

	painter->restore();

}

QSize CategoryDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QIcon icon = qvariant_cast<QIcon>(index.data(CAT_ROLE_ICON));
	QSize iconSize = icon.actualSize(option.decorationSize);
	QFont font = QApplication::font();
	QFontMetrics fm(font);

	return QSize(iconSize.width(), iconSize.height() + fm.height() + 8);
}
