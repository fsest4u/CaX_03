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

private:
	Ui::FormSort *ui;
};

#endif // FORMSORT_H
