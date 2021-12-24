#ifndef GROUPPLAYWINDOW_H
#define GROUPPLAYWINDOW_H

#include <QWidget>

#include "util/CJsonNode.h"

class GroupPlayManager;
class InfoService;
class IconService;
class GroupPlayDialog;

namespace Ui {
class GroupPlayWindow;
}

class GroupPlayWindow : public QWidget
{
	Q_OBJECT

public:
	explicit GroupPlayWindow(QWidget *parent = nullptr, const QString &addr = "", const int &eventID = -1);
	~GroupPlayWindow();

	void GroupPlayList();

signals:

	void SigPowerOff(QList<CJsonNode> list);

private slots:

	void SlotRespError(QString errMsg);
	void SlotRespGroupPlayList(QList<CJsonNode> list);

	void SlotReqCoverArt(int id, int index, int mode);
	void SlotCoverArtUpdate(QString coverArt, int index, int mode);
	void SlotSelectPlay(int index, bool muted);
	void SlotSelectTitle(int type, QString rawData);

	void SlotEventGroupPlayUpdate();

	// dialog
	void SlotGroupPlay(QString addr, bool enabled);
	void SlotAutoJoin(QString addr, bool enabled);
	void SlotMute(QString addr, bool enabled);
	void SlotPlayStop(QString addr, bool enabled);
	void SlotPowerOff(QString addr, bool enabled);

	void SlotVolumeSliderReleased(QString addr, int value);
	void SlotChangedChannel(QString addr, int value);


//	void SlotResize();

private:

	void ConnectSigToSlot();

	GroupPlayManager	*m_pMgr;
	InfoService			*m_pInfoService;
	IconService			*m_pIconService;

	GroupPlayDialog		*m_Dialog;

	int					m_EventID;
	QString				m_Addr;

	Ui::GroupPlayWindow *ui;
};

#endif // GROUPPLAYWINDOW_H
