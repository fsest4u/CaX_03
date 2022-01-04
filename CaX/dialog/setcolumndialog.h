#ifndef SETCOLUMNDIALOG_H
#define SETCOLUMNDIALOG_H

#include <QDialog>

namespace Ui {
class SetColumnDialog;
}

class SetColumnDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SetColumnDialog(QWidget *parent = nullptr);
	~SetColumnDialog();

	bool GetCBMood();
	void SetCBMood(bool bCheck);

	bool GetCBTempo();
	void SetCBTempo(bool bCheck);

	bool GetCBFormat();
	void SetCBFormat(bool bCheck);

	bool GetCBSampleRate();
	void SetCBSampleRate(bool bCheck);

	bool GetCBBitDepth();
	void SetCBBitDepth(bool bCheck);

	bool GetCBRating();
	void SetCBRating(bool bCheck);



private:
	Ui::SetColumnDialog *ui;
};

#endif // SETCOLUMNDIALOG_H
