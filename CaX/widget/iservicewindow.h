#ifndef ISERVICEWINDOW_H
#define ISERVICEWINDOW_H

#include <QWidget>

#include "util/CJsonNode.h"

namespace Ui {
class IServiceWindow;
}

class AirableManager;
class QobuzManager;

class MenuInfo;
class MenuIcon;
class MenuList;
class Loading;

class IServiceWindow : public QWidget
{
	Q_OBJECT

public:
	explicit IServiceWindow(QWidget *parent = nullptr, const QString &addr = "");
	~IServiceWindow();

	void RequestIServiceHome(QList<CJsonNode> list);
	void RequestIServiceURL(int nServiceType, QString url = "");
	void RequestQobuzHome();

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

	void SlotSelectIService(int nServiceType);
	void SlotSelectURL(QString rawData);
	void SlotReqArt(QString url, int index);

	void SlotRespQobuzLoginFail(CJsonNode node);
	void SlotRespQobuzLoginSuccess();
	void SlotRespAirableLoginFail(CJsonNode node);
	void SlotRespAirableLoginSuccess(int nServiceType, bool bSaveAuth);

	void SlotRespAuth(int nServiceType);
	void SlotRespURL(int nServiceType, QList<CJsonNode> list);
	void SlotCoverArtUpdate(QString fileName, int nIndex, int mode);

private:

	void ConnectSigToSlot();
	void SetCoverArtServiceHome(QList<CJsonNode> &srclist);
	void SetQobuzHome(QList<CJsonNode> &srclist);

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
