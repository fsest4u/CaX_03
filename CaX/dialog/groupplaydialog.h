#ifndef GROUPPLAYDIALOG_H
#define GROUPPLAYDIALOG_H

#include <QDialog>

namespace Ui {
class GroupPlayDialog;
}

class GroupPlayDialog : public QDialog
{
	Q_OBJECT

public:
	explicit GroupPlayDialog(QWidget *parent = nullptr);
	~GroupPlayDialog();

private:
	Ui::GroupPlayDialog *ui;
};

#endif // GROUPPLAYDIALOG_H
