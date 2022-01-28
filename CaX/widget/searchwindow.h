#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QWidget>
#include <QMenu>
#include <QModelIndex>

#include "util/CJsonNode.h"

class SearchManager;
class SearchCategory;
class SearchTrack;

namespace Ui {
class SearchWindow;
}

class SearchWindow : public QWidget
{
	Q_OBJECT

public:
	explicit SearchWindow(QWidget *parent = nullptr, const QString &addr = "");
	~SearchWindow();

	void SetKeyword(const QString keyword);

signals:

	void SigAddWidget(QWidget *widget, QString);
	void SigRemoveWidget(QWidget* widget);

private slots:

	void SlotAddWidget(QWidget *widget, QString);
	void SlotRemoveWidget(QWidget *widget);

	void SlotRespError(QString errMsg);
	void SlotRespAlbumList(QList<CJsonNode> list);
	void SlotRespArtistList(QList<CJsonNode> list);
	void SlotRespTrackList(QList<CJsonNode> list);

	void SlotReqCoverArt(int id, int index, int category);
	void SlotCoverArtUpdate(QString fileName, int nIndex, int category);
	void SlotSelectAlbum(int id, QString coverArt);
	void SlotSelectArtist(int id, QString coverArt);
	void SlotSelectPlay(int id, int playType);
	void SlotSelectMenu(const QModelIndex &modelIndex, QPoint point);
	void SlotMenuAction(QAction *action);

	void SlotOptionMenuAction(int id, int menuID, QString coverArt = "");

private:

	void ConnectSigToSlot();
	void Initialize();

	void SetOptionMenu();

	void DoOptionMenuGoToAlbum(int id, QString coverArt);

	SearchManager		*m_pMgr;
	SearchCategory		*m_Album;
	SearchCategory		*m_Artist;
	SearchTrack			*m_Track;

	QMap<int, QString>	m_OptionMenuMap;

	QMenu				*m_Menu;

	QModelIndex			m_ModelIndex;

	Ui::SearchWindow *ui;
};

#endif // SEARCHWINDOW_H
