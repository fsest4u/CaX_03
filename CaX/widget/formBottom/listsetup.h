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
	void SetNodeList(const QList<CJsonNode> &NodeList);
	void ClearNodeList();

	void SetEditor(int index);

	QStandardItemModel *GetModel();
	ListSetupDelegate *GetDelegate();

private:

	void Initialize();

	QListView				*m_ListView;
	QStandardItemModel		*m_Model;
	ListSetupDelegate		*m_Delegate;

	QList<CJsonNode>		m_NodeList;

	Ui::ListSetup *ui;
};

#endif // LISTSETUP_H
