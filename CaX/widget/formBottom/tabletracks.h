#ifndef TABLETRACKS_H
#define TABLETRACKS_H

#include <QWidget>
#include <QStandardItemModel>
#include <QScrollBar>
#include <QMenu>

#include "util/CJsonNode.h"

class TableTracksDelegate;

namespace Ui {
class TableTracks;
}

class TableTracks : public QWidget
{
	Q_OBJECT

public:
	explicit TableTracks(QWidget *parent = nullptr);
	~TableTracks();

	enum {
		TABLE_TRACKS_ID = 0,
		TABLE_TRACKS_SELECT,
		TABLE_TRACKS_COVER,
		TABLE_TRACKS_PLAY,
		TABLE_TRACKS_TITLE,
		TABLE_TRACKS_FAVORITE,
		TABLE_TRACKS_RATING,
		TABLE_TRACKS_TIME,
		TABLE_TRACKS_ARTIST,
		TABLE_TRACKS_ALBUM,
		TABLE_TRACKS_GENRE,
		TABLE_TRACKS_ALBUM_ARTIST,
		TABLE_TRACKS_COMPOSER,
		TABLE_TRACKS_YEAR,
		TABLE_TRACKS_MOOD,
		TABLE_TRACKS_TEMPO,
		TABLE_TRACKS_FORMAT,
		TABLE_TRACKS_SAMPLE_RATE,
		TABLE_TRACKS_BITRATE,
		TABLE_TRACKS_TRACK_COUNT,
		TABLE_TRACKS_ALBUM_GAIN,
		TABLE_TRACKS_ALBUM_ID,
		TABLE_TRACKS_ARTIST_ID,
		TABLE_TRACKS_INDEX,
		TABLE_TRACKS_MENU,
		TABLE_TRACKS_MAX
	};

	QList<CJsonNode> GetNodeList() const;
	void SetNodeList(QList<CJsonNode> list, int service);
	void ClearNodeList();

	void ClearSelectMap();
	void SetAllSelectMap();

	QMap<int, bool> GetSelectMap() const;
	void SetSelectMap(const QMap<int, bool> &SelectMap);

	QMap<int, QString> GetOptionMenuMap() const;
	void SetOptionMenuMap(const QMap<int, QString> &OptionMenuMap);

	void SetColResize(int resize);
	void SetRowResize(int resize);

	QStandardItemModel	*GetModel();
	TableTracksDelegate	*GetDelegate();

	void SetHeaderTitle(QString title);

//	void ShowColumnFavorite(bool show);
//	void ShowColumnTime(bool show);
//	void ShowColumnArtist(bool show);
//	void ShowColumnAlbum(bool show);
//	void ShowColumnGenre(bool show);
//	void ShowColumnAlbumArtist(bool show);
//	void ShowColumnComposer(bool show);
//	void ShowColumnYear(bool show);

//	void ShowColumnMood(bool show);
//	void ShowColumnTempo(bool show);
//	void ShowColumnFormat(bool show);
//	void ShowColumnSampleRating(bool show);
//	void ShowColumnBitDepth(bool show);
//	void ShowColumnRating(bool show);

	int GetCntColumnShow();
	bool GetColumnShow(int column);
	void SetColumnShow(int column, bool show);

	void ShowFrameEmpty(bool show);

signals:

	void SigReqCoverArt(int id, int index, int mode);
	void SigAppendList();

	void SigSelectPlay(int id, int playType);
	void SigSelectTitle(const QModelIndex &index);
	void SigSelectFavorite(int id, int index, int favorite);
	void SigMenuAction(const QModelIndex &index, int menuID);

protected:

	void resizeEvent(QResizeEvent *event);

private slots:

	void SlotScrollValueChanged(int value);
	void SlotScrollReleased();
	void SlotDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

	void SlotClickCell(const QModelIndex &index);
	void SlotSectionClicked(int logicalIndex);
	void SlotSectionResize(int logicalIndex, int oldWidth, int newWidth);
	void SlotMenuAction(QAction *action);

private:

	void ReadSettings();
	void WriteSettings();

	void ConnectSigToSlot();
	void Initialize();

	QStandardItemModel		*m_Model;
	TableTracksDelegate		*m_Delegate;
	QScrollBar				*m_ScrollBar;

	QList<CJsonNode>		m_NodeList;
	QMap<int, bool>			m_SelectMap;
	QMap<int, QString>		m_OptionMenuMap;

	QMenu					*m_Menu;

	QModelIndex				m_ModelIndex;

	QPoint					m_PointCurrent;

	int						m_ColWidthID;
	int						m_ColWidthSelect;
	int						m_ColWidthCover;
	int						m_ColWidthPlay;
	int						m_ColWidthTitle;
	int						m_ColWidthFavorite;
	int						m_ColWidthTime;
	int						m_ColWidthArtist;
	int						m_ColWidthAlbum;
	int						m_ColWidthGenre;
	int						m_ColWidthAlbumArtist;
	int						m_ColWidthComposer;
	int						m_ColWidthYear;
	int						m_ColWidthMood;
	int						m_ColWidthTempo;
	int						m_ColWidthFormat;
	int						m_ColWidthSampleRate;
	int						m_ColWidthBitrate;
	int						m_ColWidthRating;
	int						m_ColWidthTrackCount;
	int						m_ColWidthAlbumGain;
	int						m_ColWidthIndex;
	int						m_ColWidthMenu;

	Ui::TableTracks *ui;
};

#endif // TABLETRACKS_H
