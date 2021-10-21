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

	QList<CJsonNode> GetNodeList() const;
	void SetNodeList(QList<CJsonNode> list);
	void ClearNodeList();

	QListView::ViewMode GetViewMode();
	void SetViewMode(QListView::ViewMode mode);

	QMap<int, bool> GetSelectMap() const;
	void SetSelectMap(const QMap<int, bool> &SelectMap);

	QStandardItemModel	*GetModel();
	ListTracksDelegate	*GetDelegate();

	void SetBackgroundTask(QThread *thread);

signals:

	void SigReqCoverArt(int id, int index, int mode);
	void SigAppendList();

private slots:

	void SlotReqCoverArt();
	void SlotScrollValueChanged(int value);
	void SlotDoubleClickItem(const QModelIndex& index);

private:

	void Initialize();

	QListView				*m_ListView;
	QStandardItemModel		*m_Model;
	ListTracksDelegate		*m_Delegate;
	QScrollBar				*m_ScrollBar;

	Loading					*m_pLoading;

	QList<CJsonNode>		m_NodeList;
	QMap<int, bool>			m_SelectMap;

	Ui::ListTracks *ui;
};

#endif // LISTTRACKS_H
