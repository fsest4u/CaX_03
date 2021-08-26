#ifndef SONGCONTS_H
#define SONGCONTS_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

namespace Ui {
class SongConts;
}

class SongContsDelegate;

class SongConts : public QWidget
{
	Q_OBJECT

public:
	explicit SongConts(QWidget *parent = nullptr);
	~SongConts();

	void SetContentList(QList<CJsonNode> nodeList);
	void ClearContentList();

	QListView::ViewMode GetViewMode();
	void SetViewMode(QListView::ViewMode mode);

	QStandardItemModel	*GetModel();

	void SetBackgroundTask(QThread *thread);

signals:

	void SigReqCoverArt(int nID, int nIndex);
	void SigSelectItem(int nID);

private slots:

	void SlotReqCoverArt();
	void SlotSelectItem(QModelIndex index);
	void SlotSelectItem(int nID);

private:

	void Initialize();

	QListView			*m_ListView;
	QStandardItemModel	*m_Model;
	SongContsDelegate	*m_Delegate;

	QList<CJsonNode>	m_NodeList;

	Ui::SongConts *ui;
};

#endif // SONGCONTS_H
