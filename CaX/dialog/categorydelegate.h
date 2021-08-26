#ifndef CATEGORYDELEGATE_H
#define CATEGORYDELEGATE_H

#include <QStyledItemDelegate>

#define CATEGORY_ROLE_NONE	100
#define CATEGORY_TYPE_NONE	200

class CategoryDelegate : public QStyledItemDelegate
{
public:
	CategoryDelegate();

	enum categoryRole {
		CAT_ROLE_ID = Qt::UserRole + CATEGORY_ROLE_NONE,
		CAT_ROLE_ICON,
		CAT_ROLE_TITLE,
		CAT_ROLE_MAX
	};

	enum categoryType {
		CAT_GENRE = Qt::UserRole + CATEGORY_TYPE_NONE,
		CAT_MOOD,
		CAT_FOLDER,
		CAT_YEAR,
		CAT_MAX
	};

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // CATEGORYDELEGATE_H
