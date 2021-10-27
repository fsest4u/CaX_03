#ifndef ADDPLAYLISTDIALOG_H
#define ADDPLAYLISTDIALOG_H

#include <QDialog>

namespace Ui {
class AddPlaylistDialog;
}

class AddPlaylistDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddPlaylistDialog(QWidget *parent = nullptr);
	~AddPlaylistDialog();

	QString GetName();
	void SetName(QString name);

private:
	Ui::AddPlaylistDialog *ui;
};

#endif // ADDPLAYLISTDIALOG_H
