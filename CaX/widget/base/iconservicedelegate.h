#ifndef ICONSERVICEDELEGATE_H
#define ICONSERVICEDELEGATE_H

#include <QStyledItemDelegate>
#include <QListView>

class IconServiceDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	IconServiceDelegate();

	enum {
		ICON_SERVICE_ID = Qt::UserRole + 0,
		ICON_SERVICE_TYPE,
		ICON_SERVICE_COVER,
		ICON_SERVICE_TITLE,
		ICON_SERVICE_SUBTITLE,
		ICON_SERVICE_RAW,
		ICON_SERVICE_MAX
	};

	QListView::ViewMode GetViewMode() const;
	void SetViewMode(const QListView::ViewMode &ViewMode);

signals:

	void SigSelectCoverArt(int nType);
	void SigSelectCoverArt(int nType, QString rawData);

private slots:

	void SlotClickCoverArt(int nType, QString rawData);
	void SlotClickTitle(int nType);

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QListView::ViewMode m_ViewMode;

};

#endif // ICONSERVICEDELEGATE_H
