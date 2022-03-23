#ifndef ISERVICEWINDOW_H
#define ISERVICEWINDOW_H

#include <QWidget>
#include <QMenu>
#include <QModelIndex>

#include "util/CJsonNode.h"
#include "util/caxconstants.h"

class AirableManager;
class QobuzManager;

class InfoService;
class InfoBrowser;
class IconService;
class ListBrowser;
//class ListService;

namespace Ui {
class IServiceWindow;
}

class IServiceWindow : public QWidget
{
	Q_OBJECT

public:
	explicit IServiceWindow(QWidget *parent = nullptr, const QString &addr = "", const int &eventID = -1);
	~IServiceWindow();

	void IServiceHome(QList<CJsonNode> list);

	void RequestIServiceURL(QString url = "");
	void RequestIServicePlay(QMap<int, CJsonNode> nodeMap, int nWhere);

	void RequestQobuzSearch(int nType, QString keyword, int nStart, int nCount);
	void RequestQobuzGenre(QString strID = "");
	void RequestQobuzRecommend(int nType, QString strID, int nStart, int nCount, QString strGenreID = "");
	void RequestQobuzFavorite(int nType, int nStart, int nCount);
	void RequestQobuzPlaylist(int nStart, int nCount);
	void RequestQobuzCategory(int nType, QString strID, int nStart, int nCount);
	void RequestQobuzPlay(QMap<int, CJsonNode> nodeMap, int nWhere);

	void AddWidgetItem(bool playAll = false, bool playRandom = false, bool menu = false);

	AirableManager	*GetAirableManager();
	QobuzManager	*GetQobuzManager();

	InfoBrowser *GetInfoBrowser() const;
	ListBrowser *GetListBrowser() const;

	int GetInternetType() const;
	void SetInternetType(int internetType);

	int GetType() const;
	void SetType(int Type);

	int GetTypeParent() const;
	void SetTypeParent(int TypeParent);

	bool GetGenre() const;
	void SetGenre(bool bGenre);

	QString GetPlaylistID() const;
	void SetPlaylistID(const QString &PlaylistID);

	CJsonNode GetNode() const;
	void SetNode(const CJsonNode &Node);

	signals:

	//	void SigBtnPrev();
	//	void SigSelectURL(int nServiceType, QString url);

	void SigRespLogout();
	void SigAddWidget(QWidget *widget, QString title);


private slots:

	void SlotAddWidget(QWidget *widget, QString title);
	void SlotRespAirableLogout();

//	void SlotResize();

	void SlotRespError(QString errMsg);
	void SlotSelectIconTitle(int nType);
	void SlotSelectTitle(int nType, CJsonNode node);
//	void SlotSelectURL(QString rawData);
	void SlotSelectMenu(const QModelIndex &modelIndex, QPoint point);
	void SlotMenuAction(QAction *action);
	void SlotReqCoverArt(QString url, int index);
	void SlotAppendList();

	void SlotRespQobuzLoginFail(CJsonNode node);
	void SlotRespQobuzLoginSuccess();
	void SlotRespAirableLoginFail(CJsonNode node);
	void SlotRespAirableLoginSuccess(int nServiceType, bool bSaveAuth);

	void SlotRespQobuzList(QList<CJsonNode> list, bool genre, int start);
	void SlotListUpdate();

	void SlotRespAuth(int nServiceType);
	void SlotRespURL(int nServiceType, QList<CJsonNode> list, QString title, QString nextUrl);
	void SlotRespForm(int nServiceType, CJsonNode node);
	void SlotCoverArtUpdate(QString fileName, int nIndex, int mode);

	void SlotPlayAll(int where = PLAY_CLEAR);
	void SlotPlayRandom();
	void SlotTopMenu();
	void SlotTopMenuAction(int menuID);

//	void SlotOptionMenu(int id, int type);
//	void SlotOptionMenu(int id, int type, QString menuName);
	void SlotOptionMenuAction(int menuID, CJsonNode node);

private:

	void ConnectSigToSlot();
	void Initialize();

	void SetSelectOffTopMenu();
	void SetSelectOnTopMenu();

	void DoTopMenuSelectAll();
	void DoTopMenuClearAll();
	void DoTopMenuPlay(int nWhere);
	void DoTopMenuAddToFavorite();
	void DoTopMenuDeleteToFavorite();
	void DoTopMenuAddPlaylist();
	void DoTopMenuDeletePlaylist();
	void DoTopMenuDeleteTrack();

	void SetOptionMenu(int type, QString menuName = "");

//	void DoOptionMenuPlay(int nID, int where);
	void DoOptionMenuDeletePlaylist(CJsonNode node);
	void DoOptionMenuActionUrl(CJsonNode node);
	void DoOptionMenuSetupReservedRecord(CJsonNode node);

	void SelectSearchForQobuz(int nType, CJsonNode node);
	void SelectRecommendForQobuz(int nType, CJsonNode node);
	void SelectFavoriteForQobuz(int nType, CJsonNode node);
	void SelectUserPlaylistForQobuz(int nType, CJsonNode node);
	void SelectTitleForAirable(int nType, CJsonNode node);

	void AppendSearchForQobuz();
	void AppendRecommendForQobuz();
	void AppendFavoriteForQobuz();
	void AppendUserPlaylistForQobuz();
	void AppendTitleForAirable();

	void DoQobuzHome();
	void DoQobuzSearch();
	void DoQobuzRecommend();
	void DoQobuzFavorite();

	void DoQobuzRecommendAlbum();
	void DoQobuzRecommendPlaylist();
	void DoQobuzRecommendGenre(int nType, QString strID);

	void SetIServiceHome(QList<CJsonNode> &list);
	void SetQobuzHome(QList<CJsonNode> &list);
	void SetQobuzSearch(QList<CJsonNode> &list);
	void SetQobuzRecommend(QList<CJsonNode> &list);
	void SetQobuzFavorite(QList<CJsonNode> &list);

	void SetRecommendAlbum(QList<CJsonNode> &list);
	void SetRecommendPlaylist(QList<CJsonNode> &list);
	void SetRecommendGenre(QList<CJsonNode> &list, QString strID);

	AirableManager		*m_pAirableMgr;
	QobuzManager		*m_pQobuzMgr;
	InfoService			*m_pInfoService;
	InfoBrowser			*m_pInfoBrowser;

	IconService			*m_pIconService;
	ListBrowser			*m_pListBrowser;
//	ListService			*m_pListService;

	QMap<int, QString>		m_TopMenuMap;
	QMap<int, QString>		m_OptionMenuMap;
	QMap<int, CJsonNode>	m_SelectMap;

	QMenu				*m_Menu;

	CJsonNode			m_Node;

	QString				m_WebURL;
	QString				m_PlaylistID;

	int					m_InternetType;
	int					m_Type;
	int					m_TypeParent;
	int					m_EventID;

	bool				m_bGenre;
	bool				m_Refresh;

	int					m_StartIndex;

	QModelIndex			m_ModelIndex;

	Ui::IServiceWindow *ui;
};

#endif // ISERVICEWINDOW_H
