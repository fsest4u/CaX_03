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
	void SetFavorite(int Favorite);

	QString GetTime();
	void SetTime(const QString &time);

	QString GetArtist();
	void SetArtist(const QString &artist);

	QString GetAlbum();
	void SetAlbum(const QString &album);

	QString GetGenre();
	void SetGenre(const QString &genre);

	void ClearMenu();
	void SetMenu(QMap<int, QString> map);

	FormCoverArt *GetFormCoverArt() const;

signals:

	void SigClickCoverArt(int index);
	void SigClickPlay(int nID);
	void SigClickTitle(int nID, QString coverArt);
	void SigClickFavorite(int nID, int Favorite);
	void SigClickTime(int nID);
	void SigClickArtist(int nID);
	void SigClickAlbum(int nID);
	void SigClickGenre(int nID);
	void SigMenuAction(int nID, int menuID);


protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotMenuAction(QAction* action);
	void SlotCoverArt(int index);

private:

	void ConnectSigToSlot();
	void Initialize();

	FormCoverArt	*m_pFormCoverArt;
	QMenu			*m_Menu;

	int		m_ID;
	int		m_Favorite;

	Ui::ListTracksEditor *ui;
};

#endif // LISTTRACKSEDITOR_H
