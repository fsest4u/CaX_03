#ifndef SEARCHTRACKDELEGATE_H
#define SEARCHTRACKDELEGATE_H

#include <QStyledItemDelegate>
#include <QListView>

class SearchTrackDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	SearchTrackDelegate();

	enum {
		SEARCH_TRACKS_ID = Qt::UserRole + 0,
		SEARCH_TRACKS_COVER,
		SEARCH_TRACKS_TITLE,
		SEARCH_TRACKS_TIME,
		SEARCH_TRACKS_ARTIST,
		SEARCH_TRACKS_ALBUM_ID,
		SEARCH_TRACKS_ALBUM,
		SEARCH_TRACKS_GENRE,
		SEARCH_TRACKS_MAX
	};

//	QMap<int, QString> GetOptionMenuMap() const;
//	void SetOptionMenuMap(const QMap<int, QString> &OptionMenuMap);

signals:

	void SigSelectPlay(int nID, int playType);
	void SigSelectMenu(const QModelIndex &modelIndex, QPoint point);
//	void SigMenuAction(int nID, int menuID, QString coverArt);

private slots:

//	void SlotClickPlay(int nID);
//	void SlotMenuAction(int nID, int menuID, QString coverArt);

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
//	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
//	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
//	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

//	QMap<int, QString> m_OptionMenuMap;

};

#endif // SEARCHTRACKDELEGATE_H
