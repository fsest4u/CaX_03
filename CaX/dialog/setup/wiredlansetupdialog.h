#ifndef WIREDLANSETUPDIALOG_H
#define WIREDLANSETUPDIALOG_H

#include <QDialog>

#include "util/CJsonNode.h"

namespace Ui {
class WiredLanSetupDialog;
}

class WiredLanSetupDialog : public QDialog
{
	Q_OBJECT

public:
	explicit WiredLanSetupDialog(QWidget *parent = nullptr);
	~WiredLanSetupDialog();

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

	QString m_HiddenKey;
	QString m_HiddenValue;

	Ui::WiredLanSetupDialog *ui;
};

#endif // WIREDLANSETUPDIALOG_H
