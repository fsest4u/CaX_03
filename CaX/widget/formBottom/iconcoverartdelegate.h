#ifndef ICONCOVERARTDELEGATE_H
#define ICONCOVERARTDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>

class IconCoverArtDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	IconCoverArtDelegate();

	enum {
		ICON_COVER_ART_ID = Qt::UserRole + 0,
		ICON_COVER_ART_COVER,
		ICON_COVER_ART_SELECT,
		ICON_COVER_ART_MAX
	};

signals:
	void SigSelectCoverArt(const QModelIndex &modelIndex);

private slots:

//	void SlotClickCoverArt(int index);

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
//	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
//	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
//	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // ICONCOVERARTDELEGATE_H
