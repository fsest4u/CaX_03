#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
	Q_OBJECT

public:
	explicit LoginDialog(QWidget *parent = nullptr);
	~LoginDialog();

	QString GetUserID();
	void SetUserID(QString userID);

	QString GetPassword();
	void SetPassword(QString password);

private:
	Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
