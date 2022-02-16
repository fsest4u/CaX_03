#ifndef LISTSETUPDELEGATE_H
#define LISTSETUPDELEGATE_H

#include <QStyledItemDelegate>
#include <QListView>

#include "util/CJsonNode.h"

class ListSetupDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	ListSetupDelegate();

	enum {
		LIST_SETUP_ID = Qt::UserRole + 0,
		LIST_SETUP_TYPE,
		LIST_SETUP_TITLE,
		LIST_SETUP_SUBTITLE,
		LIST_SETUP_RAW,
		LIST_SETUP_INDEX,
		LIST_SETUP_MAX
	};


signals:

	void SigSelectMenu(const QModelIndex &modelIndex, QPoint point);


private slots:



private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
//	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
//	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
//	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;


};

#endif // LISTSETUPDELEGATE_H
