#ifndef ICONTRACKS_H
#define ICONTRACKS_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

namespace Ui {
class IconTracks;
}

class IconTracksDelegate;

class IconTracks : public QWidget
{
	Q_OBJECT

public:
	explicit IconTracks(QWidget *parent = nullptr);
	~IconTracks();

	void SetContentList(QList<CJsonNode> nodeList);
	void ClearContentList();

	QListView::ViewMode GetViewMode();
	void SetViewMode(QListView::ViewMode mode);

	QStandardItemModel		*GetModel();
	IconTracksDelegate	*GetDelegate();

	void SetBackgroundTask(QThread *thread);

signals:

	void SigReqCoverArt(int nID, int nIndex);

private slots:

	void SlotReqCoverArt();

private:

	void Initialize();

	QListView				*m_ListView;
	QStandardItemModel		*m_Model;
	IconTracksDelegate		*m_Delegate;

	QList<CJsonNode>		m_NodeList;

	Ui::IconTracks *ui;
};

#endif // ICONTRACKS_H
