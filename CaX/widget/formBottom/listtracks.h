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

//	enum {
//		LIST_TRACKS_MUSIC_DB_YEAR = 0,
//		LIST_TRACKS_MUSIC_DB_TRACK,
//		LIST_TRACKS_MAX
//	};

	QList<CJsonNode> GetNodeList() const;
	void SetNodeList(QList<CJsonNode> list, int service);
	void ClearNodeList();

	void ClearSelectMap();
	void SetAllSelectMap();

	QMap<int, bool> GetSelectMap() const;
	void SetSelectMap(const QMap<int, bool> &SelectMap);

	void SetResize(int resize);

	void SetEditor(int index);

	QStandardItemModel	*GetModel();
	ListTracksDelegate	*GetDelegate();

	void SetHeaderTitle(QString title);

	void ShowHeaderTime(bool show);
	void ShowHeaderArtist(bool show);
	void ShowHeaderAlbum(bool show);
	void ShowHeaderGenre(bool show);

signals:

	void SigReqCoverArt(int id, int index, int mode);
	void SigAppendList();

//	void SigEditAllArtist(QString value);
//	void SigEditAllAlbum(QString value);
//	void SigEditAllGenre(QString value);

private slots:

	void SlotScrollValueChanged(int value);
	void SlotSelectCheck(const QModelIndex &modelIndex);

//	void SlotEditAllArtist();
//	void SlotEditAllAlbum();
//	void SlotEditAllGenre();

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
