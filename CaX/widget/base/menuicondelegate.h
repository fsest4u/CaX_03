#ifndef MENUICONDELEGATE_H
#define MENUICONDELEGATE_H

#include <QStyledItemDelegate>
#include <QListView>

class MenuIconDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	MenuIconDelegate();

	enum menuIconRole {
		MENU_ICON_ROLE_ID = Qt::UserRole + 0,
		MENU_ICON_ROLE_COVER,
		MENU_ICON_ROLE_TITLE,
		MENU_ICON_MAX
	};

	QListView::ViewMode GetViewMode() const;
	void SetViewMode(const QListView::ViewMode &ViewMode);

signals:

	void SigSelectCoverArt(int nID);

private slots:

	void SlotClickCoverArt(int nID);
	void SlotClickTitle(int nID);

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QListView::ViewMode m_ViewMode;

};

#endif // MENUICONDELEGATE_H
