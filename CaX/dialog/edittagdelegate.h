#ifndef EDITTAGDELEGATE_H
#define EDITTAGDELEGATE_H

#include <QItemDelegate>
#include <QObject>
#include <QPushButton>

class EditTagDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	EditTagDelegate();
	~EditTagDelegate();

	QStringList GetAlbumList() const;
	void SetAlbumList(const QStringList &AlbumList);

	QStringList GetAlbumArtistList() const;
	void SetAlbumArtistList(const QStringList &AlbumArtistList);

	QStringList GetArtistList() const;
	void SetArtistList(const QStringList &ArtistList);

	QStringList GetGenreList() const;
	void SetGenreList(const QStringList &GenreList);

	QStringList GetComposerList() const;
	void SetComposerList(const QStringList &ComposerList);

	QStringList GetMoodList() const;
	void SetMoodList(const QStringList &MoodList);

signals:

	void SigClickFavorite(const QModelIndex &index);

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

	QPushButton *m_BtnFavorite;

	QStringList	m_AlbumList;
	QStringList	m_AlbumArtistList;
	QStringList	m_ArtistList;
	QStringList	m_GenreList;
	QStringList	m_ComposerList;
	QStringList	m_MoodList;


};

#endif // EDITTAGDELEGATE_H
