#ifndef MENULISTDELEGATE_H
#define MENULISTDELEGATE_H

#include <QStyledItemDelegate>
#include <QListView>

class MenuListDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	MenuListDelegate();

	enum menuListRole {
		MENU_LIST_ROLE_ID = Qt::UserRole + 0,
		MENU_LIST_ROLE_TYPE,
		MENU_LIST_ROLE_TITLE,
		MENU_LIST_ROLE_ICON,
		MENU_LIST_ROLE_ART,
		MENU_LIST_ROLE_RAW,
		MENU_LIST_MAX
	};

	QListView::ViewMode GetViewMode() const;
	void SetViewMode(const QListView::ViewMode &ViewMode);

signals:

	void SigSelectIcon(QString rawData);
	void SigSelectTitle(QString rawData);

private slots:

	void SlotClickIcon(QString rawData);
	void SlotClickTitle(QString rawData);

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QListView::ViewMode m_ViewMode;

};

#endif // MENULISTDELEGATE_H
