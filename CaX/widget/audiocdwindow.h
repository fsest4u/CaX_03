#ifndef AUDIOCDWINDOW_H
#define AUDIOCDWINDOW_H

#include <QWidget>

#include "util/CJsonNode.h"

class AudioCDManager;
class InfoTracks;
class IconTracks;
class ListTracks;
class Loading;

namespace Ui {
class AudioCDWindow;
}

class AudioCDWindow : public QWidget
{
	Q_OBJECT

public:
	explicit AudioCDWindow(QWidget *parent = nullptr, const QString &addr = "");
	~AudioCDWindow();

	void TrackList();
	void TrackInfo(int index);
	void TrackPlay(int index);
	void Eject();
	void CDRipInfo(int index, QList<int> list);
	void CDRip(CJsonNode node, QList<CJsonNode> list);

signals:

	void SigRemoveWidget(QWidget* widget);


private slots:

	void SlotRespTrackList(QList<CJsonNode> list);
	void SlotRespTrackInfo(CJsonNode node);
	void SlotRespCDRipInfo(CJsonNode node);

	void SlotSelectTitle(int id, QString coverArt);
	void SlotCalcTotalTime(int time);



private:

	void ConnectSigToSlot();
	QString MakeInfo();


	AudioCDManager		*m_pMgr;
	InfoTracks			*m_pInfoTracks;
	IconTracks			*m_pIconTracks;
	ListTracks			*m_pListTracks;

	Loading				*m_pLoading;

	QString				m_TotalCount;
	QString				m_TotalTime;
	QString				m_Format;
	QString				m_Date;

	Ui::AudioCDWindow *ui;
};

#endif // AUDIOCDWINDOW_H
