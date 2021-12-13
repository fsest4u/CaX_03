#ifndef LISTSETUPDELEGATE_H
#define LISTSETUPDELEGATE_H

#include <QStyledItemDelegate>
#include <QListView>

class ListSetupDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	ListSetupDelegate();

	enum {
		LIST_SETUP_ID = Qt::UserRole + 0,
		LIST_SETUP_TITLE,
		LIST_SETUP_MAX
	};

signals:

	void SigSelectTitle(QString strID);

private slots:

	void SlotClickTitle(QString strID);

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // LISTSETUPDELEGATE_H
