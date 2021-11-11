#ifndef CDRIPTRACKINFO_H
#define CDRIPTRACKINFO_H

#include <QWidget>

#include "util/CJsonNode.h"

namespace Ui {
class CDRipTrackInfo;
}

class CDRipTrackInfo : public QWidget
{
	Q_OBJECT

public:
	explicit CDRipTrackInfo(QWidget *parent = nullptr);
	~CDRipTrackInfo();

	CJsonNode GetInfoData();
	void SetInfoData(CJsonNode node);

	QStringList GetArtistList() const;
	void SetArtistList(const QStringList &ArtistList);

	QStringList GetGenreList() const;
	void SetGenreList(const QStringList &GenreList);

	QStringList GetComposerList() const;
	void SetComposerList(const QStringList &ComposerList);

	QStringList GetMoodList() const;
	void SetMoodList(const QStringList &MoodList);

	int GetIndex() const;
	void SetIndex(int Index);

signals:

	void SigChangeArtist(int index, QString value);
	void SigChangeGenre(int index, QString value);
	void SigChangeComposer(int index, QString value);
	void SigChangeMood(int index, QString value);
	void SigChangeTitle(int index, QString value);
	void SigChangeTempo(int index, QString value);
	void SigChangeYear(int index, QString value);

private slots:

	void SlotChangedArtist(int index);
	void SlotChangedGenre(int index);
	void SlotChangedComposer(int index);
	void SlotChangedMood(int index);
	void SlotEditFinishTitle();
	void SlotEditFinishTempo();
	void SlotEditFinishYear();

private:

	CJsonNode m_InfoData;

	int			m_Index;
	QStringList	m_ArtistList;
	QStringList	m_GenreList;
	QStringList	m_ComposerList;
	QStringList	m_MoodList;

	Ui::CDRipTrackInfo *ui;
};

#endif // CDRIPTRACKINFO_H
