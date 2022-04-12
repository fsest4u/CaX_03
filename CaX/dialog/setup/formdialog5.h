#ifndef FORMDIALOG5_H
#define FORMDIALOG5_H

#include <QDialog>

#include "util/CJsonNode.h"

namespace Ui {
class FormDialog5;
}

class FormDialog5 : public QDialog
{
	Q_OBJECT

public:
	explicit FormDialog5(QWidget *parent = nullptr);
	~FormDialog5();

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

	Ui::FormDialog5 *ui;
};


#endif // FORMDIALOG5_H
