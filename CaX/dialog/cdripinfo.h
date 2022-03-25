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

	QString GetAddr() const;
	void SetAddr(const QString &Addr);

	QString GetImage() const;
	void SetImage(const QString &Image);

	QString GetThumb() const;
	void SetThumb(const QString &Thumb);

	QString GetCoverArt() const;
	void SetCoverArt(const QString &CoverArt);

	void SetEventID(int eventID);

signals:

	void SigChangeFormat(int index);
	void SigChangeAlbum(QString value);
	void SigChangeAlbumArtist(QString value);
	void SigChangeCDYear(QString value);
	void SigChangeCDNumber(QString value);
	void SigChangeCDTotal(QString value);
	void SigChangeCoverArt(QString image, QString thumb);

protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotChangedFormat(int index);
//	void SlotChangedAlbum(int index);
//	void SlotChangedAlbumArtist(int index);
	void SlotEditFinishCDYear();
	void SlotEditFinishCDNumber();
	void SlotEditFinishCDTotal();
	void SlotClickCoverArt();

	void SlotEditTextChangedAlbum(const QString &text);
	void SlotEditTextChangedAlbumArtist(const QString &text);

	void SlotBrowserPathSelectCoverart(QString path);

private:

	void ConnectSigToSlot();
	void Initialize();

	CJsonNode	m_InfoData;

	QStringList			m_AlbumList;
	QStringList			m_AlbumArtistList;

	QString				m_Image;
	QString				m_Thumb;

	QString				m_Addr;
	int					m_EventID;

	Ui::CDRipInfo *ui;
};

#endif // CDRIPINFO_H
