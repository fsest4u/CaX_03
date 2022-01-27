#ifndef QUEUELISTWINDOW_H
#define QUEUELISTWINDOW_H

#include <QWidget>

#include "util/CJsonNode.h"

class QueuelistManager;
class QueueTrack;
class QueueLyrics;
class QueueArtist;
class FormCoverArt;
class FormTitle;

namespace Ui {
class QueuelistWindow;
}

class QueuelistWindow : public QWidget
{
	Q_OBJECT

public:
	explicit QueuelistWindow(QWidget *parent = nullptr, const QString &addr = "", const int &eventID = -1);
	~QueuelistWindow();

	void RequestQueuelist(QList<CJsonNode> list, QString src);

signals:

	void SigRemoveQueueWidget();

private slots:

	void SlotClickBtnTrack();
	void SlotClickBtnLyrics();
	void SlotClickBtnArtist();
	void SlotClickBtnClose();

	void SlotRespError(QString errMsg);
	void SlotRespCategoryInfo(CJsonNode node);
	void SlotCoverArtUpdate(QString fileName, int nIndex, int mode);
	void SlotSelectPlay(int index, int playType);

private:

	void ConnectSigToSlot();
	void Initialize();

	void RequestCoverArtMusicDB(int id, int index);
	void RequestCoverArtBrowser(QString path);
	void RequestCoverArtAudioCD(QString fullpath);

	QueuelistManager	*m_pMgr;
	QueueTrack			*m_Track;
	QueueLyrics			*m_Lyrics;
	QueueArtist			*m_Artist;

	FormCoverArt		*m_pFormCoverArt;
	FormTitle			*m_pFormTitle;

	int					m_EventID;

	QString				m_AlbumName;
	QString				m_AlbumCoverArt;
//	QString				m_ArtistName;
//	QString				m_ArtistCoverArt;

	QString				m_Src;

	Ui::QueuelistWindow *ui;
};

#endif // QUEUELISTWINDOW_H
