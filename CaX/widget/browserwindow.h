#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QWidget>

#include "util/CJsonNode.h"

class BrowserManager;
class InfoService;
class IconService;
class ListService;
class Loading;

namespace Ui {
class BrowserWindow;
}

class BrowserWindow : public QWidget
{
	Q_OBJECT

public:
	explicit BrowserWindow(QWidget *parent = nullptr, const QString &addr = "", const QString &root = "");
	~BrowserWindow();

	void RequestRoot();
	void RequestFolder(QString strPath);

signals:

	void SigAddWidget(QWidget *widget);


private slots:

	void SlotAddWidget(QWidget *widget);

	void SlotSubmenu(int nID);
	void SlotSelectTitle(int nType, QString rawData);
	void SlotSelectURL(QString rawData);
	void SlotRespList(QList<CJsonNode> list);
	void SlotReqInfoBot(QString strPath, int nIndex);
	void SlotRespNodeUpdate(CJsonNode node, int nIndex);

private:

	void ConnectSigToSlot();

	void SetCoverArt(QList<CJsonNode> &list);

	BrowserManager		*m_pMgr;
	InfoService			*m_pInfoService;
	IconService			*m_pIconService;
	ListService			*m_pListService;

	Loading				*m_pLoading;

	QString				m_Root;

	Ui::BrowserWindow *ui;
};

#endif // BROWSERWINDOW_H
