#ifndef BROWSERDIALOG_H
#define BROWSERDIALOG_H

#include <QDialog>

namespace Ui {
class BrowserDialog;
}

class BrowserDialog : public QDialog
{
	Q_OBJECT

public:
	explicit BrowserDialog(QWidget *parent = nullptr, const QString &addr = "", const int &eventID = -1);
	~BrowserDialog();

	int GetBrowserMode() const;
	void SetBrowserMode(int BrowserMode, QString optionPath = "", int optionType = -1);

	void DoBrowserHome();

	QString GetPath() const;
	void SetPath(const QString &Path);

private slots:

	void SlotAddWidget(QWidget *widget);
	void SlotRemoveWidget(QWidget *widget);

	void SlotSelectCoverart(QString path);

private:

	void UpdateStackState();

	QString				m_Addr;
	QString				m_Path;

	int					m_BrowserMode;
	int					m_EventID;


	Ui::BrowserDialog *ui;
};

#endif // BROWSERDIALOG_H
