#ifndef AUDIOCDWINDOW_H
#define AUDIOCDWINDOW_H

#include <QWidget>

#include "util/CJsonNode.h"

class AudioCDManager;
class InfoTracks;
class IconTracks;
class ListTracks;

namespace Ui {
class AudioCDWindow;
}

class AudioCDWindow : public QWidget
{
	Q_OBJECT

public:
	explicit AudioCDWindow(QWidget *parent = nullptr, const QString &addr = "");
	~AudioCDWindow();

	void AddWidgetAudioCDHome();
	void TrackList();
	void TrackInfo(int index);
	void TrackPlay(int index);
	void CDRip(CJsonNode node, QList<CJsonNode> list);

signals:

	void SigRemoveWidget(QWidget* widget);


private slots:

	void SlotRespTrackList(QList<CJsonNode> list);
	void SlotRespTrackInfo(CJsonNode node);
	void SlotRespCDRipInfo(CJsonNode node);

	void SlotRespError(QString errMsg);
	void SlotSelectTitle(int id, QString coverArt);
	void SlotCalcTotalTime(int time);
	void SlotSelectPlay(int id, int playType);

	void SlotTopMenu();
	void SlotTopMenuAction(int menuID);
	void SlotResize(int resize);


private:

	void ConnectSigToSlot();
	void Initialize();
	void SetSelectOffTopMenu();
	void SetSelectOnTopMenu();

	void DoTopMenuSelectAll();
	void DoTopMenuClearAll();
	void DoTopMenuCDRipping();
	void DoTopMenuEjectCD();

	QString MakeInfo();


	AudioCDManager		*m_pMgr;
	InfoTracks			*m_pInfoTracks;
	IconTracks			*m_pIconTracks;
	ListTracks			*m_pListTracks;

	QMap<int, QString>	m_TopMenuMap;
	QMap<int, bool>		m_SelectMap;

	QString				m_TotalCount;
	QString				m_TotalTime;
	QString				m_Format;
	QString				m_Date;

	Ui::AudioCDWindow *ui;
};

#endif // AUDIOCDWINDOW_H
