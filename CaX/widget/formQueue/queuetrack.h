#ifndef QUEUETRACK_H
#define QUEUETRACK_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

class QueuelistManager;
class QueueTrackDelegate;

namespace Ui {
class QueueTrack;
}

class QueueTrack : public QWidget
{
	Q_OBJECT

public:
	explicit QueueTrack(QWidget *parent = nullptr, const QString &addr = "", const int &eventID = -1);
	~QueueTrack();

	QList<CJsonNode> GetNodeList() const;
	int SetNodeList(QList<CJsonNode> list);
	void ClearNodeList();

	int GetCurrentIndex();
	void SetCurrentIndex(int index);

	QStandardItemModel	*GetModel();
	QueueTrackDelegate	*GetDelegate();

signals:

	void SigFavorite(int id, int favorite);
	void SigAddToPlaylist(int id);
	void SigGoToAlbum(int albumID);
	void SigGoToArtist(int artistID);
	void SigUpdateTimeStamp(uint timestamp, int count, int time);

private slots:

	void SlotRespError(QString errMsg);
	void SlotRespTrackInfo(CJsonNode node);
	void SlotRespQueueList(CJsonNode result);
	void SlotRespDeleteQueue(CJsonNode node);

	void SlotSelectMenu(const QModelIndex &modelIndex, QPoint point);
	void SlotMenuAction(QAction *action);

private:

	void ConnectSigToSlot();
	void Initialize();

	void ClearOptionMenu();
	void SetOptionMenu(QMap<int, QString> map);

	void DoOptionMenuFavorite();
	void DoOptionMenuAddToPlaylist();
	void DoOptionMenuGoToAlbum();
	void DoOptionMenuGoToArtist();
	void DoOptionMenuDeleteFromPlayQueue();

	QueuelistManager		*m_pMgr;

	QListView				*m_ListView;
	QStandardItemModel		*m_Model;
	QueueTrackDelegate		*m_Delegate;

	QList<CJsonNode>		m_NodeList;

	int						m_EventID;
	int						m_OldIndex;

	int						m_TrackID;
	int						m_TrackAlbumID;
	int						m_TrackArtistID;
	int						m_TrackFavorite;

	QMenu					*m_Menu;

	int						m_SelIndex;
	QPoint					m_SelPoint;
	uint					m_TimeStamp;


	Ui::QueueTrack *ui;
};

#endif // QUEUETRACK_H
