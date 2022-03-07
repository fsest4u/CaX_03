#ifndef AUDIOCDWINDOW_H
#define AUDIOCDWINDOW_H

#include <QWidget>
#include <QMenu>

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
	explicit AudioCDWindow(QWidget *parent = nullptr, const QString &addr = "", const int &eventID = -1);
	~AudioCDWindow();

	void AddWidgetAudioCDHome();

	void RequestTrackList();

signals:

	void SigRemoveWidget(QWidget* widget);

private slots:

	void SlotRespTrackList(CJsonNode node);
	void SlotRespTrackInfo(CJsonNode node);
	void SlotRespCDRipInfo(CJsonNode node);
	void SlotRespCategoryList(CJsonNode node);
	void SlotCoverArtUpdate(QString filename);
	void SlotCoverArtUpdate(QString coverArt, int index, int mode);

	void SlotRespError(QString errMsg);
	void SlotSelectTitle(int id, QString coverArt);
	void SlotCalcTotalTime(int time);
	void SlotSelectPlay(int id, int playType);
	void SlotSelectMenu(const QModelIndex &modelIndex, QPoint point);
	void SlotMenuAction(QAction *action);
	void SlotReqCoverArt(int id, int index, int mode);

	void SlotPlayAll();
	void SlotPlayRandom();
	void SlotTopMenu();
	void SlotTopMenuAction(int menuID);
	void SlotResize(int resize);

	void SlotOptionMenuAction(int id, int menuID);

private:

	void ReadSettings();
	void WriteSettings();

	void ConnectSigToSlot();
	void Initialize();
	void ResetSelectMap();
	void SetCategoryList(QList<CJsonNode> list);
	int GetListModeFromResize(int resize);

	void SetSelectOffTopMenu();
	void SetSelectOnTopMenu();

	void DoTopMenuSelectAll();
	void DoTopMenuClearAll();
	void DoTopMenuChangeMetaInfo();
	void DoTopMenuCDRipping();
	void DoTopMenuEjectCD();

	void SetOptionMenu();

	void DoOptionMenuCDRipping(int id);
	void DoOptionMenuTrackInfo(int id);

	QString MakeInfo();

	AudioCDManager		*m_pMgr;
	InfoTracks			*m_pInfoTracks;
	IconTracks			*m_pIconTracks;
	ListTracks			*m_pListTracks;

	QList<CJsonNode>	m_RespList;

	QMap<int, QString>	m_TopMenuMap;
	QMap<int, QString>	m_OptionMenuMap;
	QMap<int, bool>		m_SelectMap;

	QMenu				*m_Menu;

	int					m_ListMode;
	int					m_EventID;

	QString				m_TotalCount;
	QString				m_TotalTime;
	QString				m_Format;
	QString				m_Date;

	QStringList			m_AlbumList;
	QStringList			m_AlbumArtistList;
	QStringList			m_ArtistList;
	QStringList			m_GenreList;
	QStringList			m_ComposerList;
	QStringList			m_MoodList;

	QStringList			m_SourceList;

	int					m_ResizeTrack;

	int					m_ID;

	Ui::AudioCDWindow *ui;
};

#endif // AUDIOCDWINDOW_H
