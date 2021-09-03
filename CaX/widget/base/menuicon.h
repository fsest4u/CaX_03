#ifndef MENUICON_H
#define MENUICON_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

namespace Ui {
class MenuIcon;
}

class MenuIconDelegate;

class MenuIcon : public QWidget
{
	Q_OBJECT

public:
	explicit MenuIcon(QWidget *parent = nullptr);
	~MenuIcon();

	QList<CJsonNode> GetNodeList() const;
	void SetNodeList(const QList<CJsonNode> &NodeList);
	void ClearNodeList();

	QListView::ViewMode GetViewMode();
	void SetViewMode(QListView::ViewMode mode);

	QStandardItemModel		*GetModel();
	MenuIconDelegate		*GetDelegate();

signals:

private slots:

private:

	void Initialize();

	QListView				*m_ListView;
	QStandardItemModel		*m_Model;
	MenuIconDelegate		*m_Delegate;

	QList<CJsonNode>	m_NodeList;

	Ui::MenuIcon *ui;
};

#endif // MENUICON_H
