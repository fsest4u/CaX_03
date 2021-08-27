#ifndef CATEGORYCONTSDELEGATE_H
#define CATEGORYCONTSDELEGATE_H

#include <QStyledItemDelegate>
#include <QListView>

#define CATEGORY_CONTS_ROLE_NONE 100

class CategoryContsDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	CategoryContsDelegate();

	enum categoryContsRole {
		CATEGORY_ROLE_ID = Qt::UserRole + CATEGORY_CONTS_ROLE_NONE,
		CATEGORY_ROLE_COVER,
		CATEGORY_ROLE_TITLE,
		CATEGORY_ROLE_SUBTITLE,
		CATEGORY_ROLE_COUNT,
		CATEGORY_ROLE_FAVORITE,
		CATEGORY_ROLE_RATING,
		CATEGORY_ROLE_INDEX,
		CATEGORY_ROLE_MAX
	};

	QListView::ViewMode GetViewMode() const;
	void SetViewMode(const QListView::ViewMode &ViewMode);

signals:

	void SigSelectCoverArt(int nID);
	void SigSelectCount(int nID);
	void SigSelectFavorite(int nID, int nFavorite);
	void SigSelectRating(int nID, int nRating);

private slots:

	void commitAndCloseEditor();
	void SlotClickCoverArt(int nID);
	void SlotClickTitle(int nID);
	void SlotClickSubtitle(int nID);
	void SlotClickCount(int nID);
	void SlotClickFavorite(int nID, int nFavorite);
	void SlotClickRating(int nID, int nRating);

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QListView::ViewMode m_ViewMode;

	QImage	m_Image;
};

#endif // CATEGORYCONTSDELEGATE_H
