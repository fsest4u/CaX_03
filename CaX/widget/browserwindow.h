#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QWidget>

#include "util/CJsonNode.h"

class BrowserManager;
class InfoService;
class IconService;
class ListService;

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

	void SigAddWidget(QWidget *widget, QString title);


private slots:

	void SlotAddWidget(QWidget *widget, QString title);

	void SlotPlayAll();
	void SlotPlayRandom();
	void SlotTopMenu();
	void SlotTopMenuAction(int menuID);
//	void SlotResize();

	void SlotSelectTitle(int nType, QString rawData);
	void SlotSelectURL(QString rawData);
	void SlotRespList(QList<CJsonNode> list);
	void SlotReqInfoBot(QString strPath, int nIndex);
	void SlotRespNodeUpdate(CJsonNode node, int nIndex);

private:

	void ConnectSigToSlot();
	void SetSelectOffTopMenu(int type);
	void SetSelectOnTopMenu(int type);

	void SetList(QList<CJsonNode> &list);
	void AnalyzeNode(CJsonNode node);
	QString GetCoverArtIcon(CJsonNode node);
	void ShowFormPlay();

	BrowserManager		*m_pMgr;
	InfoService			*m_pInfoService;
	IconService			*m_pIconService;
	ListService			*m_pListService;

	QMap<int, QString>	m_TopMenuMap;

	QString				m_Root;
	QStringList			m_Dirs;
	QStringList			m_Files;

	Ui::BrowserWindow *ui;
};

#endif // BROWSERWINDOW_H
