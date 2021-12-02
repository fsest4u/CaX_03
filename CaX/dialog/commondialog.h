#ifndef COMMONDIALOG_H
#define COMMONDIALOG_H

#include <QDialog>

namespace Ui {
class CommonDialog;
}

class CommonDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CommonDialog(QWidget *parent = nullptr, QString title = "", QString content = "");
	~CommonDialog();

	void SetTitle(QString title);
	void SetContent(QString content);
	void ShowBtnOk(bool show);
	void ShowBtnCancel(bool show);

private slots:

	void accept();
	void reject();

private:

	void ConnectSigToSlot();

	Ui::CommonDialog *ui;
};

#endif // COMMONDIALOG_H
