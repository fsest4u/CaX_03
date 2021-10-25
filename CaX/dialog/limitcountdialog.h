#ifndef LIMITCOUNTDIALOG_H
#define LIMITCOUNTDIALOG_H

#include <QDialog>

namespace Ui {
class LimitCountDialog;
}

class LimitCountDialog : public QDialog
{
	Q_OBJECT

public:
	explicit LimitCountDialog(QWidget *parent = nullptr);
	~LimitCountDialog();

	int GetLimitCount();
	void SetLimitCount(int count);

private:
	Ui::LimitCountDialog *ui;
};

#endif // LIMITCOUNTDIALOG_H
