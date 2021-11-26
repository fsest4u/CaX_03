#ifndef PROGRESSFMDIALOG_H
#define PROGRESSFMDIALOG_H

#include <QDialog>

namespace Ui {
class ProgressFmDialog;
}

class ProgressFmDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ProgressFmDialog(QWidget *parent = nullptr);
	~ProgressFmDialog();

	void SetTitle(QString title);

private:
	Ui::ProgressFmDialog *ui;
};

#endif // PROGRESSFMDIALOG_H
