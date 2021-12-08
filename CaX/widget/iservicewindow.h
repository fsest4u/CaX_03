#ifndef ISERVICEWINDOW_H
#define ISERVICEWINDOW_H

#include <QWidget>

#include "util/CJsonNode.h"

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
	explicit IServiceWindow(QWidget *parent = nullptr, const QString &addr = "");
	~IServiceWindow();

	void IServiceHome(QList<CJsonNode> list);

	void RequestIServiceURL(int nServiceType, QString url = "");
	void RequestQobuzSearch(int nType, QString keyword, int nStart, int nCount);
	void RequestQobuzGenre(QString strID = "");
	void RequestQobuzRecommend(int nType, QString strID, int nStart, int nCount, QString strGenreID = "");
	void RequestQobuzFavorite(int nType, int nStart, int nCount);
	void RequestQobuzPlaylist(int nStart, int nCount);
	void RequestQobuzCategory(int nType, QString strID, int nStart, int nCount);

	AirableManager	*GetAirableManager();
	QobuzManager	*GetQobuzManager();

	InfoBrowser *GetInfoBrowser() const;
	ListBrowser *GetListBrowser() const;

	int GetServiceType() const;
	void SetServiceType(int ServiceType);

	QThread *GetListThread() const;

	void AddWidgetItem();



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
	void SlotSelectTitle(int nType, QString rawData);
//	void SlotSelectURL(QString rawData);
	void SlotReqCoverArt(QString url, int index);

	void SlotRespQobuzLoginFail(CJsonNode node);
	void SlotRespQobuzLoginSuccess();
	void SlotRespAirableLoginFail(CJsonNode node);
	void SlotRespAirableLoginSuccess(int nServiceType, bool bSaveAuth);

	void SlotRespList(QList<CJsonNode> list);
	void SlotRespGenreSubList(QList<CJsonNode> list);

	void SlotRespAuth(int nServiceType);
	void SlotRespURL(int nServiceType, QString title, QList<CJsonNode> list);
	void SlotCoverArtUpdate(QString fileName, int nIndex, int mode);

	void SlotOptionMenu(int id, int type, QString menuName);
	void SlotOptionMenuAction(QString url, int type, int menuID);

private:

	void ConnectSigToSlot();
	void Initialize();

	void SelectTitleForQobuz(int nType, QString rawData);
	void SelectTitleForAirable(int nType, QString rawData);

	void SetOptionMenu(int type, QString menuName);

	void DoQobuzHome();
	void DoQobuzSearch();
	void DoQobuzRecommend();
	void DoQobuzFavorite();

	void DoRecommendAlbum();
	void DoRecommendPlaylist();
	void DoRecommendGenre(int nType, QString strID);

	void SetIServiceHome(QList<CJsonNode> &list);
	void SetQobuzHome(QList<CJsonNode> &list);
	void SetQobuzSearch(QList<CJsonNode> &list);
	void SetQobuzRecommend(QList<CJsonNode> &list);
	void SetQobuzFavorite(QList<CJsonNode> &list);

	void SetRecommendAlbum(QList<CJsonNode> &list);
	void SetRecommendPlaylist(QList<CJsonNode> &list);
	void SetRecommendGenre(QList<CJsonNode> &list, QString strID);

	void SetInfoTitle(QString title);

	void ThreadStartList();
	void ThreadTerminateList();

	AirableManager		*m_pAirableMgr;
	QobuzManager		*m_pQobuzMgr;
	InfoService			*m_pInfoService;
	InfoBrowser			*m_pInfoBrowser;

	IconService			*m_pIconService;
	ListBrowser			*m_pListBrowser;
//	ListService			*m_pListService;

	QThread				*m_pListThread;

	QMap<int, QString>	m_OptionMenuMap;

	QString				m_WebURL;
	int					m_ServiceType;

	bool				m_bGenreSubmenu;


	Ui::IServiceWindow *ui;
};

#endif // ISERVICEWINDOW_H
