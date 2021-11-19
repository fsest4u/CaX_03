#ifndef TRACKINFODIALOG_H
#define TRACKINFODIALOG_H

#include <QDialog>

#include "util/CJsonNode.h"

class TrackInfo;

namespace Ui {
class TrackInfoDialog;
}

class TrackInfoDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TrackInfoDialog(QWidget *parent = nullptr);
	~TrackInfoDialog();

	CJsonNode GetInfoData() const;
	void SetInfoData(const CJsonNode &InfoData);

	QString GetAddr() const;
	void SetAddr(const QString &Addr);

	int GetMode() const;
	void SetMode(int Mode);

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

private slots:

	void SlotChangeAlbum(int index, QString value);
	void SlotChangeAlbumArtist(int index, QString value);
	void SlotChangeArtist(int index, QString value);
	void SlotChangeComposer(int index, QString value);
	void SlotChangeGenre(int index, QString value);
	void SlotChangeMood(int index, QString value);

	void SlotChangeTitle(int index, QString value);
	void SlotChangeTempo(int index, QString value);
	void SlotChangeYear(int index, QString value);
	void SlotChangeTrack(int index, QString value);
	void SlotChangePath(int index, QString value);
	void SlotChangeBitrate(int index, QString value);
	void SlotChangeDuration(int index, QString value);
	void SlotChangeSampleRate(int index, QString value);
	void SlotChangeChannel(int index, QString value);
	void SlotChangeFormat(int index, QString value);
	void SlotChangeCDYear(int index, QString value);
	void SlotChangeCDNumber(int index, QString value);
	void SlotChangeCDTotal(int index, QString value);

private:

	void ConnectSigToSlot();

	TrackInfo *m_TrackInfo;

	CJsonNode	m_InfoData;

	QStringList	m_AlbumList;
	QStringList	m_AlbumArtistList;
	QStringList	m_ArtistList;
	QStringList	m_GenreList;
	QStringList	m_ComposerList;
	QStringList	m_MoodList;

	QString		m_Addr;

	Ui::TrackInfoDialog *ui;
};

#endif // TRACKINFODIALOG_H
