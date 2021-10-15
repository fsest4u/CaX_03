#ifndef FORMSORT_H
#define FORMSORT_H

#include <QWidget>
#include <QMenu>

namespace Ui {
class FormSort;
}

class FormSort : public QWidget
{
	Q_OBJECT

public:
	explicit FormSort(QWidget *parent = nullptr);
	~FormSort();

	void ShowSort();
	void ShowIncDec();
	void ShowResize();

	void SetSortMenu(QMap<int, QString> list);
	void SetSortMenuTitle(QString title);

	bool GetIncrease() const;
	void SetIncrease(bool bIncrease);

signals:

	void SigSort(int sort);
	void SigIncDec(bool bIncrease);
	void SigResize();

protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotSortMenu(QAction *action);

private:

	QMenu *m_SortMenu;
	bool m_bIncrease;

	Ui::FormSort *ui;
};

#endif // FORMSORT_H
