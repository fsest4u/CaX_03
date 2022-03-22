#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ProgressDialog(QWidget *parent = nullptr);
	~ProgressDialog();

	void SetWindowTitle(QString title);
	void SetContent(QString content);
	void SetCurTime(int seconds);
	void SetTotTime(int seconds);
	void SetProgress(int cur, int tot);
	void SetBtnBack(bool show);
	void SetBtnStop(bool show);
	void SetBtnSkip(bool show);
	void SetBtnClose(bool show);
	void SetTaskID(int taskID);
	void SetEventID(int eventID);

signals:

	void SigClickBack(int taskID);
	void SigClickStop(int taskID);
	void SigClickSkip(int taskID);
	void SigClickClose();


private slots:

	void SlotClickBack();
	void SlotClickStop();
	void SlotClickSkip();
	void SlotClickClose();

private:

	void ConnectSigToSlot();
	void Initialize();

	int m_TaskID;
	int m_EventID;

	Ui::ProgressDialog *ui;
};

#endif // PROGRESSDIALOG_H
