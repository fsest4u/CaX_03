#ifndef SUBMENUDIALOG_H
#define SUBMENUDIALOG_H

#include <QDialog>
#include <QModelIndex>

#include "util/CJsonNode.h"


namespace Ui {
class SubmenuDialog;
}

class QListView;
class QStandardItemModel;
class SubmenuDelegate;

class SubmenuDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SubmenuDialog(QWidget *parent = nullptr);
	~SubmenuDialog();

	int GetID() const;
	void SetID(int nID);

	QList<CJsonNode> GetItemList() const;
	void SetItemList(const QList<CJsonNode> &ItemList);
	void ClearItemList();

signals:

	void SigSelectItem(int index);

private slots:

	void SlotSelectItem(QModelIndex index);

private:



	void Initialize();
	void SetMenu();
	void ClearMenu();

	QListView			*m_ListView;
	QStandardItemModel	*m_Model;
	SubmenuDelegate		*m_Delegate;

	QList<CJsonNode>	m_ItemList;

	int					m_nID;

	Ui::SubmenuDialog *ui;
};

#endif // SUBMENUDIALOG_H
