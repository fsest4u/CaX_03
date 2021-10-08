#ifndef FORMCLASSIFY_H
#define FORMCLASSIFY_H

#include <QWidget>

namespace Ui {
class FormClassify;
}

class FormClassify : public QWidget
{
	Q_OBJECT

public:
	explicit FormClassify(QWidget *parent = nullptr);
	~FormClassify();

protected:

	bool eventFilter(QObject *object, QEvent *event);

private:
	Ui::FormClassify *ui;
};

#endif // FORMCLASSIFY_H
