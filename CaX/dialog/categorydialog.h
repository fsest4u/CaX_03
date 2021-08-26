#ifndef CATEGORYDIALOG_H
#define CATEGORYDIALOG_H

#include <QDialog>
#include <QModelIndex>

namespace Ui {
class CategoryDialog;
}

class QListView;
class QStandardItemModel;
class CategoryDelegate;

class CategoryDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CategoryDialog(QWidget *parent = nullptr);
	~CategoryDialog();

	int GetID() const;
	void SetID(int nID);

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
	CategoryDelegate	*m_Delegate;

	int					m_nID;

	Ui::CategoryDialog *ui;
};

#endif // CATEGORYDIALOG_H
