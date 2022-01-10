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
		LIST_TRACKS_FAVORITE,
		LIST_TRACKS_TIME,
		LIST_TRACKS_ARTIST,
		LIST_TRACKS_ALBUM,
		LIST_TRACKS_GENRE,
		LIST_TRACKS_ALBUM_ARTIST,
		LIST_TRACKS_COMPOSER,
		LIST_TRACKS_YEAR,
		LIST_TRACKS_MOOD,
		LIST_TRACKS_TEMPO,
		LIST_TRACKS_FORMAT,
		LIST_TRACKS_SAMPLE_RATE,
		LIST_TRACKS_BIT_DEPTH,
		LIST_TRACKS_RATING,
		LIST_TRACKS_INDEX,
		LIST_TRACKS_SELECT,
		LIST_TRACKS_MAX
	};

	QMap<int, QString> GetOptionMenuMap() const;
	void SetOptionMenuMap(const QMap<int, QString> &OptionMenuMap);

	int GetResize() const;
	void SetResize(int Resize);

	bool GetShowFavorite() const;
	void SetShowFavorite(bool ShowFavorite);

	bool GetShowTime() const;
	void SetShowTime(bool ShowTime);

	bool GetShowArtist() const;
	void SetShowArtist(bool ShowArtist);

	bool GetShowAlbum() const;
	void SetShowAlbum(bool ShowAlbum);

	bool GetShowGenre() const;
	void SetShowGenre(bool ShowGenre);

	bool GetShowAlbumArtist() const;
	void SetShowAlbumArtist(bool ShowAlbumArtist);

	bool GetShowComposer() const;
	void SetShowComposer(bool ShowComposer);

	bool GetShowYear() const;
	void SetShowYear(bool ShowYear);

	bool GetShowMood() const;
	void SetShowMood(bool ShowMood);

	bool GetShowTempo() const;
	void SetShowTempo(bool ShowTempo);

	bool GetShowFormat() const;
	void SetShowFormat(bool ShowFormat);

	bool GetShowSampleRate() const;
	void SetShowSampleRate(bool ShowSampleRate);

	bool GetShowBitDepth() const;
	void SetShowBitDepth(bool ShowBitDepth);

	bool GetShowRating() const;
	void SetShowRating(bool ShowRating);


signals:

	void SigSelectCoverArt(int index);
	void SigSelectPlay(int nID, int playType);
	void SigSelectFavorite(int nID, int nFavorite);
	void SigSelectTitle(int nID, QString coverArt);
	void SigMenuAction(int nID, int menuID);

private slots:

	void SlotClickCoverArt(int index);
	void SlotClickPlay(int nID);
	void SlotClickTitle(int nID, QString coverArt);
	void SlotClickFavorite(int nID, int nFavorite);
//	void SlotClickTime(int nID);
//	void SlotClickArtist(int nID);
//	void SlotClickAlbum(int nID);
//	void SlotClickGenre(int nID);
	void SlotMenuAction(int nID, int menuID);

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QMap<int, QString> m_OptionMenuMap;

	int					m_Resize;

	bool m_ShowFavorite;
	bool m_ShowTime;
	bool m_ShowArtist;
	bool m_ShowAlbum;
	bool m_ShowGenre;
	bool m_ShowAlbumArtist;
	bool m_ShowComposer;
	bool m_ShowYear;

	bool m_ShowMood;
	bool m_ShowTempo;
	bool m_ShowFormat;
	bool m_ShowSampleRate;
	bool m_ShowBitDepth;
	bool m_ShowRating;



};

#endif // LISTTRACKSDELEGATE_H
