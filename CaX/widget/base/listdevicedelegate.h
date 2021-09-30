#ifndef LISTDEVICEDELEGATE_H
#define LISTDEVICEDELEGATE_H

#include <QStyledItemDelegate>
#include <QListView>

class ListDeviceDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	ListDeviceDelegate();

	enum {
		LIST_DEVICE_MAC = Qt::UserRole + 0,
		LIST_DEVICE_ADDR,
		LIST_DEVICE_VAL,
		LIST_DEVICE_DEV,
		LIST_DEVICE_MAX
	};

	QListView::ViewMode GetViewMode() const;
	void SetViewMode(const QListView::ViewMode &ViewMode);

signals:

	void SigSelectDevice(QString mac);
	void SigSelectCancel(QString mac);

private slots:

	void SlotClickDevice(QString mac);
	void SlotClickCancel(QString mac);

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QListView::ViewMode m_ViewMode;

};

#endif // LISTDEVICEDELEGATE_H
