#ifndef ADDSHAREDIALOG_H
#define ADDSHAREDIALOG_H

#include <QDialog>

#include "util/CJsonNode.h"

class BrowserManager;

namespace Ui {
class AddShareDialog;
}

class AddShareDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddShareDialog(QWidget *parent = nullptr);
	~AddShareDialog();

	CJsonNode GetNodeInfo();
	void SetNodeInfo(CJsonNode node);
	void SetInfo(QString addr, int EventID);

private slots:

	void SlotRespWorkGroup(QStringList list);
	void SlotRespServer(QList<CJsonNode> list);
	void SlotRespSMBShare(QStringList list);

	void SlotSelectWorkGroup(const QString& title);
	void SlotSelectServer(int index);
	void SlotSelectShare(int index);

private:

	void ConnectSigToSlot();
	void Initialize();

	BrowserManager		*m_pMgr;

	int					m_EventID;

	Ui::AddShareDialog *ui;
};

#endif // ADDSHAREDIALOG_H
