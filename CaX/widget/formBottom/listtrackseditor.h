#ifndef LISTTRACKSEDITOR_H
#define LISTTRACKSEDITOR_H

#include <QWidget>
#include <QMenu>

class FormCoverArt;

namespace Ui {
class ListTracksEditor;
}

class ListTracksEditor : public QWidget
{
	Q_OBJECT

public:
	explicit ListTracksEditor(QWidget *parent = nullptr);
	~ListTracksEditor();

	int GetID() const;
	void SetID(int ID);

	QString GetTitle();
	void SetTitle(const QString &title);

	int GetFavorite() const;
	void SetFavorite(int value);
	void ShowFavorite(bool show);

	QString GetTime();
	void SetTime(const QString &title);
	void ShowTime(bool show);

	QString GetArtist();
	void SetArtist(const QString &title);
	void ShowArtist(bool show);

	QString GetAlbum();
	void SetAlbum(const QString &title);
	void ShowAlbum(bool show);

	QString GetGenre();
	void SetGenre(const QString &title);
	void ShowGenre(bool show);

	// only music db
	QString GetAlbumArtist();
	void SetAlbumArtist(const QString &title);
	void ShowAlbumArtist(bool show);

	QString GetComposer();
	void SetComposer(const QString &title);
	void ShowComposer(bool show);

	QString GetYear();
	void SetYear(const QString &title);
	void ShowYear(bool show);

	// option
	QString GetMood();
	void SetMood(const QString &title);
	void ShowMood(bool show);

	QString GetTempo();
	void SetTempo(const QString &title);
	void ShowTempo(bool show);

	QString GetFormat();
	void SetFormat(const QString &title);
	void ShowFormat(bool show);

	QString GetSampleRate();
	void SetSampleRate(const QString &title);
	void ShowSampleRate(bool show);

	QString GetBitDepth();
	void SetBitDepth(const QString &title);
	void ShowBitDepth(bool show);

	int GetRating() const;
	void SetRating(int value);
	void ShowRating(bool show);

	void ClearMenu();
	void SetMenu(QMap<int, QString> map);

	FormCoverArt *GetFormCoverArt() const;

	void SetFrameSizeFormCoverArt(int size);

signals:

	void SigClickCoverArt(int index);
	void SigClickPlay(int nID);
	void SigClickTitle(int nID, QString coverArt);
	void SigClickFavorite(int nID, int Favorite);
//	void SigClickTime(int nID);
//	void SigClickArtist(int nID);
//	void SigClickAlbum(int nID);
//	void SigClickGenre(int nID);
	void SigMenuAction(int nID, int menuID);


protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotMenuAction(QAction* action);
	void SlotCoverArt(int index);
	void SlotCheck(int index);

private:

	void ConnectSigToSlot();
	void Initialize();

	FormCoverArt	*m_pFormCoverArt;
	QMenu			*m_Menu;

	int		m_ID;
	int		m_Favorite;
	int		m_Rating;

	Ui::ListTracksEditor *ui;
};

#endif // LISTTRACKSEDITOR_H
