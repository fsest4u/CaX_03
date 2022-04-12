#ifndef FORMDIALOG2_H
#define FORMDIALOG2_H

#include <QDialog>

#include "util/CJsonNode.h"

namespace Ui {
class FormDialog2;
}

class FormDialog2 : public QDialog
{
	Q_OBJECT

public:
	explicit FormDialog2(QWidget *parent = nullptr);
	~FormDialog2();

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

	Ui::FormDialog2 *ui;
};


#endif // FORMDIALOG2_H
