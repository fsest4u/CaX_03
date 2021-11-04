#ifndef INPUTNAMEDIALOG_H
#define INPUTNAMEDIALOG_H

#include <QDialog>

namespace Ui {
class InputNameDialog;
}

class InputNameDialog : public QDialog
{
	Q_OBJECT

public:
	explicit InputNameDialog(QWidget *parent = nullptr);
	~InputNameDialog();

	QString GetName();
	void SetName(QString name);

private:
	Ui::InputNameDialog *ui;
};

#endif // INPUTNAMEDIALOG_H
