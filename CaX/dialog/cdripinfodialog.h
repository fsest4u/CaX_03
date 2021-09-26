#ifndef CDRIPINFODIALOG_H
#define CDRIPINFODIALOG_H

#include <QDialog>

namespace Ui {
class CDRipInfoDialog;
}

class CDRipInfoDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CDRipInfoDialog(QWidget *parent = nullptr);
	~CDRipInfoDialog();

	void SetTitle(const QString &value);
	void SetArtist(const QString &value);
	void SetGenre(const QString &value);
	void SetComposer(const QString &value);
	void SetMood(const QString &value);
	void SetTempo(const QString &value);
	void SetYear(const QString &value);

private:
	Ui::CDRipInfoDialog *ui;
};

#endif // CDRIPINFODIALOG_H
