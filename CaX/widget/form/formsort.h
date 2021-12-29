#ifndef FORMSORT_H
#define FORMSORT_H

#include <QWidget>
#include <QMenu>
#include <QSlider>

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
	void SlotResizeSliderValueChanged(int value);
	void SlotResizeSliderReleased();

private:

	void Initialize();
	void SetResizeMenu();

	QMenu				*m_Menu;
	QMenu				*m_ResizeMenu;
	QSlider				*m_Slider;

	bool				m_bIncrease;

	Ui::FormSort *ui;
};

#endif // FORMSORT_H
