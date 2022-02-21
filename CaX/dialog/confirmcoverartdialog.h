#ifndef CONFIRMCOVERARTDIALOG_H
#define CONFIRMCOVERARTDIALOG_H

#include <QDialog>

namespace Ui {
class ConfirmCoverArtDialog;
}

class ConfirmCoverArtDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ConfirmCoverArtDialog(QWidget *parent = nullptr);
	~ConfirmCoverArtDialog();

	void SetImagePath(QString imagePath);

private:
	Ui::ConfirmCoverArtDialog *ui;
};

#endif // CONFIRMCOVERARTDIALOG_H
