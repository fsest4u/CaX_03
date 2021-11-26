#ifndef GROUPPLAYWINDOW_H
#define GROUPPLAYWINDOW_H

#include <QWidget>

#include "util/CJsonNode.h"

class GroupPlayManager;
class InfoService;
class IconService;

namespace Ui {
class GroupPlayWindow;
}

class GroupPlayWindow : public QWidget
{
	Q_OBJECT

public:
	explicit GroupPlayWindow(QWidget *parent = nullptr, const QString &addr = "");
	~GroupPlayWindow();

	void GroupPlayList(int eventID);

private slots:

	void SlotRespError(QString message);
	void SlotRespGroupPlayList(QList<CJsonNode> list);

	void SlotReqCoverArt(int id, int index, int mode);
	void SlotCoverArtUpdate(QString coverArt, int index, int mode);
	void SlotSelectPlay(int index, bool muted);
	void SlotSelectTitle(int type, QString rawData);

	void SlotEventGroupPlayUpdate();

//	void SlotResize();

private:

	void ConnectSigToSlot();

	GroupPlayManager	*m_pMgr;
	InfoService			*m_pInfoService;
	IconService			*m_pIconService;

	int					m_nEventID;

	Ui::GroupPlayWindow *ui;
};

#endif // GROUPPLAYWINDOW_H
