#ifndef ICONTRACKSDELEGATE_H
#define ICONTRACKSDELEGATE_H

#include <QStyledItemDelegate>
#include <QListView>


class IconTracksDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	IconTracksDelegate();

	enum {
		ICON_TRACKS_ID = Qt::UserRole + 0,
		ICON_TRACKS_COVER,
		ICON_TRACKS_TITLE,
		ICON_TRACKS_SUBTITLE,
		ICON_TRACKS_COUNT,
		ICON_TRACKS_FAVORITE,
		ICON_TRACKS_RATING,
		ICON_TRACKS_MAX
	};

	QListView::ViewMode GetViewMode() const;
	void SetViewMode(const QListView::ViewMode &ViewMode);

signals:

	void SigSelectPlay(int nID);
	void SigSelectFavorite(int nID, int nFavorite);
	void SigSelectRating(int nID, int nRating);
	void SigSelectTitle(int nID, QString coverArt);
	void SigSelectSubtitle(int nID, QString coverArt);

private slots:

	void SlotClickPlay(int nID);
	void SlotClickFavorite(int nID, int nFavorite);
	void SlotClickRating(int nID, int nRating);
	void SlotClickTitle(int nID, QString coverArt);
	void SlotClickSubtitle(int nID, QString coverArt);

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QListView::ViewMode m_ViewMode;

};

#endif // ICONTRACKSDELEGATE_H
