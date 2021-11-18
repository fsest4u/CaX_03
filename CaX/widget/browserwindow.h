#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QWidget>

#include "util/CJsonNode.h"
#include "util/caxconstants.h"

class BrowserManager;
class InfoService;
class InfoBrowser;
class IconService;
class ListBrowser;

namespace Ui {
class BrowserWindow;
}

class BrowserWindow : public QWidget
{
	Q_OBJECT

public:
	explicit BrowserWindow(QWidget *parent = nullptr, const QString &addr = "", const int &eventID = -1, const QString &root = "");
	~BrowserWindow();

	enum {
		MODE_NORMAL = 0,
		MODE_COPY,
		MODE_MOVE,
		MODE_MAX
	};

	void RequestRoot();
	void RequestFolder(QString strPath);

	int GetFolderType() const;
	void SetFolderType(int FolderType);

	int GetBrowserMode() const;
	void SetBrowserMode(int BrowserMode);

signals:

	void SigAddWidget(QWidget *widget, QString title);
	void SigRemoveWidget(QWidget* widget);

	void SigCopyHere(bool move, QString dstPath);


private slots:

	void SlotAddWidget(QWidget *widget, QString title);
	void SlotRemoveWidget(QWidget* widget);

	void SlotPlayAll(int where = PLAY_CLEAR);
	void SlotPlayRandom();
	void SlotTopMenu();
	void SlotTopMenuAction(int menuID);
//	void SlotResize();

	void SlotSelectTitle(int nType, QString rawData);
//	void SlotSelectURL(QString rawData);
	void SlotReqCoverArt(QString path, int index);
	void SlotReqInfoBot(QString path, int nIndex);

	void SlotRespList(QList<CJsonNode> list);
	void SlotCoverArtUpdate(QString filename, int index, int mode);
	void SlotInfoBotUpdate(CJsonNode node, int nIndex);

	void SlotCopyHere(bool move, QString dstPath);

private:

	void ReadSettings();
	void WriteSettings();

	void ConnectSigToSlot();
	void Initialize();

	void SetSelectOffTopMenu();
	void SetSelectOnTopMenu();

	void DoTopMenuPlay(int where);
	void DoTopMenuReload();
	void DoTopMenuSelectAll();
	void DoTopMenuClearAll();
	void DoTopMenuGain(QString gain);
	void DoTopMenuOptionPlaySubDir();
	void DoTopMenuOptionOverwrite();
	void DoTopMenuConverFormat();
	void DoTopMenuAdd();
	void DoTopMenuDelete();
	void DoTopMenuCopy(bool move);
	void DoTopMenuCopyHere(bool move);
	void DoTopMenuSearchCoverArt();

	void SetList(QList<CJsonNode> &list);
	void AnalyzeNode(CJsonNode node);
	void ShowFormPlay();

	void SetDirFile();
	void SetPaths();

	void DoDebugType(int type);

	BrowserManager		*m_pMgr;
	InfoService			*m_pInfoService;
	InfoBrowser			*m_pInfoBrowser;
	IconService			*m_pIconService;
	ListBrowser			*m_pListBrowser;

	QMap<int, QString>	m_TopMenuMap;
	QMap<QString, int>	m_SelectMap;


	QString				m_Root;
	QStringList			m_Paths;
	QStringList			m_Dirs;
	QStringList			m_Files;

	int					m_FolderType;
	int					m_EventID;

	int					m_BrowserMode;

	Ui::BrowserWindow *ui;
};

#endif // BROWSERWINDOW_H
