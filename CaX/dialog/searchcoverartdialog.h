#ifndef SEARCHCOVERARTDIALOG_H
#define SEARCHCOVERARTDIALOG_H

#include <QDialog>

namespace Ui {
class SearchCoverArtDialog;
}

class SearchCoverArtDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SearchCoverArtDialog(QWidget *parent = nullptr);
	~SearchCoverArtDialog();

	QString GetSite() const;
	void SetSite(QString site);

	QString GetKeyword() const;
	void SetKeyWord(QString value);

	QString GetArtist() const;
	void SetArtist(QString value);

private slots:

	void SlotClickSearchEngine();

	void accept();
	void reject();

private:


	QString m_Site;
	Ui::SearchCoverArtDialog *ui;
};

#endif // SEARCHCOVERARTDIALOG_H
