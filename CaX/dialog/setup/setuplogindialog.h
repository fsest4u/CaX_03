#ifndef SETUPLOGINDIALOG_H
#define SETUPLOGINDIALOG_H

#include <QDialog>

#include "util/CJsonNode.h"

namespace Ui {
class SetupLoginDialog;
}

class SetupLoginDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SetupLoginDialog(QWidget *parent = nullptr);
	~SetupLoginDialog();

	void SetNodeForm(CJsonNode node);

	QString GetUsername() const;
	QString GetPassword() const;

	QString GetUsernameKey() const;
	void SetUsernameKey(const QString &UsernameKey);

	QString GetPasswordKey() const;
	void SetPasswordKey(const QString &PasswordKey);

private:

	void ConnectSigToSlot();
	void Initialize();

	void SetOK(QString title, int action);
	void SetCancel(QString title, int action);
	void SetInputs(CJsonNode node);

	int m_ActionOK;
	int m_ActionCancel;

	QString m_UsernameKey;
	QString m_PasswordKey;

	Ui::SetupLoginDialog *ui;
};

#endif // SETUPLOGINDIALOG_H
