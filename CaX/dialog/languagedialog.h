#ifndef LANGUAGEDIALOG_H
#define LANGUAGEDIALOG_H

#include <QDialog>

namespace Ui {
class LanguageDialog;
}

class LanguageDialog : public QDialog
{
	Q_OBJECT

public:
	explicit LanguageDialog(QWidget *parent = nullptr);
	~LanguageDialog();

	QString GetLanguageData();
	void SetLanguageData(QString data);

private:

	void Initialize();

	QString GetLanguageName(QString data);

	Ui::LanguageDialog *ui;
};

#endif // LANGUAGEDIALOG_H
