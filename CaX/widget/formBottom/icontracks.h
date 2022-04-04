#ifndef ICONTRACKS_H
#define ICONTRACKS_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

class IconTracksDelegate;

namespace Ui {
class IconTracks;
}

class IconTracks : public QWidget
{
	Q_OBJECT

public:
	explicit IconTracks(QWidget *parent = nullptr);
	~IconTracks();

	enum {
		ICON_TRACKS_MUSIC_DB_YEAR = 0,
		ICON_TRACKS_MUSIC_DB_TRACK,
		ICON_TRACKS_MAX
	};

	QList<CJsonNode> GetNodeList() const;
	void SetNodeList(QList<CJsonNode> &list, int service);
	void ClearNodeList();

	void ClearSelectMap();
	void SetAllSelectMap();

	QMap<int, int> GetSelectMap() const;
	void SetSelectMap(const QMap<int, int> &SelectMap);

	void SetResize(int resize);

	void ShowFrameEmpty(bool show);

	QListView			*GetListView();
	QStandardItemModel	*GetModel();
	IconTracksDelegate	*GetDelegate();

signals:

	void SigReqCoverArt(const QModelIndex &modelIndex, int mode);
	void SigReqCount(const QModelIndex &modelIndex);
//	void SigCalcTotalTime(int time);
	void SigAppendList();

private slots:

	void SlotScrollValueChanged(int value);
	void SlotScrollReleased();
	void SlotSelectCheck(const QModelIndex &modelIndex);

private:

	void Initialize();

	QListView				*m_ListView;
	QStandardItemModel		*m_Model;
	IconTracksDelegate		*m_Delegate;
	QScrollBar				*m_ScrollBar;

	QList<CJsonNode>		m_NodeList;
	QMap<int, int>			m_SelectMap;

	QPoint					m_PointCurrent;

	int						m_Service;

	Ui::IconTracks *ui;
};

#endif // ICONTRACKS_H
