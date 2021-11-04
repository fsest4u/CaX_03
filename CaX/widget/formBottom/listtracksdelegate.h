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
//		LIST_TRACKS_SUBTITLE,
//		LIST_TRACKS_COUNT,
//		LIST_TRACKS_FAVORITE,
//		LIST_TRACKS_RATING,
		LIST_TRACKS_TIME,
		LIST_TRACKS_ARTIST,
		LIST_TRACKS_ALBUM,
		LIST_TRACKS_GENRE,
		LIST_TRACKS_SELECT,
		LIST_TRACKS_MAX
	};

	QListView::ViewMode GetViewMode() const;
	void SetViewMode(const QListView::ViewMode &ViewMode);

	QMap<int, QString> GetOptionMenuMap() const;
	void SetOptionMenuMap(const QMap<int, QString> &OptionMenuMap);

	int GetResize() const;
	void SetResize(int Resize);

signals:

	void SigSelectPlay(int nID, int playType);
	void SigSelectFavorite(int nID, int nFavorite);
	void SigSelectTitle(int nID, QString coverArt);
	void SigMenuAction(int nID, int menuID);

private slots:

	void SlotClickCoverArt(int nID);
	void SlotClickPlay(int nID);
	void SlotClickTitle(int nID, QString coverArt);
	void SlotClickFavorite(int nID, int nFavorite);
	void SlotClickTime(int nID);
	void SlotClickArtist(int nID);
	void SlotClickAlbum(int nID);
	void SlotClickGenre(int nID);
	void SlotMenuAction(int nID, int menuID);

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QListView::ViewMode m_ViewMode;	
	QMap<int, QString> m_OptionMenuMap;

	int					m_Resize;


};

#endif // LISTTRACKSDELEGATE_H
