#ifndef ADDRADIODIALOG_H
#define ADDRADIODIALOG_H

#include <QDialog>

namespace Ui {
class AddRadioDialog;
}

class AddRadioDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddRadioDialog(QWidget *parent = nullptr);
	~AddRadioDialog();

	void SetTitle(QString title);

	QString GetName();
	void SetName(QString name);

	double GetFrequency();
	void SetFrequency(double freq);

	void SetRange(double min, double max, double step);

private:
	Ui::AddRadioDialog *ui;
};

#endif // ADDRADIODIALOG_H
