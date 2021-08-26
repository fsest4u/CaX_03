#ifndef SORTDIALOG_H
#define SORTDIALOG_H

#include <QDialog>
#include <QModelIndex>

namespace Ui {
class SortDialog;
}

class QListView;
class QStandardItemModel;
class SortDelegate;

class SortDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SortDialog(QWidget *parent = nullptr);
	~SortDialog();

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
	SortDelegate		*m_Delegate;

	int					m_nID;

	Ui::SortDialog *ui;
};

#endif // SORTDIALOG_H
