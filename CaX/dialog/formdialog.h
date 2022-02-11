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

	void SetLabelTitle(CJsonNode node);

	void SetOK(QString title, int action);
	void SetCancel(QString title, int action);

	int m_ActionOK;
	int m_ActionCancel;

	Ui::FormDialog *ui;
};


#endif // FORMDIALOG_H
