#ifndef FORMLABEL_H
#define FORMLABEL_H

#include <QWidget>

namespace Ui {
class FormLabel;
}

class FormLabel : public QWidget
{
	Q_OBJECT

public:
	explicit FormLabel(QWidget *parent = nullptr);
	~FormLabel();

private:
	Ui::FormLabel *ui;
};

#endif // FORMLABEL_H
