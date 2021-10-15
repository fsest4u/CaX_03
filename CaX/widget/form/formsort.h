#ifndef FORMSORT_H
#define FORMSORT_H

#include <QWidget>

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

	bool GetIncrease() const;
	void SetIncrease(bool bIncrease);

signals:

	void SigSort();
	void SigIncDec(bool bIncrease);
	void SigResize();

protected:

	bool eventFilter(QObject *object, QEvent *event);

private:

	bool m_bIncrease;

	Ui::FormSort *ui;
};

#endif // FORMSORT_H
