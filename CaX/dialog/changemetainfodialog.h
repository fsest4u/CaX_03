#ifndef CHANGEMETAINFODIALOG_H
#define CHANGEMETAINFODIALOG_H

#include <QDialog>

namespace Ui {
class ChangeMetaInfoDialog;
}

class ChangeMetaInfoDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ChangeMetaInfoDialog(QWidget *parent = nullptr);
	~ChangeMetaInfoDialog();

	int GetIndex();
	void SetIndex(QStringList list);

private:
	Ui::ChangeMetaInfoDialog *ui;
};

#endif // CHANGEMETAINFODIALOG_H
