#ifndef SEARCHHISTORYDIALOG_H
#define SEARCHHISTORYDIALOG_H

#include <QDialog>

namespace Ui {
class SearchHistoryDialog;
}

class SearchHistoryDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SearchHistoryDialog(QWidget *parent = nullptr);
	~SearchHistoryDialog();

	QString GetKeyword();

public slots:

	void accept();
	void reject();

private:

	void ReadSettings();
	void WriteSettings();

	void Initialize();

	QStringList		m_SearchList;

	Ui::SearchHistoryDialog *ui;
};

#endif // SEARCHHISTORYDIALOG_H
