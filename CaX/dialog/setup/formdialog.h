#ifndef FORMDIALOG_H
#define FORMDIALOG_H

#include <QDialog>

#include "util/CJsonNode.h"

namespace Ui {
class FormDialog;
}

class FormDialog : public QDialog
{
	Q_OBJECT

public:
	explicit FormDialog(QWidget *parent = nullptr);
	~FormDialog();

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

	QString m_Url;

	Ui::FormDialog *ui;
};


#endif // FORMDIALOG_H
