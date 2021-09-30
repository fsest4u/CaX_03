#ifndef POWEROFFDIALOG_H
#define POWEROFFDIALOG_H

#include <QDialog>

namespace Ui {
class PowerOffDialog;
}

class PowerOffDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PowerOffDialog(QWidget *parent = nullptr);
	~PowerOffDialog();

	QPushButton* GetBtnWol();

private:
	Ui::PowerOffDialog *ui;
};

#endif // POWEROFFDIALOG_H

/// UI Reference website
/// https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=browniz1004&logNo=220707750136
