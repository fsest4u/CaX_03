#ifndef FORMDIALOG4_H
#define FORMDIALOG4_H

#include <QDialog>

#include "util/CJsonNode.h"

namespace Ui {
class FormDialog4;
}

class FormDialog4 : public QDialog
{
	Q_OBJECT

public:
	explicit FormDialog4(QWidget *parent = nullptr);
	~FormDialog4();

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

	Ui::FormDialog4 *ui;
};


#endif // FORMDIALOG4_H
