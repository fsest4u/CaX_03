#ifndef ICONTRACKSDELEGATE_H
#define ICONTRACKSDELEGATE_H

#include <QStyledItemDelegate>

class IconTracksDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	IconTracksDelegate();

	enum {
		ICON_TRACKS_ID = Qt::UserRole + 0,
		ICON_TRACKS_COVER,
		ICON_TRACKS_COUNT,
		ICON_TRACKS_FAVORITE,
		ICON_TRACKS_RATING,
		ICON_TRACKS_TITLE,
		ICON_TRACKS_SUBTITLE,
//		ICON_TRACKS_TIME,
//		ICON_TRACKS_ARTIST,
//		ICON_TRACKS_ALBUM,
//		ICON_TRACKS_GENRE,
		ICON_TRACKS_INDEX,
		ICON_TRACKS_SELECT,
		ICON_TRACKS_MAX
	};

//	int GetResize() const;
//	void SetResize(int Resize);

	int GetTypeMode() const;
	void SetTypeMode(int TypeMode);

signals:

	void SigSelectCheck(const QModelIndex &modelIndex);
	void SigSelectPlay(const QModelIndex &modelIndex, int playType);
//	void SigSelectFavorite(int nID, int nFavorite);
//	void SigSelectRating(int nID, int nRating);
	void SigSelectTitle(const QModelIndex &modelIndex);
//	void SigSelectSubtitle(int nID, QString coverArt);

protected:



private slots:

//	void SlotClickCheck(int index);
//	void SlotClickPlay(int nID);
//	void SlotClickFavorite(int nID, int nFavorite);
//	void SlotClickRating(int nID, int nRating);
//	void SlotClickTitle(int nID, QString coverArt);
//	void SlotClickSubtitle(int nID, QString coverArt);

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
//	void destroyEditor(QWidget *editor, const QModelIndex &index) const override;
//	bool helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index);
//	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
//	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
//	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	int					m_TypeMode;

//	int					m_Resize;

};

#endif // ICONTRACKSDELEGATE_H
