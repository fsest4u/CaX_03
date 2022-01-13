#ifndef INPUTTAGDIALOG_H
#define INPUTTAGDIALOG_H

#include <QDialog>

namespace Ui {
class InputTagDialog;
}

class InputTagDialog : public QDialog
{
	Q_OBJECT

public:
	explicit InputTagDialog(QWidget *parent = nullptr);
	~InputTagDialog();

	QStringList GetTagList() const;
	void SetTagList(const QStringList &tagList);

	QString GetTagName() const;
	void SetTagName(const QString name);


private:

	QStringList	m_TagList;

	Ui::InputTagDialog *ui;
};

#endif // INPUTTAGDIALOG_H
