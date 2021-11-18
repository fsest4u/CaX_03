#ifndef TRACKINFO_H
#define TRACKINFO_H

#include <QWidget>

#include "util/CJsonNode.h"

namespace Ui {
class TrackInfo;
}

class TrackInfo : public QWidget
{
	Q_OBJECT

public:
	explicit TrackInfo(QWidget *parent = nullptr);
	~TrackInfo();

	enum {
		TRACK_INFO_MODE_VIEW = 0,
		TRACK_INFO_MODE_EDIT,
		TRACK_INFO_MODE_MAX
	};

	CJsonNode GetInfoData();
	void SetInfoData(CJsonNode node);

	QStringList GetAlbumList() const;
	void SetAlbumList(const QStringList &list);

	QStringList GetAlbumArtistList() const;
	void SetAlbumArtistList(const QStringList &list);

	QStringList GetArtistList() const;
	void SetArtistList(const QStringList &list);

	QStringList GetComposerList() const;
	void SetComposerList(const QStringList &list);

	QStringList GetGenreList() const;
	void SetGenreList(const QStringList &list);

	QStringList GetMoodList() const;
	void SetMoodList(const QStringList &list);

	int GetIndex() const;
	void SetIndex(int Index);

	int GetMode() const;
	void SetMode(int Mode);

signals:

	void SigChangeAlbum(int index, QString value);
	void SigChangeAlbumArtist(int index, QString value);
	void SigChangeArtist(int index, QString value);
	void SigChangeComposer(int index, QString value);
	void SigChangeGenre(int index, QString value);
	void SigChangeMood(int index, QString value);

	void SigChangeTitle(int index, QString value);
	void SigChangeTempo(int index, QString value);
	void SigChangeYear(int index, QString value);
	void SigChangeTrack(int index, QString value);
	void SigChangePath(int index, QString value);
	void SigChangeBitrate(int index, QString value);
	void SigChangeDuration(int index, QString value);
	void SigChangeSampleRate(int index, QString value);
	void SigChangeChannel(int index, QString value);
	void SigChangeFormat(int index, QString value);
	void SigChangeCDYear(int index, QString value);
	void SigChangeCDNumber(int index, QString value);
	void SigChangeCDTotal(int index, QString value);

private slots:

	void SlotChangedAlbum(int index);
	void SlotChangedAlbumArtist(int index);
	void SlotChangedArtist(int index);
	void SlotChangedComposer(int index);
	void SlotChangedGenre(int index);
	void SlotChangedMood(int index);

	void SlotEditFinishTitle();
	void SlotEditFinishTempo();
	void SlotEditFinishYear();
	void SlotEditFinishTrack();
	void SlotEditFinishPath();
	void SlotEditFinishBitrate();
	void SlotEditFinishDuration();
	void SlotEditFinishSampleRate();
	void SlotEditFinishChannel();
	void SlotEditFinishFormat();
	void SlotEditFinishCDYear();
	void SlotEditFinishCDNumber();
	void SlotEditFinishCDTotal();

private:

	void ConnectSigToSlot();
	void Initialize();

	void SetAlbum(const QStringList &list, const QString value);
	void SetAlbumArtist(const QStringList &list, const QString value);
	void SetArtist(const QStringList &list, const QString value);
	void SetComposer(const QStringList &list, const QString value);
	void SetGenre(const QStringList &list, const QString value);
	void SetMood(const QStringList &list, const QString value);

	void SetTitle(const QString &value);
	void SetTempo(const QString &value);
	void SetYear(const QString &value);
	void SetTrack(const QString &value);
	void SetPath(const QString &value);
	void SetBitrate(const QString &value);
	void SetDuration(const QString &value);
	void SetSampleRate(const QString &value);
	void SetChannel(const QString &value);
	void SetFormat(const QString &value);
	void SetCDYear(const QString &value);
	void SetCDNumber(const QString &value);
	void SetCDTotal(const QString &value);

	CJsonNode m_InfoData;

	QStringList	m_AlbumList;
	QStringList	m_AlbumArtistList;
	QStringList	m_ArtistList;
	QStringList	m_ComposerList;
	QStringList	m_GenreList;
	QStringList	m_MoodList;

	int			m_Index;
	int			m_Mode;


	Ui::TrackInfo *ui;
};

#endif // TRACKINFO_H
