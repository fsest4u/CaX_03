#ifndef SEARCHCATEGORYDELEGATE_H
#define SEARCHCATEGORYDELEGATE_H

#include <QStyledItemDelegate>

class SearchCategoryDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	SearchCategoryDelegate();

	enum {
		SEARCH_CATEGORY_ID = Qt::UserRole + 0,
		SEARCH_CATEGORY_COVER,
		SEARCH_CATEGORY_TITLE,
		SEARCH_CATEGORY_SUBTITLE,
		SEARCH_CATEGORY_MAX
	};

signals:

	void SigSelectTitle(int id, QString coverArt);

private slots:

//	void SlotClickTitle(int id, QString coverArt);

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
//	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
//	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
//	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // SEARCHCATEGORYDELEGATE_H
