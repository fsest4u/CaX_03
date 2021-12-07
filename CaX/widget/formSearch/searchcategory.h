#ifndef SEARCHCATEGORY_H
#define SEARCHCATEGORY_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>
#include <QStyledItemDelegate>

#include "util/CJsonNode.h"

class SearchCategoryDelegate;

namespace Ui {
class SearchCategory;
}

class SearchCategory : public QWidget
{
	Q_OBJECT

public:
	explicit SearchCategory(QWidget *parent = nullptr);
	~SearchCategory();

	void ClearNodeList();
	QList<CJsonNode> GetNodeList() const;
	void SetNodeList(const QList<CJsonNode> &NodeList, int category);

	void SetTitle(QString title);

	QStandardItemModel	*GetModel();
	QStyledItemDelegate	*GetDelegate();

	void SetBackgroundTask(QThread *thread);

signals:

	void SigReqCoverArt(int id, int nIndex, int category);

private slots:

	void SlotReqCoverArt();
	void SlotFinishThread();

private:

	void Initialize();

	QListView				*m_ListView;
	QStandardItemModel		*m_Model;
	QStyledItemDelegate		*m_Delegate;

	QList<CJsonNode>		m_NodeList;

	Ui::SearchCategory *ui;
};

#endif // SEARCHCATEGORY_H
