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
		LIST_SERVICE_COVER_ART,
		LIST_SERVICE_TITLE,
		LIST_SERVICE_SUBTITLE,
		LIST_SERVICE_TIME,
		LIST_SERVICE_RAW,
		LIST_SERVICE_INDEX,
		LIST_SERVICE_SELECT,
		LIST_SERVICE_MAX
	};


signals:

	void SigSelectCoverArt(int index);
	void SigSelectTitle(QString rawData);

private slots:

	void SlotClickCoverArt(int index);
	void SlotClickTitle(QString rawData);

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;


};

#endif // LISTSERVICEDELEGATE_H
