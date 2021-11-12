#ifndef CDRIPINFODIALOG_H
#define CDRIPINFODIALOG_H

#include <QDialog>

#include "util/CJsonNode.h"

class CDRipInfo;
class CDRipTrackInfo;

namespace Ui {
class CDRipInfoDialog;
}

class CDRipInfoDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CDRipInfoDialog(QWidget *parent = nullptr);
	~CDRipInfoDialog();

	CJsonNode GetInfoData();
	void SetInfoData(CJsonNode node);

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

	QString GetAddr() const;
	void SetAddr(const QString &Addr);

private slots:

	void SlotBtnPrev();
	void SlotBtnNext();

	void SlotChangeFormat(int index);
	void SlotChangeAlbum(QString value);
	void SlotChangeAlbumArtist(QString value);
	void SlotChangeCDYear(QString value);
	void SlotChangeCDNumber(QString value);
	void SlotChangeCDTotal(QString value);
	void SlotChangeCoverArt(QString image, QString thumb);

	void SlotChangeArtist(int index, QString value);
	void SlotChangeGenre(int index, QString value);
	void SlotChangeComposer(int index, QString value);
	void SlotChangeMood(int index, QString value);
	void SlotChangeTitle(int index, QString value);
	void SlotChangeTempo(int index, QString value);
	void SlotChangeYear(int index, QString value);

	void accept();
	void reject();

private:

	void UpdateStackState();

	CDRipInfo	*m_WidgetCD;
	QList<CJsonNode>	m_ListTrack;

	CJsonNode	m_InfoData;

	QStringList	m_AlbumList;
	QStringList	m_AlbumArtistList;
	QStringList	m_ArtistList;
	QStringList	m_GenreList;
	QStringList	m_ComposerList;
	QStringList	m_MoodList;

	QString m_Addr;

	Ui::CDRipInfoDialog *ui;
};

#endif // CDRIPINFODIALOG_H
