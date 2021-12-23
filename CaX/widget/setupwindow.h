#ifndef SETUPWINDOW_H
#define SETUPWINDOW_H

#include <QWidget>

#include "util/CJsonNode.h"

class SetupManager;
class InfoService;
class ListSetup;

namespace Ui {
class SetupWindow;
}

class SetupWindow : public QWidget
{
	Q_OBJECT

public:
	explicit SetupWindow(QWidget *parent = nullptr, const QString &addr = "");
	~SetupWindow();

	void SetupHome(QList<CJsonNode> list, int eventID);

private slots:

	void SlotSelectTitle(QString strID, int index);
	void SlotMenuAction(QString strID, QString json);
	void SlotSubMenuAction(QString value, QString json);

	void SlotRespError(QString errMsg);
	void SlotRespGroup(QList<CJsonNode> list, int index);
	void SlotRespSet(CJsonNode node);

private:

	void ConnectSigToSlot();

//	void SetSetupHome(QList<CJsonNode> & list);

	void SetMenuMap(QList<CJsonNode> list);

	SetupManager		*m_pMgr;
	InfoService			*m_pInfoService;
	ListSetup			*m_pListSetup;

	int m_EventID;
	QString m_StrID;

	QMap<QString, CJsonNode>	m_MenuMap;

	Ui::SetupWindow *ui;
};

#endif // SETUPWINDOW_H
