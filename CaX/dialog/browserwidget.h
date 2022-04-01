#ifndef BROWSERWIDGET_H
#define BROWSERWIDGET_H

#include <QWidget>

#include <QMenu>
#include <QModelIndex>

#include "util/CJsonNode.h"
#include "util/caxconstants.h"

class BrowserManager;
class IconService;
class ListBrowser;

namespace Ui {
class BrowserWidget;
}

class BrowserWidget : public QWidget
{
	Q_OBJECT

public:
	explicit BrowserWidget(QWidget *parent = nullptr, const QString &addr = "", const int &eventID = -1, const QString &root = "");
	~BrowserWidget();

	void RequestRoot(QString ext = "");
	void RequestFolder(QString strPath, QString ext = "", CJsonNode upnp = CJsonNode(JSON_OBJECT));

	int GetFolderType() const;
	void SetFolderType(int FolderType);

	int GetBrowserMode() const;
	void SetBrowserMode(int BrowserMode, QString optionPath = "", int optionType = -1);

signals:

	void SigAddWidget(QWidget *widget);
	void SigRemoveWidget(QWidget* widget);

	void SigSelectCoverart(QString path);

private slots:

	void SlotAddWidget(QWidget *widget);
	void SlotRemoveWidget(QWidget* widget);

	void SlotSelectCoverart(QString path);

	void SlotIconSelectTitle(int nType, QString rawData);
//	void SlotSelectTrackPlay(int nType, CJsonNode node);
	void SlotSelectTitle(int nType, CJsonNode node);
	void SlotSelectMenu(const QModelIndex &modelIndex, QPoint point);
	void SlotMenuAction(QAction *action);
	void SlotReqCoverArt(QString path, int index);
	void SlotReqInfoBot(QString path, int nIndex);

	void SlotRespError(QString errMsg);
	void SlotRespList(QList<CJsonNode> list);
	void SlotCoverArtUpdate(QString filename, int index, int mode);
	void SlotInfoBotUpdate(CJsonNode node, int index);
	void SlotInfoTagUpdate(CJsonNode node);
	void SlotRespCategoryList(QList<CJsonNode> list);
	void SlotListUpdate();
	void SlotRespSMBInfo(CJsonNode node);

//	void SlotPlayAll(int where = PLAY_CLEAR);
//	void SlotPlayRandom();
//	void SlotTopMenu();
//	void SlotTopMenuAction(int menuID);

	void SlotOptionMenuAction(CJsonNode node, int type, int menuID);

//	void SlotCopyHere(bool move, QString dstPath, QString path, int type);

private:


	void ReadSettings();
	void WriteSettings();

	void ConnectSigToSlot();
	void Initialize();
	void SetCategoryList(QList<CJsonNode> list);

//	void SetSelectOffTopMenu();
//	void SetSelectOnTopMenu();

//	void DoTopMenuPlay(int where);
	void DoTopMenuReload();
//	void DoTopMenuSelectAll();
//	void DoTopMenuClearAll();
//	void DoTopMenuGain(QString gain);
//	void DoTopMenuOptionPlaySubDir();
//	void DoTopMenuOptionOverwrite();
//	void DoTopMenuConverFormat();
//	void DoTopMenuMakeFolder();
//	void DoTopMenuDelete();
//	void DoTopMenuCopy(bool move);
//	void DoTopMenuCopyHere(bool move);
//	void DoTopMenuSearchCoverArt();
//	void DoTopMenuAddShare();

	void SetOptionMenu(int type);

//	void DoOptionMenuPlay(CJsonNode node, int type, int where);
//	void DoOptionMenuGain(QString path, int type, QString gain);
//	void DoOptionMenuOptionPlaySubDir();
//	void DoOptionMenuOptionOverwrite();
//	void DoOptionMenuConverFormat(QString path, int type);
//	void DoOptionMenuScan(QString path, bool scanOn);
//	void DoOptionMenuRename(QString path);
//	void DoOptionMenuDelete(QString path, int type);
//	void DoOptionMenuCopy(QString path, int type, bool move);
////	void DoOptionMenuCopyHere(QString path, bool move);
//	void DoOptionMenuEditTag(QString path, int type);
//	void DoOptionMenuSearchCoverArt(QString path, int type);
//	void DoOptionMenuModifyShare(QString path);
//	void DoOptionMenuDeleteShare(QString path);
	void DoOptionMenuSelectCoverart(QString path);

//	void ShowFormPlay(bool show = true);

	void AnalyzeNode(CJsonNode node);
	void SetDirFile();
	void SetPaths();

	void SetOptionDirFile(QString path, int type, QStringList &dirs, QStringList & files);
	void SetOptionPaths(QString path, int type, QStringList &paths);

	BrowserManager		*m_pMgr;
	IconService			*m_pIconService;
	ListBrowser			*m_pListBrowser;

	QMap<int, QString>	m_TopMenuMap;
	QMap<int, QString>	m_OptionMenuMap;
	QMap<QString, int>	m_SelectMap;

	QMenu				*m_Menu;

	QModelIndex			m_ModelIndex;

	QString				m_Root;
	QStringList			m_Paths;
	QStringList			m_Dirs;
	QStringList			m_Files;

	int					m_FolderType;
	int					m_EventID;

	int					m_BrowserMode;
	QString				m_OptionPath;
	int					m_OptionType;

	QStringList			m_AlbumList;
	QStringList			m_AlbumArtistList;
	QStringList			m_ArtistList;
	QStringList			m_GenreList;
	QStringList			m_ComposerList;
	QStringList			m_MoodList;

	QString				m_Ext;
	CJsonNode			m_NodeUpnp;
	Ui::BrowserWidget *ui;
};

#endif // BROWSERWIDGET_H
