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
		ICON_TRACKS_MUSIC_DB = 0,
		ICON_TRACKS_AUDIO_CD,
		ICON_TRACKS_PLAYLIST,
		ICON_TRACKS_MAX
	};

	QList<CJsonNode> GetNodeList() const;
	void SetNodeList(QList<CJsonNode> &list, int type);
	void ClearNodeList();

	QListView::ViewMode GetViewMode();
	void SetViewMode(QListView::ViewMode mode);

	QStandardItemModel		*GetModel();
	IconTracksDelegate	*GetDelegate();

	void SetBackgroundTask(QThread *thread);

signals:

	void SigReqCoverArt(int id, int index, int mode);
	void SigCalcTotalTime(int time);

private slots:

	void SlotReqCoverArt();

private:

	void Initialize();

	QListView				*m_ListView;
	QStandardItemModel		*m_Model;
	IconTracksDelegate		*m_Delegate;

	Loading					*m_pLoading;

	QList<CJsonNode>		m_NodeList;

	Ui::IconTracks *ui;
};

#endif // ICONTRACKS_H
