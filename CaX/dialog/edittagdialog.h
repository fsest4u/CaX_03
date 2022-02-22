#ifndef EDITTAGDIALOG_H
#define EDITTAGDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

class EditTagDelegate;

namespace Ui {
class EditTagDialog;
}

class EditTagDialog : public QDialog
{
	Q_OBJECT

public:
	explicit EditTagDialog(QWidget *parent = nullptr);
	~EditTagDialog();

	enum {
		EDIT_TAG_ID = 0,
		EDIT_TAG_TITLE,
		EDIT_TAG_FAVORITE,
		EDIT_TAG_ARTIST,
		EDIT_TAG_ALBUM,
		EDIT_TAG_GENRE,
		EDIT_TAG_ALBUM_ARTIST,
		EDIT_TAG_COMPOSER,
		EDIT_TAG_YEAR,
		EDIT_TAG_MOOD,
		EDIT_TAG_CD_NUMBER,
//		EDIT_TAG_RATING,
		EDIT_TAG_MAX
	};

	void SetNodeList(QList<CJsonNode> list);

	QStandardItemModel *GetModel() const;
	QMap<QStringList, QString> GetUpdateCell() const;

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

protected:

	void resizeEvent(QResizeEvent *event);

private slots:

	void SlotSectionClicked(int logicalIndex);
	void SlotSectionResize(int logicalIndex, int oldWidth, int newWidth);
	void SlotDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
	void SlotClickFavorite(const QModelIndex &index);

private:

	void ReadSettings();
	void WriteSettings();

	void ConnectSigToSlot();
	void Initialize();

	QStringList GetTagList(int index);

	QStandardItemModel		*m_Model;
	EditTagDelegate			*m_Delegate;

	QMap<QStringList, QString> m_MapUpdateCell;

	QStringList	m_AlbumList;
	QStringList	m_AlbumArtistList;
	QStringList	m_ArtistList;
	QStringList	m_GenreList;
	QStringList	m_ComposerList;
	QStringList	m_MoodList;

	bool					m_EnableChange;

	int						m_ColWidthID;
	int						m_ColWidthTitle;
	int						m_ColWidthFavorite;
	int						m_ColWidthArtist;
	int						m_ColWidthAlbum;
	int						m_ColWidthGenre;
	int						m_ColWidthAlbumArtist;
	int						m_ColWidthComposer;
	int						m_ColWidthYear;
	int						m_ColWidthMood;
	int						m_ColWidthCdNumber;

	Ui::EditTagDialog *ui;
};

#endif // EDITTAGDIALOG_H
