#ifndef LISTTRACKSEDITOR_H
#define LISTTRACKSEDITOR_H

#include <QWidget>

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

	QString GetCoverArt() const;
	void SetCoverArt(const QString &value);

	QString GetTitle();
	void SetTitle(const QString &value);

	QString GetTime();
	void SetTime(const QString &value);

	QString GetArtist();
	void SetArtist(const QString &value);

	QString GetAlbum();
	void SetAlbum(const QString &value);

	QString GetGenre();
	void SetGenre(const QString &value);

signals:

	void SigClickCoverArt(int nID);
	void SigClickPlay(int nID);
	void SigClickTitle(int nID);
	void SigClickTime(int nID);
	void SigClickArtist(int nID);
	void SigClickAlbum(int nID);
	void SigClickGenre(int nID);
	void SigClickMore(int nID);


protected:

	bool eventFilter(QObject *object, QEvent *event);

private:

	int		m_ID;
	QString m_CoverArt;

	Ui::ListTracksEditor *ui;
};

#endif // LISTTRACKSEDITOR_H
