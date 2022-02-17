#ifndef SETUPWINDOW_H
#define SETUPWINDOW_H

#include <QWidget>
#include <QMenu>
#include <QModelIndex>

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

signals:

	void SigAddWidget(QWidget *widget, QString title);
	void SigRemoveWidget(QWidget* widget);

private slots:

	void SlotAddWidget(QWidget *widget, QString title);
	void SlotRemoveWidget(QWidget* widget);

	void SlotSelectMenu(const QModelIndex &modelIndex, QPoint point);
	void SlotSelectMenuSub(const QModelIndex &modelIndex, QPoint point);
	void SlotMenuActionSub(QAction *action);

	void SlotBrowserPath(QString path);

	void SlotRespError(QString errMsg);
	void SlotRespGroup(QList<CJsonNode> list);
	void SlotRespSet(CJsonNode node);


private:

	void ReadSettings();
	void WriteSettings();

	void ConnectSigToSlot();
	void Initialize();

	void SetMenuSubMap(QStringList keys, QStringList values);

	void DoAlarm(CJsonNode node);
	void DoAnalogInVolume(CJsonNode node);
	void DoCustomEQ(CJsonNode node);
	void DoLogin(CJsonNode node);
	void DoMaxVolume(int volume);
	void DoPowerOnVolume(CJsonNode node);
	void DoTimeManual(CJsonNode node);
	void DoWiredLanSetup(CJsonNode node);

	SetupManager		*m_pMgr;
	InfoService			*m_pInfoService;
	ListSetup			*m_pListSetup;

	int m_EventID;
	QString m_StrID;
	QString m_StrIDSub;

	int m_MaxVolume;

//	QMap<QString, CJsonNode>	m_MenuMap;
	QMap<QString, QString>	m_MenuSubMap;

	QMenu				*m_MenuSub;

	QModelIndex			m_ModelIndex;
	QPoint				m_Point;

	Ui::SetupWindow *ui;
};

#endif // SETUPWINDOW_H
