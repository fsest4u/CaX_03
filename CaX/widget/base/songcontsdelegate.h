#ifndef SONGCONTSDELEGATE_H
#define SONGCONTSDELEGATE_H

#include <QStyledItemDelegate>
#include <QListView>

#define SONG_CONTS_ROLE_NONE 100


class SongContsDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	SongContsDelegate();

	enum songContsRole {
		SONG_ROLE_ID = Qt::UserRole + SONG_CONTS_ROLE_NONE,
		SONG_ROLE_COVER,
		SONG_ROLE_TITLE,
		SONG_ROLE_FAVORITE,
		SONG_ROLE_TIME,
		SONG_ROLE_ARTIST,
		SONG_ROLE_ALBUM,
		SONG_ROLE_GENRE,
		SONG_ROLE_INDEX,
		SONG_ROLE_MAX
	};

	QListView::ViewMode GetViewMode() const;
	void SetViewMode(const QListView::ViewMode &ViewMode);

signals:

	void SigSelectPlay(int nID);
	void SigSelectFavorite(int nID);
	void SigSelectMore(int nID);

private slots:

	void SlotClickCoverArt(int nID);
	void SlotClickPlay(int nID);
	void SlotClickTitle(int nID);
	void SlotClickFavorite(int nID);
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

#endif // SONGCONTSDELEGATE_H
