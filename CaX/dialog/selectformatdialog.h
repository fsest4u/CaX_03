#ifndef SELECTFORMATDIALOG_H
#define SELECTFORMATDIALOG_H

#include <QDialog>

namespace Ui {
class SelectFormatDialog;
}

class SelectFormatDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SelectFormatDialog(QWidget *parent = nullptr);
	~SelectFormatDialog();

	QString GetFormat() const;
	void SetFormat(const QString &Format);

private slots:

	void SlotClickFormat();

private:

	QString		m_Format;

	Ui::SelectFormatDialog *ui;
};

#endif // SELECTFORMATDIALOG_H
