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

signals:

	void SigSort();
	void SigIncDec();
	void SigResize();

protected:

	bool eventFilter(QObject *object, QEvent *event);

private:
	Ui::FormSort *ui;
};

#endif // FORMSORT_H
