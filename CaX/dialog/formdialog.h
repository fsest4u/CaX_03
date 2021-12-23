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

	void SetWindowTitle(QString title);
	void SetLabelTitle(CJsonNode arrInput);

	void SetOK(QString title, int action);
	void SetCancel(QString title, int action);

private slots:

	void accept();
	void reject();

private:



	int m_ActionOK;
	int m_ActionCancel;

	Ui::FormDialog *ui;
};


#endif // FORMDIALOG_H
