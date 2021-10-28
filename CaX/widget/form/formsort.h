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

	void ShowMenu();
	void ShowIncDec();
	void ShowResize();

	void SetMenu(QMap<int, QString> list);
	void SetMenuTitle(QString title);

	bool GetIncrease() const;
	void SetIncrease(bool bIncrease);

signals:

	void SigMenu(int menuID);
	void SigIncDec(bool bIncrease);
	void SigResize();

protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotMenu(QAction *action);

private:

	void Initialize();

	QMenu *m_Menu;
	bool m_bIncrease;

	Ui::FormSort *ui;
};

#endif // FORMSORT_H
