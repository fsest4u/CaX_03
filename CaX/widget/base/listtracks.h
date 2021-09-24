#ifndef LISTTRACKS_H
#define LISTTRACKS_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

namespace Ui {
class ListTracks;
}

class ListTracksDelegate;

class ListTracks : public QWidget
{
	Q_OBJECT

public:
	explicit ListTracks(QWidget *parent = nullptr);
	~ListTracks();

	void SetContentList(QList<CJsonNode> nodeList);
	void ClearContentList();

	QListView::ViewMode GetViewMode();
	void SetViewMode(QListView::ViewMode mode);

	QStandardItemModel	*GetModel();
	ListTracksDelegate	*GetDelegate();

	void SetBackgroundTask(QThread *thread);

signals:

	void SigReqCoverArt(int nID, int nIndex);

private slots:

	void SlotReqCoverArt();

private:

	void Initialize();

	QListView			*m_ListView;
	QStandardItemModel	*m_Model;
	ListTracksDelegate	*m_Delegate;

	QList<CJsonNode>	m_NodeList;

	Ui::ListTracks *ui;
};

#endif // LISTTRACKS_H
