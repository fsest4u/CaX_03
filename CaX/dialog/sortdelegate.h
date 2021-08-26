#ifndef SORTDELEGATE_H
#define SORTDELEGATE_H

#include <QStyledItemDelegate>

#define	SORT_ROLE_NONE	100
#define SORT_TYPE_NONE	200

class SortDelegate : public QStyledItemDelegate
{
public:
	SortDelegate();

	enum sortRole {
		SORT_ROLE_ID = Qt::UserRole + SORT_ROLE_NONE,
		SORT_ROLE_ICON,
		SORT_ROLE_TITLE,
		SORT_ROLE_MAX
	};

	enum sortType {
		SORT_BY_GENRE = Qt::UserRole + SORT_TYPE_NONE,
		SORT_BY_MOOD,
		SORT_BY_FOLDER,
		SORT_BY_YEAR,
		SORT_BY_RATING,
		SORT_BY_SAMPLE_RATE,
		SORT_BY_MAX
	};

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // SORTDELEGATE_H
