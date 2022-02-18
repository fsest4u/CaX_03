#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H

#include <QWidget>

namespace Ui {
class LoadingDialog;
}

class LoadingDialog : public QWidget
{
	Q_OBJECT

public:
	explicit LoadingDialog(QWidget *parent = nullptr);
	~LoadingDialog();

protected:

	bool eventFilter(QObject *object, QEvent *event);

private:

	void Initialize();

	Ui::LoadingDialog *ui;
};

#endif // LOADINGDIALOG_H
