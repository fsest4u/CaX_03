#ifndef LISTTRACKS_H
#define LISTTRACKS_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

class ListTracksDelegate;
class Loading;

namespace Ui {
class ListTracks;
}

class ListTracks : public QWidget
{
	Q_OBJECT

public:
	explicit ListTracks(QWidget *parent = nullptr);
	~ListTracks();

	enum {
		LIST_TRACKS_MUSIC_DB = 0,
		LIST_TRACKS_AUDIO_CD,
		LIST_TRACKS_PLAYLIST,
		LIST_TRACKS_MAX
	};

	QList<CJsonNode> GetNodeList() const;
	void SetNodeList(QList<CJsonNode> list, int type);
	void ClearNodeList();

	void ClearSelectMap();
	void SetAllSelectMap();

	QMap<int, bool> GetSelectMap() const;
	void SetSelectMap(const QMap<int, bool> &SelectMap);

	void SetResize(int resize);

	QStandardItemModel	*GetModel();
	ListTracksDelegate	*GetDelegate();

	void SetBackgroundTask(QThread *thread);
	void SetLineEditReadOnly(bool readOnly);

signals:

	void SigReqCoverArt(int id, int index, int mode);
	void SigAppendList();

	void SigEditAllArtist(QString value);
	void SigEditAllAlbum(QString value);
	void SigEditAllGenre(QString value);

private slots:

	void SlotReqCoverArt();
	void SlotFinishThread();
	void SlotScrollValueChanged(int value);
	void SlotDoubleClickItem(const QModelIndex& index);

	void SlotEditAllArtist();
	void SlotEditAllAlbum();
	void SlotEditAllGenre();

private:

	void ConnectSigToSlot();
	void Initialize();

	QListView				*m_ListView;
	QStandardItemModel		*m_Model;
	ListTracksDelegate		*m_Delegate;
	QScrollBar				*m_ScrollBar;

//	Loading					*m_pLoading;

	QList<CJsonNode>		m_NodeList;
	QMap<int, bool>			m_SelectMap;

	Ui::ListTracks *ui;
};

#endif // LISTTRACKS_H
