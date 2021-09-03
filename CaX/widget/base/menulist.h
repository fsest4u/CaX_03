#ifndef MENULIST_H
#define MENULIST_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

namespace Ui {
class MenuList;
}

class MenuListDelegate;

class MenuList : public QWidget
{
	Q_OBJECT

public:
	explicit MenuList(QWidget *parent = nullptr);
	~MenuList();

	QListView::ViewMode GetViewMode();
	void SetViewMode(QListView::ViewMode mode);

	QStandardItemModel	*GetModel();
	MenuListDelegate	*GetDelegate();

	void ClearNodeList();
	QList<CJsonNode> GetNodeList() const;
	void SetNodeList(const QList<CJsonNode> &NodeList);

signals:

	void SigReqArt(QString url, int nIndex);

private:

	void Initialize();

	QListView			*m_ListView;
	QStandardItemModel	*m_Model;
	MenuListDelegate	*m_Delegate;

	QList<CJsonNode>	m_NodeList;

	Ui::MenuList *ui;
};

#endif // MENULIST_H
