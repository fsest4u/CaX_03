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

	void ShowMenu(bool show = true);
	void ShowIncDec(bool show = true);
	void ShowResize(bool show = true);

	void ClearMenu();
	void SetMenu(QMap<int, QString> list);
	void SetMenuTitle(QString title);

	void SetResize(int resize);
	void SetSliderMinimum(int value);

	bool GetIncrease() const;
	void SetIncrease(bool bIncrease);

signals:

	void SigMenu(int menuID);
	void SigIncDec(bool bIncrease);
	void SigResize(int resize);

protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotMenu(QAction *action);

	void SlotBtnResize();
	void SlotResizeSliderValueChanged(int value);

private:

	void Initialize();

	QMenu				*m_Menu;

	bool				m_bIncrease;

	int					m_ResizeMin;
	int					m_ResizeValue;

	Ui::FormSort *ui;
};

#endif // FORMSORT_H
