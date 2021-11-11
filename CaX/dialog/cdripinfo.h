#ifndef CDRIPINFO_H
#define CDRIPINFO_H

#include <QWidget>

#include "util/CJsonNode.h"

namespace Ui {
class CDRipInfo;
}

class CDRipInfo : public QWidget
{
	Q_OBJECT

public:
	explicit CDRipInfo(QWidget *parent = nullptr);
	~CDRipInfo();

	CJsonNode GetInfoData();
	void SetInfoData(CJsonNode node);

	QStringList GetAlbumList() const;
	void SetAlbumList(const QStringList &AlbumList);

	QStringList GetAlbumArtistList() const;
	void SetAlbumArtistList(const QStringList &AlbumArtistList);

signals:

	void SigChangeFormat(int index);
	void SigChangeAlbum(QString value);
	void SigChangeAlbumArtist(QString value);
	void SigChangeCDYear(QString value);
	void SigChangeCDNumber(QString value);
	void SigChangeCDTotal(QString value);
	void SigChangeCoverArt(QString value);

private slots:

	void SlotChangedFormat(int index);
	void SlotChangedAlbum(int index);
	void SlotChangedAlbumArtist(int index);
	void SlotEditFinishCDYear();
	void SlotEditFinishCDNumber();
	void SlotEditFinishCDTotal();

private:

	CJsonNode	m_InfoData;

	QStringList			m_AlbumList;
	QStringList			m_AlbumArtistList;

	Ui::CDRipInfo *ui;
};

#endif // CDRIPINFO_H
