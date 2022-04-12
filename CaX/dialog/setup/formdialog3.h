#ifndef FORMDIALOG3_H
#define FORMDIALOG3_H

#include <QDialog>

#include "util/CJsonNode.h"

namespace Ui {
class FormDialog3;
}

class FormDialog3 : public QDialog
{
	Q_OBJECT

public:
	explicit FormDialog3(QWidget *parent = nullptr);
	~FormDialog3();

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

	QString m_Url;

	Ui::FormDialog3 *ui;
};


#endif // FORMDIALOG3_H
