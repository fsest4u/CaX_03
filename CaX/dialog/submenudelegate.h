#ifndef SUBMENUDELEGATE_H
#define SUBMENUDELEGATE_H

#include <QStyledItemDelegate>

class SubmenuDelegate : public QStyledItemDelegate
{
public:
	SubmenuDelegate();

	enum {
		SUBMENU_ID = Qt::UserRole + 0,
		SUBMENU_ICON,
		SUBMENU_TITLE,
		SUBMENU_MAX
	};



private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // SUBMENUDELEGATE_H
