#ifndef QUEUETRACK_H
#define QUEUETRACK_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

class QueueTrackDelegate;

namespace Ui {
class QueueTrack;
}

class QueueTrack : public QWidget
{
	Q_OBJECT

public:
	explicit QueueTrack(QWidget *parent = nullptr);
	~QueueTrack();

	QList<CJsonNode> GetNodeList() const;
	int SetNodeList(QList<CJsonNode> list);
	void ClearNodeList();

	QStandardItemModel	*GetModel();
	QueueTrackDelegate	*GetDelegate();

private:

	void Initialize();

	QListView				*m_ListView;
	QStandardItemModel		*m_Model;
	QueueTrackDelegate		*m_Delegate;

	QList<CJsonNode>		m_NodeList;

	Ui::QueueTrack *ui;
};

#endif // QUEUETRACK_H
