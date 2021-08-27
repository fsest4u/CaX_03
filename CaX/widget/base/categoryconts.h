#ifndef CATEGORYCONTS_H
#define CATEGORYCONTS_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

namespace Ui {
class CategoryConts;
}

class CategoryContsDelegate;

class CategoryConts : public QWidget
{
	Q_OBJECT

public:
	explicit CategoryConts(QWidget *parent = nullptr);
	~CategoryConts();

	void SetContentList(QList<CJsonNode> nodeList);
	void ClearContentList();

	QListView::ViewMode GetViewMode();
	void SetViewMode(QListView::ViewMode mode);

	QStandardItemModel		*GetModel();
	CategoryContsDelegate	*GetDelegate();

	void SetBackgroundTask(QThread *thread);

signals:

	void SigReqCoverArt(int nID, int nIndex);

private slots:

	void SlotReqCoverArt();

private:

	void Initialize();

	QListView				*m_ListView;
	QStandardItemModel		*m_Model;
	CategoryContsDelegate	*m_Delegate;

	QList<CJsonNode>		m_NodeList;

	Ui::CategoryConts *ui;
};

#endif // CATEGORYCONTS_H
