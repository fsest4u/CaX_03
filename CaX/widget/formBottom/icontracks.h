#ifndef ICONTRACKS_H
#define ICONTRACKS_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

class IconTracksDelegate;
class Loading;

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

	QMap<int, bool> GetSelectMap() const;
	void SetSelectMap(const QMap<int, bool> &SelectMap);

	void SetResize(int resize);

	QListView			*GetListView();
	QStandardItemModel	*GetModel();
	IconTracksDelegate	*GetDelegate();

	void SetBackgroundTask(QThread *thread);

signals:

	void SigReqCoverArt(int id, int index, int mode);
//	void SigCalcTotalTime(int time);
	void SigAppendList();

private slots:

	void SlotReqCoverArt();
	void SlotFinishThread();
	void SlotScrollValueChanged(int value);
	void SlotSelectCoverArt(int index);

private:

	void Initialize();

	QListView				*m_ListView;
	QStandardItemModel		*m_Model;
	IconTracksDelegate		*m_Delegate;
	QScrollBar				*m_ScrollBar;

//	Loading					*m_pLoading;

	QList<CJsonNode>		m_NodeList;
	QMap<int, bool>			m_SelectMap;

	Ui::IconTracks *ui;
};

#endif // ICONTRACKS_H
