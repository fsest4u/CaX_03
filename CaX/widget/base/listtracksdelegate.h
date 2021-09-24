#ifndef LISTTRACKSDELEGATE_H
#define LISTTRACKSDELEGATE_H

#include <QStyledItemDelegate>
#include <QListView>

class ListTracksDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	ListTracksDelegate();

	enum {
		LIST_TRACKS_ID = Qt::UserRole + 0,
		LIST_TRACKS_COVER,
		LIST_TRACKS_TITLE,
		LIST_TRACKS_TIME,
		LIST_TRACKS_ARTIST,
		LIST_TRACKS_ALBUM,
		LIST_TRACKS_GENRE,
		LIST_TRACKS_INDEX,
		LIST_TRACKS_MAX
	};

	QListView::ViewMode GetViewMode() const;
	void SetViewMode(const QListView::ViewMode &ViewMode);

signals:

	void SigSelectPlay(int nID);
	void SigSelectMore(int nID);

private slots:

	void SlotClickCoverArt(int nID);
	void SlotClickPlay(int nID);
	void SlotClickTitle(int nID);
	void SlotClickTime(int nID);
	void SlotClickArtist(int nID);
	void SlotClickAlbum(int nID);
	void SlotClickGenre(int nID);
	void SlotClickMore(int nID);

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

#endif // LISTTRACKSDELEGATE_H
