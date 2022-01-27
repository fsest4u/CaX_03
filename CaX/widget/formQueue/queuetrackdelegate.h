#ifndef QUEUETRACKDELEGATE_H
#define QUEUETRACKDELEGATE_H

#include <QStyledItemDelegate>

class QueueTrackDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	QueueTrackDelegate();

	enum {
		QUEUE_TRACKS_ID = Qt::UserRole + 0,
		QUEUE_TRACKS_TITLE,
		QUEUE_TRACKS_TIME,
		QUEUE_TRACKS_ARTIST,
		QUEUE_TRACKS_INDEX,
		QUEUE_TRACKS_MAX
	};

signals:

	void SigSelectPlay(int index, int playType);
//	void SigSelectTitle(int index);
//	void SigSelectTime(int index);
//	void SigSelectArtist(int index);

private slots:

//	void SlotClickPlay(int index);
//	void SlotClickTitle(int index);
//	void SlotClickTime(int index);
//	void SlotClickArtist(int index);

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
//	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
//	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
//	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;


};

#endif // QUEUETRACKDELEGATE_H
