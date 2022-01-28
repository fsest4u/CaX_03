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

	QList<CJsonNode> GetNodeList() const;
	void SetNodeList(const QList<CJsonNode> &NodeList, int category);
	void ClearNodeList();

	void SetTitle(QString title);

	QStandardItemModel	*GetModel();
	QStyledItemDelegate	*GetDelegate();

signals:

	void SigReqCoverArt(int id, int nIndex, int category);

private slots:


private:

	void Initialize();

	QListView				*m_ListView;
	QStandardItemModel		*m_Model;
	QStyledItemDelegate		*m_Delegate;

	QList<CJsonNode>		m_NodeList;

	Ui::SearchCategory *ui;
};

#endif // SEARCHCATEGORY_H
