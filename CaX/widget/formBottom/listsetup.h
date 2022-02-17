#ifndef LISTSETUP_H
#define LISTSETUP_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

class ListSetupDelegate;

namespace Ui {
class ListSetup;
}

class ListSetup : public QWidget
{
	Q_OBJECT

public:
	explicit ListSetup(QWidget *parent = nullptr);
	~ListSetup();

	QList<CJsonNode> GetNodeList() const;
	QList<CJsonNode> GetNodeListSub() const;
	void SetNodeList(const QList<CJsonNode> &NodeList);
	void SetNodeListSub(const QList<CJsonNode> &NodeList);
	void ClearNodeList();
	void ClearNodeListSub();


	QListView *GetListView();
	QListView *GetListViewSub();
	QStandardItemModel *GetModel();
	QStandardItemModel *GetModelSub();
	ListSetupDelegate *GetDelegate();
	ListSetupDelegate *GetDelegateSub();


private:

	void Initialize();

	QListView				*m_ListView;
	QListView				*m_ListViewSub;
	QStandardItemModel		*m_Model;
	QStandardItemModel		*m_ModelSub;
	ListSetupDelegate		*m_Delegate;
	ListSetupDelegate		*m_DelegateSub;

	QList<CJsonNode>		m_NodeList;
	QList<CJsonNode>		m_NodeListSub;

	Ui::ListSetup *ui;
};

#endif // LISTSETUP_H
