#ifndef ISERVICEWINDOW_H
#define ISERVICEWINDOW_H

#include <QWidget>

#include "util/CJsonNode.h"

class AirableManager;
class QobuzManager;

class InfoService;
class IconService;
class ListService;

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


signals:

//	void SigBtnPrev();
//	void SigSelectURL(int nServiceType, QString url);

	void SigRespLogout();
	void SigAddWidget(QWidget *widget, QString title);


private slots:

	void SlotAddWidget(QWidget *widget, QString title);
	void SlotRespAirableLogout();

	void SlotResize();

	void SlotSelectTitle(int nType);
	void SlotSelectTitle(int nType, QString rawData);
	void SlotSelectURL(QString rawData);
	void SlotReqArt(QString url, int index);

	void SlotRespQobuzLoginFail(CJsonNode node);
	void SlotRespQobuzLoginSuccess();
	void SlotRespAirableLoginFail(CJsonNode node);
	void SlotRespAirableLoginSuccess(int nServiceType, bool bSaveAuth);

	void SlotRespList(QList<CJsonNode> list);
	void SlotRespGenreSubList(QList<CJsonNode> list);

	void SlotRespAuth(int nServiceType);
	void SlotRespURL(int nServiceType, QString title, QList<CJsonNode> list);
	void SlotCoverArtUpdate(QString fileName, int nIndex, int mode);

private:

	void ConnectSigToSlot();

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

	AirableManager		*m_pAirableMgr;
	QobuzManager		*m_pQobuzMgr;
	InfoService			*m_pInfoService;
	IconService			*m_pIconService;
	ListService			*m_pListService;

	QString				m_WebURL;
	int					m_ServiceType;

	bool				m_bGenreSubmenu;


	Ui::IServiceWindow *ui;
};

#endif // ISERVICEWINDOW_H
