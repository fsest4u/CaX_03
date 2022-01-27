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
		LIST_TRACKS_SELECT,
		LIST_TRACKS_MAX
	};

//	QMap<int, QString> GetOptionMenuMap() const;
//	void SetOptionMenuMap(const QMap<int, QString> &OptionMenuMap);

	int GetResize() const;
	void SetResize(int Resize);


signals:

	void SigSelectCheck(const QModelIndex &index);
	void SigSelectPlay(int nID, int playType);
	void SigSelectTitle(int nID, QString coverArt);
	void SigSelectMenu(const QModelIndex &index, QPoint point);
//	void SigMenuAction(int nID, int menuID);

private slots:

//	void SlotClickCoverArt(int index);
//	void SlotClickPlay(int nID);
//	void SlotClickTitle(int nID, QString coverArt);
//	void SlotClickFavorite(int nID, int nFavorite);
////	void SlotClickTime(int nID);
////	void SlotClickArtist(int nID);
////	void SlotClickAlbum(int nID);
////	void SlotClickGenre(int nID);
//	void SlotMenuAction(int nID, int menuID);

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
//	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
//	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
//	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

//	QMap<int, QString> m_OptionMenuMap;

	int					m_Resize;

};

#endif // LISTTRACKSDELEGATE_H
