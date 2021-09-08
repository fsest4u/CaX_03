#ifndef ISERVICEWINDOW_H
#define ISERVICEWINDOW_H

#include <QWidget>

#include "util/CJsonNode.h"

class AirableManager;
class QobuzManager;

class MenuInfo;
class MenuIcon;
class MenuList;
class Loading;

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
	void RequestQobuzGenre();
	void RequestQobuzRecommend(int nType, QString strID, int nStart, int nCount);
	void RequestQobuzFavorite(int nType, int nStart, int nCount);
	void RequestQobuzPlaylist(int nStart, int nCount);
	void RequestQobuzCategory(int nType, QString strID, int nStart, int nCount);

	AirableManager	*GetAirableManager();
	QobuzManager	*GetQobuzManager();


signals:

//	void SigBtnPrev();
//	void SigSelectURL(int nServiceType, QString url);

	void SigRespLogout();
	void SigAddWidget(QWidget *widget);


private slots:

	void SlotAddWidget(QWidget *widget);
	void SlotRespAirableLogout();

	void SlotPlayAll();
	void SlotPlayRandom();
	void SlotSubmenu();
	void SlotSort();

	void SlotSelectCoverArt(int nType);
	void SlotSelectCoverArt(int nType, QString rawData);
	void SlotSelectURL(QString rawData);
	void SlotReqArt(QString url, int index);

	void SlotRespQobuzLoginFail(CJsonNode node);
	void SlotRespQobuzLoginSuccess();
	void SlotRespAirableLoginFail(CJsonNode node);
	void SlotRespAirableLoginSuccess(int nServiceType, bool bSaveAuth);

	void SlotRespList(QList<CJsonNode> list);

	void SlotRespAuth(int nServiceType);
	void SlotRespURL(int nServiceType, QList<CJsonNode> list);
	void SlotCoverArtUpdate(QString fileName, int nIndex, int mode);

private:

	void ConnectSigToSlot();

	void DoQobuzHome();
	void DoQobuzSearch();
	void DoQobuzRecommend();
	void DoQobuzFavorite();

	void DoRecommendAlbum();
	void DoRecommendPlaylist();

	void SetIServiceHome(QList<CJsonNode> &srclist);
	void SetQobuzHome(QList<CJsonNode> &srclist);
	void SetQobuzSearch(QList<CJsonNode> &srclist);
	void SetQobuzRecommend(QList<CJsonNode> &srclist);
	void SetQobuzFavorite(QList<CJsonNode> &srclist);

	void SetRecommendAlbum(QList<CJsonNode> &srclist);
	void SetRecommendPlaylist(QList<CJsonNode> &srclist);


	AirableManager		*m_pAirableMgr;
	QobuzManager		*m_pQobuzMgr;
	MenuInfo			*m_pMenuInfo;
	MenuIcon			*m_pMenuIcon;
	MenuList			*m_pMenuList;

	Loading				*m_pLoading;

	QString				m_WebURL;
	int					m_ServiceType;


	Ui::IServiceWindow *ui;
};

#endif // ISERVICEWINDOW_H
