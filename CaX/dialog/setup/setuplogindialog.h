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

	CJsonNode GetNodeForm();
	void SetNodeForm(CJsonNode node);

private slots:

	void accept();
	void reject();

private:

	void ConnectSigToSlot();
	void Initialize();

	void SetOK(QString title, int action);
	void SetCancel(QString title, int action);
	void SetInputs(CJsonNode node);

	int m_ActionOK;
	int m_ActionCancel;

	Ui::SetupLoginDialog *ui;
};

#endif // SETUPLOGINDIALOG_H
