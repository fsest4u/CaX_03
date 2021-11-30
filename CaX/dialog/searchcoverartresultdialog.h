#ifndef SEARCHCOVERARTRESULTDIALOG_H
#define SEARCHCOVERARTRESULTDIALOG_H

#include <QDialog>

#include "util/CJsonNode.h"

class TCPClient;
class SearchCoverArtResultManager;
class IconCoverArt;

namespace Ui {
class SearchCoverArtResultDialog;
}

class SearchCoverArtResultDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SearchCoverArtResultDialog(QWidget *parent = nullptr, const QString &addr = "");
	~SearchCoverArtResultDialog();

	void RequestCoverArtList(QString site, QString keyword, QString artist);

	void SetAddr(const QString &addr);

	QString GetImage() const;
	void SetImage(const QString &Image);

	QString GetThumb() const;
	void SetThumb(const QString &Thumb);

	QByteArray GetImageData() const;
	void SetImageData(const QByteArray &ImageData);

//	QString GetImagePath() const;
//	void SetImagePath(const QString &ImagePath);

private slots:

	void SlotRespCoverArtList(CJsonNode node);
//	void SlotRespSearchCoverArt(QByteArray data, int index);
	void SlotRespSearchCoverArt(QString filename, int index);
	void SlotSearchCoverArt(int index, QString url);
	void SlotAppendIconList();
	void SlotSelectCoverArt(int index);

private:

	void ConnectSigToSlot();
	void Initialize();

//	TCPClient			*m_pTcpClient;
	SearchCoverArtResultManager	*m_pMgr;
	IconCoverArt		*m_pIconCoverArt;
	QList<CJsonNode>	m_NodeList;

	QString m_NextUrl;
	QString m_Site;
	QString m_Keyword;
	QString m_Artist;

	QString m_Image;
	QString m_Thumb;
	QByteArray m_ImageData;
//	QString m_ImagePath;

	Ui::SearchCoverArtResultDialog *ui;
};

#endif // SEARCHCOVERARTRESULTDIALOG_H
