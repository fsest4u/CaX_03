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

	void SetNodeForm(CJsonNode node);

	QString GetKey0() const;
	QString GetKey1() const;
	QString GetKey2() const;
	QString GetKey3() const;

	QString GetValue0() const;
	QString GetValue1() const;
	QString GetValue2() const;
	QString GetValue3() const;

	QString GetHiddenKey() const;
	void SetHiddenKey(const QString &HiddenKey);

	QString GetHiddenValue() const;
	void SetHiddenValue(const QString &HiddenValue);

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
