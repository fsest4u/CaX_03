#ifndef LISTSERVICEDELEGATE_H
#define LISTSERVICEDELEGATE_H

#include <QStyledItemDelegate>
#include <QListView>

class ListServiceDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	ListServiceDelegate();

	enum {
		LIST_SERVICE_ID = Qt::UserRole + 0,
		LIST_SERVICE_TYPE,
		LIST_SERVICE_TITLE,
		LIST_SERVICE_ICON,
		LIST_SERVICE_ART,
		LIST_SERVICE_BOT,
		LIST_SERVICE_DURATION,
		LIST_SERVICE_RAW,
		LIST_SERVICE_MAX
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

#endif // LISTSERVICEDELEGATE_H
