#ifndef SEARCHTRACK_H
#define SEARCHTRACK_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

class SearchTrackDelegate;

namespace Ui {
class SearchTrack;
}

class SearchTrack : public QWidget
{
	Q_OBJECT

public:
	explicit SearchTrack(QWidget *parent = nullptr);
	~SearchTrack();

	void ClearNodeList();
	QList<CJsonNode> GetNodeList() const;
	void SetNodeList(const QList<CJsonNode> &NodeList, int category);

	void SetTitle(QString title);

	QStandardItemModel	*GetModel();
	SearchTrackDelegate	*GetDelegate();

	void SetBackgroundTask(QThread *thread);

signals:

	void SigReqCoverArt(int id, int index, int category);

private slots:

	void SlotReqCoverArt();
	void SlotFinishThread();

private:

	void Initialize();

	QListView				*m_ListView;
	QStandardItemModel		*m_Model;
	SearchTrackDelegate		*m_Delegate;

	QList<CJsonNode>		m_NodeList;

	Ui::SearchTrack *ui;
};

#endif // SEARCHTRACK_H
