#ifndef LISTTRACKSEDITOR_H
#define LISTTRACKSEDITOR_H

#include <QWidget>

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
	void SetFavorite(int Favorite);

	QString GetTime();
	void SetTime(const QString &time);

	QString GetArtist();
	void SetArtist(const QString &artist);

	QString GetAlbum();
	void SetAlbum(const QString &album);

	QString GetGenre();
	void SetGenre(const QString &genre);

	FormCoverArt *GetFormCoverArt() const;

signals:

	void SigClickCoverArt(int nID);
	void SigClickPlay(int nID);
	void SigClickTitle(int nID);
	void SigClickFavorite(int nID, int Favorite);
	void SigClickTime(int nID);
	void SigClickArtist(int nID);
	void SigClickAlbum(int nID);
	void SigClickGenre(int nID);
	void SigClickMore(int nID);


protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

private:

	void ConnectSigToSlot();

	FormCoverArt	*m_pFormCoverArt;

	int		m_ID;
	int		m_Favorite;


	Ui::ListTracksEditor *ui;
};

#endif // LISTTRACKSEDITOR_H
