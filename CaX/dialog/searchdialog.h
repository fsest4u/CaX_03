#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SearchDialog(QWidget *parent = nullptr);
	~SearchDialog();

	QString GetKeyword();
	void SetKeyword(QString keyword);

private slots:

	void accept();
	void reject();

private:
	Ui::SearchDialog *ui;
};

#endif // SEARCHDIALOG_H
