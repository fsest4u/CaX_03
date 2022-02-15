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
//	QList<CJsonNode> GetNodeListDetail() const;
	void SetNodeList(const QList<CJsonNode> &NodeList);
	void SetNodeListSub(const QList<CJsonNode> &NodeList);
//	void SetNodeListDetail(const QList<CJsonNode> &NodeList);
	void ClearNodeList();
	void ClearNodeListSub();
//	void ClearNodeListDetail();

//	void SetEditor(int index);

	QListView *GetListView();
	QListView *GetListViewSub();
//	QListView *GetListViewDetail();
	QStandardItemModel *GetModel();
	QStandardItemModel *GetModelSub();
//	QStandardItemModel *GetModelDetail();
	ListSetupDelegate *GetDelegate();
	ListSetupDelegate *GetDelegateSub();
//	ListSetupDelegate *GetDelegateDetail();


private:

	void Initialize();

	QListView				*m_ListView;
	QListView				*m_ListViewSub;
//	QListView				*m_ListViewDetail;
	QStandardItemModel		*m_Model;
	QStandardItemModel		*m_ModelSub;
//	QStandardItemModel		*m_ModelDetail;
	ListSetupDelegate		*m_Delegate;
	ListSetupDelegate		*m_DelegateSub;
//	ListSetupDelegate		*m_DelegateDetail;

	QList<CJsonNode>		m_NodeList;
	QList<CJsonNode>		m_NodeListSub;
//	QList<CJsonNode>		m_NodeListDetail;

	Ui::ListSetup *ui;
};

#endif // LISTSETUP_H
