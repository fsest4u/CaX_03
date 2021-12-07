#ifndef SEARCHTRACKEDITOR_H
#define SEARCHTRACKEDITOR_H

#include <QWidget>
#include <QMenu>

class FormCoverArt;

namespace Ui {
class SearchTrackEditor;
}

class SearchTrackEditor : public QWidget
{
	Q_OBJECT

public:
	explicit SearchTrackEditor(QWidget *parent = nullptr);
	~SearchTrackEditor();

	int GetID() const;
	void SetID(int ID);

	QString GetTitle() const;
	void SetTitle(const QString &title);

	QString GetTime() const;
	void SetTime(const QString &time);

	QString GetArtist() const;
	void SetArtist(const QString &artist);

	int GetAlbumID() const;
	void SetAlbumID(const int id);

	QString GetAlbum() const;
	void SetAlbum(const QString &album);

	QString GetGenre() const;
	void SetGenre(const QString &genre);

	void ClearMenu();
	void SetMenu(QMap<int, QString> map);

	FormCoverArt *GetFormCoverArt() const;

signals:

	void SigClickPlay(int nID);
	void SigMenuAction(int nID, int menuID, QString coverArt);

protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotMenuAction(QAction* action);

private:

	void ConnectSigToSlot();
	void Initialize();

	FormCoverArt	*m_pFormCoverArt;
	QMenu			*m_Menu;

	int m_ID;
	int m_AlbumID;

	Ui::SearchTrackEditor *ui;
};

#endif // SEARCHTRACKEDITOR_H
